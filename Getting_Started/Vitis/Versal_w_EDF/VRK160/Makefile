# /*
# Copyright (C) 2026, Advanced Micro Devices, Inc.
# All rights reserved.
# SPDX-License-Identifier: MIT
# */

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
# Set YOCTO_ARTIFACTS with local artifacts downloaded path
YOCTO_ARTIFACTS ?=
SYSROOT := $(YOCTO_ARTIFACTS)/amd-cortexa72-common_meta-edf-app-sdk/sdk/sysroots/cortexa72-cortexa53-amd-linux/
PLATFORM := ${PLATFORM_REPO_PATHS}/vrk160_base/vrk160_base.xpfm
DISK_IMAGES := amd-cortexa72-common_edf-platform-disk-image/
YOCTO_QEMU_ARTIFACTS ?=
QEMU_IMAGES	:= amd-cortexa72-common_vrk160_qemu_prebuilt
AIE_WORK = $(ROOT_DIR)/vrk160_work/
.PHONY: all sd_card clean ultraclean help

# === Top-Level Targets ===

all: check-EDF-common-image vrk160_emu
sd_card: check-EDF-common-image vrk160_hw

# === Checks ===

#target : check  whether the EDF common image is ready, *.wic.qemu-sd image is for hardware emulation
#IMAGE1 flag is used for internal users.
#IMAGE2 flag is used for external users.
check-EDF-common-image:
	@IMAGE1="$(YOCTO_ARTIFACTS)/$(DISK_IMAGES)/edf-platform-disk-image-amd-cortexa72-common.rootfs.wic.qemu-sd"; \
	IMAGE2="$(YOCTO_QEMU_ARTIFACTS)/edf-platform-disk-image-amd-cortexa72-common.rootfs.wic.qemu-sd"; \
	if [ ! -f "$$IMAGE1" ] && [ ! -f "$$IMAGE2" ]; then \
	    echo "Error: EDF image not found in either:"; \
	    echo "  - $$IMAGE1"; \
	    echo "  - $$IMAGE2"; \
	    echo "Please contact your FAE or download it from the secure site."; \
	    exit 1; \
	fi; \
	echo "EDF common image exists. Proceeding..."

# Stage the inner Makefile + runtime script into vrk160_work/ and dispatch
# the actual build/run from there.  Design sources live in aie/ at the
# tutorial root and are referenced by the inner Makefile via ../aie/
# relative paths (mirrors the VEK385 / aie2ps/ layout).
vrk160_emu:
	rm -rf $(AIE_WORK)
	mkdir -p $(AIE_WORK)
	cp makefile_vrk160 $(AIE_WORK)/Makefile
	cp ./run_app_hw_emu.sh $(AIE_WORK)/
	$(MAKE) -C $(AIE_WORK) all TARGET=hw_emu PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 SYSROOT=$(SYSROOT)/

vrk160_hw:
	rm -rf $(AIE_WORK)
	mkdir -p $(AIE_WORK)
	cp makefile_vrk160 $(AIE_WORK)/Makefile
	cp ./embedded_exec.sh $(AIE_WORK)/
	$(MAKE) -C $(AIE_WORK) sd_card TARGET=hw PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=versal SYSROOT=$(SYSROOT)

# === Cleanup ===

clean:
	rm -rf $(AIE_WORK)/_aie
	rm -rf $(AIE_WORK)/_x
	rm -rf $(AIE_WORK)/Work
	rm -rf $(AIE_WORK)/package.hw_emu
	rm -rf $(AIE_WORK)/package.hw

ultraclean: clean
	rm -rf $(AIE_WORK)

# === Help ===

help:
	@echo "================ Vitis EDF Flow Makefile Help ================"
	@echo " make all        : Run VRK160 HW emulation flow"
	@echo " make sd_card    : Run production hardware flow"
	@echo " make clean      : Clean all intermediate files"
	@echo " make ultraclean : Clean everything including outputs"
	@echo "=============================================================="
