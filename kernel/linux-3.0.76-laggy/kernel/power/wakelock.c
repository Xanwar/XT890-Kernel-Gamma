/* kernel/power/wakelock.c
 *
 * Copyright (C) 2005-2008 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/rtc.h>
#include <linux/suspend.h>
#include <linux/syscalls.h> /* sys_sync */
#include <linux/wakelock.h>
#include <linux/debugfs.h>

#ifndef CREATE_TRACE_POINTS
#define CREATE_TRACE_POINTS
#include <trace/events/wakelock.h>
#undef CREATE_TRACE_POINTS
#endif

#ifdef CONFIG_WAKELOCK_STAT
#include <linux/proc_fs.h>
#ifdef CONFIG_PM_DEBUG
#include <linux/earlysuspend.h>
#endif
#endif
#include "power.h"

enum {
	DEBUG_EXIT_SUSPEND = 1U << 0,
	DEBUG_WAKEUP = 1U << 1,
	DEBUG_SUSPEND = 1U << 2,
	DEBUG_EXPIRE = 1U << 3,
	DEBUG_WAKE_LOCK = 1U << 4,
};
static int debug_mask = DEBUG_EXIT_SUSPEND | DEBUG_WAKEUP;
module_param_named(debug_mask, debug_mask, int, S_IRUGO | S_IWUSR | S_IWGRP);

#define WAKE_LOCK_TYPE_MASK              (0x0f)
#define WAKE_LOCK_INITIALIZED            (1U << 8)
#define WAKE_LOCK_ACTIVE                 (1U << 9)
#define WAKE_LOCK_AUTO_EXPIRE            (1U << 10)
#define WAKE_LOCK_PREVENTING_SUSPEND     (1U << 11)

static DEFINE_SPINLOCK(list_lock);
static LIST_HEAD(inactive_locks);
static struct list_head active_wake_locks[WAKE_LOCK_TYPE_COUNT];
static int current_event_num;
struct workqueue_struct *suspend_work_queue;
struct wake_lock main_wake_lock;
suspend_state_t requested_suspend_state = PM_SUSPEND_MEM;
static struct wake_lock unknown_wakeup;
static struct wake_lock suspend_backoff_lock;
static struct wake_lock suspend_sync_wake_lock;
#ifdef CONFIG_PM_DEBUG
static struct wake_lock *longest_background_lock;
static s64 longest;
#endif

static unsigned suspend_short_count;

#ifdef CONFIG_WAKELOCK_STAT
static struct wake_lock deleted_wake_locks;
static ktime_t last_sleep_time_update;
static int wait_for_wakeup;
#ifdef CONFIG_PM_DEBUG
static int early_suspend_called;
static ktime_t early_suspend_time;
static ktime_t wakeup_time;
#endif

static unsigned int suspend_backoff_threshold =
	CONFIG_SUSPEND_BACKOFF_THRESHOLD;
static unsigned int suspend_backoff_residency =
	CONFIG_SUSPEND_BACKOFF_RESIDENCY;
static unsigned int suspend_backoff_interval =
	CONFIG_SUSPEND_BACKOFF_INTERVAL;

static struct dentry *backoff_dir;

static int backoff_threshold_show(struct seq_file *s, void *unused)
{
	seq_printf(s, "%u\n", suspend_backoff_threshold);
	return 0;
}

static int backoff_residency_show(struct seq_file *s, void *unused)
{
	seq_printf(s, "%u\n", suspend_backoff_residency);
	return 0;
}

static int backoff_interval_show(struct seq_file *s, void *unused)
{
	seq_printf(s, "%u\n", suspend_backoff_interval);
	return 0;
}

static int backoff_threshold_open(struct inode *inode, struct file *file)
{
	return single_open(file, backoff_threshold_show, NULL);
}

static int backoff_interval_open(struct inode *inode, struct file *file)
{
	return single_open(file, backoff_interval_show, NULL);
}

static int backoff_residency_open(struct inode *inode, struct file *file)
{
	return single_open(file, backoff_residency_show, NULL);
}

static size_t get_backoff_param(size_t count, const char __user *ubuf,
				unsigned int *param)
{
	int ret;
	char buf[10];
	unsigned long val;

	if (count >= sizeof(buf))
		return -EINVAL;

	if (copy_from_user(&buf, ubuf, count))
		return -EFAULT;

	buf[count] = 0;

	ret = kstrtoul(buf, 10, &val);
	if (ret)
		return ret;

	*param = val;

	return count;
}

static ssize_t backoff_threshold_write(struct file *file,
				       const char __user *ubuf,
				       size_t count, loff_t *off)
{
	return get_backoff_param(count, ubuf, &suspend_backoff_threshold);
}

