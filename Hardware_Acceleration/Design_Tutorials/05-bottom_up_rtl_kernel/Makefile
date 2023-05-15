# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11

ECHO=@echo

.PHONY: help build_krnl_aes build_krnl_cbc all

help::
	$(ECHO) ""
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make build_krnl_aes TARGET=<hw/hw_emu> PLATFORM=platform_name"
	$(ECHO) "      Command to build xclbin file for krnl_aes design"
	$(ECHO) ""
	$(ECHO) "  make build_krnl_cbc TARGET=<hw/hw_emu> PLATFORM=platform_name"
	$(ECHO) "      Command to build xclbin file for krnl_cbc design"
	$(ECHO) ""
	$(ECHO) "  make all"
	$(ECHO) "      Command to build both xclbin files for krnl_aes and krnl_cbc designs"
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

PART := xcu200-fsgd2104-2-e
#PART := xcu250-figd2104-2L-e
#PART := xcu50-fsvh2104-2-e
#PART := xcu55c-fsvh2892-2L-e
#PART := xcu280-fsvh2892-2L-e

PLATFORM := xilinx_u200_gen3x16_xdma_2_202110_1
#PLATFORM := xilinx_u250_gen3x16_xdma_4_1_202210_1
#PLATFORM := xilinx_u50_gen3x16_xdma_5_202210_1
#PLATFORM := xilinx_u55c_gen3x16_xdma_3_202210_1
#PLATFORM := xilinx_u280_gen3x16_xdma_1_202211_1

TARGET := hw


# Build hareware (xclbin) objects
build_krnl_aes:
	make -C ./krnl_aes all PART=$(PART) PLATFORM=$(PLATFORM) TARGET=$(TARGET)

build_krnl_cbc: build_krnl_aes
	make -C ./krnl_cbc all PART=$(PART) PLATFORM=$(PLATFORM) TARGET=$(TARGET)

all: build_krnl_cbc
