#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

all: linker packager
TARGET                ?= hw_emu
MODE	 = linux
ARCH = aie

ifeq ($(ARCH), aie)
	PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202520_1/xilinx_vck190_base_202520_1.xpfm
else ifeq ($(ARCH), aie-ml)
	PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vek280_base_202520_1/xilinx_vek280_base_202520_1.xpfm
else
$(error ARCH is not supported)
endif

# Rootfs and Image
export IMAGE  		= ${COMMON_IMAGE_VERSAL}/Image
export ROOTFS           = ${COMMON_IMAGE_VERSAL}/rootfs.ext4

s2mm=../HLS_Kernels/s2mm/s2mm.xo
mm2s=../HLS_Kernels/mm2s/mm2s.xo
libadf=../AIE_Kernel/output_dir/libadf.a

linker: $(s2mm) $(mm2s) $(libadf)
	v++ --link --debug --save-temps \
			--target ${TARGET} \
			--platform  ${PLATFORM} \
			--config ./system.cfg \
			../HLS_Kernels/s2mm/s2mm.xo ../HLS_Kernels/mm2s/mm2s.xo \
			../AIE_Kernel/output_dir/libadf.a 
 
packager:
	v++ --package --target ${TARGET}  \
	--platform  ${PLATFORM} \
	--config pack.cfg \
	--package.defer_aie_run \
	--package.kernel_image=${IMAGE} \
	--package.rootfs=${ROOTFS}  \
	../AIE_Kernel/output_dir/libadf.a \
	./a.xsa -o a.xclbin

run: 
	cd pack_out_dir;	./launch_hw_emu.sh


clean:
	rm v++*
	rm ./pack_out_dir -rf
	rm -rf _x _ide
	rm a.*
	rm xcd.log
	rm vpp_link.log
	rm vivado.*
