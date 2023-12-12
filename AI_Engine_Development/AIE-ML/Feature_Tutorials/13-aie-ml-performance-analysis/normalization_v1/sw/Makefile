
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

HOST_EXE     = ../host.exe
HOST_INC   = -I../ -I../aie
HOST_OBJ   =  host.o
AIE_CTRL_CPP = ../Work/ps/c_rts/aie_control_xrt.cpp
SYSROOT = ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2023.2/sysroots/aarch64-xilinx-linux/

HOST_CXX      = aarch64-linux-gnu-g++

CXXFLAGS += -std=c++17 -I$(XILINX_HLS)/include/ -I${SYSROOT}/usr/include/xrt/ -g -Wall -c -fmessage-length=0 --sysroot=${SYSROOT} -I${XILINX_VITIS}/aietools/include ${HOST_INC}

LDFLAGS += -ladf_api_xrt -lgcc -lc -lxilinxopencl -lxrt_coreutil -lpthread -luuid -lrt -ldl -lcrypt -lstdc++ -L${SYSROOT}/usr/lib/ --sysroot=${SYSROOT} -L$(XILINX_VITIS)/aietools/lib/aarch64.o


.PHONY: clean

${HOST_EXE}: ${HOST_OBJ}
	${HOST_CXX} -o $@ $^ ${LDFLAGS}

%.o: %.cpp
	${HOST_CXX} ${CXXFLAGS} -o $@ $<

#aie_control_xrt.cpp: ${AIE_CTRL_CPP}
#	cp -f ${AIE_CTRL_CPP} .

clean:
	rm -rf ${HOST_OBJ} .Xil ${HOST_EXE} aie_control_xrt.cpp
