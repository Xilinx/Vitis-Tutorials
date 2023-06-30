# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */
DESIGN   =flat
PLATFORM =./${DESIGN}_wa.xsa
TARGET   =hw
VMA      =../vitis/vma/${TARGET}.vma
TCLARGS  =-tclargs ${DESIGN} ${VMA}

.PHONY: clean import export xsa

xsa: ${PLATFORM}
${PLATFORM}: ${DESIGN}.tcl ${DESIGN}_pre_bdc.tcl create_bdc.tcl
	vivado -mode batch -source ${DESIGN}.tcl

#export: ${VMA}
#	vivado -mode batch -source export.tcl ${TCLARGS}

#import: ${VMA}
#	cp -r ${DESIGN} ${DESIGN}_bk
#	vivado -mode batch -source import_vma.tcl ${TCLARGS}

clean:
	rm -rf ${PLATFORM} ${DESIGN} ${DESIGN}_bk ${DESIGN}_vma ${VMA} 
	rm -rf vivado* .Xil *.pdi *.json .gen *.xsa
	rm -rf .crashReporter flat_wrappe*


