#
# Copyright (C) 2021-2022, Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO            := @echo

PLATFORM_CUSTOM ?= vck190_thin

#XPFM              := ../../xpfm_export/build/platform/${PLATFORM_CUSTOM}/export/${PLATFORM_CUSTOM}/${PLATFORM_CUSTOM}.xpfm
XSA               := ../../../vivado/build/xsa_platform/${PLATFORM_CUSTOM}.xsa


KERNEL          := mm2s
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

${XO}: ${KERNEL_SRC} ${XSA}
	v++ --compile --debug --save-temps \
		--platform ${XSA} \
		--kernel   ${KERNEL} \
		--output   ${XO} \
		${KERNEL_SRC}

clean:
	rm -f $(XO)* *.log
	rm -rf .Xil _x/ .crashReporter

