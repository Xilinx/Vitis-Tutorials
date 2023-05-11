#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
ABS_HW_PATH=$(ABS_BUILD_PATH)/vivado
BD_TCL=run.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)

.PHONY: help

help:
	@echo "Makefile usage"
	@echo "make all 	## to generate hw platform and export XSA file"


all: hw 
sd_card :all
#petalinuximage

#generate the XSA file 
hw:
	mkdir -p $(ABS_HW_PATH)
	cd $(ABS_HW_PATH); vivado -mode batch -notrace -source $(ABS_BD_TCL) -source 
#Generate petalinux image for validation

clean:
	$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou

ultraclean: clean