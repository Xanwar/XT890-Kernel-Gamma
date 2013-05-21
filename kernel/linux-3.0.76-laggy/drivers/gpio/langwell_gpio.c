/* langwell_gpio.c Moorestown platform Langwell chip GPIO driver
 * Copyright (c) 2008 - 2009,  Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* Supports:
 * Moorestown platform Langwell chip.
 * Medfield platform Penwell chip.
 * Whitney point.
 */

#include <linux/module.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/stddef.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/lnw_gpio.h>
#include <linux/pm_runtime.h>

/*
 * Langwell chip has 64 pins and thus there are 2 32bit registers to control
 * each feature, while Penwell chip has 96 pins for each block, and need 3 32bit
 * registers to control them, so we only define the order here instead of a
 * structure, to get a bit offset for a pin (use GPDR as an example):
 *
 * nreg = ngpio / 32;
 * reg = offset / 32;
 * bit = offset % 32;
 * reg_addr = reg_base + GPDR * nreg * 4 + reg * 4;
 *
 * so the bit of reg_addr is to control pin offset's GPDR feature
*/

enum GPIO_REG {
	GPLR = 0,	/* pin level read-only */
	GPDR,		/* pin direction */
	GPSR,		/* pin set */
	GPCR,		/* pin clear */
	GRER,		/* rising edge detect */
	GFER,		/* falling edge detect */
	GEDR,		/* edge detect result */
	GAFR,		/* alt function */
};

struct lnw_gpio {
	struct gpio_chip		chip;
	void				*reg_base;
	spinlock_t			lock;
	unsigned			irq_base;
	int				wakeup;
	struct pci_dev			*pdev;
};

static void __iomem *gpio_reg(struct gpio_chip *chip, unsigned offset,
			enum GPIO_REG reg_type)
{
	struct lnw_gpio *lnw = container_of(chip, struct lnw_gpio, chip);
	unsigned nreg = chip->ngpio / 32;
	u8 reg = offset / 32;
	void __iomem *ptr;

	ptr = (void __iomem *)(lnw->reg_base + reg_type * nreg * 4 + reg * 4);
	return ptr;
}

void lnw_gpio_set_alt(int gpio, int alt)
{
	struct lnw_gpio *lnw;
	u32 __iomem *mem;
	int reg;
	int bit;
	u32 value;
	unsigned long flags;

	/* use this trick to get memio */
	lnw = irq_get_chip_data(gpio_to_irq(gpio));
	if (!lnw) {
		pr_err("langwell_gpio: can not find pin %d\n", gpio);
		return;
	}
	if (gpio < lnw->chip.base || gpio >= lnw->chip.base + lnw->chip.ngpio) {
		dev_err(lnw->chip.dev, "langwell_gpio: wrong pin %d to config alt\n", gpio);
		return;
	}
	if (lnw->irq_base + gpio - lnw->chip.base != gpio_to_irq(gpio)) {
		dev_err(lnw->chip.dev, "langwell_gpio: wrong chip data for pin %d\n", gpio);
		return;
	}
	gpio -= lnw->chip.base;
	reg = gpio / 16;
	bit = gpio % 16;

	mem = gpio_reg(&lnw->chip, 0, GAFR);
	spin_lock_irqsave(&lnw->lock, flags);
	value = readl(mem + reg);
	value &= ~(3 << (bit * 2));
	value |= (alt & 3) << (bit * 2);
	writel(value, mem + reg);
	spin_unlock_irqrestore(&lnw->lock, flags);
	dev_dbg(lnw->chip.dev, "ALT: writing 0x%x to %p\n", value, mem + reg);
}
EXPORT_SYMBOL_GPL(lnw_gpio_set_alt);

static int lnw_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	void __iomem *gplr = gpio_reg(chip, offset, GPLR);

	return readl(gplr) & BIT(offset % 32);
}

static void lnw_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	void __iomem *gpsr, *gpcr;

	if (value) {
		gpsr = gpio_reg(chip, offset, GPSR);
		writel(BIT(offset % 32), gpsr);
	} else {
		gpcr = gpio_reg(chip, offset, GPCR);
		writel(BIT(offset % 32), gpcr);
	}
}

static int lnw_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
	struct lnw_gpio *lnw = container_of(chip, struct lnw_gpio, chip);
	void __iomem *gpdr = gpio_reg(chip, offset, GPDR);
	u32 value;
	unsigned long flags;

	if (lnw->pdev)
		pm_runtime_get(&lnw->pdev->dev);

	spin_lock_irqsave(&lnw->lock, flags);
	value = readl(gpdr);
	value &= ~BIT(offset % 32);
	writel(value, gpdr);
	spin_unlock_irqrestore(&lnw->lock, flags);

	if (lnw->pdev)
		pm_runtime_put(&lnw->pdev->dev);

	return 0;
}

