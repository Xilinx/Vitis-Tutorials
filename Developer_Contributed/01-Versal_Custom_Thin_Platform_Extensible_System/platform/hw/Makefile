# © Copyright 2020 Xilinx, Inc.
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

# =========================================================
# DEVICE SELECTION
# =========================================================
DEVICE_NAME ?= xcvc1902-vsva2197-2MP-e-S-es1

# =========================================================
# PLATFORM NAME
# =========================================================
PLATFORM_CUSTOM ?= you_forgot_to_set_platform_name
PRE_SYNTH ?=true

# =========================================================
# XSA NAME
# =========================================================
XSA ?= ${PLATFORM_CUSTOM}.xsa

# =========================================================
# SOURCE DIRECTORIES
# =========================================================
SRC_DIR := src
QOR_SCRIPTS_DIR := $(SRC_DIR)/qor_scripts

# =========================================================
# XSA CREATION SCRIPT
# =========================================================
CREATE_XSA_TCL := platform_xsa.tcl

# =========================================================
#  BOARD PART SELECTION (ES1/Production)
# =========================================================
ifeq ($(DEVICE_NAME), xcvc1902-vsva2197-2MP-e-S)
  BOARD_LABEL:=vck190
  BOARD_VER:=2.2
else ifeq ($(DEVICE_NAME), xcvc1902-vsva2197-2MP-e-S-es1)
  BOARD_LABEL:=vck190_es
  BOARD_VER:=1.3
endif

# =========================================================
# DEPENDENCIES
# =========================================================
DEPS := $(CREATE_XSA_TCL)
# DEPS += pfm_decls.tcl
DEPS += $(QOR_SCRIPTS_DIR)/post_place.tcl
DEPS += $(QOR_SCRIPTS_DIR)/pre_place.tcl
DEPS += $(QOR_SCRIPTS_DIR)/post_route.tcl
DEPS += $(QOR_SCRIPTS_DIR)/post_physopt.tcl
DEPS += $(QOR_SCRIPTS_DIR)/prohibitCascBramAcrossRbrk.tcl
DEPS += $(QOR_SCRIPTS_DIR)/prohibitCascDspAcrossRbrk.tcl
DEPS += $(QOR_SCRIPTS_DIR)/prohibitCascUramAcrossRbrk.tcl
DEPS += $(QOR_SCRIPTS_DIR)/timing.xdc
DEPS += $(QOR_SCRIPTS_DIR)/waive_BLI_AIE_timing_violations_postplace.tcl
DEPS += $(QOR_SCRIPTS_DIR)/waive_BLI_AIE_timing_violations_preplace.tcl
DEPS += $(SRC_DIR)/post_link_pfm_config.tcl
DEPS += $(SRC_DIR)/dr.bd.tcl
DEPS += $(SRC_DIR)/ddr4_dmc_3200_triplet_1.xdc

# =========================================================
# BUILD PRODUCTS
# ========================================================
BUILD_DIR := build
XSA := $(PLATFORM_CUSTOM).xsa

xsa: $(BUILD_DIR)/$(XSA)

$(BUILD_DIR)/$(XSA): $(DEPS)
	vivado -mode batch -source $(CREATE_XSA_TCL) -tclargs $(PLATFORM_CUSTOM) $(DEVICE_NAME) $(PRE_SYNTH) $(BOARD_LABEL) $(BOARD_VER)

#	mv NOC_Power.xpe $(BUILD_DIR)/.
#	mv vivado* $(BUILD_DIR)/.

clean:
	rm -rf $(BUILD_DIR)/$(XSA)
	rm -rf $(BUILD_DIR)
	rm -rf *.log *.jou .Xil


