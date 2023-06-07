# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */

# Compile ADF graph to build libadf.a
AIECC    =v++ -c --mode aie
AIEFLAGS = --platform ../vivado_impl/flat_wa.xsa
#AIEFLAGS1 =--part ${PART}
 

.PHONY: clean

all: libadf.a

libadf.a: graph.cpp graph.h 
	${AIECC} ${AIEFLAGS} $<

clean:
	rm -rf .Xil Work libadf.a Map_Report.csv sol.db xcd.log AIECompiler.log temp

