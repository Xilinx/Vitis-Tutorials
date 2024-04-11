# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */
PLATFORM =../hw/build/my_project/vck190_prj_ext_hw.xsa
XOS      =../pl_kernels/polar_clip.xo
AIELIB   =../aie/libadf.a
TARGET   =hw
VMA      =vitis_design_${TARGET}.vma
VCC      =v++
VPP_LINK_SPEC =system.cfg
VPP_LINK_FLAGS=--save-temps --export_archive --config ${VPP_LINK_SPEC}

.PHONY: clean vma

vma: ${VMA}
${VMA}: 
	${VCC} -l --platform ${PLATFORM} ${XOS} ${AIELIB} ${VPP_LINK_FLAGS} -o $@

clean:
	rm -rf _x v++_* ${TARGET}.* .Xil *.log ${VMA}
	rm -rf *.vma *.vma.link_summary vitis_impl .crashReporter .ipcache
