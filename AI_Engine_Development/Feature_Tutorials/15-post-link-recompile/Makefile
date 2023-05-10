#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#




.PHONY: all_phases


FILES_DIR := $(PWD)/Files

all_phases:
	make -C WithoutIntermediatePlatform FILES_DIR=$(FILES_DIR) all
	make -C WithIntermediatePlatform FILES_DIR=$(FILES_DIR) all_phases
