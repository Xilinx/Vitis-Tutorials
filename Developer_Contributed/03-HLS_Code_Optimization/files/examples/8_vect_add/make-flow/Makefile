# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

.PHONY: help all clean zcu102_vadd_clean zcu102_vadd_build zcu102_vadd_prepare

all:		zcu102_vadd_prepare zcu102_vadd_build
clean: 	zcu102_vadd_clean


zcu102_vadd_clean:
	$(MAKE) -C ./zcu102_vadd/Hardware  clean
	$(MAKE) -C ./zcu102_vadd_kernels/Hardware clean
	$(MAKE) -C ./zcu102_vadd_system_hw_link/Hardware clean
	$(MAKE) -C ./zcu102_vadd_system/Hardware clean
	rm -rf ./zcu102_vadd/src
	rm -rf ./zcu102_vadd_kernels/src
	rm -rf ./zcu102_vadd_system/src
	rm -f  ./zcu102_vadd_system/Hardware/*.log
	rm -f  ./zcu102_vadd_system/Hardware/binary*
	rm -f  ./zcu102_vadd_system/Hardware/*~
	rm -rf ./zcu102_vadd_system/Hardware/.ipcache
	rm -rf ./zcu102_vadd_system_hw_link/Hardware/.ipcache/
	rm -rf ./zcu102_vadd_system_hw_link/src

zcu102_vadd_build:
		$(MAKE) -C ./zcu102_vadd/Hardware all
		$(MAKE) -C ./zcu102_vadd_kernels/Hardware all
		$(MAKE) -C ./zcu102_vadd_system_hw_link/Hardware all
		$(MAKE) -C ./zcu102_vadd_system/Hardware all

zcu102_vadd_prepare:
		mkdir -p $(realpath ./zcu102_vadd)/src
		mkdir -p $(realpath ./zcu102_vadd_kernels)/src
		cp -f $(realpath ../src/host)/*							$(realpath ./zcu102_vadd)/src/
		cp -f $(realpath ../src/hls)/vadd_include.h	$(realpath ./zcu102_vadd)/src/
		cp -f $(realpath ../src/hls)/hls_krnl*			$(realpath ./zcu102_vadd_kernels)/src/
		cp -f $(realpath ../src/hls)/*.h						$(realpath ./zcu102_vadd_kernels)/src/
#		mkdir -p $(realpath ./zcu102_vadd_system_hw_link)/src
