ECHO=@echo

.PHONY: help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make xclbin TARGET=hw option sw_emu or hw_emu"
	$(ECHO) "      Command to build xclbin files for Alveo platform"
	$(ECHO) ""
	$(ECHO) "  make host"
	$(ECHO) "      Command to build host software for xclbin test"
	$(ECHO) ""
	$(ECHO) "  make build TARGET=hw option sw_emu or hw_emu"
	$(ECHO) "      Command to build sw and hw"
	$(ECHO) ""
	$(ECHO) "  make run TARGET=hw option sw_emu or hw_emu"      
	$(ECHO) "      Command to build sw and hw"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

# PART setting: uncomment the lines matching your Alveo card, or override them by make variable

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CUR_DIR := $(patsubst %/,%,$(dir $(MK_PATH)))
XFLIB_DIR = $(VITIS_LIBS_PATHS)/graph



#setting PLATFORM
ifeq ($(PLATFORM),)
PLATFORM := $(DEVICE)
endif
ifeq ($(PLATFORM),)
PLATFORM := xilinx_u50_gen3x16_xdma_5_202210_1
endif


# TARGET: set the build target, can be hw or hw_emu
TARGET ?= hw


BUILD_DIR := build_$(TARGET)
REPORT_DIR := $(CUR_DIR)/reports/_x.$(TARGET)
WEIGHT := $(BUILD_DIR)/weight
SHORT := $(BUILD_DIR)/sssp
SEARCH := $(BUILD_DIR)/search


################## hardware build 
COMMFLAGS := --platform $(PLATFORM) --target $(TARGET) --save-temps --debug 
COMMFLAGS +=  -I $(XFLIB_DIR)/L2/include -I $(CUR_DIR)/kernel -I $(XFLIB_DIR)/../database/L1/include/hw -I $(XFLIB_DIR)/../quantitative_finance/L1/include -I $(XFLIB_DIR)/../utils/L1/include -I $(XFLIB_DIR)/L2/include -I $(XFLIB_DIR)/../database/L1/include/hw -I $(XFLIB_DIR)/../quantitative_finance/L1/include -I $(XFLIB_DIR)/../utils/L1/include
HLSCFLAGS := --compile $(COMMFLAGS)
LINKFLAGS := --link --optimize 3 $(COMMFLAGS) 
VPP_FLAGS_shortestPath_top +=  -D KERNEL_NAME=shortestPath_top
VPP_FLAGS_shortestPath_top += --hls.clock 300000000:shortestPath_top
VPP_LDFLAGS_shortestPath_top += --clock.defaultFreqHz 300000000

XCLBIN_OBJ := shortpath_$(TARGET).xclbin


$(BUILD_DIR)/weight_average.xo:  $(CUR_DIR)/kernel/weight_average.cpp
	mkdir -p $(WEIGHT)
	v++ $(HLSCFLAGS) --kernel weight_average --temp_dir $(WEIGHT) --report_dir $(REPORT_DIR) --output $@ $^

$(BUILD_DIR)/shortestPath_top.xo: $(CUR_DIR)/kernel/shortestPath_top.cpp
	mkdir -p $(SHORT)
	v++ $(HLSCFLAGS) $(VPP_FLAGS_shortestPath_top) --kernel shortestPath_top --temp_dir $(SHORT) --report_dir $(REPORT_DIR) --output $@ $^	


$(BUILD_DIR)/search.xo: $(CUR_DIR)/kernel/search.cpp
	mkdir -p $(SEARCH)
	v++ $(HLSCFLAGS) --kernel search --temp_dir $(SEARCH) --report_dir $(REPORT_DIR) --output $@ $^

$(XCLBIN_OBJ): $(BUILD_DIR)/weight_average.xo $(BUILD_DIR)/shortestPath_top.xo $(BUILD_DIR)/search.xo
	v++ $(LINKFLAGS) $(VPP_LDFLAGS_shortestPath_top) --temp_dir $(BUILD_DIR) --report_dir $(REPORT_DIR) --output $@ $(BUILD_DIR)/weight_average.xo $(BUILD_DIR)/shortestPath_top.xo $(BUILD_DIR)/search.xo 
################## host build

CXXFLAGS  += -fmessage-length=0 -I$(CUR_DIR)/host -I$(CUR_DIR)/kernel -I$(XILINX_XRT)/include -I$(XILINX_HLS)/include -std=c++14 -O3 -Wall -Wno-unknown-pragmas -Wno-unused-label
ifeq ($(TARGET),sw_emu)
CXXFLAGS += -D SW_EMU_TEST
endif
ifeq ($(TARGET),hw_emu)
CXXFLAGS += -D HW_EMU_TEST
endif
CXXFLAGS +=  -D USE_HBM
CXXFLAGS +=  -I $(XFLIB_DIR)/L2/include -I $(CUR_DIR)/host -I $(CUR_DIR)/kernel -I $(XFLIB_DIR)/ext/xcl2 -I $(XFLIB_DIR)/../utils/L1/include -I $(XFLIB_DIR)/L2/include -I $(XFLIB_DIR)/../database/L1/include/hw -I $(XFLIB_DIR)/../quantitative_finance/L1/include -I $(XFLIB_DIR)/../utils/L1/include -I $(XFLIB_DIR)/ext/xcl2
CXXFLAGS += -O3 
LDFLAGS := -pthread -L$(XILINX_XRT)/lib -L$(XILINX_HLS)/lnx64/tools/fpo_v7_1  -Wl,--as-needed -lOpenCL -lxrt_coreutil -lgmp -lmpfr -lIp_floating_point_v7_1_bitacc_cmodel
ifeq (,$(findstring opencv,$(CXXFLAGS)))
CXXFLAGS += $(XRT_CXXFLAGS)
endif

EXECUTABLE := shortpath

HOST_SRCS := ./host/main.cpp
HOST_SRCS += $(XFLIB_DIR)/ext/xcl2/xcl2.cpp

HOST_ARGS :=  -xclbin $(XCLBIN_OBJ) -o csr_data/offset.mtx -c csr_data/indicesweights.mtx -n csr_data/node.mtx



$(EXECUTABLE): $(HOST_SRCS) $(EMCONFIG)
	$(CXX) -o $(EXECUTABLE) $^ $(CXXFLAGS) $(LDFLAGS)

## Emulaiton FIles Generation

EMCONFIG_FILE := emconfig.json

$(EMCONFIG_FILE):
	emconfigutil --platform $(PLATFORM)

################## all flow

.phony: all clean

xclbin: $(XCLBIN_OBJ)
host: $(EXECUTABLE)
emconfig: $(EMCONFIG_FILE)


build: host xclbin emconfig


run: build
ifeq ($(TARGET), hw)
	cd $(CUR_DIR) && unset XCL_EMULATION_MODE && ./env.sh; ./shortpath $(HOST_ARGS);
	
else
	cd $(CUR_DIR) && export XCL_EMULATION_MODE=$(TARGET) && ./env.sh; ./shortpath $(HOST_ARGS);
	
endif


################## clean up
clean:
	$(RM) -rf $(BUILD_DIR) *.xo *.xclbin *.xclbin.info *.xclbin.link_summary *.jou *.log *.xo.compile_summary _x
	$(RM) -rf *.dat *.pb xsim.dir *.xml *.ltx *.csv *.json *.protoinst *.wdb *.wcfg shortpath