static int lnw_gpio_direction_output(struct gpio_chip *chip,
			unsigned offset, int value)
{
	struct lnw_gpio *lnw = container_of(chip, struct lnw_gpio, chip);
	void __iomem *gpdr = gpio_reg(chip, offset, GPDR);
	unsigned long flags;

	lnw_gpio_set(chip, offset, value);

	if (lnw->pdev)
		pm_runtime_get(&lnw->pdev->dev);

	spin_lock_irqsave(&lnw->lock, flags);
	value = readl(gpdr);
	value |= BIT(offset % 32);
	writel(value, gpdr);
	spin_unlock_irqrestore(&lnw->lock, flags);

	if (lnw->pdev)
		pm_runtime_put(&lnw->pdev->dev);

	return 0;
}

static int lnw_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	struct lnw_gpio *lnw = container_of(chip, struct lnw_gpio, chip);
	return lnw->irq_base + offset;
}

static int lnw_irq_type(struct irq_data *d, unsigned type)
{
	struct lnw_gpio *lnw = irq_data_get_irq_chip_data(d);
	u32 gpio = d->irq - lnw->irq_base;
	unsigned long flags;
	u32 value;
	void __iomem *grer = gpio_reg(&lnw->chip, gpio, GRER);
	void __iomem *gfer = gpio_reg(&lnw->chip, gpio, GFER);

	if (gpio >= lnw->chip.ngpio)
		return -EINVAL;

	if (lnw->pdev)
		pm_runtime_get(&lnw->pdev->dev);

	spin_lock_irqsave(&lnw->lock, flags);
	if (type & IRQ_TYPE_EDGE_RISING)
		value = readl(grer) | BIT(gpio % 32);
	else
		value = readl(grer) & (~BIT(gpio % 32));
	writel(value, grer);

	if (type & IRQ_TYPE_EDGE_FALLING)
		value = readl(gfer) | BIT(gpio % 32);
	else
		value = readl(gfer) & (~BIT(gpio % 32));
	writel(value, gfer);
	spin_unlock_irqrestore(&lnw->lock, flags);

	if (lnw->pdev)
		pm_runtime_put(&lnw->pdev->dev);

	return 0;
}

static void lnw_irq_unmask(struct irq_data *d)
{
}

static void lnw_irq_mask(struct irq_data *d)
{
}

static int lnw_irq_wake(struct irq_data *d, unsigned on)
{
	return 0;
}

static void lnw_irq_ack(struct irq_data *d)
{
}

static void lnw_irq_shutdown(struct irq_data *d)
{
	struct lnw_gpio *lnw = irq_data_get_irq_chip_data(d);
	u32 gpio = d->irq - lnw->irq_base;
	unsigned long flags;
	u32 value;
	void __iomem *grer = gpio_reg(&lnw->chip, gpio, GRER);
	void __iomem *gfer = gpio_reg(&lnw->chip, gpio, GFER);

	spin_lock_irqsave(&lnw->lock, flags);
	value = readl(grer) & (~BIT(gpio % 32));
	writel(value, grer);
	value = readl(gfer) & (~BIT(gpio % 32));
	writel(value, gfer);
	spin_unlock_irqrestore(&lnw->lock, flags);
};

static struct irq_chip lnw_irqchip = {
	.name		= "LNW-GPIO",
	.irq_mask	= lnw_irq_mask,
	.irq_unmask	= lnw_irq_unmask,
	.irq_set_type	= lnw_irq_type,
	.irq_set_wake	= lnw_irq_wake,
	.irq_ack	= lnw_irq_ack,
	.irq_shutdown	= lnw_irq_shutdown,
};

static DEFINE_PCI_DEVICE_TABLE(lnw_gpio_ids) = {   /* pin number */
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x080f), .driver_data = 64 },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x081f), .driver_data = 96 },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x081a), .driver_data = 96 },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x08eb), .driver_data = 96 },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x08f7), .driver_data = 96 },
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, lnw_gpio_ids);

