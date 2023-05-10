#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11



all: clean hws

hws:
	vivado -mode batch -source run_tb_dlbf_slave.tcl 

clean:
	rm -rf vivado*.log
	rm -rf vivado*.jou
	rm -rf project_*
	rm -rf NOC_Power.xpe
	rm -rf .Xil
