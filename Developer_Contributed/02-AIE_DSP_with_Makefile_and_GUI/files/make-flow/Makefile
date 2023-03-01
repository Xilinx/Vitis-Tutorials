# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

SHELL := /bin/bash

PHONY  = help all clean
PHONY += kernels hw_link system host aie
PHONY += prepare_files build
.PHONY: $(PHONY)


# =========================================================
# Source directories
# =========================================================
RELATIVE_PROJECT_DIR := ./
PROJECT_REPO := $(shell readlink -f $(RELATIVE_PROJECT_DIR))


# =========================================================
# commands directories
# =========================================================

hw:
		$(MAKE) all

all:
		$(MAKE) prepare_files
		$(MAKE) sd_card


prepare_files:
	cp -fr $(PROJECT_REPO)/src/ip/aie/data  		$(PROJECT_REPO)/dsp_AIE/dsp_AIE/
	cp -fr $(PROJECT_REPO)/src/ip/aie/src               	$(PROJECT_REPO)/dsp_AIE/
	cp -fr $(PROJECT_REPO)/src/ip/mm2s_aie/src          	$(PROJECT_REPO)/dsp_PL/
	cp -f  $(PROJECT_REPO)/src/ip/s2mm_aie/src/s2mm.cpp 	$(PROJECT_REPO)/dsp_PL/src/
	cp -fr $(PROJECT_REPO)/src/ps_apps/aie_test/src     	$(PROJECT_REPO)/dsp_PS/
	cp -f  $(PROJECT_REPO)/src/vitis/src/system.cfg		$(PROJECT_REPO)/dsp_system_hw_link/

sd_card:
		$(MAKE) kernels
		$(MAKE) aie
		$(MAKE) hw_link
		$(MAKE) host
		$(MAKE) system

clean:
	$(MAKE) -C $(PROJECT_REPO)/dsp_system_hw_link/Hardware clean
	$(MAKE) -C $(PROJECT_REPO)/dsp_system/Hardware clean
	$(MAKE) -C $(PROJECT_REPO)/dsp_PS/Hardware clean
	$(MAKE) -C $(PROJECT_REPO)/dsp_PL/Hardware clean
	$(MAKE) -C $(PROJECT_REPO)/dsp_AIE/Hardware clean
	rm -rf $(PROJECT_REPO)/dsp_AIE/src
	rm -rf $(PROJECT_REPO)/dsp_AIE/data
	rm -rf $(PROJECT_REPO)/dsp_PL/src
	rm -rf $(PROJECT_REPO)/dsp_PS/src


aie:
	$(MAKE) -C $(PROJECT_REPO)/dsp_AIE/Hardware all

kernels:
	$(MAKE) -C $(PROJECT_REPO)/dsp_PL/Hardware all

hw_link:
	$(MAKE) -C $(PROJECT_REPO)/dsp_system_hw_link/Hardware all

system:
	$(MAKE) -C $(PROJECT_REPO)/dsp_system/Hardware all

host:
	$(MAKE) -C $(PROJECT_REPO)/dsp_PS/Hardware all
