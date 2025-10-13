
#
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Luke Millar

SHELL := /bin/bash
VERSION ?= 2025.2

all: clean zcu102

zcu102:
	vivado -mode batch -source create_zcu102_xsa.tcl
	vitis -s build_workspace.py -s zcu102

vck190:
	vivado -mode batch -source create_vck190_xsa.tcl
	vitis -s build_workspace.py -s vck190

vek280:
	vivado -mode batch -source create_vek280_xsa.tcl
	vitis -s build_workspace.py -s vek280

vek385:
	vivado -mode batch -source create_vek385_xsa.tcl
	vitis -s build_workspace.py -s vek385

clean:
	$(RM) -rf ./unified_workspace
