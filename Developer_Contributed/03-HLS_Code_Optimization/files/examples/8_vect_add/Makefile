# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


.PHONY: help hls_kernels hls_clean zcu102_all zcu102_clean all clean

hls_clean:
	rm -rf *_prj *.log *~ ./logs 

hls_kernels:
	vitis-run --mode hls --tcl  ./run_vect_add_script.tcl

zcu102_all:
	$(MAKE) -C ./make-flow all

zcu102_clean:
	$(MAKE) -C ./make-flow clean
#	mkdir -p ./make-flow/zcu102_vadd/src
#	mkdir -p ./make-flow/zcu102_vadd_kernels/src
#	mkdir -p ./make-flow/zcu102_vadd_system_hw_link/src

kernels:	hls_kernels
all:			zcu102_all hls_kernels
clean:		hls_clean zcu102_clean
