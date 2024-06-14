#
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#


#################################################################################
##     Directories
#################################################################################
ROOTDIR ?= $(PWD)

AIE_DIR := $(ROOTDIR)/aie
KERNELS_DIR := $(ROOTDIR)/pl_kernels
HOST_DIR := $(ROOTDIR)/sw
ADDON_DIR := $(ROOTDIR)/addon_files
UTILS_DIR := $(ROOTDIR)/Utils

#################################################################################
## Project Parameters
#################################################################################


# hw_emu|hw
TARGET ?= hw

# Platform
ARCH ?= aie

# Design Type
MULTIRATE ?= false

export TARGET ARCH MULTIRATE


$(info "-------------------------------------------------------------------------")
ifeq ($(ARCH), aie)
PFM_NAME := xilinx_vck190_base_202410_1
REPO := $(PLATFORM_REPO_PATHS)
$(info "ARCH set for AI Engine Architecture   (ARCH = $(ARCH))")
else ifeq ($(ARCH), aie-ml)
# PFM_NAME := xilinx_vek280_base_202410_1
# REPO := $(PLATFORM_REPO_PATHS)
PFM_NAME := xilinx_vek280_es1_base_202410_1
REPO := $(XILINX_VITIS)/base_platforms
$(info "ARCH set for AI Engine ML Architecture   (ARCH = $(ARCH))")
else
$(error "ARCH should be either aie for AI Engine or aie-ml for AI Engine ML   (ARCH = $(ARCH))")
endif
$(info "-------------------------------------------------------------------------")


PFM_NAME := $(strip $(PFM_NAME))
PLATFORM := ${REPO}/${PFM_NAME}/${PFM_NAME}.xpfm
PNAME := aie_pl_${TARGET}

# Allow tracing in hw: true/false
ENABLE_TRACE ?= true

# Trace channel  gmio/plio
TRACE_CHANNEL ?= gmio

# Allow profiling in hw: true/false
ENABLE_PROFILE ?= true


export ENABLE_TRACE TRACE_CHANNEL ENABLE_PROFILE 

#################################################################################
## AI Engine Design and PL Parameters
#################################################################################

# Default number of Antennas
NAntenna ?= 4
export NAntenna

#Default Number of iterations
NIterations ?= 16
export NIterations

# Width of the PLIOs
PLIOW ?= 128
export PLIOW

# Width of the Data
DATAW ?= 16
export DATAW

# PL Frequency (PL kernels)
FREQ_HZ ?= 325000000
export FREQ_HZ

# AI Engine Design Parameters
FRAME_LENGTH ?= 256
MARGIN_LENGTH ?= 16
export FRAME_LENGTH MARGIN_LENGTH

# PL internal array size
ARRAY_SIZE ?= 512
export ARRAY_SIZE


