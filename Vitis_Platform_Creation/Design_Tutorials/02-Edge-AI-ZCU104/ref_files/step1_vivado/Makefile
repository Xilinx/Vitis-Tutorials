#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
ABS_HW_PATH=$(ABS_BUILD_PATH)/vivado
BD_TCL=system_step1.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)

.PHONY: help

help:
	@echo "Makefile usage"
	@echo "make all 	## to generate hw platform and export XSA file"


all: hw
sd_card:all 


hw:
	mkdir -p $(ABS_HW_PATH)
	cd $(ABS_HW_PATH); vivado -mode batch -notrace -source $(ABS_BD_TCL) -source ../../export_xsa.tcl


clean:
	$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou .Xil

ultraclean: clean