static ssize_t backoff_interval_write(struct file *file,
				      const char __user *ubuf,
				      size_t count, loff_t *off)
{
	return get_backoff_param(count, ubuf, &suspend_backoff_interval);
}

static ssize_t backoff_residency_write(struct file *file,
				       const char __user *ubuf,
				       size_t count, loff_t *off)
{
	return get_backoff_param(count, ubuf, &suspend_backoff_residency);
}


static const struct file_operations backoff_fops[3] = {
	{
		.owner = THIS_MODULE,
		.open = backoff_threshold_open,
		.read = seq_read,
		.write = backoff_threshold_write,
		.llseek = seq_lseek,
		.release = single_release,
	},
	{
		.owner = THIS_MODULE,
		.open = backoff_interval_open,
		.read = seq_read,
		.write = backoff_interval_write,
		.llseek = seq_lseek,
		.release = single_release,
	},
	{
		.owner = THIS_MODULE,
		.open = backoff_residency_open,
		.read = seq_read,
		.write = backoff_residency_write,
		.llseek = seq_lseek,
		.release = single_release,
	},
};

static void debugfs_backoff_exit(void)
{
	if (backoff_dir)
		debugfs_remove_recursive(backoff_dir);
}

static int __init debugfs_backoff_init(void)
{
	struct dentry *ent;

	backoff_dir = debugfs_create_dir("suspend-backoff", NULL);
	if (!backoff_dir) {
		pr_err("unable to create suspend-backoff entry (%ld)\n",
		       PTR_ERR(backoff_dir));
		return 0;
	}

	ent =  debugfs_create_file("threshold",
				   S_IFREG | S_IRUGO | S_IWUSR | S_IWGRP,
				   backoff_dir, (void *)0, &backoff_fops[0]);
	if (!ent) {
		pr_err("unable to create threshold entry (%ld)\n",
		       PTR_ERR(ent));
		goto fail;
	}

	ent = debugfs_create_file("interval",
				  S_IFREG | S_IRUGO | S_IWUSR | S_IWGRP,
				  backoff_dir, (void *)0, &backoff_fops[1]);
	if (!ent) {
		pr_err("unable to create interval entry (%ld)\n",
		       PTR_ERR(ent));
		goto fail;
	}

	ent = debugfs_create_file("residency",
				  S_IFREG | S_IRUGO | S_IWUSR | S_IWGRP,
				  backoff_dir, (void *)0, &backoff_fops[2]);
	if (!ent) {
		pr_err("unable to create residency entry (%ld)\n",
		       PTR_ERR(ent));
		goto fail;
	}

	return 0;
fail:
	debugfs_backoff_exit();
	return 0;
}

int get_expired_time(struct wake_lock *lock, ktime_t *expire_time)
{
	struct timespec ts;
	struct timespec kt;
	struct timespec tomono;
	struct timespec delta;
	struct timespec sleep;
	long timeout;

	if (!(lock->flags & WAKE_LOCK_AUTO_EXPIRE))
		return 0;
	get_xtime_and_monotonic_and_sleep_offset(&kt, &tomono, &sleep);
	timeout = lock->expires - jiffies;
	if (timeout > 0)
		return 0;
	jiffies_to_timespec(-timeout, &delta);
	set_normalized_timespec(&ts, kt.tv_sec + tomono.tv_sec - delta.tv_sec,
				kt.tv_nsec + tomono.tv_nsec - delta.tv_nsec);
	*expire_time = timespec_to_ktime(ts);
	return 1;
}


static int print_lock_stat(struct seq_file *m, struct wake_lock *lock)
{
	int lock_count = lock->stat.count;
	int expire_count = lock->stat.expire_count;
	ktime_t active_time = ktime_set(0, 0);
	ktime_t total_time = lock->stat.total_time;
	ktime_t max_time = lock->stat.max_time;

	ktime_t prevent_suspend_time = lock->stat.prevent_suspend_time;
	if (lock->flags & WAKE_LOCK_ACTIVE) {
		ktime_t now, add_time;
		int expired = get_expired_time(lock, &now);
		if (!expired)
			now = ktime_get();
		add_time = ktime_sub(now, lock->stat.last_time);
		lock_count++;
		if (!expired)
			active_time = add_time;
		else
			expire_count++;
		total_time = ktime_add(total_time, add_time);
		if (lock->flags & WAKE_LOCK_PREVENTING_SUSPEND)
			prevent_suspend_time = ktime_add(prevent_suspend_time,
					ktime_sub(now, last_sleep_time_update));
		if (add_time.tv64 > max_time.tv64)
			max_time = add_time;
	}

	return seq_printf(m,
		     "\"%s\"\t%d\t%d\t%d\t%lld\t%lld\t%lld\t%lld\t%lld\n",
		     lock->name, lock_count, expire_count,
		     lock->stat.wakeup_count, ktime_to_ns(active_time),
		     ktime_to_ns(total_time),
		     ktime_to_ns(prevent_suspend_time), ktime_to_ns(max_time),
		     ktime_to_ns(lock->stat.last_time));
}

