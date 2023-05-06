# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


.PHONY: help kernels clean all

clean:
			rm -rf *_prj *.log *~

kernels:
			vitis_hls ./run_src3_hls_script.tcl
			vitis_hls ./run_src4_hls_script.tcl


all:	kernels
