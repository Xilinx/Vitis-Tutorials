#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

HOST_EXE     = ../host.exe
HOST_INC   = -I../ -I$(AIE_DIR)/src
HOST_OBJ   = aie_control_xrt.o host.o
AIE_CTRL_CPP = $(AIE_DIR)/Work/ps/c_rts/aie_control_xrt.cpp

CXXFLAGS += -std=c++17 -I$(XILINX_HLS)/include/ -I${SDKTARGETSYSROOT}/usr/include/xrt/ -g -Wall -c -fmessage-length=0 --sysroot=${SDKTARGETSYSROOT} -I${XILINX_VITIS}/aietools/include ${HOST_INC} -I${AIE_DIR}

LDFLAGS += -ladf_api_xrt -lgcc -lc -lxilinxopencl -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ -L${SDKTARGETSYSROOT}/usr/lib/ --sysroot=${SDKTARGETSYSROOT} -L$(XILINX_VITIS)/aietools/lib/aarch64.o


.PHONY: clean

${HOST_EXE}: ${HOST_OBJ}
	${CXX} -o $@ $^ ${LDFLAGS}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -o $@ $<

aie_control_xrt.cpp: ${AIE_CTRL_CPP}
	cp -f ${AIE_CTRL_CPP} .

clean:
	rm -rf ${HOST_OBJ} .Xil ${HOST_EXE} aie_control_xrt.cpp
