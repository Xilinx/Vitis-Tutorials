#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
ABS_HW_PATH=$(ABS_BUILD_PATH)/vivado
ABS_SW_PATH=$(ABS_BUILD_PATH)/$(PETALINUX_PROJ_NAME)
BD_TCL=run.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)

XSA_NAME=vck190_custom_dfx
ABS_XSA=$(ABS_HW_PATH)/$(XSA_NAME)_static.xsa
ABS_XSA_RP=$(ABS_HW_PATH)/$(XSA_NAME)_rp.xsa
ABS_XSA_HWEMU=$(ABS_HW_PATH)/$(XSA_NAME)_hw_emu.xsa

.PHONY: all hw petalinux clean

all: hw
sd_card:all

# target: hw
# Build bare minial Vivado design and export XSA
hw: $(ABS_BD_TCL)
	mkdir -p $(ABS_HW_PATH)
	cd $(ABS_HW_PATH); vivado -mode batch -notrace -source $(ABS_BD_TCL) -tclargs VCK190


clean:
	$(RM) -r $(ABS_HW_PATH)/project_1 $(ABS_XSA) $(ABS_XSA_RP) $(ABS_XSA_HWEMU)

cleanlog:
	$(RM) $(ABS_HW_PATH)/vivado*.log $(ABS_HW_PATH)/vivado*.jou 