static int wakelock_stat_headers(struct seq_file *m)
{
	seq_printf(m, "name\tcount\texpire_count\twake_count\tactive_since"
			"\ttotal_time\tsleep_time\tmax_time\tlast_change\n");
	return 0;
}

static void *wakelock_seq_start(struct seq_file *m, loff_t *pos)
{
	int type, n = *pos;
	unsigned long irqflags;
	struct wake_lock *lock;

	if (n == 0)
		return SEQ_START_TOKEN;
	n--;

	spin_lock_irqsave(&list_lock, irqflags);
	list_for_each_entry(lock, &inactive_locks, link)
		if (n-- == 0)
			goto found;

	for (type = 0; type < WAKE_LOCK_TYPE_COUNT; type++) {
		list_for_each_entry(lock, &active_wake_locks[type], link)
			if (n-- == 0)
				goto found;
	}
	lock = NULL;
found:
	/* Copy to private data to avoid the lock being free'd before
	 * getting to _show
	 */
	if (lock)
		memcpy(m->private, lock, sizeof(struct wake_lock));
	spin_unlock_irqrestore(&list_lock, irqflags);
	return lock;
}

static void *wakelock_seq_next(struct seq_file *m, void *v, loff_t *pos)
{
	(*pos)++;
	return wakelock_seq_start(m, pos);
}

static void wakelock_seq_stop(struct seq_file *m, void *v)
{
}

static int wakelock_seq_show(struct seq_file *m, void *v)
{
	int ret;
	unsigned long irqflags;

	if (v == SEQ_START_TOKEN)
		return wakelock_stat_headers(m);

	spin_lock_irqsave(&list_lock, irqflags);
	ret = print_lock_stat(m, m->private);
	spin_unlock_irqrestore(&list_lock, irqflags);
	return ret;
}

static void wake_unlock_stat_locked(struct wake_lock *lock, int expired)
{
	ktime_t duration;
	ktime_t now;
	if (!(lock->flags & WAKE_LOCK_ACTIVE))
		return;
	if (get_expired_time(lock, &now))
		expired = 1;
	else
		now = ktime_get();
	lock->stat.count++;
	if (expired)
		lock->stat.expire_count++;
#ifdef CONFIG_PM_DEBUG
	if (early_suspend_called) {
		if (ktime_to_ns(lock->stat.last_unlock_time)
				< ktime_to_ns(early_suspend_time))
				lock->stat.background_locked_time
						= ktime_set(0, 0);
		/* If last_time is later than early_suspend_time, then this lock
		   was held after early_suspend called */
		if (ktime_to_ns(lock->stat.last_time) >
			ktime_to_ns(early_suspend_time))
			duration = ktime_sub(now, lock->stat.last_time);
		else
			duration = ktime_sub(now, early_suspend_time);
	} else {
		/* If last_unlock_time is earlier than wakeup_time, then this
		   lock was held for the first time after system resumed*/
		if (ktime_to_ns(lock->stat.last_unlock_time)
				< ktime_to_ns(wakeup_time)) {
				lock->stat.background_locked_time
						= ktime_set(0, 0);
		}
		duration = ktime_sub(
				now, lock->stat.last_time);
	}
	lock->stat.background_locked_time = ktime_add(
			lock->stat.background_locked_time,
			duration);
	if (ktime_to_ns(lock->stat.background_locked_time) > longest) {
			longest = ktime_to_ns(
					lock->stat.background_locked_time);
			longest_background_lock = lock;
	}
	lock->stat.last_unlock_time = now;
#endif
	duration = ktime_sub(now, lock->stat.last_time);
	lock->stat.total_time = ktime_add(lock->stat.total_time, duration);
	if (ktime_to_ns(duration) > ktime_to_ns(lock->stat.max_time))
		lock->stat.max_time = duration;
	lock->stat.last_time = ktime_get();
	if (lock->flags & WAKE_LOCK_PREVENTING_SUSPEND) {
		duration = ktime_sub(now, last_sleep_time_update);
		lock->stat.prevent_suspend_time = ktime_add(
			lock->stat.prevent_suspend_time, duration);
		lock->flags &= ~WAKE_LOCK_PREVENTING_SUSPEND;
	}
}

