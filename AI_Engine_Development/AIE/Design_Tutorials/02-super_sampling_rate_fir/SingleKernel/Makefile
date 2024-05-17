#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#



PFM_NAME ?= xilinx_vck190_base_202410_1

all:
	make data
	make -C Emulation-AIE PFM_NAME=$(PFM_NAME) all

run:
	make data
	make -C Emulation-AIE PFM_NAME=$(PFM_NAME) aie aiesim

aie:
	make -C Emulation-AIE PFM_NAME=$(PFM_NAME) aie

aiesim:
	make -C Emulation-AIE PFM_NAME=$(PFM_NAME) aiesim

aieviz:
	make -C Emulation-AIE PFM_NAME=$(PFM_NAME) aieviz

clean:
	rm -rf data
	make -C Emulation-AIE clean
	rm -f vitis_analyzer*

data:
	mkdir -p data
	../Utils/GenerateStreams cint16 64 1 1 512 4 Dirac data/PhaseIn

.PHONY: all clean aie aiesim aieviz data
