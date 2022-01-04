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
ABS_BUILD_PATH=$(ROOT_DIR)/build
BSP=xilinx-k26-starterkit-v2021.1-final.bsp
PETALINUX_PROJ_NAME=xilinx-k26-starterkit-2021.1
PETALINUX_DIR=$(ABS_BUILD_PATH)/$(PETALINUX_PROJ_NAME)
BD_TCL=system_step0.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)
PETALINUX_CONFIG=$(PETALINUX_DIR)/project-spec/configs/config
PETALINUX_ROOTFS_CONFIG=$(PETALINUX_DIR)/project-spec/configs/rootfs_config

.PHONY: all check_bsp get_xsa clean dt dtbo petalinux_proj petalinux_build petalinux_build_sdk

all: check_bsp get_xsa petalinux_proj petalinux_build petalinux_build_sdk dtbo

check_bsp: $(BSP)

$(BSP):
ifneq (,$(wildcard $(BSP)))
	@echo "INFO: BSP found. Start buliding software components."
else
	@echo "ERROR: Please download KV260 Starter Kit BSP from https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/1641152513/Kria+K26+SOM to step2_petalinux directory."
	exit
endif

get_xsa:
	cd $(ROOT_DIR); cp ../step1_vivado/build/vivado/*.xsa .


dtg:
	git clone https://github.com/Xilinx/device-tree-xlnx.git -b xlnx_rel_v2021.1 dtg


dt: dt_output/pl.dtsi

dt_output/pl.dtsi: dtg
	@echo "INFO: Generating device tree from XSA" 
	xsct gen_dt.tcl && rm psu_*

# Compile dt overlay for PL
dtbo: dt_output/pl.dtsi
	sed -i 's/firmware-name = "kv260_custom_platform.bit.bin"/firmware-name = "vadd.bit.bin"/g' dt_output/pl.dtsi
	dtc -@ -O dtb -o dt_output/pl.dtbo dt_output/pl.dtsi
	@echo	
	@echo "INFO: Device tree generated in dt_output directory"


petalinux_proj: $(BSP)
	mkdir -p $(ABS_BUILD_PATH);cd $(ABS_BUILD_PATH); petalinux-create -t project -s ../$(BSP)
	echo "CONFIG_YOCTO_BUILDTOOLS_EXTENDED=y" >> $(PETALINUX_CONFIG)
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_rootfs_config

update_rootfs_config:
	# PetaLinux Rootfs: Enable XRT
	echo 'CONFIG_xrt=y' >> $(PETALINUX_ROOTFS_CONFIG)

petalinux_build:
	petalinux-build -p $(PETALINUX_DIR)
	cd $(PETALINUX_DIR) && petalinux-package --boot --u-boot

petalinux_build_sdk:
	petalinux-build -p $(PETALINUX_DIR) --sdk


clean:
	$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou *.xsa dt_output dtg
