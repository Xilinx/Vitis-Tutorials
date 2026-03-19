#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Luke Millar

SHELL := /bin/bash
VERSION ?= 2025.2

all: clean hw sw

hw:
	vivado -mode batch -source create_project.tcl

sw:
	vitis -s build_workspace.py 

clean:
	$(RM) -rf ./unified_workspace
