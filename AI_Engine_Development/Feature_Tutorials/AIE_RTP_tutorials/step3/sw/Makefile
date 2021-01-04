#
#Copyright 2020 Xilinx, Inc.
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#    http://www.apache.org/licenses/LICENSE-2.0
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
#
HOST_EXE     = ../host.exe
HOST_INC   = -I../ -I../aie
HOST_OBJ   = aie_control_xrt.o host.o
AIE_CTRL_CPP = ../Work/ps/c_rts/aie_control_xrt.cpp
SYSROOT = ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2020.2/sysroots/aarch64-xilinx-linux/

HOST_CXX      = aarch64-linux-gnu-g++


CXXFLAGS += -std=c++14 -I$(XILINX_HLS)/include/ -I${SYSROOT}/usr/include/xrt/ -g -Wall -c -fmessage-length=0 --sysroot=${SYSROOT} -I${XILINX_VITIS}/aietools/include ${HOST_INC}

LDFLAGS += -ladf_api_xrt -lgcc -lc -lxilinxopencl -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ -L${SYSROOT}/usr/lib/ --sysroot=${SYSROOT} -L$(XILINX_VITIS)/aietools/lib/aarch64.o


.PHONY: clean

${HOST_EXE}: ${HOST_OBJ}
	${HOST_CXX} -o $@ $^ ${LDFLAGS}

%.o: %.cpp
	${HOST_CXX} ${CXXFLAGS} -o $@ $<

aie_control_xrt.cpp: ${AIE_CTRL_CPP}
	cp -f ${AIE_CTRL_CPP} .

clean:
	rm -rf ${HOST_OBJ} .Xil ${HOST_EXE} aie_control_xrt.cpp
