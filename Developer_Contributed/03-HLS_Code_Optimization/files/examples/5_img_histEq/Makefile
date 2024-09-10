# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


.PHONY: help kernels clean all

clean:
			rm -rf *_prj *.log *~ ./common/test_data/c_out_*.bmp

kernels:
			vitis-run --mode hls --tcl  ./run_src3_hls_script.tcl
			vitis-run --mode hls --tcl  ./run_src4_hls_script.tcl


all:	kernels
