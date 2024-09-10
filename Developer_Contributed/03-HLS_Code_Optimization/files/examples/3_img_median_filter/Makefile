# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


.PHONY: help kernels clean all

clean:
			rm -rf *_prj *.log *~ ./src/test_data/c_out_*.bmp ./src/*~

kernels:
			vitis-run --mode hls --tcl ./run_median_script.tcl


all:	kernels
