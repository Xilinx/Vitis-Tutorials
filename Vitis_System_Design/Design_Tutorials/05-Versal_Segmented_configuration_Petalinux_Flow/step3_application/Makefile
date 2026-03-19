#/*
#Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
#Copyright (C) 2022-2026 Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

# =============================================================================
# Project Configuration
# =============================================================================
ROOT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
TARGET ?= hw
MODE := $(TARGET)
VERSION=2025.2
# =============================================================================
# Package Configuration
# =============================================================================
STEP2_DIR := $(ROOT_DIR)/../step2_vitis_integration
PKG_CFG := $(ROOT_DIR)/cfg/package.cfg
XCLBIN := $(ROOT_DIR)/gm2aie.xclbin
PKG_DIR_XCLBIN := $(ROOT_DIR)/package_xclbin.$(TARGET)
PKG_DIR_SD := $(ROOT_DIR)/package_sd.$(TARGET)
# Step2 build artifacts (from step2 link)
FIXED_XSA_NAME := sys_gm2aie
LINK_OUTPUT := $(STEP2_DIR)/$(FIXED_XSA_NAME)_$(TARGET).xsa
AIE_GRAPH := $(STEP2_DIR)/libadf.a

# =============================================================================
# Platform Configuration
# =============================================================================
FIXED_XSA_PATH := $(STEP2_DIR)
PLATFORM_NAME := vck190_fixed_platform_$(TARGET)
PLATFORM := $(ROOT_DIR)/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
DTB:=$(ROOT_DIR)/$(PLATFORM_NAME)/hw/linux_dt/system.dtb
DTBO := $(PKG_DIR_XCLBIN)/gm2aie.dtbo
PDI := $(PKG_DIR_XCLBIN)/gm2aie.pdi

# =============================================================================
# Application Configuration
# =============================================================================
ELF := $(ROOT_DIR)/application/build/application
HOST_SRC := $(ROOT_DIR)/../step2_vitis_integration/_aie/ps/c_rts/aie_control_xrt.cpp $(ROOT_DIR)/host_src/host.cpp

