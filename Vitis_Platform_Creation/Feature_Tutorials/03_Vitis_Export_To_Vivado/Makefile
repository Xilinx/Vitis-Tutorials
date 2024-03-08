#******************************************************************************
# Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#******************************************************************************

include platform.mk

TARGET := hw_emu

.PHONY: adf xsa xos vma host package run clean

xsa: $(XSA)

$(XSA):
	$(MAKE) -C hw all


adf:
	$(MAKE) -C aie all

xos: 
	$(MAKE) -C pl_kernels all

vma:
	$(MAKE) -C vitis_impl vma

HOST_CFLAGS = -Isrc -Isw -Iaie -ladf_api_xrt -lxrt_coreutil -D__SYNCBO_ENABLE__ $(TARGET_FLAGS) -DTARGET_DEVICE=\"$(DEVICE)\"  -std=c++17 -O0 -g -Wall -fmessage-length=0 -I${GITHUB_COMMON_REPO}/common/includes/xcl2 --sysroot=${SYSROOT} -I=/usr/include -I=/usr/include/xrt -L=/usr/lib -ldl -lcrypt -lstdc++ -lxilinxopencl -lpthread -lrt -I$(XILINX_VITIS)/aietools/include -L$(XILINX_VITIS)/aietools/lib/aarch64.o -I${XILINX_VITIS}/include -I${XILINX_HLS}/include
HOST_SRC = sw/host.cpp aie/Work/ps/c_rts/aie_control_xrt.cpp
HOST_EXE = host.exe


# Host rules
host: $(HOST_SRC)
	mkdir -p host_code_dir
	$(CXX) $+ -o $(HOST_EXE) $(HOST_CFLAGS)
	@echo "INFO: Compiled Host Executable: $(HOST_EXE)"

EMBEDDED_EXEC_SCRIPT = embedded_exec.sh
EMCONFIGUTIL = emconfigutil

##################################################################################################################################################
## Package
##################################################################################################################################################

ifeq ($(TARGET),hw_emu)

package:
	echo "INFO: Generating ${EMCONFIG_FILE}";\
	${EMCONFIGUTIL} --platform ./hw/build/my_project/vck190_prj_fixed_hw_emu.xsa --nd 1;\
	cd ./sw
	v++ -p -t $(TARGET) -f ./hw/build/my_project/vck190_prj_fixed_hw_emu.xsa \
	--package.sd_dir ${PLATFORM_REPO_PATHS}/sw/versal/aie ./aie/libadf.a \
	--package.out_dir package.hw_emu \
	--package.rootfs ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/rootfs.ext4 \
	--package.sd_file ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/Image \
	--package.sd_file emconfig.json \
	--package.bl31_elf ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_bdc_202410_1/sw/xilinx_vck190_base_bdc_202410_1/boot/bl31.elf \
	--package.boot_mode sd \
	--package.uboot ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_bdc_202410_1/sw/xilinx_vck190_base_bdc_202410_1/boot/u-boot.elf \
	--package.sd_file ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/boot.scr \
	--package.sd_file host.exe \
	--package.sd_file embedded_exec.sh \
	--package.defer_aie_run \
	--package.dtb ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_bdc_202410_1/sw/xilinx_vck190_base_bdc_202410_1/boot/system.dtb \
	-o vck190_prj_hw_emu.xclbin 

else

package:
	v++ -p -t $(TARGET) -f ./hw/build/my_project/vck190_prj_fixed_hw.xsa \
	--package.sd_dir ${PLATFORM_REPO_PATHS}/sw/versal/aie ./aie/libadf.a \
	--package.out_dir package.hw \
	--package.rootfs ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/rootfs.ext4 \
	--package.sd_file ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/Image \
	--package.bl31_elf ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_bdc_202410_1/sw/xilinx_vck190_base_bdc_202410_1/boot/bl31.elf \
	--package.boot_mode sd \
	--package.uboot ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_bdc_202410_1/sw/xilinx_vck190_base_bdc_202410_1/boot/u-boot.elf \
	--package.sd_file ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/boot.scr \
	--package.sd_file host.exe \
	--package.defer_aie_run \
	--package.dtb ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_bdc_202410_1/sw/xilinx_vck190_base_bdc_202410_1/boot/system.dtb \
	-o vck190_prj_hw.xclbin 


endif
#########################################################################################################################################################
 
run:
	@echo "INFO: Running Embedded Emulation: ${TARGET}"
	./package.hw_emu/launch_hw_emu.sh -verbose -no-reboot -login "petalinux" -password "petalinux" -run-app embedded_exec.sh | tee embedded_run.log


clean:
	$(MAKE) -C hw clean
	$(MAKE) -C sw clean
	${RM} -r $(PLATFORM_DIR)
