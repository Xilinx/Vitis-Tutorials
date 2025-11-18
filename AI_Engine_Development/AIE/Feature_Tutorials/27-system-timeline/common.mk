#
#Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#


#################################################################################
##     Get Version Number
#################################################################################
TOOLV := $(shell basename  $(XILINX_VITIS))
ifeq ($(TOOLV), Vitis)
DIRNAME := $(shell dirname $(XILINX_VITIS))
TOOLV := $(shell basename $(DIRNAME))
endif

V1 := $(subst .1,10,$(TOOLV))
V2 := $(subst .2,20,$(V1))
PLATFORM_VERSION := $(V2)
export TOOLV

$(info "Tool Version:  $(TOOLV)")
$(info "")
$(info "")


#################################################################################
##     Directories
#################################################################################
ROOTDIR ?= $(PWD)

AIE_DIR := $(ROOTDIR)/aie
KERNELS_DIR := $(ROOTDIR)/pl_kernels
HOST_DIR := $(ROOTDIR)/sw
ADDON_DIR := $(ROOTDIR)/addon_files
UTILS_DIR := $(ROOTDIR)/Utils

export ROOTDIR AIE_DIR KERNELS_DIR HOST_DIR ADDON_DIR UTILS_DIR

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

# Allow tracing in hw: true/false
ENABLE_TRACE ?= true

# Trace channel  gmio/plio
TRACE_CHANNEL ?= plio

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
FREQ_HZ ?= 312500000
export FREQ_HZ

# AI Engine Design Parameters
FRAME_LENGTH ?= 256
MARGIN_LENGTH ?= 16
export FRAME_LENGTH MARGIN_LENGTH

# PL internal array size
ARRAY_SIZE ?= 512
export ARRAY_SIZE


#################################################################################
## Platform selection
#################################################################################

ifeq ($(ARCH), aie)
PFM_NAME := xilinx_vck190_base_$(PLATFORM_VERSION)_1
$(info "ARCH set for AI Engine Architecture   (ARCH = $(ARCH))")
else ifeq ($(ARCH), aie-ml)
PFM_NAME := xilinx_vek280_base_$(PLATFORM_VERSION)_1
$(info "ARCH set for AI Engine ML Architecture   (ARCH = $(ARCH))")
else
$(error "ARCH should be either aie for AI Engine or aie-ml for AI Engine ML   (ARCH = $(ARCH))")
endif


PFM_NAME := $(strip $(PFM_NAME))
PLATFORM := ${PLATFORM_REPO_PATHS}/${PFM_NAME}/${PFM_NAME}.xpfm
PNAME := aie_pl_${TARGET}

export PFM_NAME PLATFORM PNAME
