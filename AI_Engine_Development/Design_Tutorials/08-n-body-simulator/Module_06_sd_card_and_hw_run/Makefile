#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

help::
	@echo "Makefile Usage:"
	@echo "  make all TARGET=<hw_emu|hw>"
	@echo "       Command runs make sd_card"
	@echo "       Default TARGET=hw" 
	@echo ""
	@echo "  make sd_card TARGET=<hw_emu|hw>"
	@echo "       Command runs v++ compiler -p to create the sd_card.img"
	@echo "       Default TARGET=hw"
	@echo ""
	@echo "  make run"
	@echo "       Command launches hardware emulation"
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove the generated files."
	@echo ""

# =========================================================
# PROJECT DIRECTORIES
# =========================================================
RELATIVE_PROJECT_DIR := ../
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

#Design Directory 
DESIGN_DIR := $(PROJECT_DIR)

AIE_APP_DIR := $(DESIGN_DIR)/Module_02_aie
XSA_DIR := $(DESIGN_DIR)/Module_04_full_system_design
HOST_SW_DIR := $(DESIGN_DIR)/Module_05_host_sw
SD_CARD_DIR := $(DESIGN_DIR)/Module_06_sd_card_and_hw_run
DATA_DIR := $(DESIGN_DIR)/data

EMBEDDED_EXEC_SCRIPT := $(DESIGN_DIR)/Module_06_sd_card_and_hw_run/run_script.sh

# =========================================================
# TARGET SELECTION
# TARGET=hw (default)
# TARGET=hw_emu 
# =========================================================
TARGET ?=hw

# =========================================================
# HW_EMU COMMAND
# =========================================================
EMU_CMD := ./launch_hw_emu.sh

# =========================================================
# BUILD DIRECTORIES
# =========================================================
BUILD_DIR := $(SD_CARD_DIR)/build.$(TARGET)
XSA_OUTPUT_DIR := $(XSA_DIR)/build.$(TARGET)
EMBEDDED_PACKAGE_OUT := $(BUILD_DIR)

# =========================================================
# V++ PACKAGE FLAGS
# =========================================================
PACKAGE_FILES := $(HOST_SW_DIR)/build.$(TARGET)/ps_app_animate.exe
PACKAGE_FILES += $(HOST_SW_DIR)/build.$(TARGET)/ps_app.exe
PACKAGE_FILES += $(DATA_DIR)/m2s_i.txt
PACKAGE_FILES += $(DATA_DIR)/input_j.txt
PACKAGE_FILES += $(DATA_DIR)/s2m_golden_i_k0.txt
PACKAGE_FILES += $(DATA_DIR)/s2m_golden_i_k1.txt
PACKAGE_FILES += $(DATA_DIR)/s2m_golden_i_k2.txt
PACKAGE_FILES += $(DATA_DIR)/s2m_golden_i_k3.txt

SD_FILES_WITH_PREFIX = $(foreach sd_file,$(PACKAGE_FILES),--package.sd_file $(sd_file))

# ################ TARGET: make host ################
host: 
	$(MAKE) host -C $(HOST_SW_DIR)

# ################ TARGET: make xsa ################
xsa: 
	$(MAKE) xsa -C $(XSA_DIR)

# ################ TARGET: make all ################
all: sd_card

# ################ TARGET: make sd_card ################
sd_card: host xsa $(EMBEDDED_PACKAGE_OUT)/sd_card.img

$(EMBEDDED_PACKAGE_OUT)/sd_card.img: $(PACKAGE_FILES) $(XSA_OUTPUT_DIR)/hpc.xsa $(AIE_APP_DIR)/build/libadf.a
	mkdir -p $(BUILD_DIR); \
	cd $(BUILD_DIR); \
	v++ -t $(TARGET) \
	--platform $(PLATFORM) \
	--package.defer_aie_run \
	-p $(XSA_OUTPUT_DIR)/hpc.xsa $(AIE_APP_DIR)/build/libadf.a \
	--package.out_dir $(EMBEDDED_PACKAGE_OUT) \
	--package.rootfs $(COMMON_IMAGE_VERSAL)/rootfs.ext4 \
	--package.kernel_image $(COMMON_IMAGE_VERSAL)/Image \
	--package.sd_file $(EMBEDDED_EXEC_SCRIPT) \
	--package.boot_mode=sd \
	--package.image_format=ext4 \
	$(SD_FILES_WITH_PREFIX)
	@echo "### ***** sd_card generation done! ***** ###"

# ################ TARGET: make run ################
run: sd_card
ifeq ($(TARGET),$(filter $(TARGET),hw_emu))
ifeq ($(findstring vck190, $(PFM)), vck190)
	@echo "-net nic,netdev=eth0 -netdev user,id=eth0,hostfwd=tcp::1114-:22 -net nic" >> $(EMBEDDED_PACKAGE_OUT)/qemu_args.txt
	export XCL_EMULATION_MODE=$(TARGET)
	xterm & 
	cd $(EMBEDDED_PACKAGE_OUT) ;\
	$(EMU_CMD) ; \
	-run-app $(EMBEDDED_EXEC_SCRIPT)
endif
endif

# ################ TARGET: make clean ################
clean:
	rm -rf $(BUILD_DIR)
	rm -rf *~
