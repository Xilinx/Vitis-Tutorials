#
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#



HOST_EXE     = ../host.exe
HOST_INC   = -I../ -I$(AIE_DIR)/src
HOST_OBJ   =  host.o

SIZES_D := -DsizeM=$(sizeM) -DsizeK=$(sizeK) -DsizeN=$(sizeN) -DsubM=$(subM) -DsubK=$(subK) -DsubN=$(subN) -DNIterations=$(NIterations) -DPLIOW=$(PLIOW)

# CXXFLAGS += -std=c++17 -I$(XILINX_HLS)/include/ -I${SDKTARGETSYSROOT}/usr/include/xrt/ -g -Wall -c -fmessage-length=0 --sysroot=${SDKTARGETSYSROOT} -I${XILINX_VITIS}/aietools/include ${HOST_INC} -I${AIE_DIR}

CXXFLAGS += -std=c++17 -I$(XILINX_HLS)/include/ -I${SDKTARGETSYSROOT}/usr/include/xrt/ -g -Wall -c -fmessage-length=0 --sysroot=${SDKTARGETSYSROOT}  ${HOST_INC} -I${AIE_DIR} -O1

# LDFLAGS += -ladf_api_xrt -lgcc -lc -lxilinxopencl -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ -L${SDKTARGETSYSROOT}/usr/lib/ --sysroot=${SDKTARGETSYSROOT} -L$(XILINX_VITIS)/aietools/lib/aarch64.o

LDFLAGS +=  -lgcc -lc  -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ -L${SDKTARGETSYSROOT}/usr/lib/ --sysroot=${SDKTARGETSYSROOT} -L$(XILINX_VITIS)/aietools/lib/aarch64.o


.PHONY: clean

${HOST_EXE}: ${HOST_OBJ}
	${CXX} -o $@ $^ ${LDFLAGS}

%.o: %.cpp
	${CXX} ${CXXFLAGS} $(SIZES_D) -o $@ $<


clean:
	rm -rf ${HOST_OBJ} .Xil ${HOST_EXE}
