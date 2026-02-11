#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT


vitis := ./vitis_dir/a.xclbin

all: aie hls host vitis


# The below command will do cd to AIE_Kernel directory and run the makefile available in AIE_Kernel directory
# make all -C AIE_Kernel will compile the graph.cpp using the aiecompiler to generate libadf.a and Word directory
aie: 
	make all -C AIE_Kernel


# The below command will do cd to HLS_Kernels directory and run the makefile available in HLS_Kernels directory
# make all -C HLS_Kernels will compile the s2mm.cpp and mm2s.cpp file to generate HLS kernels s2mm.xo and m2ss.xo
hls:
	make all -C HLS_Kernels


# The below command will do cd to Host_srcs directory and run the makefile available in Host_srcs directory
# make all -C Host_srcs will compile the host.cpp to generate the host.exe executable file to run on the processor
host:
	make all -C Host_srcs


# The below command will do cd to Vits_dir directory and run the makefile available in Vits_dir directory
# make all -C Vits_dir will run the linker and packager to target the hw_emu/hw to run the design on hw_emu/hw
vitis: 
	make all -C vitis_dir

run_hw_emu: all 
	make run -C vitis_dir

clean:
	make clean -C AIE_Kernel
	make clean -C HLS_Kernels
	make clean -C Host_srcs
	make clean -C vitis_dir
