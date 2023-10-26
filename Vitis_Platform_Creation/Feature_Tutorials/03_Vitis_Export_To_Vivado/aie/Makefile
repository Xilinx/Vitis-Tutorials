# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */

# Compile ADF graph to build libadf.a
AIECC    =v++ -c --mode aie
AIEFLAGS = --platform ../vivado_impl/prj_bdc_ext_wa.xsa

 

.PHONY: clean

all: libadf.a

libadf.a: graph.cpp graph.h 
	${AIECC} ${AIEFLAGS} $<

clean:
	rm -rf .Xil Work libadf.a Map_Report.csv sol.db xcd.log AIECompiler.log temp

