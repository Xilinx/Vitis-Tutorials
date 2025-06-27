
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

HOST_EXE     = ../host1.exe
HOST_OBJ   = host.o

CXXFLAGS += -c -I${SDKTARGETSYSROOT}/usr/include/xrt/

LDFLAGS += -lxrt_coreutil 

.PHONY: clean

${HOST_EXE}: ${HOST_OBJ}
	${CXX} -o $@ $^ ${LDFLAGS}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -o $@ $<

clean:
	rm -rf ${HOST_OBJ} .Xil ${HOST_EXE} 
