#!/bin/bash
echo "master launched"
echo "$PWD"
export IPC_XTLM=$XILINX_VIVADO/data/emulation/cpp/
export LD_LIBRARY_PATH=$IPC_XTLM/lib:$XILINX_VIVADO/lib/lnx64.o/Default:$XILINX_VIVADO/lib/lnx64.o/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$XILINX_VIVADO/data/emulation/cpp/lib/:$LD_LIBRARY_PATH
export GCC=$XILINX_VIVADO/tps/lnx64/gcc-9.3.0/bin/gcc

export IPC_XTLM=$XILINX_VIVADO/data/emulation/cpp/src/xtlm_ipc
export IPC_XTLM_INC=$XILINX_VIVADO/data/emulation/cpp/inc/xtlm_ipc
export LOCAL_IPC=$IPC_XTLM

export LD_LIBRARY_PATH=$XILINX_VITIS/lib/lnx64.o/Default/:$LD_LIBRARY_PATH

$GCC -g ./TrafficGenerator/Cpp/main.cpp -I$IPC_XTLM_INC -L$XILINX_VIVADO/lib/lnx64.o/ -L$GCC/../../lib64/ -lstdc++ -lpthread -L$XILINX_VIVADO/data/emulation/cpp/lib/ -lxtlm_ipc -L$XILINX_VIVADO/lib/lnx64.o/ -o chain

./chain