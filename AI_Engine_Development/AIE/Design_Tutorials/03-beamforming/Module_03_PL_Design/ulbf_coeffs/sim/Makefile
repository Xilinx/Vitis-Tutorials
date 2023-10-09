#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11



all: clean hws

hws:
	vivado -source run_tb_ulbf_coeffs.tcl 

clean:
	rm -rf vivado*
	rm -rf project_*
	rm -rf NOC_Power.xpe
	rm -rf .Xil
