#
#Copyright 2021-2022 Xilinx, Inc.
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

PLATFORM := xilinx_u50_gen3x16_xdma_5_202210_1
MODE := hw

KERNEL_SRC := kernel.cpp 
KERNEL_XO := kernel.xo   
KERNEL_XCLBIN := kernel.xclbin

HOST_SRC := host.cpp
#HOST_SRC := host_p2p.cpp
EXE := host.exe

VPPFLAGS := -s -t $(MODE) --platform $(PLATFORM)


${KERNEL_XO}: ${KERNEL_SRC} 
	v++ -c -k increment $(VPPFLAGS) -o $@  $^

${KERNEL_XCLBIN}: ${KERNEL_XO} 
	v++ -l $(VPPFLAGS) -o $@ $^


${EXE} : ${HOST_SRC}
	g++ -g -o $@ ${HOST_SRC} -std=c++17 -I${XILINX_XRT}/include -L${XILINX_XRT}/lib -lxrt_coreutil -pthread


xclbin : $(KERNEL_XCLBIN)
app : ${EXE}
