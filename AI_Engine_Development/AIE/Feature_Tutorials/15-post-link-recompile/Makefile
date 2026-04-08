#
# Copyright Advanced Micro Devices, Inc.
# SPDX-License-Identifier: MIT




.PHONY: all_phases


FILES_DIR := $(PWD)/Files

all_phases:
	make -C WithoutIntermediatePlatform FILES_DIR=$(FILES_DIR) all
	make -C WithIntermediatePlatform FILES_DIR=$(FILES_DIR) all_phases