static void update_sleep_wait_stats_locked(int done)
{
	struct wake_lock *lock;
	ktime_t now, etime, elapsed, add;
	int expired;

	now = ktime_get();
	elapsed = ktime_sub(now, last_sleep_time_update);
	list_for_each_entry(lock, &active_wake_locks[WAKE_LOCK_SUSPEND], link) {
		expired = get_expired_time(lock, &etime);
		if (lock->flags & WAKE_LOCK_PREVENTING_SUSPEND) {
			if (expired)
				add = ktime_sub(etime, last_sleep_time_update);
			else
				add = elapsed;
			lock->stat.prevent_suspend_time = ktime_add(
				lock->stat.prevent_suspend_time, add);
		}
		if (done || expired)
			lock->flags &= ~WAKE_LOCK_PREVENTING_SUSPEND;
		else
			lock->flags |= WAKE_LOCK_PREVENTING_SUSPEND;
	}
	last_sleep_time_update = now;
}
#endif


static void expire_wake_lock(struct wake_lock *lock)
{
#ifdef CONFIG_WAKELOCK_STAT
	wake_unlock_stat_locked(lock, 1);
#endif
	lock->flags &= ~(WAKE_LOCK_ACTIVE | WAKE_LOCK_AUTO_EXPIRE);
	list_del(&lock->link);
	list_add(&lock->link, &inactive_locks);
	if (debug_mask & (DEBUG_WAKE_LOCK | DEBUG_EXPIRE))
		pr_info("expired wake lock %s\n", lock->name);
}

/* Caller must acquire the list_lock spinlock */
static void print_active_locks(int type)
{
	struct wake_lock *lock;
	bool print_expired = true;
	ktime_t active_time = ktime_set(0, 0);
	ktime_t total_time;

	BUG_ON(type >= WAKE_LOCK_TYPE_COUNT);
	list_for_each_entry(lock, &active_wake_locks[type], link) {
		total_time = lock->stat.total_time;
		if (lock->flags & WAKE_LOCK_ACTIVE) {
			ktime_t now, add_time;
			int expired = get_expired_time(lock, &now);
			if (!expired)
				now = ktime_get();
			add_time = ktime_sub(now, lock->stat.last_time);
			if (!expired)
				active_time = add_time;
			total_time = ktime_add(total_time, add_time);
		}
		if (lock->flags & WAKE_LOCK_AUTO_EXPIRE) {
			long timeout = lock->expires - jiffies;
			if (timeout > 0)
				pr_info("active wake lock %s, time left %ld\n",
					lock->name, timeout);
			else if (print_expired)
				pr_info("wake lock %s, expired\n", lock->name);
		} else {
			pr_info("active wake lock %s active %lld total %lld\n",
				lock->name, ktime_to_ns(active_time),
				ktime_to_ns(total_time));
			if (!(debug_mask & DEBUG_EXPIRE))
				print_expired = false;
		}
	}
}

static long has_wake_lock_locked(int type)
{
	struct wake_lock *lock, *n;
	long max_timeout = 0;

	BUG_ON(type >= WAKE_LOCK_TYPE_COUNT);
	list_for_each_entry_safe(lock, n, &active_wake_locks[type], link) {
		if (lock->flags & WAKE_LOCK_AUTO_EXPIRE) {
			long timeout = lock->expires - jiffies;
			if (timeout <= 0)
				expire_wake_lock(lock);
			else if (timeout > max_timeout)
				max_timeout = timeout;
		} else
			return -1;
	}
	return max_timeout;
}

long has_wake_lock(int type)
{
	long ret;
	unsigned long irqflags;
	spin_lock_irqsave(&list_lock, irqflags);
	ret = has_wake_lock_locked(type);
	if (ret && (debug_mask & DEBUG_WAKEUP) && type == WAKE_LOCK_SUSPEND)
		print_active_locks(type);
	spin_unlock_irqrestore(&list_lock, irqflags);
	return ret;
}

static void suspend_backoff(void)
{
	pr_info("suspend: too many immediate wakeups, back off\n");
	wake_lock_timeout(&suspend_backoff_lock,
			  msecs_to_jiffies(suspend_backoff_interval));
}

DECLARE_COMPLETION(suspend_sync_complete);

static void do_suspend_sync(struct work_struct *work)
{
	/* The Max sys_sync timeout value is 1000 secs */
	wake_lock_timeout(&suspend_sync_wake_lock, 1000*HZ);
	sys_sync();
	/* sys_sync done, unlock suspend_sync_wake_lock immediately */
	wake_unlock(&suspend_sync_wake_lock);
	kfree(work);
	complete(&suspend_sync_complete);
}

