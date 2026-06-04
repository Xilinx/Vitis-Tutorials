#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

SHELL := /bin/bash
TOOLS ?= ""
VERSION ?= 2026.1

all: create_xsa unified_workspace

create_xsa:
	vivado -mode tcl -source create_xsa.tcl

unified_workspace:
	vitis -s unified_workspace.py ./zcu102/design_1_wrapper.xsa

clean:
	$(RM) -rf ./zcu102 ./classic_workspace ./unified_workspace .Xil *.log *.jou .wsdata
