#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO        := @echo

DEVICE_NAME ?= xcvc1902-vsva2197-2MP-e-S

IP_TCL      := ./pack_kernel.tcl
KERNEL      := subtractor
KERNEL_SRC  := ./src/* ./${KERNEL}.xdc
XO          := ../xo_hw/${KERNEL}.xo

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the IPs used in this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

.PHONY: help all clean

all: ${XO}

${XO}: ${IP_TCL} ${KERNEL_SRC}
	vivado -mode batch -source $(IP_TCL) -tclargs $(KERNEL) $(DEVICE_NAME) $(XO)

clean:
	$(RM) -rf build .Xil .crashReporter
	$(RM) -rf $(XO) vivado.* *.xml *.jou *.log

