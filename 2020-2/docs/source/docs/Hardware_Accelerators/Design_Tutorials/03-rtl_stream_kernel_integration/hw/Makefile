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

ECHO=@echo

.PHONY: help

help::
	$(ECHO) ""
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<hw/hw_emu> PLATFORM=platform_name"
	$(ECHO) "      Command to build all the rtc_gen_test and rtc_alpha xclbin and necessary kernel files (xo) for specified Target and Shell."
	$(ECHO) "      By default, TARGET=hw, PLATFORM=xilinx_u200_gen3x16_xdma_1_1_202020_1"
	$(ECHO) ""
	$(ECHO) "  make all_xo TARGET=<hw/hw_emu> PLATFORM=platform_name"
	$(ECHO) "      Command to build all the kernel files (xo), including rtc_gen.xo, alpha_mix.xo and strm_dump.xo"
	$(ECHO) "      By default, TARGET=hw, PLATFORM=xilinx_u200_gen3x16_xdma_1_1_202020_1"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""
	$(ECHO) "--------------------------------------------------------------------------------------------------------------------------------"
	$(ECHO) ""
	$(ECHO) "Supported Platform (platform_name):"
	$(ECHO) "  xilinx_u200_xdma_201830_2"
	$(ECHO) "  xilinx_u200_gen3x16_xdma_1_1_202020_1"
	$(ECHO) "  xilinx_u250_xdma_201830_2"
	$(ECHO) "  xilinx_u250_gen3x16_xdma_3_1_202020_1"
	$(ECHO) "  xilinx_u50_gen3x16_xdma_201920_3"
	$(ECHO) "  xilinx_u280_xdma_201920_3"
	$(ECHO) ""

PLATFORM := xilinx_u200_gen3x16_xdma_1_1_202020_1
TARGET := hw

NCPUS := $(shell grep -c ^processor /proc/cpuinfo)
JOBS := $(shell expr $(NCPUS) - 1)

XOCCFLAGS := --platform $(PLATFORM) -t $(TARGET)  -s -g
XOCCLFLAGS += --kernel_frequency 200

XOS_RTC_ALPHA = rtc_gen.xo alpha_mix_$(TARGET).xo strm_dump_$(TARGET).xo
XOS_RTC_GEN_TEST = rtc_gen.xo strm_dump_$(TARGET).xo

KERNEL_FLAGS_ALPHA_MIX := --kernel alpha_mix -I. -I./include --advanced.prop kernel.alpha_mix.kernel_flags="-std=c++0x -D__SDSVHLS__ -DHLS_NO_XIL_FPO_LIB"
KERNEL_FLAGS_STRM_DUMP := --kernel strm_dump -I. -I./include --advanced.prop kernel.strm_dump.kernel_flags="-std=c++0x -D__SDSVHLS__ -DHLS_NO_XIL_FPO_LIB"

.phony: clean traces

include config_gen.mk

all: rtc_alpha_$(TARGET).xclbin rtc_gen_test_$(TARGET).xclbin

all_xo: $(XOS_RTC_ALPHA)

rtc_gen.xo:
	./build_rtc_gen_xo.sh

alpha_mix_$(TARGET).xo: alpha_mix.cpp
	v++ $(XOCCFLAGS) $(KERNEL_FLAGS_ALPHA_MIX) -c -o $@ $<

strm_dump_$(TARGET).xo: strm_dump.cpp
	v++ $(XOCCFLAGS) $(KERNEL_FLAGS_STRM_DUMP) -c -o $@ $<

rtc_alpha_$(TARGET).xclbin: $(XOS_RTC_ALPHA) config_rtc_alpha
	v++ -l $(XOCCFLAGS) $(XOCCLFLAGS) --config xclbin_rtc_alpha.cfg -o $@ $(XOS_RTC_ALPHA)

rtc_gen_test_$(TARGET).xclbin: $(XOS_RTC_GEN_TEST) config_rtc_gen_test
	v++ -l $(XOCCFLAGS) $(XOCCLFLAGS) --config xclbin_rtc_gen_test.cfg -o $@ $(XOS_RTC_GEN_TEST)

clean:
	$(RM) -r _x .Xil .ipcache *.ltx *.log *.jou *.info *.xclbin *.xclbin.* *.xo *.xo.* *.str *.cfg
	
