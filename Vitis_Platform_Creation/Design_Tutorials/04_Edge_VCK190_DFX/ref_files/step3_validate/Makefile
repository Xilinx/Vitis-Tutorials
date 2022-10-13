# Copyright 2020 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
XSA_NAME=vck190_custom_dfx
PLATFORM=$(ROOT_DIR)/../step2_sw/build/pfm/$(XSA_NAME)/export/$(XSA_NAME)/$(XSA_NAME).xpfm
COMMON_IMAGE_VERSAL ?= $(ROOT_DIR)/../step2_sw/build/sw_comp/
SYSROOT_VERSAL ?= $(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-xilinx-linux


PL_EXAMPLE_PATH=Vitis_Accel_Examples/cpp_kernels/simple_vadd/
AIE_EXAMPLE_PATH=Vitis_Accel_Examples/aie_kernels/aie_adder/

.PHONY: all vadd aie_adder platforminfo clean ultraclean
#all: platforminfo vadd aie_adder 
all: platforminfo vadd aie_adder vadd_hw


Vitis_Accel_Examples:
	git clone https://github.com/Xilinx/Vitis_Accel_Examples.git
	cd Vitis_Accel_Examples/ && git checkout 2022.1 && sed -i "s/vck190_base_dfx/dfx/g" cpp_kernels/simple_vadd/makefile_versal_ps.mk
# Modify Vitis_Accel_Examples to allow running vadd on DFX platforms
	

vadd: Vitis_Accel_Examples
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw_emu PLATFORM=$(PLATFORM) EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT_VERSAL)
	$(MAKE) -C $(PL_EXAMPLE_PATH) test TARGET=hw_emu PLATFORM=$(PLATFORM) EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT_VERSAL)

# vadd_hw is a place-holder for manual build and test on hardware
# In this flow, vadd_hw and vadd are mutually exclusive. 
# Please run `make clean` before 
vadd_hw: Vitis_Accel_Examples
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw PLATFORM=$(PLATFORM) EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT_VERSAL)

vadd_hw_pack: Vitis_Accel_Examples
	$(MAKE) -C $(PL_EXAMPLE_PATH) sd_card TARGET=hw PLATFORM=$(PLATFORM) EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL)


aie_adder: 
	$(MAKE) -C $(AIE_EXAMPLE_PATH) run TARGET=hw_emu DEVICE=$(PLATFORM) HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT_VERSAL)

aie_adder_hw: 
	$(MAKE) -C $(AIE_EXAMPLE_PATH) run TARGET=hw DEVICE=$(PLATFORM) HOST_ARCH=aarch64 EDGE_COMMON_SW=$(COMMON_IMAGE_VERSAL) SYSROOT=$(SYSROOT_VERSAL)

platforminfo: platforminfo.txt

platforminfo.txt: $(PLATFORM)
	platforminfo $(PLATFORM) > platforminfo.txt

clean:
	$(MAKE) -C $(PL_EXAMPLE_PATH) cleanall
	$(MAKE) -C $(AIE_EXAMPLE_PATH) cleanall

ultraclean:
	rm -rf Vitis_Accel_Examples