static void suspend_sync(void)
{
	struct work_struct *work;
	/* Avoid to loop to sys_sync */
	if (wake_lock_active(&suspend_sync_wake_lock))
		return;

	/*
	* Ignored previous sys_sync()
	* because maybe it was done long time ago
	*/
	INIT_COMPLETION(suspend_sync_complete);

	work = kmalloc(sizeof(*work), GFP_ATOMIC);
	if (work) {
		INIT_WORK(work, do_suspend_sync);
		wake_lock(&suspend_sync_wake_lock);
		schedule_work(work);
	} else {
		pr_err("suspend: failed to do sync because ENOMEM");
	}
}

static void suspend(struct work_struct *work)
{
	int ret;
	int entry_event_num;
	struct timespec ts_entry, ts_exit;

	if (has_wake_lock(WAKE_LOCK_SUSPEND) || !alarm_pm_wake_check()) {
		if (debug_mask & DEBUG_SUSPEND)
			pr_info("suspend: abort suspend\n");
		return;
	}

	entry_event_num = current_event_num;
	suspend_sync();

	wait_for_completion_timeout(&suspend_sync_complete, HZ / 4);

	if (wake_lock_active(&suspend_sync_wake_lock)) {
		if (debug_mask & DEBUG_SUSPEND)
			pr_info("suspend: abort suspend because syncing\n");
		return;
	}

	if (debug_mask & DEBUG_SUSPEND)
		pr_info("suspend: enter suspend\n");

	getnstimeofday(&ts_entry);
	ret = pm_suspend(requested_suspend_state);
	getnstimeofday(&ts_exit);

	if (debug_mask & DEBUG_EXIT_SUSPEND) {
		struct rtc_time tm;
		rtc_time_to_tm(ts_exit.tv_sec, &tm);
		pr_info("suspend: exit suspend, ret = %d "
			"(%d-%02d-%02d %02d:%02d:%02d.%09lu UTC)\n", ret,
			tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			tm.tm_hour, tm.tm_min, tm.tm_sec, ts_exit.tv_nsec);
	}

	if (ts_exit.tv_sec - ts_entry.tv_sec <= suspend_backoff_residency) {
		++suspend_short_count;

		if (suspend_short_count == suspend_backoff_threshold) {
			suspend_backoff();
			suspend_short_count = 0;
		}
	} else {
		suspend_short_count = 0;
	}

	if (current_event_num == entry_event_num) {
		if (debug_mask & DEBUG_SUSPEND)
			pr_info("suspend: pm_suspend returned with no event\n");
		wake_lock_timeout(&unknown_wakeup, HZ / 2);
	}
}
static DECLARE_WORK(suspend_work, suspend);

static void expire_wake_locks(unsigned long data)
{
	long has_lock;
	unsigned long irqflags;
	if (debug_mask & DEBUG_EXPIRE)
		pr_info("expire_wake_locks: start\n");
	spin_lock_irqsave(&list_lock, irqflags);
	if (debug_mask & DEBUG_SUSPEND)
		print_active_locks(WAKE_LOCK_SUSPEND);
	has_lock = has_wake_lock_locked(WAKE_LOCK_SUSPEND);
	if (debug_mask & DEBUG_EXPIRE)
		pr_info("expire_wake_locks: done, has_lock %ld\n", has_lock);
	if (has_lock == 0)
		queue_work(suspend_work_queue, &suspend_work);
	spin_unlock_irqrestore(&list_lock, irqflags);
}
static DEFINE_TIMER(expire_timer, expire_wake_locks, 0, 0);

static int power_suspend_late(struct device *dev)
{
	int ret = has_wake_lock(WAKE_LOCK_SUSPEND) ? -EAGAIN : 0;
#ifdef CONFIG_WAKELOCK_STAT
	wait_for_wakeup = !ret;
#ifdef CONFIG_PM_DEBUG
	if (longest_background_lock && longest_background_lock->flags
			& WAKE_LOCK_INITIALIZED) {
		s64 ns;
		ns = ktime_to_ns(
				longest_background_lock
				->stat.background_locked_time);
		pr_info("longest wake lock: [%s][%lld]\n",
				longest_background_lock->name,
				ns);
	}
	longest_background_lock = NULL;
	longest = 0;
	early_suspend_called = 0;
#endif
#endif
	if (debug_mask & DEBUG_SUSPEND)
		pr_info("power_suspend_late return %d\n", ret);
	return ret;
}

