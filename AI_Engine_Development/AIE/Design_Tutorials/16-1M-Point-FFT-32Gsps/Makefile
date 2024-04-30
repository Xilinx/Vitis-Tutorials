#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

#SHELL := /bin/bash

#CWD = $(shell readlink -f .)

#--------------------------------------------------
# Define TARGET: 'hw' or 'hw_emu'
#--------------------------------------------------
export TARGET = hw

#--------------------------------------------------
# Define Part Number for Platform and Design Build
#--------------------------------------------------
export PART_NUMBER = xcvc1902-vsva2197-2MP-e-S

#------------------------------------
# Define PLATFORM DIRECTORY
#------------------------------------
export PF_STDNAME  = base_pfm_vck190
export PLATFORMNAME = platform/$(PF_STDNAME)/export/$(PF_STDNAME)/$(PF_STDNAME).xpfm

.PHONY: all clean cleanall

all:
	$(MAKE) all -C platform
	$(MAKE) all -C aie_src
	$(MAKE) all -C demo_ip
	$(MAKE) all -C link
	$(MAKE) all -C host
	$(MAKE) all -C package

clean:
	$(MAKE) clean -C platform
	$(MAKE) clean -C aie_src
	$(MAKE) clean -C demo_ip
	$(MAKE) clean -C link
	$(MAKE) clean -C host
	$(MAKE) clean -C package

cleanall:
	$(MAKE) cleanall -C platform
	$(MAKE) cleanall -C aie_src
	$(MAKE) cleanall -C demo_ip
	$(MAKE) cleanall -C link
	$(MAKE) cleanall -C host
	$(MAKE) cleanall -C package

