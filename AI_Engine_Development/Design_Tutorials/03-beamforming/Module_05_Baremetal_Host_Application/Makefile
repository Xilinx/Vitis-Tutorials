#  © Copyright 2020 Xilinx, Inc.
#  Licensed under the Apache License, Version 2.0 (the 'License');
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  
#       http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an 'AS IS' BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.


# =========================================================
# PROJECT and MODULE REPOS
# =========================================================
#relative beamforming directory
RELATIVE_PROJECT_DIR := ../

#absolute beamforming directory = <user path>/Tutorials/AI_Engine/beamforming
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

MODULE_04_DIR := $(PROJECT_DIR)/Module_04_AI_Engine_and_PL_Integration

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET := hw_emu

# =========================================================
# MAIN can be set as:
#   main_partial:  check only 10 samples of data
#   main_full   :  check all 4 iterations with entire dataset
# =========================================================
MAIN := main_partial

# =========================================================
# XSA SELECTION
# =========================================================
XSA := $(MODULE_04_DIR)/build/rev1/hw/beamforming.rev1.hw.xsa

# =========================================================
# SOURCE DIRECTORIES AND FILES 
# =========================================================
MAIN_CPP := $(MAIN).cpp
BAREMETAL_SRC_DIR 	:= baremetal_src
BAREMETAL_SRC_CPP 	:= $(wildcard $(BAREMETAL_SRC_DIR)/*.cpp) 

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR := build
VCK190_BAREMETAL_DIR := vck190_baremetal
XPARAMETERS_H := $(BUILD_DIR)/$(VCK190_BAREMETAL_DIR)/psv_cortexa72_0/standalone_domain/bsp/psv_cortexa72_0/include/xparameters.h
MAIN_O := $(MAIN).o
APP_ELF := $(MAIN)_ps.elf

DLBF_CIN_O 		:= $(BUILD_DIR)/dlbf_cin.o 
DLBF_DIN_O 		:= $(BUILD_DIR)/dlbf_din.o
DLBF_GOLD0_O 	:= $(BUILD_DIR)/dlbf_gold0.o
ULBF_CIN_O 		:= $(BUILD_DIR)/ulbf_cin.o
ULBF_DIN_O 		:= $(BUILD_DIR)/ulbf_din.o
ULBF_GOLD0_O 	:= $(BUILD_DIR)/ulbf_gold0.o
UTILS_DLBF_O 	:= $(BUILD_DIR)/utils_dlbf.o
UTILS_ULBF_O 	:= $(BUILD_DIR)/utils_ulbf.o
UTILS_O 		:= $(BUILD_DIR)/utils.o

BAREMETAL_OBJ_O := 	$(DLBF_CIN_O)	\
			$(DLBF_DIN_O)	\
			$(DLBF_GOLD0_O)	\
			$(ULBF_CIN_O)	\
			$(ULBF_DIN_O)	\
			$(ULBF_GOLD0_O)	\
			$(UTILS_ULBF_O)	\
			$(UTILS_DLBF_O)	\
			$(UTILS_O)

# =========================================================
# COMPILER SELECTION
# =========================================================
CXX := aarch64-none-elf-g++

# =========================================================
# COMPILER OPTIONS
# =========================================================
CXX_FLAGS :=  -Wall		    \
		-O0 				\
		-g 					\
		-mcpu=cortex-a72	\
		-MMD 				\
		-MP					\
		-I $(BUILD_DIR)/$(VCK190_BAREMETAL_DIR)/psv_cortexa72_0/standalone_domain/bsp/psv_cortexa72_0/include

LD_FLAGS :=	-Wl,-T			\
		-Wl,./lscript.ld	\
		-mcpu=cortex-a72	\
		-Wl,--start-group,-lxil,-lgcc,-lc,-lstdc++,--end-group	\
		-L $(BUILD_DIR)/$(VCK190_BAREMETAL_DIR)/psv_cortexa72_0/standalone_domain/bsp/psv_cortexa72_0/lib

application: $(BUILD_DIR)/$(APP_ELF)

$(BUILD_DIR)/$(APP_ELF): $(BUILD_DIR)/$(MAIN_O) $(BAREMETAL_OBJ_O)
	$(CXX) $^ $(LD_FLAGS) -o $@

utils: $(BAREMETAL_OBJ_O)

$(BUILD_DIR)/%.o: $(BAREMETAL_SRC_DIR)/%.cpp $(XPARAMETERS_H)
	$(CXX) -c $(CXX_FLAGS) -o $@ $< 

main: $(BUILD_DIR)/$(MAIN_O) 

$(BUILD_DIR)/$(MAIN_O): $(MAIN_CPP) $(XPARAMETERS_H)
	$(CXX) -c $(CXX_FLAGS) -o $(BUILD_DIR)/$(MAIN_O) $(MAIN_CPP)

bsp: $(XPARAMETERS_H)

$(XPARAMETERS_H): $(XSA)
	generate-platform.sh -name $(VCK190_BAREMETAL_DIR) 		\
				-hw $< 										\
				-domain ai_engine:aie_runtime				\
				-domain psv_cortexa72_0:standalone 			\
				-out-dir $(BUILD_DIR)	
clean:
	rm -rf $(BUILD_DIR)/$(APP_ELF)
	rm -rf $(BAREMETAL_OBJ_O) 
	rm -rf $(BUILD_DIR)/$(MAIN_O) 
	rm -rf $(BUILD_DIR)/$(VCK190_BAREMETAL_DIR)
	rm -rf $(BUILD_DIR)
	rm -rf .Xil