#ifdef CONFIG_WAKELOCK_STAT
#ifdef CONFIG_PM_DEBUG
static void power_early_suspend(struct early_suspend *h)
{
	early_suspend_called = 1;
	early_suspend_time = ktime_get();
	longest = 0;
	longest_background_lock = NULL;
}
static struct early_suspend power_early_suspend_desc = {
	.level = EARLY_SUSPEND_LEVEL_STOP_DRAWING,
	.suspend = power_early_suspend,
};
#endif
#endif

static struct dev_pm_ops power_driver_pm_ops = {
	.suspend_noirq = power_suspend_late,
};

static struct platform_driver power_driver = {
	.driver.name = "power",
	.driver.pm = &power_driver_pm_ops,
};
static struct platform_device power_device = {
	.name = "power",
};

void wake_lock_init(struct wake_lock *lock, int type, const char *name)
{
	unsigned long irqflags = 0;

	if (name)
		lock->name = name;
	BUG_ON(!lock->name);

	if (debug_mask & DEBUG_WAKE_LOCK)
		pr_info("wake_lock_init name=%s\n", lock->name);
#ifdef CONFIG_WAKELOCK_STAT
	lock->stat.count = 0;
	lock->stat.expire_count = 0;
	lock->stat.wakeup_count = 0;
	lock->stat.total_time = ktime_set(0, 0);
	lock->stat.prevent_suspend_time = ktime_set(0, 0);
	lock->stat.max_time = ktime_set(0, 0);
	lock->stat.last_time = ktime_set(0, 0);
#ifdef CONFIG_PM_DEBUG
	lock->stat.last_unlock_time = ktime_set(0, 0);
	lock->stat.background_locked_time = ktime_set(0, 0);
#endif
#endif
	lock->flags = (type & WAKE_LOCK_TYPE_MASK) | WAKE_LOCK_INITIALIZED;

	INIT_LIST_HEAD(&lock->link);
	spin_lock_irqsave(&list_lock, irqflags);
	list_add(&lock->link, &inactive_locks);
	spin_unlock_irqrestore(&list_lock, irqflags);
}
EXPORT_SYMBOL(wake_lock_init);

void wake_lock_destroy(struct wake_lock *lock)
{
	unsigned long irqflags;
	if (debug_mask & DEBUG_WAKE_LOCK)
		pr_info("wake_lock_destroy name=%s\n", lock->name);
	spin_lock_irqsave(&list_lock, irqflags);
	lock->flags &= ~WAKE_LOCK_INITIALIZED;
#ifdef CONFIG_WAKELOCK_STAT
#ifdef CONFIG_PM_DEBUG
	if (longest_background_lock == lock) {
		s64 ns;
		ns = ktime_to_ns(
			longest_background_lock
			->stat.background_locked_time);
		pr_info("longest wake lock: [%s][%lld], destroyed\n",
			longest_background_lock->name,
			ns);
		longest_background_lock = NULL;
		}
#endif
	if (lock->stat.count) {
		deleted_wake_locks.stat.count += lock->stat.count;
		deleted_wake_locks.stat.expire_count += lock->stat.expire_count;
		deleted_wake_locks.stat.total_time =
			ktime_add(deleted_wake_locks.stat.total_time,
				  lock->stat.total_time);
		deleted_wake_locks.stat.prevent_suspend_time =
			ktime_add(deleted_wake_locks.stat.prevent_suspend_time,
				  lock->stat.prevent_suspend_time);
		deleted_wake_locks.stat.max_time =
			ktime_add(deleted_wake_locks.stat.max_time,
				  lock->stat.max_time);
	}
#endif
	list_del(&lock->link);
	spin_unlock_irqrestore(&list_lock, irqflags);
}
EXPORT_SYMBOL(wake_lock_destroy);

static void wake_lock_internal(
	struct wake_lock *lock, long timeout, int has_timeout)
{
	int type;
	unsigned long irqflags;
	long expire_in;

