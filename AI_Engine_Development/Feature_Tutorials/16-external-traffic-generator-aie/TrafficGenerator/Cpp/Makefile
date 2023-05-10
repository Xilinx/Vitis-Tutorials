
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

PROTO_PATH=$(XILINX_VIVADO)/data/simmodels/xsim/2023.1/lnx64/9.3.0/ext/protobuf/
GCC=${XILINX_VIVADO}/tps/lnx64/gcc-9.3.0/bin/gcc
# GCC_DIR := $(XILINX_VITIS)/lnx64/tools/gcc
# GCC=$(GCC_DIR)/bin/gcc
IPC_XTLM= $(XILINX_VIVADO)/data/emulation/cpp/src/xtlm_ipc
IPC_XTLM_INC= $(XILINX_VIVADO)/data/emulation/cpp/inc/xtlm_ipc
LOCAL_IPC= $(IPC_XTLM)

LD_LIBRARY_PATH:=$(XILINX_VIVADO)/data/simmodels/xsim/2023.1/lnx64/9.3.0/ext/protobuf/:$(XILINX_VIVADO)/lib/lnx64.o/Default:$(XILINX_VIVADO)/lib/lnx64.o/:$(LD_LIBRARY_PATH)

PLKERNELS_DIR := ../../pl_kernels
PLKERNELS := $(PLKERNELS_DIR)/polar_clip.cpp


IPC_SRC := $(IPC_XTLM)/axis/*.cpp $(IPC_XTLM)/common/*.cpp $(IPC_XTLM)/common/*.cc
INC_FLAGS := -I$(IPC_XTLM_INC) -I$(IPC_XTLM_INC)/axis/ -I$(IPC_XTLM_INC)/common/  -I$(PROTO_PATH)/include/ -I$(PLKERNELS_DIR) -I$(XILINX_HLS)/include -I$(XILINX_VIVADO)/data/simmodels/xsim/2023.1/lnx64/9.3.0/ext/protobuf/include
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
