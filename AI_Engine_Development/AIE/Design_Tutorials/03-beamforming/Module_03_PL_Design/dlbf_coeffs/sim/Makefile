#Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT



all: clean hws

hws:
	vivado -source run_tb_dlbf_coeffs.tcl 

clean:
	rm -rf vivado*
	rm -rf project_*
	rm -rf NOC_Power.xpe
	rm -rf .Xil
