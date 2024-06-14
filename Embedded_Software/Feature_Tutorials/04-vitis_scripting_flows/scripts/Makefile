#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

SHELL := /bin/bash
VERSION ?= 2024.1

all: clean create_xsa unified_workspace

create_xsa:
	vivado -mode tcl -source create_xsa.tcl

unified_workspace:
	vitis -s unified_workspace.py ./zcu102/design_1_wrapper.xsa

clean:
	$(RM) -rf ./zcu102 ./unified_workspace
