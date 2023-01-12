# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


PLATFORM_CUSTOM ?= vck190_thin
BASE_PFM        ?=./platform/sw/build/${PLATFORM_CUSTOM}/export/${PLATFORM_CUSTOM}/${PLATFORM_CUSTOM}.xpfm
XPFM            := ../../${BASE_PFM}
KERNEL          := mm2s_aie
KERNEL_SRC      := ./src/${KERNEL}.cpp
XO              := ../xo_hw/${KERNEL}.xo

.PHONY: clean all help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the hls-mm2s-kernel used in this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: ${XO}

${XO}: ${KERNEL_SRC} ${XPFM}
	v++ -c -g -t hw --save-temps --platform ${XPFM} -k ${KERNEL} -o ${XO} ${KERNEL_SRC}

clean:
	rm -f $(XO)* *.log
	rm -rf .Xil _x/
