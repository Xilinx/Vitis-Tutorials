#******************************************************************************
# Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#******************************************************************************

include platform.mk

TCL_SOURCE=hw/my_project_changes.tcl
TCL_SOURCE_1=hw/import_vma_hw.tcl

TARGET := hw

.PHONY: all xsa adf xos vma vma_import host package run clean

all: xsa viv_changes adf xos vma vma_import host package

xsa: $(XSA)

$(XSA):
	$(MAKE) -C hw all

viv_changes:
	$(VIVADO) -mode batch -notrace -source $(TCL_SOURCE) 

adf:
	$(MAKE) -C aie all

xos: 
	$(MAKE) -C pl_kernels all

vma:
	$(MAKE) -C vitis_impl vma


vma_import: $(TCL_SOURCE_1)
	$(VIVADO) -mode batch -notrace -source $(TCL_SOURCE_1) 



HOST_SRC = sw/host.cpp 
HOST_EXE = host.exe
GCC_FLAGS := -Wall -c -std=c++17 -Wno-int-to-pointer-cast --sysroot=${SDKTARGETSYSROOT} 
GCC_INCLUDES := -I$(SDKTARGETSYSROOT)usr/include/xrt -I$(SDKTARGETSYSROOT)usr/include 
GCC_LIB := -lxrt_coreutil --sysroot=${SDKTARGETSYSROOT} 


# Host rules
host: $(HOST_SRC)
	mkdir -p host_code_dir
	$(CXX)  $(GCC_FLAGS) $(GCC_INCLUDES) $(HOST_SRC)  
	$(CXX) *.o $(GCC_LIB) -std=c++17 $(GCC_LIB) -o $(HOST_EXE) 
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
	$(MAKE) -C aie clean
	$(MAKE) -C pl_kernels clean
	$(MAKE) -C vitis_impl clean
	rm -rf host_code_dir package.hw package.hw_emu _x *.hwdef
	rm host.exe host.o *.xclbin *.log *.jou *.package_summary *.pb
