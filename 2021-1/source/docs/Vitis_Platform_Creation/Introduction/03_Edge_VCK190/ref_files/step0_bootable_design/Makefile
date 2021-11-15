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
ABS_HW_PATH=$(ABS_BUILD_PATH)/vivado
PETALINUX_PROJ_NAME=petalinux
ABS_SW_PATH=$(ABS_BUILD_PATH)/$(PETALINUX_PROJ_NAME)
BD_TCL=run.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)
PETALINUX_DIR=$(ABS_SW_PATH)
PETALINUX_CONFIG=$(PETALINUX_DIR)/project-spec/configs/config
ABS_XSA=$(ABS_HW_PATH)/vck190_custom.xsa

.PHONY: all hw petalinux clean

all: hw

# target: hw
# Build bare minial Vivado design and export XSA
hw: $(ABS_BD_TCL)
	mkdir -p $(ABS_HW_PATH)
	cd $(ABS_HW_PATH); vivado -mode batch -notrace -source $(ABS_BD_TCL) -tclargs VCK190

# target: petalinux
# Use Vivado exported XSA to create PetaLinux design
# Copy the generated boot.bin and image.ub to SD card FAT32 partition, it should boot properly
petalinux: $(ABS_XSA)
	# Create PetaLinux project
	cd $(ABS_BUILD_PATH); petalinux-create -t project --template versal --force -n $(PETALINUX_PROJ_NAME)
	# Import XSA  
	cd $(ABS_SW_PATH);	petalinux-config --get-hw-description=$(ABS_HW_PATH) --silentconfig
	# Update device tree for VCK190
	echo 'CONFIG_SUBSYSTEM_MACHINE_NAME="versal-vck190-reva-x-ebm-02-reva"' >> $(PETALINUX_CONFIG)
	cd $(ABS_SW_PATH);	petalinux-config --silentconfig
	# Build
	petalinux-build -p $(ABS_SW_PATH) && \
	petalinux-package --boot --u-boot -p $(ABS_SW_PATH)

clean:
	$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou 
