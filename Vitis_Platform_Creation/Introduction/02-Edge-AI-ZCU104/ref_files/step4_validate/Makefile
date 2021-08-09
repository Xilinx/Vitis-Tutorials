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
PLATFORM = $(ROOT_DIR)/../step3_pfm/platform_repo/zcu104_custom_platform/export/zcu104_custom_platform/zcu104_custom_platform.xpfm
SW_COMP = $(ROOT_DIR)/../step3_pfm/sw_comp/

PL_EXAMPLE_PATH = Vitis_Accel_Examples/cpp_kernels/simple_vadd/

.PHONY: all vadd platforminfo clean ultraclean
all: vadd platforminfo


Vitis_Accel_Examples:
	git clone https://github.com/Xilinx/Vitis_Accel_Examples.git

$(PL_EXAMPLE_PATH)/krnl_vadd.xclbin: Vitis_Accel_Examples $(PLATFORM)
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw_emu DEVICE=$(PLATFORM) HOST_ARCH=aarch64 EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SW_COMP)/sysroots/cortexa72-cortexa53-xilinx-linux

vadd: $(PL_EXAMPLE_PATH)/krnl_vadd.xclbin $(PLATFORM)
	$(MAKE) -C $(PL_EXAMPLE_PATH) test TARGET=hw_emu DEVICE=$(PLATFORM) HOST_ARCH=aarch64 EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SW_COMP)/sysroots/cortexa72-cortexa53-xilinx-linux

vadd_hw: 
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw DEVICE=$(PLATFORM) HOST_ARCH=aarch64 EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SW_COMP)/sysroots/cortexa72-cortexa53-xilinx-linux

platforminfo:
	$(MAKE) -C platforminfo

clean:
	$(MAKE) -C $(PL_EXAMPLE_PATH) clean

ultraclean:
	rm -rf Vitis_Accel_Examples