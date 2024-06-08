#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

SHELL := /bin/bash
TOOLS ?= ""
VERSION ?= 2024.1

all: create_xsa classic_workspace

create_xsa:
	vivado -mode tcl -source create_xsa.tcl

classic_workspace:
	xsct -eval "source classic_workspace.tcl"

clean:
	$(RM) -rf zcu102 classic_workspace
