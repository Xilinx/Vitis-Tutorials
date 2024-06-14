# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */

# Compile ADF graph to build libadf.a
AIECC    =v++ -c --mode aie
AIEFLAGS = --platform ../hw/build/my_project/vck190_prj_ext_hw.xsa

 

.PHONY: all clean

all: graph.cpp graph.h 
	${AIECC} ${AIEFLAGS} $<

clean:
	rm -rf .Xil Work *.a Map_Report.csv sol.db xcd.log AIECompiler.log
	rm -rf aiesimlator_output function_wdb_dir trdata.aiesim 
	rm *.log

