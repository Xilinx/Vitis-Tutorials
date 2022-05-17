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
# PL KERNEL NAMES
# =========================================================
DLBF_DATA 	:= dlbf_data
DLBF_COEFFS 	:= dlbf_coeffs
DLBF_SLAVE 	:= dlbf_slave
ULBF_DATA 	:= ulbf_data
ULBF_COEFFS 	:= ulbf_coeffs
ULBF_SLAVE 	:= ulbf_slave
AXI4S_REGSLICE 	:= axi4s_regslice_64b

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR := build
DLBF_DATA_XO 		:= $(DLBF_DATA).xo
DLBF_COEFFS_XO 		:= $(DLBF_COEFFS).xo
DLBF_SLAVE_XO 		:= $(DLBF_SLAVE).xo
ULBF_DATA_XO 		:= $(ULBF_DATA).xo
ULBF_COEFFS_XO 		:= $(ULBF_COEFFS).xo
ULBF_SLAVE_XO 		:= $(ULBF_SLAVE).xo
AXI4S_REGSLICE_XO 	:= $(AXI4S_REGSLICE).xo

kernels: $(BUILD_DIR)/$(DLBF_DATA_XO) 	\
	$(BUILD_DIR)/$(DLBF_COEFFS_XO)	\
	$(BUILD_DIR)/$(DLBF_SLAVE_XO) 	\
	$(BUILD_DIR)/$(ULBF_DATA_XO) 	\
	$(BUILD_DIR)/$(ULBF_COEFFS_XO)	\
	$(BUILD_DIR)/$(ULBF_SLAVE_XO)	\
	$(BUILD_DIR)/$(AXI4S_REGSLICE_XO)

$(BUILD_DIR)/$(DLBF_DATA_XO):
	$(MAKE) kernel -C $(DLBF_DATA)

$(BUILD_DIR)/$(DLBF_COEFFS_XO): 
	$(MAKE) kernel -C $(DLBF_COEFFS)

$(BUILD_DIR)/$(DLBF_SLAVE_XO):
	$(MAKE) kernel -C $(DLBF_SLAVE)

$(BUILD_DIR)/$(ULBF_DATA_XO):
	$(MAKE) kernel -C $(ULBF_DATA)

$(BUILD_DIR)/$(ULBF_COEFFS_XO):
	$(MAKE) kernel -C $(ULBF_COEFFS)

$(BUILD_DIR)/$(ULBF_SLAVE_XO):	
	$(MAKE) kernel -C $(ULBF_SLAVE)

$(BUILD_DIR)/$(AXI4S_REGSLICE_XO):
	$(MAKE) kernel -C $(AXI4S_REGSLICE)

clean:
	$(MAKE) clean -C $(DLBF_DATA)
	$(MAKE) clean -C $(DLBF_COEFFS)
	$(MAKE) clean -C $(ULBF_DATA)
	$(MAKE) clean -C $(ULBF_COEFFS)
	$(MAKE) clean -C $(DLBF_SLAVE)
	$(MAKE) clean -C $(ULBF_SLAVE)
	$(MAKE) clean -C $(AXI4S_REGSLICE)