	spin_lock_irqsave(&list_lock, irqflags);
	type = lock->flags & WAKE_LOCK_TYPE_MASK;
	BUG_ON(type >= WAKE_LOCK_TYPE_COUNT);
	BUG_ON(!(lock->flags & WAKE_LOCK_INITIALIZED));
#ifdef CONFIG_WAKELOCK_STAT
	if (type == WAKE_LOCK_SUSPEND && wait_for_wakeup) {
		if (debug_mask & DEBUG_WAKEUP)
			pr_info("wakeup wake lock: %s\n", lock->name);
		wait_for_wakeup = 0;
		lock->stat.wakeup_count++;
	}
	if ((lock->flags & WAKE_LOCK_AUTO_EXPIRE) &&
	    (long)(lock->expires - jiffies) <= 0) {
		wake_unlock_stat_locked(lock, 0);
		lock->stat.last_time = ktime_get();
	}
#ifdef CONFIG_PM_DEBUG
	wakeup_time = ktime_get();
#endif
#endif
	if (!(lock->flags & WAKE_LOCK_ACTIVE)) {
		lock->flags |= WAKE_LOCK_ACTIVE;
#ifdef CONFIG_WAKELOCK_STAT
		lock->stat.last_time = ktime_get();
#endif
	}
	list_del(&lock->link);
	if (has_timeout) {
		if (debug_mask & DEBUG_WAKE_LOCK)
			pr_info("wake_lock: %s, type %d, timeout %ld.%03lu\n",
				lock->name, type, timeout / HZ,
				(timeout % HZ) * MSEC_PER_SEC / HZ);
		lock->expires = jiffies + timeout;
		lock->flags |= WAKE_LOCK_AUTO_EXPIRE;
		list_add_tail(&lock->link, &active_wake_locks[type]);
	} else {
		if (debug_mask & DEBUG_WAKE_LOCK)
			pr_info("wake_lock: %s, type %d\n", lock->name, type);
		lock->expires = LONG_MAX;
		lock->flags &= ~WAKE_LOCK_AUTO_EXPIRE;
		list_add(&lock->link, &active_wake_locks[type]);
	}
	if (type == WAKE_LOCK_SUSPEND) {
		current_event_num++;
#ifdef CONFIG_WAKELOCK_STAT
		if (lock == &main_wake_lock)
			update_sleep_wait_stats_locked(1);
		else if (!wake_lock_active(&main_wake_lock))
			update_sleep_wait_stats_locked(0);
#endif
		if (has_timeout)
			expire_in = has_wake_lock_locked(type);
		else
			expire_in = -1;
		if (expire_in > 0) {
			if (debug_mask & DEBUG_EXPIRE)
				pr_info("wake_lock: %s, start expire timer, "
					"%ld\n", lock->name, expire_in);
			mod_timer(&expire_timer, jiffies + expire_in);
		} else {
			if (del_timer(&expire_timer))
				if (debug_mask & DEBUG_EXPIRE)
					pr_info("wake_lock: %s, stop expire timer\n",
						lock->name);
			if (expire_in == 0)
				queue_work(suspend_work_queue, &suspend_work);
		}
	}
	trace_wake_lock(lock);
	spin_unlock_irqrestore(&list_lock, irqflags);
}

void wake_lock(struct wake_lock *lock)
{
	wake_lock_internal(lock, 0, 0);
}
EXPORT_SYMBOL(wake_lock);

void wake_lock_timeout(struct wake_lock *lock, long timeout)
{
	wake_lock_internal(lock, timeout, 1);
}
EXPORT_SYMBOL(wake_lock_timeout);

void wake_unlock(struct wake_lock *lock)
{
	int type;
	unsigned long irqflags;
	spin_lock_irqsave(&list_lock, irqflags);
	trace_wake_unlock(lock);
	type = lock->flags & WAKE_LOCK_TYPE_MASK;
#ifdef CONFIG_WAKELOCK_STAT
	wake_unlock_stat_locked(lock, 0);
#endif
	if (debug_mask & DEBUG_WAKE_LOCK)
		pr_info("wake_unlock: %s\n", lock->name);
	lock->flags &= ~(WAKE_LOCK_ACTIVE | WAKE_LOCK_AUTO_EXPIRE);
	list_del(&lock->link);
	list_add(&lock->link, &inactive_locks);
	if (type == WAKE_LOCK_SUSPEND) {
		long has_lock = has_wake_lock_locked(type);
		if (has_lock > 0) {
			if (debug_mask & DEBUG_EXPIRE)
				pr_info("wake_unlock: %s, start expire timer, "
					"%ld\n", lock->name, has_lock);
			mod_timer(&expire_timer, jiffies + has_lock);
		} else {
			if (del_timer(&expire_timer))
				if (debug_mask & DEBUG_EXPIRE)
					pr_info("wake_unlock: %s, stop expire "
						"timer\n", lock->name);
			if (has_lock == 0)
				queue_work(suspend_work_queue, &suspend_work);
		}
		if (lock == &main_wake_lock) {
			if (debug_mask & DEBUG_SUSPEND)
				print_active_locks(WAKE_LOCK_SUSPEND);
#ifdef CONFIG_WAKELOCK_STAT
			update_sleep_wait_stats_locked(0);
#endif
		}
	}
	spin_unlock_irqrestore(&list_lock, irqflags);
}
EXPORT_SYMBOL(wake_unlock);

