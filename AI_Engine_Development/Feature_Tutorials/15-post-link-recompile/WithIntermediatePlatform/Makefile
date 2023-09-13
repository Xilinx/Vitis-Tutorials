#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#




# Makefile input options
TARGET ?= hw_emu


.PHONY: all_phases

FILES_DIR := $(PWD)/../Files

phase1:
	make FILES_DIR=$(FILES_DIR) -C Phase1 phase1

phase2:
	make FILES_DIR=$(FILES_DIR) -C Phase2 phase2

all_phases: phase1 phase2

clean:
	make -C Phase1 clean
	make -C Phase2 clean
