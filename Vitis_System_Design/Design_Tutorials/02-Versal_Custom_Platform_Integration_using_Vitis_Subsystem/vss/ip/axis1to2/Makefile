#
# Copyright (C) 2023-2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO            := @echo

PLATFORM_CUSTOM       ?= vck190_thin
PART                  ?= xcvc1902-vsva2197-2MP-e-S
FREQHZ                ?= 250000000

#XPFM              := ../../xpfm_export/build/platform/${PLATFORM_CUSTOM}/export/${PLATFORM_CUSTOM}/${PLATFORM_CUSTOM}.xpfm
XSA               := ../../../vivado/build/xsa_platform/${PLATFORM_CUSTOM}.xsa

KERNEL          := axis1to2
KERNEL_TOP      := ./src/${KERNEL}.cpp
KERNEL_SRC      := ./src/*
XO              := ../xo_hw/${KERNEL}.xo

.PHONY: clean all help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the hls-vadd-kernel used in this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: ${XO}

#	v++ --compile --mode hls \
#		--part     ${PART} \
#		--work_dir build \
#		--hls.flow_target vitis \
#		--hls.export_mode xo:${XO} \

#${XO}: ${KERNEL_SRC} ${XSA}
#	v++ --compile \
#		--platform ${XSA} \
#		--freqhz   ${FREQHZ} \
#		--kernel   ${KERNEL} \
#		--output   ${XO} \
#		${KERNEL_SRC}

${XO}: ${KERNEL_SRC}
	v++ --compile --mode hls\
		--part ${PART} \
		--freqhz   ${FREQHZ} \
		--hls.syn.top   ${KERNEL} \
		--hls.syn.file  ${KERNEL_TOP} \
		--hls.package.ip.name   ${KERNEL} \
		--hls.package.output.file ${XO} \
		--config hls.cfg \
		${KERNEL_TOP}

clean:
	rm -f $(XO)* *.log
	rm -rf .Xil _x/ .crashReporter $(KERNEL)

