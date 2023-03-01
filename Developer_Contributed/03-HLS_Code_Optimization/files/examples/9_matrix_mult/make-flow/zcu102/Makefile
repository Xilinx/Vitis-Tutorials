# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

SHELL := /bin/bash

PHONY  = help kernels hw_link system host all clean
PHONY += zcu102_build zcu102_prepare
.PHONY: $(PHONY)

all:	zcu102_prepare zcu102_build

#source $(realpath ../../../../scripts)/set_proj_env_2022v2_zcu102.sh
zcu102_build:
	$(MAKE) kernels
	$(MAKE) hw_link
	$(MAKE) host
	$(MAKE) system

clean:
	$(MAKE) -C mmult_kernels/Hardware clean
	$(MAKE) -C mmult_system_hw_link/Hardware clean
	$(MAKE) -C mmult_system/Hardware clean
	$(MAKE) -C mmult/Hardware clean
	rm -rf ./mmult/src
	rm -rf ./mmult_system/src
	rm -rf ./mmult_kernels/src
	rm -rf ./mmult_system_hw_link/src
	rm -f  ./muult_system/Hardware/*.log
	rm -f  ./mmult_system/Hardware/*~
	rm -rf  ./mmult_system/Hardware/.ipcache
	rm -rf  ./mmult_system_hw_link/Hardware/.ipcache


zcu102_prepare:
	mkdir -p $(realpath ../zcu102/mmult)/src
	mkdir -p $(realpath ../zcu102/mmult_kernels)/src
	cp -f $(realpath ../../vitis_src)/xrt_mmult_test.cpp	$(realpath ./mmult)/src/
	cp -f $(realpath ../../src)/mmult.h			$(realpath ./mmult)/src/
	cp -f $(realpath ../../src)/*accel.cpp			$(realpath ./mmult_kernels)/src/
	cp -f $(realpath ../../src)/mmult.h			$(realpath ./mmult_kernels)/src
	cp -f $(realpath ../../vitis_src)/mm2s.cpp		$(realpath ./mmult_kernels)/src/
	cp -f $(realpath ../../vitis_src)/s2mm.cpp		$(realpath ./mmult_kernels)/src/
	cp -f $(realpath ../../vitis_src)/system.cfg		$(realpath ./mmult_system_hw_link)/

kernels:
	$(MAKE) -C mmult_kernels/Hardware all

hw_link:
	$(MAKE) -C mmult_system_hw_link/Hardware all

system:
	$(MAKE) -C mmult_system/Hardware all

host:
	$(MAKE) -C mmult/Hardware all
