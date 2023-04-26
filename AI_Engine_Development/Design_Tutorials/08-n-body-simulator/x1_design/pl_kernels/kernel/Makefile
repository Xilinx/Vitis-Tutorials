#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

help::
	@echo "Makefile Usage:"
	@echo "  make all "
	@echo "       Command runs make pre_build and creates Vitis HLS projects for each kernel"
	@echo ""
	@echo "  make pre_build "
	@echo "       Command to create the HLS *.cpp files from kernel/spec.json file"
	@echo ""
	@echo "  make vitis_hls "
	@echo "       Command to create Vitis HLS projects for each *.cpp HLS kernel"
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove the generated files."
	@echo ""

# #########################################################################
#            Makefile used for PL HLS Kernel Simulation 
# #########################################################################

# =========================================================
# PROJECT DIRECTORIES
# =========================================================
#relative beamforming directory
RELATIVE_PROJECT_DIR := ../../

#absolute beamforming directory = <user path>/Tutorials/AI_Engine/beamforming
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CUR_DIR := $(patsubst %/,%,$(dir $(MK_PATH)))
XFLIB_DIR_REL_PATH :=../../xf_utils_hw
XFLIB_DIR := $(shell readlink -f $(XFLIB_DIR_REL_PATH))

# =========================================================
# PLATFORM SELECTION
# =========================================================
PLATFORM ?=xilinx_vck190_base_202310_1

all: pre_build vitis_hls

# ################ TARGET: make pre_build ################
pre_build: get_xflib _krnlgen

get_xflib: 
	$(MAKE) get_xflib -C $(PROJECT_DIR)

_krnlgen: $(XFLIB_DIR)
	make -f $(CUR_DIR)/../ksrc.mk GENKERNEL=$(XFLIB_DIR)/L2/scripts/generate_kernels SPEC=$(CUR_DIR)/spec.json TOOLDIR=$(CUR_DIR)/../_krnlgen

# ################ TARGET: make vitis_hls ################
vitis_hls: packet_sender packet_receiver

packet_sender: packet_sender.cpp packet_sender.hpp packet_sender_test.cpp run_packet_sender.tcl
	vitis_hls -f run_packet_sender.tcl

packet_receiver: packet_receiver.cpp packet_receiver.hpp packet_receiver_test.cpp run_packet_receiver.tcl
	vitis_hls -f run_packet_receiver.tcl 

# ################ TARGET: make clean ################
clean: 
	rm -rf m2s_x2.cpp
	rm -rf s2m_x4.cpp
	rm -rf packet_sender
	rm -rf packet_receiver
	rm -rf *.log
	rm -rf *~
	rm -rf *#
	rm -rf .stamp
	rm -rf ../_krnlgen
