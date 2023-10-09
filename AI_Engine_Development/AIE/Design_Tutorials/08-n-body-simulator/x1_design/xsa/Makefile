#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

help::
	@echo "Makefile Usage:"
	@echo "  make all TARGET=<hw_emu|hw>"
	@echo "       Command runs make xsa"
	@echo "       Default TARGET=hw"
	@echo ""
	@echo "  make xsa TARGET=<hw_emu|hw>"
	@echo "       Command to create the XSA from libadf.a and HLS kernels *.xo"
	@echo "       Default TARGET=hw"
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
#relative project directory
RELATIVE_PROJECT_DIR := ../../

#absolute project directory
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

#Design Directory 
DESIGN_DIR := $(PROJECT_DIR)/$(INST)_design

AIE_DIR :=  $(DESIGN_DIR)/aie
PL_KERNELS_DIR := $(DESIGN_DIR)/pl_kernels
XSA_DIR := $(DESIGN_DIR)/xsa

# =========================================================
# TARGET SELECTION
# TARGET=hw (default)
# TARGET=hw_emu 
# =========================================================
TARGET ?=hw

# =========================================================
# PL KERNELS DIR
# =========================================================
TEMP_DIR := $(PL_KERNELS_DIR)/build.$(TARGET)/_x_temp

# =========================================================
# V++ COMPILER FLAGS
# =========================================================
VPP_FLAGS += -t $(TARGET) --platform $(PLATFORM) --save-temps -g --optimize 2
VPP_FLAGS += --hls.jobs 8
VPP_FLAGS += --config $(XSA_DIR)/conn.cfg
VPP_FLAGS += --clock.defaultFreqHz 150000000

# =========================================================
# V++ LINKER FLAGS
# =========================================================
VPP_LDFLAGS := --vivado.synth.jobs 8 --vivado.impl.jobs 8

# =========================================================
# BUILD DIRECTORIES
# =========================================================
BUILD_DIR := $(XSA_DIR)/build.$(TARGET)
BUILD_REPORT_DIR := $(BUILD_DIR)/reports/_x

# ################ TARGET: make pl_kernels ################
pl_kernels: 
	$(MAKE) pl_kernels -C $(PL_KERNELS_DIR)

# ################ TARGET: make all ################
all: pl_kernels xsa

# ################ TARGET: make xsa ################
xsa: $(BUILD_DIR)/hpc.xsa
$(BUILD_DIR)/hpc.xsa: $(TEMP_DIR)/packet_sender.xo $(TEMP_DIR)/m2s_x2.xo $(TEMP_DIR)/packet_receiver.xo $(TEMP_DIR)/s2m_x4.xo $(AIE_DIR)/build/libadf.a
	@echo "### ***** linking pl kernels into hpc.xsa ... *****"
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR); \
	v++ -l $(VPP_FLAGS) \
	--temp_dir $(BUILD_DIR)/_x_temp \
	--report_dir $(BUILD_REPORT_DIR)/hpc \
	$(VPP_LDFLAGS) \
	-o '$@' $(+) | tee $(BUILD_DIR)/hpc_xsa.log
	@echo "### ***** hpc.xsa linking done! *****"

# ################ TARGET: make clean ################
clean:
	rm -rf $(BUILD_DIR)
	rm -rf *~
	rm -rf $(TEMP_DIR)/link
