#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#


PLATFORM := xilinx_u200_gen3x16_xdma_2_202110_1
MODE := hw

KERNEL_SRC := kernel.cpp 
KERNEL_XO := kernel.xo   
KERNEL_XCLBIN := kernel.xclbin

HOST_SRC := host.cpp
#HOST_SRC := host_p2p.cpp
EXE := host.exe

VPPFLAGS := -s -t $(MODE) --platform $(PLATFORM)


${KERNEL_XO}: ${KERNEL_SRC} 
	v++ -c -k increment $(VPPFLAGS) -o $@  $^

${KERNEL_XCLBIN}: ${KERNEL_XO} 
	v++ -l $(VPPFLAGS) -o $@ $^


${EXE} : ${HOST_SRC}
	g++ -g -o $@ ${HOST_SRC} -std=c++17 -I${XILINX_XRT}/include -L${XILINX_XRT}/lib -lxrt_coreutil -pthread


xclbin : $(KERNEL_XCLBIN)
app : ${EXE}
