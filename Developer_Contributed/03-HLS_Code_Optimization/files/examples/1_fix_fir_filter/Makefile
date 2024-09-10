# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


.PHONY: help kernels clean all

clean:
			rm -rf *_prj *.log* *~ .Xil .wsdata ./logs 

kernels:		
			vitis-run --mode hls --tcl ./fix_fir_script.tcl
#vitis_hls ./fix_fir_script.tcl