static void lnw_irq_handler(unsigned irq, struct irq_desc *desc)
{
	struct irq_data *data = irq_desc_get_irq_data(desc);
	struct lnw_gpio *lnw = irq_data_get_irq_handler_data(data);
	struct irq_chip *chip = irq_data_get_irq_chip(data);
	u32 base, gpio, mask;
	unsigned long pending;
	void __iomem *gedr;

	/* check GPIO controller to check which pin triggered the interrupt */
	for (base = 0; base < lnw->chip.ngpio; base += 32) {
		gedr = gpio_reg(&lnw->chip, base, GEDR);
		pending = readl(gedr);
		while (pending) {
			gpio = __ffs(pending);
			if (!lnw->wakeup)
				dev_info(&lnw->pdev->dev,
				"wakeup source: gpio %d\n", base + gpio);
			mask = BIT(gpio);
			pending &= ~mask;
			/* Clear before handling so we can't lose an edge */
			writel(mask, gedr);
			generic_handle_irq(lnw->irq_base + base + gpio);
		}
	}

	chip->irq_eoi(data);
}

#ifdef CONFIG_PM
static int lnw_gpio_suspend_noirq(struct device *dev)
{
	struct pci_dev *pdev = container_of(dev, struct pci_dev, dev);
	struct lnw_gpio *lnw = pci_get_drvdata(pdev);

	lnw->wakeup = 0;

	return 0;
}

static int lnw_gpio_resume_noirq(struct device *dev)
{
	struct pci_dev *pdev = container_of(dev, struct pci_dev, dev);
	struct lnw_gpio *lnw = pci_get_drvdata(pdev);

	lnw->wakeup = 1;

	return 0;
}

static int lnw_gpio_runtime_resume(struct device *dev)
{
	return 0;
}

static int lnw_gpio_runtime_suspend(struct device *dev)
{
	return 0;
}

static int lnw_gpio_runtime_idle(struct device *dev)
{
	int err = pm_schedule_suspend(dev, 500);

	if (!err)
		return 0;

	return -EBUSY;
}

#else
#define lnw_gpio_suspend_noirq		NULL
#define lnw_gpio_resume_noirq		NULL
#define lnw_gpio_runtime_suspend	NULL
#define lnw_gpio_runtime_resume		NULL
#define lnw_gpio_runtime_idle		NULL
#endif

static const struct dev_pm_ops lnw_gpio_pm_ops = {
	.suspend_noirq = lnw_gpio_suspend_noirq,
	.resume_noirq = lnw_gpio_resume_noirq,
	.runtime_suspend = lnw_gpio_runtime_suspend,
	.runtime_resume = lnw_gpio_runtime_resume,
	.runtime_idle = lnw_gpio_runtime_idle,
};

static int __devinit lnw_gpio_probe(struct pci_dev *pdev,
			const struct pci_device_id *id)
{
	void *base;
	int i;
	resource_size_t start, len;
	struct lnw_gpio *lnw;
	u32 irq_base;
	u32 gpio_base;
	int retval = 0;

	retval = pci_enable_device(pdev);
	if (retval)
		goto done;

	retval = pci_request_regions(pdev, "langwell_gpio");
	if (retval) {
		dev_err(&pdev->dev, "error requesting resources\n");
		goto err2;
	}
	/* get the irq_base from bar1 */
	start = pci_resource_start(pdev, 1);
	len = pci_resource_len(pdev, 1);
	base = ioremap_nocache(start, len);
	if (!base) {
		dev_err(&pdev->dev, "error mapping bar1\n");
		goto err3;
	}
	irq_base = *(u32 *)base;
	gpio_base = *((u32 *)base + 1);
	/* release the IO mapping, since we already get the info from bar1 */
	iounmap(base);
	/* get the register base from bar0 */
	start = pci_resource_start(pdev, 0);
	len = pci_resource_len(pdev, 0);
	base = ioremap_nocache(start, len);
	if (!base) {
		dev_err(&pdev->dev, "error mapping bar0\n");
		retval = -EFAULT;
		goto err3;
	}

	lnw = kzalloc(sizeof(struct lnw_gpio), GFP_KERNEL);
	if (!lnw) {
		dev_err(&pdev->dev, "can't allocate langwell_gpio chip data\n");
		retval = -ENOMEM;
		goto err4;
	}
	lnw->reg_base = base;
	lnw->irq_base = irq_base;
	lnw->wakeup = 1;
	lnw->chip.label = dev_name(&pdev->dev);
	lnw->chip.direction_input = lnw_gpio_direction_input;
	lnw->chip.direction_output = lnw_gpio_direction_output;
	lnw->chip.get = lnw_gpio_get;
	lnw->chip.set = lnw_gpio_set;
	lnw->chip.to_irq = lnw_gpio_to_irq;
	lnw->chip.base = gpio_base;
	lnw->chip.ngpio = id->driver_data;
	lnw->chip.can_sleep = 0;
	lnw->pdev = pdev;
	pci_set_drvdata(pdev, lnw);
	retval = gpiochip_add(&lnw->chip);
	if (retval) {
		dev_err(&pdev->dev, "langwell gpiochip_add error %d\n", retval);
		goto err5;
	}
	lnw->irq_base = irq_alloc_descs(-1, lnw->irq_base, lnw->chip.ngpio,
					NUMA_NO_NODE);
	if (lnw->irq_base < 0) {
		dev_err(&pdev->dev, "langwell irq_alloc_desc failed %d\n",
			lnw->irq_base);
		goto err5;
	}
	irq_set_handler_data(pdev->irq, lnw);
	irq_set_chained_handler(pdev->irq, lnw_irq_handler);
	for (i = 0; i < lnw->chip.ngpio; i++) {
		irq_set_chip_and_handler_name(i + lnw->irq_base, &lnw_irqchip,
					      handle_edge_irq, "demux");
		irq_set_chip_data(i + lnw->irq_base, lnw);
	}

	spin_lock_init(&lnw->lock);

	pm_runtime_put_noidle(&pdev->dev);
	pm_runtime_allow(&pdev->dev);

	goto done;
err5:
	kfree(lnw);
err4:
	iounmap(base);
err3:
	pci_release_regions(pdev);
err2:
	pci_disable_device(pdev);
done:
	return retval;
}