# =============================================================================
# Common Image Configuration (for SD card generation)
# =============================================================================
# COMMON_IMAGE_VERSAL: Path to pre-built Petalinux common image directory
# Should contain: Image, rootfs.ext4, and other common image files
# Download from: https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html
COMMON_IMAGE_VERSAL ?= ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/
SYSROOT := $(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-amd-linux/
SD_IMAGE_FILE := $(COMMON_IMAGE_VERSAL)/Image
ROOTFS_FILE := $(COMMON_IMAGE_VERSAL)/rootfs.ext4


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
.PHONY: all pfm host clean ultraclean help sd_card prepare_run_hw package package_xclbin package_sd check-common-image-sd check_edge_sw check_step2_build

all: sd_card
	@echo "INFO: Step 3 completed"
sd_card: package_sd

# Create platform
pfm:  $(PLATFORM)


$(PLATFORM):
	@echo "INFO: Creating platform $(PLATFORM_NAME) in mode: $(MODE)"
	@mkdir -p tmp && export XILINX_VITIS_DATA_DIR="tmp"
	vitis -s platform_creation.py \
		--platform_name $(PLATFORM_NAME) \
		--xsa_path $(FIXED_XSA_PATH)/$(FIXED_XSA_NAME)_hw.xsa \
		--sysroot $(SYSROOT) \
		--platform=$(PLATFORM) \
		--user_dtsi=./system_user.dtsi \
		--mode=hw
	@echo "INFO: Platform created: $(PLATFORM)"
#user should download Petalinux SDK to install it manually.
#For Petalinux, sysroot is typically generated from the Petalinux project
#Example: petalinux-build --sdk
#Then install: ./images/linux/sdk.sh -d ./sysroots/
$(SYSROOT):
	@echo "INFO: Please install the Petalinux SDK manually"
	@echo "Example:"
	@echo "  cd <petalinux_project>"
	@echo "  petalinux-build --sdk"
	@echo "  ./images/linux/sdk.sh -d $(ROOT_DIR)/sysroots/"
	@exit 1
sysroot: $(SYSROOT)

# Build host application
host: $(ELF)

$(ELF): $(HOST_SRC)
	@echo "INFO: Creating and building application"
	@echo "build platform firstly"
	make pfm MODE=hw
	vitis -s platform_creation.py --sysroot $(SYSROOT) --platform=$(PLATFORM) --mode=app

elf: host

# =============================================================================
# Package generation (generating XCLBIN in step3 using step2 link output)
# =============================================================================
package: package_xclbin
	@echo "INFO: Package (XCLBIN) generated: $(XCLBIN)"

# Generate XCLBIN using step2's link output
package_xclbin: check_step2_build $(XCLBIN)

$(XCLBIN): $(LINK_OUTPUT) $(AIE_GRAPH) $(PKG_CFG) | $(PKG_DIR_XCLBIN)
	@echo "INFO: Generating XCLBIN from step2 link output..."
	@echo "INFO: Input XSA: $(LINK_OUTPUT)"
	@echo "INFO: AIE Graph: $(AIE_GRAPH)"
	@echo "INFO: Package config: $(PKG_CFG)"
	v++ -p \
		--package.defer_aie_run \
		--package.no_image \
		--package.out_dir $(PKG_DIR_XCLBIN) \
		-s -f $(LINK_OUTPUT) \
		$(AIE_GRAPH) \
		-t $(TARGET) \
		-o $(XCLBIN)
	@echo "INFO: XCLBIN generated: $(XCLBIN)"

# Check if step2 build artifacts exist
check_step2_build:
	@echo "INFO: Checking step2 build artifacts..."
	@for file in "$(LINK_OUTPUT)" "$(AIE_GRAPH)"; do \
		if [ ! -f "$$file" ]; then \
			echo "ERROR: Step2 artifact not found: $$file"; \
			echo "Please build step2 first: cd $(STEP2_DIR) && make build TARGET=$(TARGET)"; \
			exit 1; \
		fi; \
	done
	@echo "INFO: Step2 build artifacts verified"

# Check COMMON_IMAGE_VERSAL configuration
check-common-image-sd check_edge_sw:
	@if [ -z "$(COMMON_IMAGE_VERSAL)" ]; then \
		echo "ERROR: COMMON_IMAGE_VERSAL variable is not set"; \
		echo "Please download from:"; \
		echo "https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html"; \
		echo "Then set: export COMMON_IMAGE_VERSAL=/path/to/common_image_directory"; \
		exit 1; \
	fi
	@if [ ! -d "$(COMMON_IMAGE_VERSAL)" ]; then \
		echo "ERROR: COMMON_IMAGE_VERSAL directory not found: $(COMMON_IMAGE_VERSAL)"; \
		exit 1; \
	fi
	@for file in "$(SD_IMAGE_FILE)" "$(ROOTFS_FILE)"; do \
		if [ ! -f "$$file" ]; then \
			echo "ERROR: Required file not found: $$file"; \
			exit 1; \
		fi; \
	done
	@echo "INFO: COMMON_IMAGE_VERSAL verified: $(COMMON_IMAGE_VERSAL)"

# Generate SD card image using v++ package with common image
package_sd: check_step2_build check-common-image-sd $(XCLBIN) $(ELF) | $(PKG_DIR_SD)
	@echo "INFO: Generating SD card image for target: $(TARGET)..."
	@echo "INFO: Packaging for hardware..."
	v++ -p -t hw -f $(LINK_OUTPUT) --package.generate_sdcard \
		--package.out_dir $(PKG_DIR_SD) \
		--package.rootfs $(COMMON_IMAGE_VERSAL)/rootfs.ext4 \
		--package.bl31_elf $(COMMON_IMAGE_VERSAL)/bl31.elf \
		--package.uboot $(COMMON_IMAGE_VERSAL)/u-boot.elf \
		--package.dtb $(DTB) \
		--package.image_format=ext4 \
		--package.boot_mode=sd \
		--package.kernel_image=$(COMMON_IMAGE_VERSAL)/Image \
		--package.defer_aie_run \
		--package.sd_file $(EMBEDDED_EXEC_SCRIPT) \
		--package.sd_file $(ELF) \
		--package.sd_file $(COMMON_IMAGE_VERSAL)/boot.scr \
		--package.sd_file $(XCLBIN) \
		--package.sd_file $(DTBO) \
		--package.sd_file $(PDI) \
		$(AIE_GRAPH)
	@if [ -d "$(PKG_DIR_SD)/sd_card" ]; then \
		echo ""; \
		echo "INFO: ============================================================"; \
		echo "INFO: SD card image generated successfully!"; \
		echo "INFO: SD card directory: $(PKG_DIR_SD)/sd_card"; \
		echo "INFO: Copy contents to your SD card"; \
		echo "INFO: ============================================================"; \
		ls -lh $(PKG_DIR_SD)/sd_card/ 2>/dev/null || true; \
	fi

# Create package directory (order-only prerequisite)
$(PKG_DIR_SD):
	@mkdir -p $(PKG_DIR_SD)

$(PKG_DIR_XCLBIN):
	@mkdir -p $(PKG_DIR_XCLBIN)


# =============================================================================
# Cleanup Rules
# =============================================================================
clean:
	@echo "Cleaning build artifacts..."
	$(RM) application vck190* *.pdi sd_card_* gm2aie.xclbin *.xclbin
	@if [ -d "$(PKG_DIR_XCLBIN)/sd_card" ]; then \
		echo "INFO: SD card directory exists at: $(PKG_DIR_XCLBIN)/sd_card"; \
	fi

ultraclean: clean
	@echo "Performing complete cleanup..."
	$(RM) vck190* -rf hw_run
	$(RM) application -rf
	$(RM) package* -rf
	-$(RM) _x *.log *.compile_summary *.link_summary tmp -rf *summary
	$(RM) sysroot -rf

# =============================================================================
# Help
# =============================================================================
help:
	@echo "Makefile usage:"
	@echo "  make sd_card TARGET=hw"
	@echo "      Generate SD card image using v++ package with Petalinux common image"
	@echo "      Requires COMMON_IMAGE_VERSAL environment variable to be set"
	@echo ""
	@echo "  make package TARGET=hw"
	@echo "      Generate package (XCLBIN and PDI) using step3 package.cfg"
	@echo ""
	@echo "  make pfm MODE=hw TARGET=hw"
	@echo "      Build platform"
	@echo ""
	@echo "  make host"
	@echo "      Build host executable"
	@echo ""
	@echo "  make prepare_run_hw TARGET=hw"
	@echo "      Build and prepare artifacts for hardware run"
	@echo ""
	@echo "  make check_edge_sw"
	@echo "      Check if COMMON_IMAGE_VERSAL is properly configured"
	@echo ""
	@echo "  make clean"
	@echo "      Clean build artifacts"
	@echo ""
	@echo "  make ultraclean"
	@echo "      Perform complete cleanup"
	@echo ""
	@echo "Examples:"
	@echo "  # Set COMMON_IMAGE_VERSAL to common image directory"
	@echo "  export COMMON_IMAGE_VERSAL=/path/to/common_image"
	@echo "  # Generate SD card image"
	@echo "  make sd_card TARGET=hw"
	@echo ""
	@echo "Note:"
	@echo "  COMMON_IMAGE_VERSAL should contain Image and rootfs.ext4 files"
	@echo "  Download from: https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html"
	@echo ""
