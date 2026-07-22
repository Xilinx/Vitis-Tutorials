# Copyright (C) 2026, Advanced Micro Devices, Inc.
# All rights reserved.
# SPDX-License-Identifier: MIT

ROOT_DIR := $(shell pwd)
TARGET=hw_emu
XSA := $(ROOT_DIR)/step1_vivado/build/vivado/project_1/vek385_$(TARGET).xsa
FIXED_XSA_EMU := step2_vitis_integration/ys_gm2aie_hw_emu.xsa
FIXED_XSA_HW := step2_vitis_integration/ys_gm2aie_hw.xsa
YOCTO_ARTIFACTS ?= 
DISK_IMAGES := amd-cortexa78-mali-common_edf-platform-disk-image/
YOCTO_QEMU_ARTIFACTS ?=
QEMU_IMAGES	:= amd-cortexa78-mali-common_vek385_qemu_prebuilt
.PHONY: all sd_card clean ultraclean help

# === Top-Level Targets ===

all: check-EDF-common-image step1 step2_hwemu step3_hwemu
sd_card: check-EDF-common-image step1 step2_hw step3_hw

# === Checks ===

#target : check  whether the EDF common image is ready, *.wic.ufs image is for hardware emulation 
#IMAGE1 flag is used for internal users.
#IMAGE2 flag is used for external users.
check-EDF-common-image:
	@IMAGE1="$(YOCTO_ARTIFACTS)/$(DISK_IMAGES)/edf-platform-disk-image-amd-cortexa78-mali-common.rootfs.wic.ufs"; \
	IMAGE2="$(YOCTO_QEMU_ARTIFACTS)/edf-platform-disk-image-amd-cortexa78-mali-common.rootfs.wic.ufs"; \
	if [ ! -f "$$IMAGE1" ] && [ ! -f "$$IMAGE2" ]; then \
	    echo "Error: EDF image not found in either:"; \
	    echo "  - $$IMAGE1"; \
	    echo "  - $$IMAGE2"; \
	    echo "Please contact your FAE or download it from the secure site."; \
	    exit 1; \
	fi; \
	echo "EDF common image exists. Proceeding..."
# === Step 1: Generate Vivado XSA ===

step1: $(XSA)

$(XSA):
	$(MAKE) -C step1_vivado all

# === Step 2: Use Vitis integrate components to Generate fixed XSAs ===

step2_hwemu: $(FIXED_XSA_EMU)
step2_hw: $(FIXED_XSA_HW)

$(FIXED_XSA_EMU): $(XSA)
	$(MAKE) -C step2_vitis_integration all TARGET-=hw_emu

$(FIXED_XSA_HW): $(XSA)
	$(MAKE) -C step2_vitis_integration sd_card TARGET=hw

# === Step 3: Build and validate application ===

step3_hwemu: $(FIXED_XSA_EMU)
	$(MAKE) -C step3_application all target=hw_emu YOCTO_QEMU_ARTIFACTS=$(abspath $(YOCTO_QEMU_ARTIFACTS))

step3_hw: $(FIXED_XSA_HW)
	$(MAKE) -C step3_application sd_card TARGET=hw

# === Cleanup ===

clean:
	$(MAKE) -C step1_vivado clean
	$(MAKE) -C step2_vitis_integration clean
	$(MAKE) -C step3_application clean

ultraclean: clean
	$(MAKE) -C step2_vitis_integration ultraclean
	$(MAKE) -C step3_application ultraclean

# === Help ===

help:
	@echo "================ Vitis Flow Makefile Help ================"
	@echo " make all        : Run HW emulation flow"
	@echo " make sd_card    : Run production hardware flow"
	@echo " make clean      : Clean all intermediate files"
	@echo " make ultraclean : Clean everything including outputs"
	@echo "=========================================================="
