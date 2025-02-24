#******************************************************************************
# Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2024 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#******************************************************************************

pfm = xilinx_vck190_base
dev = vck190
dev_name = base

PRE_SYNTH       ?= TRUE
VER              ?= 202420.1
PLATFORM_NAME = xilinx_vck190_base_202420_1

TCL_SOURCE=xsa_scripts/xsa.tcl

.PHONY: all xsa

all: $(TCL_SOURCE)
	vivado -mode batch -notrace -source $(TCL_SOURCE) -tclargs $(PLATFORM_NAME) $(VER) $(PRE_SYNTH)

clean:
	$(RM) -r vivado* build .Xil *dynamic* *.log *.xpe 
