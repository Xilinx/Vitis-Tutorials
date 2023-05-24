# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */


LANE := 4

ECHO=@echo

.PHONY: help all gen_ip pack_kernel build_hw clean

help:
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make gen_ip"
	$(ECHO) "      Command to generate the IPs used in this design"
	$(ECHO) ""
	$(ECHO) "  make pack_kernel"
	$(ECHO) "      Command to pack the RTL design into Vitis kernel"
	$(ECHO) ""
	$(ECHO) "  make build_hw"
	$(ECHO) "      Command to build xclbin files for Alveo platform"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."


# Target hardware setting, uncomment appropriate lines 
BOARD    := u200
PLATFORM ?= xilinx_u200_gen3x16_xdma_2_202110_1
PART     := xcu200-fsgd2104-2-e

# TARGET: set the build target
TARGET := hw

XOCCLFLAGS := --platform $(PLATFORM) -t $(TARGET)  -s -g
XOCCLFLAGS += --link --optimize 3

all: gen_ip pack_kernel build_hw

gen_ip:
	rm -rf ip_generation; mkdir ip_generation; vivado -mode batch -source ./gen_ip_x$(LANE).tcl -tclargs $(PART) $(BOARD)

pack_kernel:
	rm -rf vivado_pack_krnl_project
	mkdir vivado_pack_krnl_project
	cd vivado_pack_krnl_project; vivado -mode batch -source ../pack_eth_x$(LANE)_kernel.tcl -tclargs $(PART); vivado -mode batch -source ../pack_data_fifo_kernel.tcl -tclargs $(PART)

build_hw:
	v++ $(XOCCLFLAGS) --config connectivity_x$(LANE).cfg -o top_level_x$(LANE).xclbin data_fifo_krnl.xo ethernet_krnl_axis_x$(LANE).xo

clean:
	$(RM) -rf ip_generation vivado_pack_krnl_project
	$(RM) -rf *.xml *.xo *.xclbin *.xclbin.info *.xclbin.link_summary *.jou *.log _x
