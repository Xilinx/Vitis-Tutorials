# /*
# Copyright (C) 2026, Advanced Micro Devices, Inc.
# All rights reserved.
# SPDX-License-Identifier: MIT
# */

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
# Set YOCTO_ARTIFACTS with local artifacts downloaded path
YOCTO_ARTIFACTS ?=
SYSROOT := $(YOCTO_ARTIFACTS)/amd-cortexa72-common_meta-edf-app-sdk/sdk/sysroots/cortexa72-cortexa53-amd-linux/
PLATFORM := ${PLATFORM_REPO_PATHS}/vck190_base/vck190_base.xpfm
DISK_IMAGES := amd-cortexa72-common_edf-platform-disk-image/
YOCTO_QEMU_ARTIFACTS ?=
QEMU_IMAGES	:= amd-cortexa72-common_vck190_qemu_prebuilt
AIE_WORK = $(ROOT_DIR)/vck190_work/
# Tutorial sources live in the PetaLinux sibling — same design, different flow.
PETALINUX_SRC := $(abspath $(ROOT_DIR)/../../Versal_w_PetaLinux/VCK190)
.PHONY: all sd_card clean ultraclean help cpSources

# === Top-Level Targets ===

all: check-EDF-common-image vck190_emu
sd_card: check-EDF-common-image vck190_hw

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

# Copy tutorial source files from PetaLinux sibling into the work directory.
# This avoids duplicating sources in the repo while keeping the build self-contained.
cpSources:
	cp -r $(PETALINUX_SRC)/AIE_Kernel $(AIE_WORK)/
	mkdir -p $(AIE_WORK)/vitis_dir
	cp $(PETALINUX_SRC)/vitis_dir/system.cfg $(AIE_WORK)/vitis_dir/

vck190_emu:
	rm -rf vck190_work
	mkdir vck190_work
	cp makefile_vck190 $(AIE_WORK)/Makefile
	cp ./run_app_hw_emu.sh $(AIE_WORK)/
	$(MAKE) cpSources
	$(MAKE) -C $(AIE_WORK) all TARGET=hw_emu PLATFORM=$(PLATFORM) SYSROOT=$(SYSROOT)/

vck190_hw:
	rm -rf vck190_work
	mkdir vck190_work
	cp makefile_vck190 $(AIE_WORK)/Makefile
	cp ./embedded_exec.sh $(AIE_WORK)/
	$(MAKE) cpSources
	$(MAKE) -C $(AIE_WORK) clean
	$(MAKE) -C $(AIE_WORK) sd_card TARGET=hw PLATFORM=$(PLATFORM) SYSROOT=$(SYSROOT)

# === Cleanup ===

clean:
	rm -rf vck190_work/_aie
	rm -rf vck190_work/_x
	rm -rf vck190_work/application
	rm -rf vck190_work/package.hw_emu
	rm -rf vck190_work/package.hw

ultraclean: clean
	rm -rf vck190_work

# === Help ===

help:
	@echo "================ Vitis EDF Flow Makefile Help ================"
	@echo " make all        : Run HW emulation flow"
	@echo " make sd_card    : Run production hardware flow"
	@echo " make clean      : Clean all intermediate files"
	@echo " make ultraclean : Clean everything including outputs"
	@echo "=============================================================="
