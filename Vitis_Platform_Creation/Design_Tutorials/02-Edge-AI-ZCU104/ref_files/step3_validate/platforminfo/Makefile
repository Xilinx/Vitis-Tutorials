#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

PLATFORM_NAME=zcu104_custom
PLATFOMR_PATH = ../../step2_pfm/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm

.PHONY: all clean

all:
	platforminfo $(PLATFOMR_PATH) > platforminfo.txt

clean:
	rm platforminfo.txt
