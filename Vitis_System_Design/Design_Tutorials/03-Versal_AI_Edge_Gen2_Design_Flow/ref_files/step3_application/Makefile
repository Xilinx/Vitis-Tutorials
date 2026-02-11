# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# =============================================================================
# Project Configuration
# =============================================================================
ROOT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
VERSION := 2025.2
TARGET = hw_emu
MODE := hw_emu

# =============================================================================
# Toolchain Configuration
# =============================================================================

SYSROOT := $(ROOT_DIR)/sysroots/cortexa72-cortexa53-amd-linux/

# =============================================================================
# Platform Configuration
# =============================================================================
FIXED_XSA_PATH := $(ROOT_DIR)/../step2_vitis_integration/
FIXED_XSA_NAME := sys_gm2aie
PLATFORM_NAME := vek385_fixed_platform_$(TARGET)
PLATFORM := $(ROOT_DIR)/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
DTBO := $(ROOT_DIR)/$(PLATFORM_NAME)/hw/linux_dt/pl.dtbo

# =============================================================================
# Application Configuration
# =============================================================================
ELF := $(ROOT_DIR)/application/build/application
HOST_SRC := $(ROOT_DIR)/../step2_vitis_integration/_aie/ps/c_rts/aie_control_xrt.cpp host_src/host.cpp

# =============================================================================
# QEMU Configuration
# =============================================================================
export QEMU_COMBINED := base_camp_images
export COMBINED := combined
export WIC_PARTITION := edf-linux-disk-image-amd-cortexa78-mali-common.rootfs.wic.ufs:2

YOCTO_ARTIFACTS :=
DISK_IMAGES := amd-cortexa78-mali-common_edf-linux-disk-image
QEMU_IMAGES	:= amd-cortexa78-mali-common_vek385_qemu_prebuilt
YOCTO_QEMU_ARTIFACTS := 
# =============================================================================
# SoC Configuration
# =============================================================================
EMBEDDED_EXEC_SCRIPT := embedded_exec.sh


# =============================================================================
# Utility Commands
# =============================================================================
SHELL := /bin/bash
RM := rm -rf
CPY := cp -prf

# =============================================================================
# Build Rules
# =============================================================================
.PHONY: check-EDF-common-image all pfm host clean ultraclean help run_emu sd_card prepare_run_hw qemu_combined  insert_image

all: run_emu
sd_card: prepare_run_hw

#target : check  whether the EDF common image is ready, *.wic.ufs image is for hardware emulation 
#IMAGE1 flag is used for internal users.
#IMAGE2 flag is used for external users.
check-EDF-common-image:
	@IMAGE1="$(YOCTO_ARTIFACTS)/$(QEMU_IMAGES)/edf-linux-disk-image-amd-cortexa78-mali-common.rootfs.wic.ufs"; \
	IMAGE2="$(YOCTO_QEMU_ARTIFACTS)/edf-linux-disk-image-amd-cortexa78-mali-common.rootfs.wic.ufs"; \
	if [ ! -f "$$IMAGE1" ] && [ ! -f "$$IMAGE2" ]; then \
	    echo "Error: EDF image not found in either:"; \
	    echo "  - $$IMAGE1"; \
	    echo "  - $$IMAGE2"; \
	    echo "Please contact your FAE or download it from the secure site."; \
	    exit 1; \
	fi; \
	echo "EDF common image exists. Proceeding..."

pfm: $(PLATFORM)
$(PLATFORM): 
	@echo "INFO: Creating Platform $(PLATFORM_NAME) in mode: $(MODE)"
	$(RM) -rf $(PLATFORM)
	@echo "Set place for Vitis to store configuration files"
	@mkdir -p tmp && export XILINX_VITIS_DATA_DIR="tmp"  
	vitis -s platform_creation.py \
		--platform_name $(PLATFORM_NAME) \
		--xsa_path $(FIXED_XSA_PATH)/$(FIXED_XSA_NAME)_hw.xsa \
		--xsa-emu_path $(FIXED_XSA_PATH)/$(FIXED_XSA_NAME)_hw_emu.xsa \
		--sysroot $(SYSROOT) \
		--platform=$(PLATFORM) \
		--mode=$(MODE)
	@echo "INFO: Platform created:" && find . -name "*.xpfm"
#user should download SDk to install it manually.
$(SYSROOT):
	bash -c '\
	unset LD_LIBRARY_PATH; \
	sh ${YOCTO_ARTIFACTS}//amd-cortexa78-mali-common_meta-edf-app-sdk/installer/amd-edf-glibc-x86_64-meta-edf-app-sdk-cortexa72-cortexa53-amd-cortexa78-mali-common-toolchain.sh -d . -y '
sysroot: $(SYSROOT)

host: $(ELF)
$(ELF): $(HOST_SRC)
	@echo "INFO: Creating and building application"
	vitis -s platform_creation.py --sysroot $(SYSROOT) --platform=$(PLATFORM) --mode=app

elf: host

