#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

TCL_SOURCE := create_initial_platform.tcl
XSA := custom_pfm_vck190/custom_platform_vck190.xsa
PFM := base_pfm_vck190/export/base_pfm_vck190/base_pfm_vck190.xpfm

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
	generate-platform.sh -name base_pfm_vck190 -hw $(XSA) -domain ai_engine:aie_runtime \
		-domain psv_cortexa72_0:standalone -out-dir ./
	@echo "------------------------------------------------------------"
	@echo " Created $(PFM)"
	@echo "------------------------------------------------------------"

clean:
	/bin/rm -rf vivado* build .Xil *dynamic* *.log *.xpe .crash* custom_pfm_vck190 base_pfm_vck190
	/bin/rm -rf pspmc.hwdef vivado.* .crash*

cleanall:	clean

