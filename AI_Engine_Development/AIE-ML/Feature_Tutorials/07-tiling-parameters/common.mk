# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT



# PLATFORM

ARCH ?= aie-ml

# Get version number
TOOLV := $(shell basename  $(XILINX_VITIS))
ifeq ($(TOOLV), Vitis)
DIRNAME := $(shell dirname $(XILINX_VITIS))
TOOLV := $(shell basename $(DIRNAME))
endif

V1 := $(subst .1,10,$(TOOLV))
V2 := $(subst .2,20,$(V1))
PLATFORM_VERSION := $(V2)


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

export PFM_NAME PLATFORM

# Tutorial Section  memtile | doc | memmodule | extbuf
SECTION ?= extbuf

export SECTION

# Other simulation related variables
ifeq ($(SECTION), memtile)
NIterations_Data ?= 2
FrameSize ?= 8
Dim0 ?= $(FrameSize) 
Dim1 ?= $(FrameSize)
Dim2 ?= 4
Dim3 ?= 4
else ifeq ($(SECTION), doc)
NIterations_Data ?= 2
FrameSize ?= 256
Dim0 ?= $(FrameSize) 
Dim1 ?= 4
Dim2 ?= 2
Dim3 ?= 2
else ifeq ($(SECTION), memmodule)
NIterations_Data ?= 2
FrameSize ?= 8
Dim0 ?= $(FrameSize) 
Dim1 ?= $(FrameSize)
Dim2 ?= 4
Dim3 ?= 4
else ifeq ($(SECTION), extbuf)
NIterations_Data ?= 2
FrameSize ?= 8
Dim0 ?= $(FrameSize) 
Dim1 ?= $(FrameSize)
Dim2 ?= 4
Dim3 ?= 4
else
$(error "SECTION should be either memtile | doc | memmodule | extbuf   (SECTION = $(SECTION))")
endif

Dimensions := $(Dim0) $(Dim1) $(Dim2) $(Dim3)

# Some Basic computations
Base := $(shell echo "10^((l($(FrameSize))/l(10))+0.999)" | bc -l)  # Next power of 10 for the number base

# Each frame is a 2D image: Dim0xDim1
# Number of iterations should consume all data Dim2xDim3
MaxIterations := $(shell echo "$(NIterations_Data)*$(Dim2)*$(Dim3)*2" | bc -l)
export NIterations_Data FrameSize Dim0 Dim1 Dim2 Dim3 Dimensions Base MaxIterations


# For hw implementation choose either hw or hw_emu
TARGET ?= hw_emu
export TARGET

$(info "===============================================================")
$(info "             Tutorial Parameters")
$(info "===============================================================")
$(info "")
$(info "Tutorial Section: $(SECTION)")
$(info "Platform Name: $(PFM_NAME)")
$(info "NIterations (Data):   $(NIterations_Data)")
$(info "MaxIterations     : $(MaxIterations)")
$(info "Sizes: $(Dimensions)")
$(info "Base: $(Base)")
$(info "")
$(info "===============================================================")
