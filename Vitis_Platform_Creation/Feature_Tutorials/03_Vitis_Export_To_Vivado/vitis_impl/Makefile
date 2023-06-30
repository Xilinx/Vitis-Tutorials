# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */
PLATFORM =../vivado_impl/flat_wa.xsa 
XOS      =../pl_kernels/*.xo
AIELIB   =../aie/libadf.a
TARGET   =hw
VMA      =${TARGET}.vma
VCC      =v++
VPP_LINK_SPEC =system.cfg
VPP_LINK_FLAGS=--save-temps --export_archive --config ${VPP_LINK_SPEC}

.PHONY: clean vma

vma: ${VMA}
${VMA}: ${XOS} ${VPP_LINK_SPEC} ${AIELIB} ${PLATFORM}
	${VCC} -l --platform ${PLATFORM} ${XOS} ${AIELIB} ${VPP_LINK_FLAGS} -o $@

clean:
	rm -rf _x v++_* ${TARGET}.* .Xil *.log ${VMA}
	rm -rf *.vma *.vma.link_summary vitis_impl .crashReporter .ipcache
