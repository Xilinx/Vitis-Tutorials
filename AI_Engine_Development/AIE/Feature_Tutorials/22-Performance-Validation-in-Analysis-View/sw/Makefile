#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

include ../common.mk

$(info "-------------------------------------------------------------------------")
$(info "Host Software Build")
$(info "-------------------------------------------------------------------------")
$(info "Number of Antenna   : $(NAntenna)")
$(info "-------------------------------------------------------------------------")


HOST_EXE     = ../host.exe
HOST_API_EXE     = ../host_api.exe
HOST_INC   = -I../ -I$(AIE_DIR)/src
HOST_OBJ   =  host.o
HOST_API_OBJ   =  host_api.o
HOST_SRC = host.cpp

CXXFLAGS += -std=c++17 -I$(XILINX_HLS)/include/ -I${SDKTARGETSYSROOT}/usr/include/xrt/ -g -Wall -c -fmessage-length=0 --sysroot=${SDKTARGETSYSROOT}  ${HOST_INC} -I${AIE_DIR} -DFRAME_SIZE=$(FRAME_SIZE)

LDFLAGS += -lgcc -lc -lxilinxopencl -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ 
LDFLAGS += -L${SDKTARGETSYSROOT}/usr/lib/ --sysroot=${SDKTARGETSYSROOT} -L$(XILINX_VITIS)/aietools/lib/aarch64.o
 

SYSTEM_DEF :=  -DNAntenna=$(NAntenna)  -DPLIOW=$(PLIOW) -DDATAW=$(DATAW) -DFRAME_LENGTH=$(FRAME_LENGTH) 

.PHONY: clean all

all: $(HOST_EXE) $(HOST_API_EXE)

${HOST_EXE}: ${HOST_OBJ}
	${CXX} -o $@ $^ ${LDFLAGS} 

${HOST_API_EXE}: ${HOST_API_OBJ}
	${CXX} -o $@ $^ ${LDFLAGS} 

${HOST_OBJ}: ${HOST_SRC}
	${CXX} ${CXXFLAGS} $(SYSTEM_DEF) -DUSE_XRT_EVENT_API=false -o $@ $<

${HOST_API_OBJ}: ${HOST_SRC}
	${CXX} ${CXXFLAGS} $(SYSTEM_DEF) -DUSE_XRT_EVENT_API=true -o $@ $<

clean:
	rm -rf ${HOST_OBJ} ${HOST_API_OBJ} .Xil ${HOST_EXE} ${HOST_API_EXE}
