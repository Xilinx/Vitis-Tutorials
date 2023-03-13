# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

SHELL := /bin/bash

.PHONY: help hls_kernels hls_clean zcu102_all zcu102_clean all clean

hls_clean:
	rm -rf *_prj *.log *~

hls_kernels:
	vitis_hls ./run_matrix_mult_script.tcl

zcu102_all:
	$(MAKE) -C ./make-flow/zcu102 all

zcu102_clean:
	$(MAKE) -C ./make-flow/zcu102 clean


kernels:		hls_kernels
all:				zcu102_all hls_kernels
clean:			hls_clean zcu102_clean
