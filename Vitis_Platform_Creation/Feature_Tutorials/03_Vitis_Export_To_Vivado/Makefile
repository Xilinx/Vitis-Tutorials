# Copyright (C) 2023, Advanced Micro Devices, Inc.
# SPDX-License-Identifier: MIT

DESIGN   =flat
PLATFORM =vivado_impl/${DESIGN}_wa.xsa
TARGET   =hw
ADF     =aie/libadf.a
XOS  	=s2mm.xo mm2s.xo polar_clip.xo
VMA     =flat_${TARGET}.vma
PART    =xcvc1902-vsvd1760-2MP-e-S


.PHONY: clean xsa adf xos vma

all: xsa adf xos vma import

# Step-1 : Create the vivado hardware design through TCL script and generate Extensbile.xsa file.
xsa: ${PLATFORM}
${PLATFORM}:
	make -C vivado_impl DESIGN=${DESIGN}

# Step-2: Compile AIE code and generate libadf.a
adf: ${ADF}
${ADF}:
	make -C aie ADF=${ADF}

#Step-3: Compile pl kernels 
xos: ${XOS}
${XOS}:
	make -C pl_kernels ${XOS} 

#Step-4: Export vma file using v++ link
vma: ${VMA}
${VMA}:
	make -C vitis_impl VMA=${VMA}

#Step-5: Import VMA in Vivado, run implementation and generate fixed.xsa
import: 
	vivado -mode batch -source ./vivado_impl/export.tcl -tclargs ${DESIGN} ${VMA}
	
	
clean:
	make clean -C vitis_impl
	make clean -C vivado_impl
	make clean -C pl_kernels
	make clean -C aie
	rm -rf vivado.* .Xil _x sd_card package .crashReporter {DESIGN}
	rm -rf v++* xsa .gen flat *.log BOOT.BIN
	rm -rf *.xclbin *.bin *.img *.xclbin.package_summary *.bif *.xsa
