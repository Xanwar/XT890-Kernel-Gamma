# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Hacked up a simple Makefile for the XT890 Kernel, this has a lot of optimized
# flags and went a little extreme on specifying the flags since this is for a 
# remote compile target.
#
#                                               Mar 2013 - Omar Avelar
#

############################################################################
########################## GLOBAL MAKE ARGUMENTS ###########################
############################################################################

export ARCH := i386
ifneq ($(wildcard /usr/bin/gcc),)
  export CC := /usr/bin/gcc
else
  export CROSS_COMPILE := $(PWD)/gcc/i686-linux-android-4.7/bin/i686-linux-android-
endif
export KBUILD_VERBOSE := 0
export LDFLAGS := "-Wl,-O1"

############################################################################
##################### LOCAL SETUP AND FILE STRUCTURES ######################
############################################################################

KVERSION = linux-3.0
KDEFCONFIG = $(ARCH)_mfld_oxavelar_defconfig
KSRC_PATH = $(PWD)/kernel/$(KVERSION)
WL12XX_SRC_PATH = $(PWD)/hardware/ti/wlan/wl12xx-compat
OUT_PATH = $(PWD)/out
KBUILD_OUT_PATH = $(OUT_PATH)/kbuild
MBUILD_OUT_PATH = $(OUT_PATH)/mbuild

############################################################################
############# KERNEL OPTIMIZATION FLAGS FOR THE ATOM Z2480 ################
############################################################################

export Z2480_OPTIMIZATION_FLAGS := \
        -march=core2 \
        -mtune=pentium3 \
        -mx32 \
        -mssse3 \
        -mcx16 \
        -msahf \
        -mmovbe \
        -fomit-frame-pointer \
        --param l1-cache-line-size=64 \
        --param l1-cache-size=24 \
        --param l2-cache-size=512 \

export ANDROID_TOOLCHAIN_FLAGS := \
        -pipe \
        -flto \
        -mno-android \
        -O2 \
        -ftree-vectorize \
        -floop-parallelize-all \
        -ftree-parallelize-loops=2 \
        -floop-block \
        -floop-interchange \
        -floop-strip-mine \
        -fgraphite-identity \
        -ftree-loop-distribution \
        -ftree-loop-linear \
        -ftree-loop-im \
        -ftree-loop-if-convert \
        -ftree-loop-if-convert-stores \
        -foptimize-register-move \
        -fmodulo-sched \
        -fmodulo-sched-allow-regmoves \
        $(Z2480_OPTIMIZATION_FLAGS) \

export MODULES_TOOLCHAIN_FLAGS := \
        -O2 \
        -ftree-vectorize \
        -floop-block \
        -floop-interchange \
        -floop-strip-mine \
        -fgraphite-identity \
        -ftree-loop-distribution \
        -ftree-loop-linear \
        -ftree-loop-im \
        -ftree-loop-if-convert \
        -ftree-loop-if-convert-stores \
        -foptimize-register-move \
        -fmodulo-sched \
        -fmodulo-sched-allow-regmoves \
        $(Z2480_OPTIMIZATION_FLAGS) \

ifeq ($(KVERSION), linux-3.0)
    # The following modules have problems with -ftree-vectorize
    # and if removed will get battery reading errors
    export CFLAGS_platform_max17042.o       := -fno-tree-vectorize
    export CFLAGS_max17042_battery.o        := -fno-tree-vectorize
    export CFLAGS_intel_mdf_battery.o       := -fno-tree-vectorize
endif


############################################################################
########################### KERNEL BUILD STEPS #############################
############################################################################

BOOT_CMDLINE="init=/init pci=noearly console=logk0 vmalloc=272M earlyprintk=nologger hsu_dma=7 kmemleak=off androidboot.bootmedia=sdcard androidboot.hardware=sc1 emmc_ipanic.ipanic_part_number=6 slub_max_order=2 zram.num_devices=2 loglevel=4"

.PHONY: bootimage
bootimage: kernel modules
	rm -fR "/tmp/smi-ramdisk"
	cp -R "$(PWD)/root" "/tmp/smi-ramdisk"
	mkdir -p "/tmp/smi-ramdisk/lib/modules"
	# Copy the created modules to the ramdisk path and strip debug symbols
	find "$(MBUILD_OUT_PATH)" -iname *.ko -exec cp -f \{\} "/tmp/smi-ramdisk/lib/modules/" \;
	find "$(WL12XX_SRC_PATH)" -iname *.ko -exec cp -f \{\} "/tmp/smi-ramdisk/lib/modules/" \;
	strip --strip-debug --strip-unneeded --remove-section=.comment --remove-section=.note /tmp/smi-ramdisk/lib/modules/*.*o
	"$(PWD)/tools/pack-ramdisk" "/tmp/smi-ramdisk"
	mv "/tmp/ramdisk.cpio.gz" "$(OUT_PATH)/ramdisk.cpio.gz"
	# Pack the boot.img
	"$(PWD)/tools/mkbootimg" --kernel "$(OUT_PATH)/kernel" \
	 --ramdisk "$(OUT_PATH)/ramdisk.cpio.gz"               \
	 --cmdline $(BOOT_CMDLINE) --output "$(OUT_PATH)"

.PHONY: kernel
kernel:
	mkdir -p "$(KBUILD_OUT_PATH)"
	$(MAKE) -C "$(KSRC_PATH)" O="$(KBUILD_OUT_PATH)" "$(KDEFCONFIG)"
	$(MAKE) -C "$(KSRC_PATH)" O="$(KBUILD_OUT_PATH)" bzImage
	cp "$(KBUILD_OUT_PATH)/arch/x86/boot/bzImage" "$(OUT_PATH)/kernel"

.PHONY: modules
modules:
	# General modules from the kernel
	mkdir -p "$(MBUILD_OUT_PATH)"
	$(MAKE) -C "$(KSRC_PATH)" O="$(MBUILD_OUT_PATH)" "$(KDEFCONFIG)"
	$(MAKE) -C "$(KSRC_PATH)" O="$(MBUILD_OUT_PATH)" ANDROID_TOOLCHAIN_FLAGS="$(MODULES_TOOLCHAIN_FLAGS)" modules
	# Wireless modules
	cd "$(WL12XX_SRC_PATH)"; scripts/driver-select wl12xx
	$(MAKE) -C "$(WL12XX_SRC_PATH)" KLIB="$(MBUILD_OUT_PATH)" KLIB_BUILD="$(MBUILD_OUT_PATH)" ANDROID_TOOLCHAIN_FLAGS="$(MODULES_TOOLCHAIN_FLAGS)"

.PHONY: clean
clean:
	$(MAKE) -C "$(WL12XX_SRC_PATH)" clean
	$(MAKE) -C "$(KSRC_PATH)" mrproper
	rm -rf "$(OUT_PATH)"

