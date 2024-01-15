# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */

VCC = v++
XSA = ../hw/build/my_project/vck190_prj_ext_hw.xsa

.PHONY: all clean

all: polar_clip.cpp
	${VCC} -c -k $(basename $<) -f ${XSA} -s $< -o polar_clip.xo 

clean:
	rm -rf _x hls *.log *.xo* .Xil .crashReporter *.xo.compile_summary