int wake_lock_active(struct wake_lock *lock)
{
	return !!(lock->flags & WAKE_LOCK_ACTIVE);
}
EXPORT_SYMBOL(wake_lock_active);

#ifdef CONFIG_WAKELOCK_STAT
static const struct seq_operations wakelock_seq_ops = {
	.start = wakelock_seq_start,
	.next  = wakelock_seq_next,
	.stop  = wakelock_seq_stop,
	.show  = wakelock_seq_show,
};

static int wakelock_stats_open(struct inode *inode, struct file *file)
{
	int ret;
	struct seq_file *m;
	struct wake_lock *lock;

	lock = kmalloc(sizeof(struct wake_lock), GFP_KERNEL);
	if (!lock)
		return -ENOMEM;

	ret = seq_open(file, &wakelock_seq_ops);
	if (ret) {
		kfree(lock);
		return ret;
	}

	m = file->private_data;
	m->private = lock;
	return ret;
}

static int wakelock_stats_release(struct inode *inode, struct file *file)
{
	struct seq_file *m = file->private_data;

	kfree(m->private);
	return seq_release(inode, file);
}

static const struct file_operations wakelock_stats_fops = {
	.owner = THIS_MODULE,
	.open = wakelock_stats_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = wakelock_stats_release,
};
#endif

static int __init wakelocks_init(void)
{
	int ret;
	int i;

	for (i = 0; i < ARRAY_SIZE(active_wake_locks); i++)
		INIT_LIST_HEAD(&active_wake_locks[i]);

#ifdef CONFIG_WAKELOCK_STAT
	wake_lock_init(&deleted_wake_locks, WAKE_LOCK_SUSPEND,
			"deleted_wake_locks");
#endif
	wake_lock_init(&main_wake_lock, WAKE_LOCK_SUSPEND, "main");
	wake_lock(&main_wake_lock);
	wake_lock_init(&unknown_wakeup, WAKE_LOCK_SUSPEND, "unknown_wakeups");
	wake_lock_init(&suspend_backoff_lock, WAKE_LOCK_SUSPEND,
		       "suspend_backoff");
	wake_lock_init(&suspend_sync_wake_lock, WAKE_LOCK_SUSPEND,
		       "suspend_sync");

	ret = platform_device_register(&power_device);
	if (ret) {
		pr_err("wakelocks_init: platform_device_register failed\n");
		goto err_platform_device_register;
	}
	ret = platform_driver_register(&power_driver);
	if (ret) {
		pr_err("wakelocks_init: platform_driver_register failed\n");
		goto err_platform_driver_register;
	}

	suspend_work_queue = create_singlethread_workqueue("suspend");
	if (suspend_work_queue == NULL) {
		ret = -ENOMEM;
		goto err_suspend_work_queue;
	}

	/* Disable VT switch */
	pm_set_vt_switch(0);

#ifdef CONFIG_WAKELOCK_STAT
	proc_create("wakelocks", S_IRUGO, NULL, &wakelock_stats_fops);
#ifdef CONFIG_PM_DEBUG
	register_early_suspend(&power_early_suspend_desc);
	wakeup_time = ktime_get();
	longest = 0;
	longest_background_lock = NULL;
#endif
#endif

	return 0;

err_suspend_work_queue:
	platform_driver_unregister(&power_driver);
err_platform_driver_register:
	platform_device_unregister(&power_device);
err_platform_device_register:
	wake_lock_destroy(&suspend_sync_wake_lock);
	wake_lock_destroy(&suspend_backoff_lock);
	wake_lock_destroy(&unknown_wakeup);
	wake_lock_destroy(&main_wake_lock);
#ifdef CONFIG_WAKELOCK_STAT
	wake_lock_destroy(&deleted_wake_locks);
#endif
	return ret;
}

static void  __exit wakelocks_exit(void)
{
#ifdef CONFIG_WAKELOCK_STAT
	remove_proc_entry("wakelocks", NULL);
#endif
	destroy_workqueue(suspend_work_queue);
	platform_driver_unregister(&power_driver);
	platform_device_unregister(&power_device);
	wake_lock_destroy(&suspend_sync_wake_lock);
	wake_lock_destroy(&suspend_backoff_lock);
	wake_lock_destroy(&unknown_wakeup);
	wake_lock_destroy(&main_wake_lock);
#ifdef CONFIG_WAKELOCK_STAT
#ifdef CONFIG_PM_DEBUG
	unregister_early_suspend(&power_early_suspend_desc);
#endif
	wake_lock_destroy(&deleted_wake_locks);
#endif
	debugfs_backoff_exit();
}

core_initcall(wakelocks_init);
late_initcall(debugfs_backoff_init);

module_exit(wakelocks_exit);
