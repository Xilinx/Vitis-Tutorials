#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

SHELL := /bin/bash
VERSION ?= 2025.2
REMOTE ?= localhost
PREBUILT ?= 0
LAB ?= 1

all: create_xsa unified_workspace

build_hw: create_xsa
build_sw: unified_workspace

create_xsa:
	if [ $(PREBUILT) == 0 ]; then \
		$(RM) -rf ./zcu102; \
		vivado -mode tcl -source create_xsa.tcl; \
	else \
		mkdir ./zcu102; \
		cp ./prebuilt/design_1_wrapper.xsa ./zcu102; \
	fi

unified_workspace:
	$(RM) -rf ./unified_workspace
	vitis -s create_sw.py generate_workspace --config ./zcu102/design_1_wrapper.xsa --lab $(LAB)

clean:
	$(RM) -rf ./zcu102 ./unified_workspace .Xil *.log *.jou .wsdata logs  __pycache__ 
