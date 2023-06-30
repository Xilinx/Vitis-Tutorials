# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */

VCC = v++
XSA = ../vivado_impl/flat_wa.xsa

.PHONY: clean

%.xo: %.cpp
	${VCC} -c -k $(basename $<) -f ${XSA} -s -o $@ $< 

clean:
	rm -rf _x hls *.log *.xo* .Xil .crashReporter *.xo.compile_summary

