#!/bin/bash
export LD_LIBRARY_PATH=$XILINX_VITIS/lib/lnx64.o/Default/:$LD_LIBRARY_PATH

echo "Launched Python Master"
echo "$PWD"
export PYTHONPATH=$XILINX_VIVADO/data/emulation/python/xtlm_ipc_v2/

export LD_LIBRARY_PATH=$XILINX_VIVADO/tps/lnx64/python-3.8.3/lib/:$LD_LIBRARY_PATH
$XILINX_VIVADO/tps/lnx64/python-3.8.3/bin/python3 ./TrafficGenerator/Python/xtg_aie.py 