
#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

SHELL := /bin/bash
VERSION ?= 2024.1

all: clean unified_workspace

unified_workspace:
	vitis -s build_workspace.py
clean:
	$(RM) -rf ./unified_workspace
