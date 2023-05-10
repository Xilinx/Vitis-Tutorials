# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202310_1/xilinx_vck190_base_202310_1.xpfm

.PHONY: clean

all: $(subst .cpp,.xo,$(wildcard *.cpp))
%.xo: %.cpp
	v++ -c --platform ${PLATFORM} -k $(basename $<) --kernel_frequency 250 $< -o $@ --verbose --save-temps

clean:
	rm -rf _x *.xo* *.log
