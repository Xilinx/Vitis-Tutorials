# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

ECHO            := @echo

PLATFORM_CUSTOM ?= vck190_thin

XPFM              := ../../xpfm_export/build/platform/${PLATFORM_CUSTOM}/export/${PLATFORM_CUSTOM}/${PLATFORM_CUSTOM}.xpfm

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

${XO}: ${KERNEL_SRC} ${XPFM}
	v++ --compile --debug --save-temps \
		--platform ${XPFM} \
		--kernel   ${KERNEL} \
		--output   ${XO} \
		${KERNEL_SRC}

clean:
	rm -f $(XO)* *.log
	rm -rf .Xil _x/