prepare_run_hw:
	@echo "Preparing hardware run environment..."
	$(MAKE) -C ../step2_vitis_integration package TARGET=hw
	$(MAKE) pfm MODE=$(TARGET) 
	$(MAKE) sysroot
	$(MAKE) host 
	@mkdir -p hw_run/
	@cp -rf $(DTBO) ../step2_vitis_integration/package.hw/gm2aie.pdi ../step2_vitis_integration/gm2aie.xclbin $(ELF) $(EMBEDDED_EXEC_SCRIPT) hw_run/
	@mv hw_run/gm2aie.pdi hw_run/vpl_gen_fixed_pld.pdi
	@echo "Copy hw_run to your board and execute embedded_exec.sh"

qemu_combined:
	@echo "Preparing QEMU combined images..."
	$(RM) -rf ${QEMU_COMBINED}
	@mkdir -p ${QEMU_COMBINED}/${COMBINED}
	@if ls ${YOCTO_ARTIFACTS}/${DISK_IMAGES}/*.bin >/dev/null 2>&1; then \
		echo "Copying from YOCTO_ARTIFACTS..."; \
		cp -prf ${YOCTO_ARTIFACTS}/${DISK_IMAGES}/* ${QEMU_COMBINED}/${COMBINED}/; \
		cp -prf ${YOCTO_ARTIFACTS}/${QEMU_IMAGES}/* ${QEMU_COMBINED}/${COMBINED}/; \
	elif ls ${YOCTO_QEMU_ARTIFACTS}/*.bin >/dev/null 2>&1; then \
		echo "Primary source not found, copying from YOCTO_QEMU_ARTIFACTS..."; \
		cp -prf ${YOCTO_QEMU_ARTIFACTS}/* ${QEMU_COMBINED}/${COMBINED}/; \
	else \
		echo "Error: Required .bin files not found in either source directory."; \
		exit 1; \
	fi


insert_image:
	@echo "Inserting images into base camp WIC..."
	@bash -c '\
	unset LD_LIBRARY_PATH; \
	source $(XILINX_VITIS)/data/emulation/qemu/comp/qemu/environment-setup-x86_64-petalinux-linux; \
	echo "Sourced environment"; \
	wic cp --sector-size=4096 run_app_hw_emu.sh ./$(QEMU_COMBINED)/$(COMBINED)/$(WIC_PARTITION); \
	echo "Copied run_app_hw_emu.sh"; \
	wic cp --sector-size=4096 $(ELF) ./$(QEMU_COMBINED)/$(COMBINED)/$(WIC_PARTITION); \
	echo "Copied host application"; \
	wic cp --sector-size=4096 ../step2_vitis_integration/gm2aie.xclbin ./$(QEMU_COMBINED)/$(COMBINED)/$(WIC_PARTITION); \
	echo "Copied xclbin"; \
	wic cp  --sector-size=4096 ../step2_vitis_integration/package.hw_emu/gm2aie.dtbo ./$(QEMU_COMBINED)/$(COMBINED)/$(WIC_PARTITION); \
	echo "Copied dtbo"; \
	wic cp --sector-size=4096 ../step2_vitis_integration/package.hw_emu/gm2aie.pdi ./$(QEMU_COMBINED)/$(COMBINED)/$(WIC_PARTITION); \
	echo "Copied pdi"'

run_emu:
	@echo "Starting hardware emulation..."
	$(MAKE)  check-EDF-common-image
	$(MAKE) -C ../step2_vitis_integration package TARGET=hw_emu
	$(MAKE) pfm MODE=hw_emu TARGET=hw_emu
	$(MAKE) sysroot
	$(MAKE) host
	$(MAKE) qemu_combined
	$(MAKE) insert_image TARGET=hw_emu
	$(MAKE) start_emu
start_emu:
	../step2_vitis_integration/package.hw_emu/launch_hw_emu.sh \
		-qemu-config ${PWD}/${QEMU_COMBINED}/${COMBINED}/${COMBINED}.qemuboot.conf \
		-login "amd-edf" -password "amd-edf" \
		-run-app "mount /dev/sda2 /media; cd /media; ./run_app_hw_emu.sh" | tee embedded_run.log; exit $${PIPESTATUS[0]}

# =============================================================================
# Cleanup Rules
# =============================================================================
clean:
	@echo "Cleaning build artifacts..."
	$(RM) application vek385* *.pdi

ultraclean: clean
	@echo "Performing complete cleanup..."
	$(RM) vek385* -rf hw_run
	$(RM) application -rf
	$(RM) ${QEMU_COMBINED}
	-$(RM) _x *.log *.compile_summary *.link_summary tmp -rf
	$(RM) sysroots *.txt _ide .rigel_lopper sdt_out .Xil

# =============================================================================
# Help
# =============================================================================
help:
	@echo "Makefile usage:"
	@echo "  make pfm MODE=pfm_hw/pfm_emu TARGET=hw/hw_emu"
	@echo "      Build platform"
	@echo ""
	@echo "  make -C ../step2_vitis_integration package TARGET=hw/hw_emu"
	@echo "      Build xclbin"
	@echo ""
	@echo "  make host"
	@echo "      Build host executable"
	@echo ""
	@echo "  make run_emu TARGET=hw_emu"
	@echo "      Run hardware emulation"
	@echo ""
	@echo "  make prepare_run_hw TARGET=hw"
	@echo "      Build and prepare artifacts for hardware run"
	@echo ""
	@echo "  make clean"
	@echo "      Clean build artifacts"
	@echo ""
	@echo "  make cleanall"
	@echo "      Perform complete cleanup"
