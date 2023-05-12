# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
XSA_NAME=vck190_custom_dfx
PLATFORM = $(ROOT_DIR)/../../step2_sw/build/pfm/$(XSA_NAME)/export/$(XSA_NAME)/$(XSA_NAME).xpfm

# XSA_NAME=vck190_custom_dfx
#PLATFOMR_PATH = ../../step2_pfm/platform_repo/$(XSA_NAME)/export/$(XSA_NAME)/$(XSA_NAME).xpfm
# PLATFORM = ../step2_sw/build/pfm/$(XSA_NAME)/export/$(XSA_NAME)/$(XSA_NAME).xpfm
.PHONY: all clean

all: platforminfo.txt

platforminfo.txt: $(PLATFORM)
	platforminfo $(PLATFORM) > platforminfo.txt

clean:
	rm platforminfo.txt