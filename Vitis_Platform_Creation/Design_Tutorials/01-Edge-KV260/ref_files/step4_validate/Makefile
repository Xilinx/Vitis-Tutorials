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
XSA_NAME=kv260_custom_platform
PLATFORM = $(ROOT_DIR)/../step3_pfm/platform_repo/$(XSA_NAME)/export/$(XSA_NAME)/$(XSA_NAME).xpfm
SW_COMP = $(ROOT_DIR)/../step3_pfm/sw_comp/

PL_EXAMPLE_PATH = Vitis_Accel_Examples/cpp_kernels/simple_vadd/

.PHONY: all vadd_hw platforminfo clean ultraclean
all: platforminfo vadd_hw_package


Vitis_Accel_Examples:
	git clone https://github.com/Xilinx/Vitis_Accel_Examples.git

# Quick command for generating vadd hw xclbin
vadd_hw: $(PL_EXAMPLE_PATH)/krnl_vadd.xclbin

# vadd_hw generates the files for running on hardware. User needs to pick the files required by KV260 and transfer them to the board
# Do not use the generated sd_card.img
$(PL_EXAMPLE_PATH)/krnl_vadd.xclbin: Vitis_Accel_Examples $(PLATFORM) 
	$(MAKE) -C $(PL_EXAMPLE_PATH) all TARGET=hw DEVICE=$(PLATFORM) HOST_ARCH=aarch64 EDGE_COMMON_SW=$(SW_COMP) SYSROOT=$(SW_COMP)/sysroots/cortexa72-cortexa53-xilinx-linux

# Package #1: Convert bit to bit.bin
$(PL_EXAMPLE_PATH)/vadd.bit.bin: $(PL_EXAMPLE_PATH)/krnl_vadd.xclbin
	cd $(PL_EXAMPLE_PATH) && cp ./_x.hw.kv260_custom_platform/link/int/system.bit .
	cd $(PL_EXAMPLE_PATH) && echo "all:{system.bit}" > bootgen.bif
	cd $(PL_EXAMPLE_PATH) && bootgen -w -arch zynqmp -process_bitstream bin -image bootgen.bif
	cd $(PL_EXAMPLE_PATH) && mv system.bit.bin vadd.bit.bin

$(PL_EXAMPLE_PATH)/vadd.dtbo:
	cp ../step2_petalinux/dt_output/pl.dtbo $(PL_EXAMPLE_PATH)/vadd.dtbo

$(PL_EXAMPLE_PATH)/shell.json:
	cd $(PL_EXAMPLE_PATH) && printf '{\n  "shell_type" : "XRT_FLAT",\n  "num_slots": "1"\n}' > shell.json

vadd_hw_package: $(PL_EXAMPLE_PATH)/vadd.bit.bin $(PL_EXAMPLE_PATH)/vadd.dtbo $(PL_EXAMPLE_PATH)/shell.json
	@echo "INFO: Packaging Completed"
	@echo "Please copy the following files from $(PL_EXAMPLE_PATH) to /lib/firmware/xilinx/vadd on target board and use xmutil to load hardware."
	@echo "- vadd.bit.bin"
	@echo "- vadd.dtbo"
	@echo "- shell.json"
	@echo "Please copy the acceleation application files from $(PL_EXAMPLE_PATH) to target board and run the host application."
	@echo "- simple_vadd"
	@echo "- krnl_vadd.xclbin"
	@echo "$ ./simple_vadd ./krnl_vadd.xclbin"



platforminfo:
	$(MAKE) -C platforminfo

clean:
	$(MAKE) -C $(PL_EXAMPLE_PATH) clean

ultraclean:
	rm -rf Vitis_Accel_Examples
