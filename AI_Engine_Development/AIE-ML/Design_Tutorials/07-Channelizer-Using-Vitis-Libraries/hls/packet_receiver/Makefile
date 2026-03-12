#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

DO_COSIM        := false
ECHO            := @echo

KERNEL          := packet_receiver_wrapper
KERNEL_SRC      := packet_receiver.cpp
XO              := ${KERNEL}.xo

.PHONY: clean all help csim synth cosim

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the kernel used in this design"
	$(ECHO) ""
	$(ECHO) "  make csim"
	$(ECHO) "      Command to run C simulation"
	$(ECHO) ""
	$(ECHO) "  make synth"
	$(ECHO) "      Command to run synthesis only"
	$(ECHO) ""
	$(ECHO) "  make cosim"
	$(ECHO) "      Command to run RTL co-simulation"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: ${XO}

csim:
	vitis-run --mode hls --config hls.cfg --csim --work_dir ${KERNEL}

synth:
	v++ --config hls.cfg --compile --mode hls --work_dir ${KERNEL}

cosim:
	vitis-run --mode hls --config hls.cfg --cosim --work_dir ${KERNEL}

${XO}: ${KERNEL_SRC}
	v++ --config hls.cfg --compile --mode hls --work_dir ${KERNEL}
ifeq (${DO_COSIM},true)
	vitis-run --mode hls --config hls.cfg --csim --work_dir ${KERNEL}
	vitis-run --mode hls --config hls.cfg --cosim --work_dir ${KERNEL}
	vitis-run --mode hls --config hls.cfg --impl --work_dir ${KERNEL}
endif

clean:
	rm -f $(XO)* *.log vitis-comp.json
	rm -rf .Xil _x/ .crashReporter*
	rm -rf ${KERNEL}
