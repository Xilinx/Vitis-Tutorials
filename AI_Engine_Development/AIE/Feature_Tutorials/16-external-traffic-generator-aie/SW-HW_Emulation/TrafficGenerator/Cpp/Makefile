
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

GCC=${XILINX_VIVADO}/tps/lnx64/gcc-9.3.0/bin/gcc
# GCC_DIR := $(XILINX_VITIS)/lnx64/tools/gcc
# GCC=$(GCC_DIR)/bin/gcc
IPC_XTLM= $(XILINX_VIVADO)/data/emulation/cpp/src/xtlm_ipc
IPC_XTLM_INC= $(XILINX_VIVADO)/data/emulation/cpp/inc/xtlm_ipc
LOCAL_IPC= $(IPC_XTLM)

LD_LIBRARY_PATH=$(XILINX_VITIS)/lib/lnx64.o/Default/:$LD_LIBRARY_PATH


INC_FLAGS := -I$(IPC_XTLM_INC)
LIB_FLAGS := -L$(XILINX_VIVADO)/lib/lnx64.o/ -L$(GCC)/../../lib64/ -lstdc++ -lpthread -L$(XILINX_VIVADO)/data/emulation/cpp/lib/ -lxtlm_ipc -L$(XILINX_VIVADO)/lib/lnx64.o/

all: compile

compile: main.cpp 
	$(GCC) -g main.cpp  $(IPC_SRC) $(INC_FLAGS) $(LIB_FLAGS)  -o chain

clean :
	rm -f chain*