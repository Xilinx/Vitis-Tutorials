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
ABS_SW_PATH=$(ABS_BUILD_PATH)/$(PETALINUX_PROJ_NAME)
BD_TCL=run.tcl
ABS_BD_TCL=$(ROOT_DIR)/$(BD_TCL)

XSA_NAME=vck190_custom_dfx
ABS_XSA=$(ABS_HW_PATH)/$(XSA_NAME)_static.xsa
ABS_XSA_RP=$(ABS_HW_PATH)/$(XSA_NAME)_rp.xsa
ABS_XSA_HWEMU=$(ABS_HW_PATH)/$(XSA_NAME)_hw_emu.xsa

.PHONY: all hw petalinux clean

all: hw

# target: hw
# Build bare minial Vivado design and export XSA
hw: $(ABS_BD_TCL)
	mkdir -p $(ABS_HW_PATH)
	cd $(ABS_HW_PATH); vivado -mode batch -notrace -source $(ABS_BD_TCL)

hw_proj: $(ABS_BD_TCL)
	mkdir -p $(ABS_HW_PATH)
	cd $(ABS_HW_PATH); vivado -mode tcl -notrace -source $(ROOT_DIR)/create_project.tcl


clean:
	$(RM) -r $(ABS_HW_PATH)/myproj $(ABS_XSA) $(ABS_XSA_RP) $(ABS_XSA_HWEMU)

cleanlog:
	$(RM) $(ABS_HW_PATH)/vivado*.log $(ABS_HW_PATH)/vivado*.jou 
