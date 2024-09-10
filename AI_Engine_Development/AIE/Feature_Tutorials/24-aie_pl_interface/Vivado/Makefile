#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#
# =========================================================
# DIRECTORIES
# =========================================================
SRC_DIR 		:= src
BUILD_DIR 		:= build
XSA_PLATFORM	:= xsa_platform
XSA_PLATFORM_TCL:= $(SRC_DIR)/custom_pfm_strmIn_strmOut.tcl
XSA				:= custom_pfm_strmIn_strmOut.xsa
INCLUDE_ILAS	:= 0
RTL_OUT_BD		:= 0
AXI4S_BROADCAST	:= 0

# =========================================================
# XSA CREATION SCRIPT
# =========================================================
ifeq ($(INCLUDE_ILAS), 1)
	XSA					:= custom_pfm_strmIn_strmOut_ILAs.xsa
else
	ifeq ($(RTL_OUT_BD), 1)
		XSA					:= custom_pfm_strmIn_strmOut_RTL_out_BD.xsa
	else
		ifeq ($(AXI4S_BROADCAST), 1)
			XSA					:= custom_pfm_strmIn_strmOut_AXIS_Broadcast.xsa
		else
			XSA					:= custom_pfm_strmIn_strmOut.xsa
		endif
	endif
endif

# =========================================================
# DEPENDENCIES
# =========================================================
DEPS_PLATFORM   := $(XSA_PLATFORM_TCL)

platform: $(BUILD_DIR)/$(XSA_PLATFORM)/$(XSA)

$(BUILD_DIR)/$(XSA_PLATFORM)/$(XSA): $(DEPS_PLATFORM)
	vivado -mode batch -source $(XSA_PLATFORM_TCL) -tclargs $(INCLUDE_ILAS) $(RTL_OUT_BD) $(AXI4S_BROADCAST)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf .Xil vivado* .crash* *hwdef
