# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */
HOST_EXE   = ../main.elf
HOST_INC   = -I../ -I../src
HOST_OBJ   = aie_control.o main.o
HOST_CXX   = ${XILINX_VITIS}/gnu/aarch64/lin/aarch64-none/bin/aarch64-none-elf-gcc
AIE_FILE   = ../Work/ps/c_rts/aie_control.cpp 


############################################################################################################################################################
# Modify below this line for 2020.1 software flow changes i.e. host application compilation, prep_target and emulation launch etc.
#########################################################################################################################################

.PHONY: clean

${HOST_EXE}: ${HOST_OBJ}
	${HOST_CXX} *.o -g -mcpu=cortex-a72 -Wl,-T -Wl,./lscript.ld -L../fixed_platform/export/fixed_platform/sw/fixed_platform/standalone_domain/bsplib/lib  -L${XILINX_VITIS}/aietools/lib/aarchnone64.o -ladf_api -Wl,--start-group,-lxil,-lgcc,-lc,-lstdc++,--end-group ${HOST_INC} -o ../main.elf 
	
main.o: main.cpp aie_control.o 
	 ${HOST_CXX} -I../fixed_platform/export/fixed_platform/sw/fixed_platform/standalone_domain/bspinclude/include -I${XILINX_VITIS}/aietools/include -I${XILINX_VITIS}/include -I${XILINX_HLS}/include -I.. -I. -I../src -g -c -D__PS_BARE_METAL__ -std=c++17 -o main.o main.cpp

aie_control.o: ${AIE_FILE} 
	 ${HOST_CXX} -I../fixed_platform/export/fixed_platform/sw/fixed_platform/standalone_domain/bspinclude/include -I${XILINX_VITIS}/aietools/include -I${XILINX_VITIS}/include -I${XILINX_HLS}/include -I.. -I. -I../src -g -c -D__PS_BARE_METAL__ -std=c++17 -o aie_control.o ../Work/ps/c_rts/aie_control.cpp 

clean:
	rm -rf ${HOST_OBJ} .Xil ${HOST_EXE} *.elf 
