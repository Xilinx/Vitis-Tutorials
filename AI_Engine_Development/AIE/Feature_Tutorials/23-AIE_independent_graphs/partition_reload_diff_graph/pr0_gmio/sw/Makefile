
#Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

SDKTARGETSYSROOT ?= ${SYSROOT}

HOST_EXE     = ../host0.exe
HOST_OBJ   = host.o

CXXFLAGS += -c --std=c++17 -I${SDKTARGETSYSROOT}/usr/include/xrt/

LDFLAGS += -lxrt_coreutil

.PHONY: clean

${HOST_EXE}: ${HOST_OBJ}
	${CXX} -o $@ $^ ${LDFLAGS}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -o $@ $<

clean:
	rm -rf ${HOST_OBJ} .Xil ${HOST_EXE} aie_control_xrt.cpp
