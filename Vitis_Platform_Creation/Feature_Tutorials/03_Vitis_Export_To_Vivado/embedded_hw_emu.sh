#!/bin/bash
export LD_LIBRARY_PATH=/mnt:/tmp:$LD_LIBRARY_PATH
export XCL_EMULATION_MODE=hw_emu
export XILINX_VITIS=/mnt
export PATH=$PATH:/sbin
export XILINX_XRT=/usr
if [ -f platform_desc.txt ]; then
	echo "INFO: Copying the file platform_desc.txt"
	cp platform_desc.txt /etc/xocl.txt
fi
echo "INFO: Running application on board."
./host.exe krnl_lab8.xclbin
return_code=$?
if [ $return_code -ne 0 ]; then
	echo "ERROR: Embedded host run failed, RC=$return_code"
fi
echo "INFO: Embedded host run completed."
