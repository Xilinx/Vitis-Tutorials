#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

PLATFORM_NAME=kv260_custom
ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PLATFORM=$(ROOT_DIR)/../../step2_pfm/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm

.PHONY: help 

help:
	@echo "Makefile usage"
	@echo "make all 	## to check the platform info"


all:
	platforminfo  $(PLATFORM) > platforminfo.txt

clean:
	rm platforminfo.txt