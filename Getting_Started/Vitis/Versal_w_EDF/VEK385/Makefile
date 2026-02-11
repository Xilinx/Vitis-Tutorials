# /*
# Copyright (C) 2025, Advanced Micro Devices, Inc.
# All rights reserved.
# SPDX-License-Identifier: MIT
# */

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
# Set YOCTO_ARTIFACTS with local artifacts downloaded path
YOCTO_ARTIFACTS := ${YOCTO_ARTIFACTS}
SYSROOT := ${YOCTO_ARTIFACTS}/sysroots/cortexa72-cortexa53-amd-linux
PLATFORM := ${PLATFORM_REPO_PATHS}/vek385_base_reva/vek385_base_reva.xpfm

DISK_IMAGES := amd-cortexa78-mali-common_edf-linux-disk-image/
YOCTO_QEMU_ARTIFACTS := 
QEMU_IMAGES	:= amd-cortexa78-mali-common_vek385_qemu_prebuilt
AIE_WORK = $(ROOT_DIR)/aieml_work/
.PHONY: all sd_card clean ultraclean help

# === Top-Level Targets ===

all: check-EDF-common-image aieml_emu
sd_card: check-EDF-common-image aieml_hw

# === Checks ===

#target : check  whether the EDF common image is ready, *.wic.ufs image is for hardware emulation 
#IMAGE1 flag is used for internal users.
#IMAGE2 flag is used for external users.
check-EDF-common-image:
	@IMAGE1="$(YOCTO_ARTIFACTS)/$(DISK_IMAGES)/edf-linux-disk-image-amd-cortexa78-mali-common.rootfs.wic.ufs"; \
	IMAGE2="$(YOCTO_QEMU_ARTIFACTS)/edf-linux-disk-image-amd-cortexa78-mali-common.rootfs.wic.ufs"; \
	if [ ! -f "$$IMAGE1" ] && [ ! -f "$$IMAGE2" ]; then \
	    echo "Error: EDF image not found in either:"; \
	    echo "  - $$IMAGE1"; \
	    echo "  - $$IMAGE2"; \
	    echo "Please contact your FAE or download it from the secure site."; \
	    exit 1; \
	fi; \
	echo "EDF common image exists. Proceeding..."

aieml_emu:
	rm -rf aieml_work
	mkdir aieml_work	
	cp makefile_aieml $(AIE_WORK)/Makefile
	cp ./run_app_hw_emu.sh $(AIE_WORK)/
	$(MAKE) -C $(AIE_WORK) all TARGET=hw_emu PLATFORM=$(PLATFORM) EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/

aieml_hw:
	rm -rf aieml_work
	mkdir aieml_work	
	cp makefile_aieml $(AIE_WORK)/Makefile
	cp ./embedded_exec.sh $(AIE_WORK)/
	$(MAKE) -C $(AIE_WORK) clean
	$(MAKE) -C $(AIE_WORK) sd_card TARGET=hw PLATFORM=$(PLATFORM) EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)

# === Cleanup ===

clean:
	rm -rf aieml_work/_aie 
	rm -rf aieml_work/_x 
	rm -rf aieml_work/application
	rm -rf aieml_work/package.hw_emu
	rm -rf aieml_work/package.hw

ultraclean: clean
	rm -rf aieml_work

# === Help ===

help:
	@echo "================ Vitis Flow Makefile Help ================"
	@echo " make all        : Run HW emulation flow"
	@echo " make sd_card    : Run production hardware flow"
	@echo " make clean      : Clean all intermediate files"
	@echo " make ultraclean : Clean everything including outputs"
	@echo "=========================================================="
