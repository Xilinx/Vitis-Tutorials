#  (c) Copyright 2022 Xilinx, Inc.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

PROTO_PATH=$(XILINX_VIVADO)/data/simmodels/xsim/2022.1/lnx64/6.2.0/ext/protobuf/
GCC=/tools/batonroot/rodin/devkits/lnx64/gcc-6.2.0/bin/gcc
# GCC_DIR := $(XILINX_VITIS)/lnx64/tools/gcc
# GCC=$(GCC_DIR)/bin/gcc
IPC_XTLM= $(XILINX_VIVADO)/data/emulation/ip_utils/xtlm_ipc/xtlm_ipc_v1_0/cpp/src/
IPC_XTLM_INC= $(XILINX_VIVADO)/data/emulation/ip_utils/xtlm_ipc/xtlm_ipc_v1_0/cpp/inc/
LOCAL_IPC= $(IPC_XTLM)../

LD_LIBRARY_PATH:=$(XILINX_VIVADO)/data/simmodels/xsim/2022.1/lnx64/6.2.0/ext/protobuf/:$(XILINX_VIVADO)/lib/lnx64.o/Default:$(XILINX_VIVADO)/lib/lnx64.o/:$(LD_LIBRARY_PATH)

PLKERNELS_DIR := ../../pl_kernels
PLKERNELS := $(PLKERNELS_DIR)/polar_clip.cpp


IPC_SRC := $(LOCAL_IPC)/src/axis/*.cpp $(LOCAL_IPC)/src/common/*.cpp $(LOCAL_IPC)/src/common/*.cc
INC_FLAGS := -I$(LOCAL_IPC)/inc -I$(LOCAL_IPC)/inc/axis/ -I$(LOCAL_IPC)/inc/common/  -I$(PROTO_PATH)/include/ -I$(PLKERNELS_DIR) -I$(XILINX_HLS)/include
LIB_FLAGS := -L$(PROTO_PATH)/ -lprotobuf -L$(XILINX_VIVADO)/lib/lnx64.o/ -lrdizlib -L$(GCC)/../../lib64/ -lstdc++ -lpthread


# INC_FLAGS += -I$(GCC_DIR)/include/c++/4.6.3
# LIB_FLAGS += -L$(GCC_DIR)/lib

all: compile run

compile: main.cpp mm2s.hpp s2mm.hpp polar_clip.hpp  $(PLKERNELS)
	$(GCC) -g main.cpp  $(PLKERNELS) $(IPC_SRC) $(INC_FLAGS) $(LIB_FLAGS)  -o chain

run:
	./chain &

clean :
	rm -f chain*
