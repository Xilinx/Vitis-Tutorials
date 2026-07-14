#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
XSA_NAME=vek280_custom_dfx
CUSTOM_PLATFORM=$(ROOT_DIR)/../step2_sw/$(XSA_NAME)/export/$(XSA_NAME)/$(XSA_NAME).xpfm
VERSION=2026.1
COMMON_IMAGE_VERSAL ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/
SYSROOT=$(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-amd-linux/



VADD_WORK=$(ROOT_DIR)/vadd_work
AIE_WORK=$(ROOT_DIR)/aie_work

.PHONY: all vadd aie_adder platforminfo clean ultraclean
#all: To support emulation
all: platforminfo vadd_emu aie_adder_emu  
sd_card:platforminfo vadd_hw aie_adder_hw 


cpAieCode:
	rm -rf aie_work
	mkdir -p aie_work
	cp -rf $(XILINX_VITIS)/samples/aie_system_examples/aie-ml_sys_design/* aie_work/
	cp makefile_aieml aie_work/Makefile
	@sed -i 's|auto xclbin_uuid = device.load_xclbin(xclbinFilename);|auto xclbin_uuid = device.load_xclbin(xclbinFilename);\n    auto kernel = xrt::kernel(device, xclbin_uuid, "mm2s_8_128");\n    auto bank_grp_arg0 = kernel.group_id(0);|' aie_work/src/host.cpp
	@sed -i 's|xrt::bo(device, pl_bramA_size_in_bytes, 0, 0)|xrt::bo(device, pl_bramA_size_in_bytes, bank_grp_arg0)|g' aie_work/src/host.cpp
	@sed -i 's|xrt::bo(device, pl_bramB_size_in_bytes, 0, 0)|xrt::bo(device, pl_bramB_size_in_bytes, bank_grp_arg0)|g' aie_work/src/host.cpp
	@sed -i 's|xrt::bo(device, pl_bramC1_size_in_bytes, 0, 0)|xrt::bo(device, pl_bramC1_size_in_bytes, bank_grp_arg0)|g' aie_work/src/host.cpp
	@sed -i 's|xrt::bo(device, pl_bramC2_size_in_bytes, 0, 0)|xrt::bo(device, pl_bramC2_size_in_bytes, bank_grp_arg0)|g' aie_work/src/host.cpp
	@sed -i 's|int host_ref_32b\[OUTPUT_SIZEC\];|int host_ref_32b[OUTPUT_SIZEC] = {0};|; s|int host_ref_16b\[OUTPUT_SIZEC\];|int host_ref_16b[OUTPUT_SIZEC] = {0};|' aie_work/src/host.cpp

cpVaddCode:
	mkdir -p vadd_work
	cp -rf $(XILINX_VITIS)/samples/vadd/* vadd_work/
	cp makefile_vadd vadd_work/Makefile




vadd_emu: cpVaddCode 
	$(MAKE) -C $(VADD_WORK) run TARGET=hw_emu PLATFORM=$(CUSTOM_PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/

# vadd_hw is a place-holder for manual build and test on hardware
vadd_hw: cpVaddCode 
	$(MAKE) -C $(VADD_WORK) sd_card TARGET=hw PLATFORM=$(CUSTOM_PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/
aie_adder_emu: cpAieCode
	$(MAKE) -C $(AIE_WORK) all TARGET=hw_emu PLATFORM=$(CUSTOM_PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/
	@echo "INFO: DFX hw_emu build complete. Launch the emulator manually with:"
	@echo "      $(AIE_WORK)/package.hw_emu/launch_hw_emu.sh -run-app run_aieml.sh"

aie_adder_hw: cpAieCode
	$(MAKE) -C $(AIE_WORK) sd_card TARGET=hw PLATFORM=$(CUSTOM_PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT)/


clean:
	rm -rf $(DTB_PLT_NAME) $(PLATFORM_NAME)  *journal.py __pycache__ logs ip_cache vitis_journal* 
	-$(MAKE) -C $(VADD_WORK) clean
	-$(MAKE) -C $(AIE_WORK) clean
	rm -rf ./analyzer_output
	rm -rf *.jou *.link_summary _x .Xil *.txt
	rm -rf aie_work vadd_work *.log
	rm -rf analyzer_input Work

ultraclean:clean
	rm -rf $(ABS_BUILD_PATH) device-tree-xlnx/ logs tmp
	rm -rf  $(AIE_WORK)
	rm -rf $(VADD_WORK) 
