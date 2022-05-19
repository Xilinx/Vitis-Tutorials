# Copyright 2021 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
# NUMBER OF COMPUTE UNITS INSTANCES 
# x1   = 1   AIE TILE
# x10  = 10  AIE TILES
# X100 = 100 AIE TILES
# =========================================================
INST		= x1

# =========================================================
# PROJECT DIRECTORIES
# =========================================================
RELATIVE_PROJECT_DIR := ../../
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

#Design Directory 
DESIGN_DIR := $(PROJECT_DIR)/$(INST)_design

AIE_APP_DIR := $(DESIGN_DIR)/aie
XCLBIN_DIR := $(DESIGN_DIR)/xclbin
HOST_SW_DIR := $(DESIGN_DIR)/host_sw
SD_CARD_DIR := $(DESIGN_DIR)/sd_card
DATA_DIR := $(DESIGN_DIR)/data

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
BUILD_DIR := $(SD_CARD_DIR)/build.$(TARGET).$(PLATFORM)
XCLBIN_OUTPUT_DIR := $(XCLBIN_DIR)/build.$(TARGET).$(PLATFORM)
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

# ################ TARGET: make xclbin ################
xclbin: 
	$(MAKE) xclbin -C $(XCLBIN_DIR)

# ################ TARGET: make all ################
all: sd_card

# ################ TARGET: make sd_card ################
sd_card: host xclbin $(EMBEDDED_PACKAGE_OUT)/sd_card.img

$(EMBEDDED_PACKAGE_OUT)/sd_card.img: $(PACKAGE_FILES) $(XCLBIN_OUTPUT_DIR)/hpc.xclbin $(AIE_APP_DIR)/build/libadf.a
	mkdir -p $(BUILD_DIR); \
	cd $(BUILD_DIR); \
	v++ -t $(TARGET) \
	--platform $(PLATFORM) \
	-p $(XCLBIN_OUTPUT_DIR)/hpc.xclbin $(AIE_APP_DIR)/build/libadf.a \
	--package.out_dir $(EMBEDDED_PACKAGE_OUT) \
	--package.rootfs $(EDGE_COMMON_SW)/rootfs.ext4 \
	--package.kernel_image $(EDGE_COMMON_SW)/Image \
	--package.boot_mode=sd \
	--package.image_format=ext4 \
	$(SD_FILES_WITH_PREFIX)
	@echo "### ***** sd_card generation done! ***** ###"

# ################ TARGET: make run ################
run: sd_card
ifeq ($(TARGET),$(filter $(TARGET),hw_emu))
ifeq ($(findstring vck190, $(PLATFORM)), vck190)
	@echo "-net nic,netdev=eth0 -netdev user,id=eth0,hostfwd=tcp::1114-:22 -net nic" >> $(EMBEDDED_PACKAGE_OUT)/qemu_args.txt
	export XCL_EMULATION_MODE=$(TARGET)
	xterm & 
	cd $(EMBEDDED_PACKAGE_OUT) ;\
	$(EMU_CMD)
endif
endif

# ################ TARGET: make clean ################
clean:
	rm -rf $(BUILD_DIR)
	rm -rf *~
