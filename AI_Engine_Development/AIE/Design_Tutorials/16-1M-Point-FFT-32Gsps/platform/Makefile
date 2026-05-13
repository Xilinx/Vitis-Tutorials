#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

TCL_SOURCE := create_initial_platform.tcl
XSA := custom_pfm_vck190/custom_platform_vck190.xsa
PFM := workspace/base_pfm_vck190/export/base_pfm_vck190/base_pfm_vck190.xpfm
WORKSPACE := workspace

.PHONY: all xsa pfm

all:	xsa pfm

xsa:	$(XSA)

pfm:	$(PFM)

$(XSA):	$(TCL_SOURCE)
	vivado -mode batch -notrace -source $(TCL_SOURCE)
	@echo "------------------------------------------------------------"
	@echo " Created $(XSA)"
	@echo "------------------------------------------------------------"

$(PFM):	$(XSA)
	mkdir $(WORKSPACE)
	vitis -s vitis_platform.py
	@echo "------------------------------------------------------------"
	@echo " Created $(PFM)"
	@echo "------------------------------------------------------------"

clean:
	/bin/rm -rf vivado* build .Xil *dynamic* *.log *.xpe .crash* custom_pfm_vck190 base_pfm_vck190
	/bin/rm -rf pspmc.hwdef vivado.* .crash* ${WORKSPACE}

cleanall:	clean

