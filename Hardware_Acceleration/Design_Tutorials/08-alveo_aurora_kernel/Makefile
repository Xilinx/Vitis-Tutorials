# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

ECHO=@echo

.PHONY: help xclbin host all clean

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make xclbin"
	$(ECHO) "      Command to build xclbin files for Alveo platform"
	$(ECHO) ""
	$(ECHO) "  make host"
	$(ECHO) "      Command to build host software for xclbin test"
	$(ECHO) ""
	$(ECHO) "  make all"
	$(ECHO) "      Command to build sw and hw"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

# PART setting: uncomment the lines matching your Alveo card, or override them by make variable
PART := xcu200-fsgd2104-2-e
PLATFORM ?= xilinx_u200_gen3x16_xdma_2_202110_1

#PART := xcu250-figd2104-2L-e
#PLATFORM ?= xilinx_u250_gen3x16_xdma_4_1_202210_1

#PART := xcu55c-fsvh2892-2L-e
#PLATFORM ?= xilinx_u55c_gen3x16_xdma_3_202210_1

#PART := xcu50-fsvh2104-2-e
#PLATFORM ?= xilinx_u50_gen3x16_xdma_5_202210_1

#PART := xcu280-fsvh2892-2L-e
#PLATFORM ?= xilinx_u280_gen3x16_xdma_1_202211_1

# TARGET: set the build target, only hw target is supported for designs including GT kernel
TARGET := hw


################## IP resource generation 

./ip_generation/aurora_64b66b_0/aurora_64b66b_0.xci: ./tcl/gen_aurora_ip.tcl
	mkdir -p ip_generation; rm -rf ip_generation/aurora_64b66b_0; vivado -mode batch -source $^ -tclargs $(PART)

./ip_generation/axis_data_fifo_0/axis_data_fifo_0.xci: ./tcl/gen_fifo_ip.tcl
	mkdir -p ip_generation; rm -rf ip_generation/axis_data_fifo_0; vivado -mode batch -source $^ -tclargs $(PART)


################## hardware build 
COMMFLAGS := --platform $(PLATFORM) --target $(TARGET) --save-temps --debug 
HLSCFLAGS := --compile $(COMMFLAGS)
LINKFLAGS := --link --optimize 3 $(COMMFLAGS)

RTL_SRC := ./rtl/*.v
RTL_SRC += ./ip_generation/aurora_64b66b_0/aurora_64b66b_0.xci 
RTL_SRC += ./ip_generation/axis_data_fifo_0/axis_data_fifo_0.xci

XCLBIN_OBJ := krnl_aurora_test_$(TARGET).xclbin

krnl_aurora.xo: $(RTL_SRC) ./tcl/pack_kernel.tcl
	rm -rf vivado_pack_krnl_project; mkdir vivado_pack_krnl_project; cd vivado_pack_krnl_project; vivado -mode batch -source ../tcl/pack_kernel.tcl -tclargs $(PART)

strm_dump_$(TARGET).xo: ./hls/strm_dump.cpp
	v++ $(HLSCFLAGS) --kernel strm_dump --output $@ $^

strm_issue_$(TARGET).xo: ./hls/strm_issue.cpp
	v++ $(HLSCFLAGS) --kernel strm_issue --output $@ $^

$(XCLBIN_OBJ): krnl_aurora.xo strm_issue_$(TARGET).xo strm_dump_$(TARGET).xo krnl_aurora_test.cfg
	v++ $(LINKFLAGS) --config krnl_aurora_test.cfg --output $@ krnl_aurora.xo strm_dump_$(TARGET).xo strm_issue_$(TARGET).xo 


################## software build for XRT Native API code

CXXFLAGS += -std=c++17 -Wno-deprecated-declarations
CXXFLAGS += -I$(XILINX_XRT)/include
LDFLAGS := -L$(XILINX_XRT)/lib
LDFLAGS += $(LDFLAGS) -lxrt_coreutil
EXECUTABLE := host_krnl_aurora_test

HOST_SRCS := ./host/host_krnl_aurora_test.cpp

$(EXECUTABLE): $(HOST_SRCS)
	$(CXX) -o $(EXECUTABLE) $^ $(CXXFLAGS) $(LDFLAGS)


################## all flow
xclbin: $(XCLBIN_OBJ)
host: $(EXECUTABLE)

all: xclbin host


################## clean up
clean:
	$(RM) -rf ip_generation vivado_pack_krnl_project
	$(RM) -rf *.xo *.xclbin *.xclbin.info *.xclbin.link_summary *.jou *.log *.xo.compile_summary _x
	$(RM) -rf *.dat *.pb xsim.dir *.xml *.ltx *.csv *.protoinst *.wdb *.wcfg host_krnl_aurora_test
