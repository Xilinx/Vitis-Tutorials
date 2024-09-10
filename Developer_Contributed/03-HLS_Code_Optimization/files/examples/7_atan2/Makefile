# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


.PHONY: help kernels clean all

clean:
			rm -rf *_prj *.log *~ ./logs

kernels:
			vitis-run --mode hls --tcl ./run_atan2_script.tcl


all:	kernels
