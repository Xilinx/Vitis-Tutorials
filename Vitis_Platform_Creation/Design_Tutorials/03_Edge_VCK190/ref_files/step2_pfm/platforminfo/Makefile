#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/
ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PLATFORM_NAME=custom_platform
PLATFORM = $(ROOT_DIR)/../$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm
.PHONY: all clean

all:
	platforminfo $(PLATFORM) > platforminfo.txt

clean:
	rm platforminfo.txt