static struct pci_driver lnw_gpio_driver = {
	.name		= "langwell_gpio",
	.id_table	= lnw_gpio_ids,
	.probe		= lnw_gpio_probe,
	.driver		= {
		.pm	= &lnw_gpio_pm_ops,
	},
};


static int __devinit wp_gpio_probe(struct platform_device *pdev)
{
	struct lnw_gpio *lnw;
	struct gpio_chip *gc;
	struct resource *rc;
	int retval = 0;

	rc = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!rc)
		return -EINVAL;

	lnw = kzalloc(sizeof(struct lnw_gpio), GFP_KERNEL);
	if (!lnw) {
		dev_err(&pdev->dev,
			"can't allocate whitneypoint_gpio chip data\n");
		return -ENOMEM;
	}
	lnw->reg_base = ioremap_nocache(rc->start, resource_size(rc));
	if (lnw->reg_base == NULL) {
		retval = -EINVAL;
		goto err_kmalloc;
	}
	spin_lock_init(&lnw->lock);
	gc = &lnw->chip;
	gc->label = dev_name(&pdev->dev);
	gc->owner = THIS_MODULE;
	gc->direction_input = lnw_gpio_direction_input;
	gc->direction_output = lnw_gpio_direction_output;
	gc->get = lnw_gpio_get;
	gc->set = lnw_gpio_set;
	gc->to_irq = NULL;
	gc->base = 0;
	gc->ngpio = 64;
	gc->can_sleep = 0;
	retval = gpiochip_add(gc);
	if (retval) {
		dev_err(&pdev->dev, "whitneypoint gpiochip_add error %d\n",
								retval);
		goto err_ioremap;
	}
	platform_set_drvdata(pdev, lnw);
	return 0;
err_ioremap:
	iounmap(lnw->reg_base);
err_kmalloc:
	kfree(lnw);
	return retval;
}

static int __devexit wp_gpio_remove(struct platform_device *pdev)
{
	struct lnw_gpio *lnw = platform_get_drvdata(pdev);
	int err;
	err = gpiochip_remove(&lnw->chip);
	if (err)
		dev_err(&pdev->dev, "failed to remove gpio_chip.\n");
	iounmap(lnw->reg_base);
	kfree(lnw);
	platform_set_drvdata(pdev, NULL);
	return 0;
}

static struct platform_driver wp_gpio_driver = {
	.probe		= wp_gpio_probe,
	.remove		= __devexit_p(wp_gpio_remove),
	.driver		= {
		.name	= "wp_gpio",
		.owner	= THIS_MODULE,
	},
};

static int __init lnw_gpio_init(void)
{
	int ret;
	ret =  pci_register_driver(&lnw_gpio_driver);
	if (ret < 0)
		return ret;
	ret = platform_driver_register(&wp_gpio_driver);
	if (ret < 0)
		pci_unregister_driver(&lnw_gpio_driver);
	return ret;
}

fs_initcall(lnw_gpio_init);
