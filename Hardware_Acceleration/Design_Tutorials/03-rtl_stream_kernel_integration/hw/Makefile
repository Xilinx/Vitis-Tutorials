#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO=@echo

.PHONY: help all xll_xo clean

help::
	$(ECHO) ""
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<hw/hw_emu> PLATFORM=platform_name"
	$(ECHO) "      Command to build all the rtc_gen_test and rtc_alpha xclbin and necessary kernel files (xo) for specified Target and Shell."
	$(ECHO) "      By default, TARGET=hw, PLATFORM=xilinx_u200_gen3x16_xdma_2_202110_1"
	$(ECHO) ""
	$(ECHO) "  make all_xo TARGET=<hw/hw_emu> PLATFORM=platform_name"
	$(ECHO) "      Command to build all the kernel files (xo), including rtc_gen.xo, alpha_mix.xo and strm_dump.xo"
	$(ECHO) "      By default, TARGET=hw, PLATFORM=xilinx_u200_gen3x16_xdma_2_202110_1"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""
	$(ECHO) "--------------------------------------------------------------------------------------------------------------------------------"
	$(ECHO) ""
	$(ECHO) "Supported Platform (platform_name):"
	$(ECHO) "  xilinx_u200_gen3x16_xdma_2_202110_1"
	$(ECHO) "  xilinx_u250_gen3x16_xdma_4_1_202210_1"
	$(ECHO) "  xilinx_u50_gen3x16_xdma_5_202210_1"
	$(ECHO) "  xilinx_u55c_gen3x16_xdma_3_202210_1"
	$(ECHO) "  xilinx_u280_gen3x16_xdma_1_202211_1"
	$(ECHO) ""

PLATFORM ?= xilinx_u200_gen3x16_xdma_2_202110_1
TARGET := hw

XOCCFLAGS := --platform $(PLATFORM) -t $(TARGET)  -s -g
XOCCLFLAGS += --kernel_frequency 200

XOS_RTC_ALPHA = rtc_gen.xo alpha_mix_$(TARGET).xo strm_dump_$(TARGET).xo
XOS_RTC_GEN_TEST = rtc_gen.xo strm_dump_$(TARGET).xo

KERNEL_FLAGS_ALPHA_MIX := --kernel alpha_mix -I. -I./include --advanced.prop kernel.alpha_mix.kernel_flags="-std=c++0x -D__SDSVHLS__ -DHLS_NO_XIL_FPO_LIB"
KERNEL_FLAGS_STRM_DUMP := --kernel strm_dump -I. -I./include --advanced.prop kernel.strm_dump.kernel_flags="-std=c++0x -D__SDSVHLS__ -DHLS_NO_XIL_FPO_LIB"

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
