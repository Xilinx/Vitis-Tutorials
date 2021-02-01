# © Copyright 2020 Xilinx, Inc.
# 
# Licensed under the Apache License, Version 2.0 (the 'License');
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an 'AS IS' BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


# =========================================================
# PL KERNEL NAME
# =========================================================
KERNEL_NAME := dlbf_slave

# =========================================================
# DEVICE SELECTION
# =========================================================
DEVICE_NAME := xcvc1902-vsva2197-1LP-e-S-es1

# =========================================================
# XO CREATION SCRIPT
# =========================================================
CREATE_XO_TCL := run_$(KERNEL_NAME).tcl

# =========================================================
# DEPENDENCIES
# =========================================================
DEPS := $(CREATE_XO_TCL)
DEPS += bd_$(KERNEL_NAME).tcl
DEPS += kernel_$(KERNEL_NAME).xml 
DEPS += $(shell find ./hdl -type f -name "*.v")

# =========================================================
# COMMAND OPTIONS
# =========================================================
RUN_OPT := NO_SIM

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR := ../build
KERNEL_XO := $(KERNEL_NAME).xo

kernel: $(BUILD_DIR)/$(KERNEL_NAME).xo
	echo "[Info] All complete - xo $< exists"

$(BUILD_DIR)/$(KERNEL_NAME).xo: $(DEPS)
	vivado -mode batch -source $(CREATE_XO_TCL) -tclargs $(RUN_OPT) $(DEVICE_NAME)

clean:
	rm -rf vivado*.log
	rm -rf vivado*.jou
	rm -rf project_$(KERNEL_NAME) kernel_$(KERNEL_NAME)_auto.xml
	rm -rf .Xil
	rm -rf $(BUILD_DIR)/$(KERNEL_NAME) 
	rm -rf $(BUILD_DIR)/$(KERNEL_NAME).xo
	$(MAKE) clean -C sim
