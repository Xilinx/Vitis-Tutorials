#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
XSA_NAME=vck190_custom_dfx
PLATFORM=$(ROOT_DIR)/../step2_sw/build/pfm/$(XSA_NAME)/export/$(XSA_NAME)/$(XSA_NAME).xpfm
COMMON_IMAGE ?= $(ROOT_DIR)/../step2_sw/build/sw_comp/
SYSROOT ?= $(ROOT_DIR)/../step2_sw/build/sw_comp/sysroot/sysroots/cortexa72-cortexa53-xilinx-linux/


PL_EXAMPLE_PATH=Vitis_Accel_Examples/cpp_kernels/simple_vadd/
AIE_EXAMPLE_PATH=Vitis_Accel_Examples/aie_kernels/aie_adder/

.PHONY: all vadd aie_adder platforminfo clean ultraclean
#all: To support emulation
all: platforminfo vadd  
sd_card:platforminfo vadd_hw aie_adder_hw 


Vitis_Accel_Examples:
	git clone https://github.com/Xilinx/Vitis_Accel_Examples.git
	cd Vitis_Accel_Examples/  && sed -i "s/vck190_base_dfx/vck190_custom_dfx/g"  aie_kernels/aie_adder/makefile_versal_ps.mk
# Modify Vitis_Accel_Examples to allow running vadd on DFX platforms
	

vadd: Vitis_Accel_Examples  
	cd Vitis_Accel_Examples/ && sed -i '116 i\VPP_FLAGS += --config system.cfg' cpp_kernels/simple_vadd/makefile_versal_ps.mk
	cp system.cfg workaround.tcl Vitis_Accel_Examples/cpp_kernels/simple_vadd/
	$(MAKE) -C $(PL_EXAMPLE_PATH) run TARGET=hw_emu PLATFORM=$(PLATFORM) DEV_ARCH=versal EDGE_COMMON_SW=$(COMMON_IMAGE) SYSROOT=$(SYSROOT)

# vadd_hw is a place-holder for manual build and test on hardware
# In this flow, vadd_hw and vadd are mutually exclusive. 
# Please run `make clean` before 
vadd_hw: Vitis_Accel_Examples
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) DEV_ARCH=versal EDGE_COMMON_SW=$(COMMON_IMAGE) SYSROOT=$(SYSROOT)

vadd_hw_package: Vitis_Accel_Examples
	$(MAKE) -C $(PL_EXAMPLE_PATH) sd_card TARGET=hw PLATFORM=$(PLATFORM) DEV_ARCH=versal EDGE_COMMON_SW=$(COMMON_IMAGE)


#aie_adder:Vitis_Accel_Examples 
#	cd Vitis_Accel_Examples/ && sed -i '8 i\[advanced] param=compiler.userPostSysLinkOverlayTcl="workaround.tcl"' aie_kernels/aie_adder/system.cfg
#	cp  workaround.tcl Vitis_Accel_Examples/aie_kernels/aie_adder/
#	$(MAKE) -C $(AIE_EXAMPLE_PATH) run TARGET=hw_emu PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE) SYSROOT=$(SYSROOT)

aie_adder_hw: Vitis_Accel_Examples 
	$(MAKE) -C $(AIE_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) DEV_ARCH=versal HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE) SYSROOT=$(SYSROOT)

platforminfo: platforminfo.txt

platforminfo.txt: $(PLATFORM)
	platforminfo $(PLATFORM) > platforminfo.txt

clean:
	$(MAKE) -C $(PL_EXAMPLE_PATH) cleanall
	$(MAKE) -C $(AIE_EXAMPLE_PATH) cleanall

ultraclean:
	rm -rf Vitis_Accel_Examples
