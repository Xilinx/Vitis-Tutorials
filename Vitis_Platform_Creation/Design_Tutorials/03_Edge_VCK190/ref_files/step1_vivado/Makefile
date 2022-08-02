# Copyright 2022 Xilinx Inc.
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
BD_TCL=run.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)

.PHONY: help

help:
	@echo "Makefile usage"
	@echo "make all 	## to generate hw platform and export XSA file"


all: hw petalinuximage
#petalinuximage

#generate the XSA file 
hw:
	mkdir -p $(ABS_HW_PATH)
	cd $(ABS_HW_PATH); vivado -mode batch -notrace -source $(ABS_BD_TCL) -source 
#Generate petalinux image for validation
petalinuximage:
	petalinux-create -t project --template versal --force -n petalinux
	cd petalinux && petalinux-config --get-hw-description=../build/vivado/xsa_file_for_petalinux.xsa  --silentconfig 
	cp -f ../step2_pfm/system-user.dtsi petalinux/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi 
	cd petalinux &&  petalinux-build && petalinux-package --boot --u-boot

clean:
	$(RM) -r $(ABS_BUILD_PATH) vivado*.log vivado*.jou petalinux petalinux.old

ultraclean: clean