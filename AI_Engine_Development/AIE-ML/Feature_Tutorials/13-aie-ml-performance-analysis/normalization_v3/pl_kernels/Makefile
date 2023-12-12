
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

PLATFORM = ${PLATFORM_REPO_PATHS}/xilinx_vek280_es1_base_202320_1/xilinx_vek280_es1_base_202320_1.xpfm

.PHONY: clean

all:$(subst .cpp,.xo,$(wildcard *.cpp))
%.xo: %.cpp
	sed 's/s2mm/$(basename $<)/' config.cfg > tmp.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config tmp.cfg

clean:
	rm -rf _x *.xo* *.log
