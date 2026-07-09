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
PLATFORM := ${PLATFORM_REPO_PATHS}/vek280_base/vek280_base.xpfm
DISK_IMAGES := amd-cortexa72-common_edf-platform-disk-image/
YOCTO_QEMU_ARTIFACTS ?=
QEMU_IMAGES	:= amd-cortexa72-common_vek280_qemu_prebuilt
AIE_WORK = $(ROOT_DIR)/aieml_work/
.PHONY: all sd_card clean ultraclean help cpAieCode

# === Top-Level Targets ===

all: check-EDF-common-image aieml_emu
sd_card: check-EDF-common-image aieml_hw

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

# Stage the AIE-ML sample design from the Vitis installation into the work dir.
cpAieCode:
	rm -rf $(AIE_WORK)
	mkdir -p $(AIE_WORK)
	cp $(XILINX_VITIS)/samples/aie_system_examples/aie-ml_sys_design/* $(AIE_WORK) -a
	cp makefile_aieml $(AIE_WORK)/Makefile
	@sed -i 's|int host_ref_32b\[OUTPUT_SIZEC\];|int host_ref_32b[OUTPUT_SIZEC] = {0};|; s|int host_ref_16b\[OUTPUT_SIZEC\];|int host_ref_16b[OUTPUT_SIZEC] = {0};|' $(AIE_WORK)/src/host.cpp

aieml_emu: cpAieCode
	cp ./run_app_hw_emu.sh $(AIE_WORK)/
	$(MAKE) -C $(AIE_WORK) all TARGET=hw_emu PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 SYSROOT=$(SYSROOT)/

aieml_hw: cpAieCode
	$(MAKE) -C $(AIE_WORK) clean
	cp ./embedded_exec.sh $(AIE_WORK)/
	$(MAKE) -C $(AIE_WORK) sd_card TARGET=hw PLATFORM=$(PLATFORM) HOST_ARCH=aarch64 DEV_ARCH=versal SYSROOT=$(SYSROOT)

# === Cleanup ===

clean:
	rm -rf $(AIE_WORK)/_aie
	rm -rf $(AIE_WORK)/_x
	rm -rf $(AIE_WORK)/aieml_system
	rm -rf $(AIE_WORK)/package.hw_emu
	rm -rf $(AIE_WORK)/package.hw

ultraclean: clean
	rm -rf $(AIE_WORK)

# === Help ===

help:
	@echo "================ Vitis EDF Flow Makefile Help ================"
	@echo " make all        : Run AIE-ML HW emulation flow"
	@echo " make sd_card    : Run production hardware flow"
	@echo " make clean      : Clean all intermediate files"
	@echo " make ultraclean : Clean everything including outputs"
	@echo "=============================================================="
