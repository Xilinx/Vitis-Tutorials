
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

SDKTARGETSYSROOT ?= ${SYSROOT}

HOST_EXE     = ../host.exe
HOST_OBJ   = host.o
SDKTARGETSYSROOT ?= ${SYSROOT}

CXXFLAGS += -c -I${SDKTARGETSYSROOT}/usr/include/xrt/

LDFLAGS += -lxrt_coreutil 

.PHONY: clean

${HOST_EXE}: ${HOST_OBJ}
	${CXX} -o $@ $^ ${LDFLAGS}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -o $@ $<

clean:
	rm -rf ${HOST_OBJ} .Xil ${HOST_EXE} 
