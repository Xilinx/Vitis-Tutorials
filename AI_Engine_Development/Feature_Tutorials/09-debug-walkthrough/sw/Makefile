# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

HOST_EXE     = ../host.exe
HOST_INC     = -I../ -I../aie
HOST_OBJ     = aie_control_xrt.o host.o
AIE_CTRL_CPP = ../Work/ps/c_rts/aie_control_xrt.cpp

CXXFLAGS += -std=c++17 -I$(XILINX_VIVADO)/include/ -I${SDKTARGETSYSROOT}/usr/include/xrt/ -I../aie/inc -I../aie/kernels -O0 -g -Wall -c -fmessage-length=0 --sysroot=${SDKTARGETSYSROOT} -I${XILINX_VITIS}/aietools/include ${HOST_INC}

LDFLAGS += -ladf_api_xrt -lxrt_coreutil -lxrt_core -lxaiengine -lgcc -lc -lxilinxopencl -lpthread -lrt -ldl -lcrypt -lstdc++ --sysroot=${SDKTARGETSYSROOT} -L${SDKTARGETSYSROOT}/usr/lib/ -L$(XILINX_VITIS)/aietools/lib/aarch64.o

.PHONY: clean

${HOST_EXE}: ${HOST_OBJ}
	${CXX} -o $@ $^ ${LDFLAGS}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -o $@ $<

aie_control_xrt.cpp: ${AIE_CTRL_CPP}
	cp -f ${AIE_CTRL_CPP} .

clean:
	rm -rf ${HOST_OBJ} .Xil ${HOST_EXE}
