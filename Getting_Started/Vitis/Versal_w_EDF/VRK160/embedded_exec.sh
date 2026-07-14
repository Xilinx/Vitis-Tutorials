#!/bin/bash

#/*
#Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier:MIT
#*/

cat /dev/null
export LD_LIBRARY_PATH=/mnt:/tmp:$LD_LIBRARY_PATH
export PATH=$PATH:/sbin
export XILINX_XRT=/usr
if [ -f platform_desc.txt ]; then
	echo "INFO: Copying the file platform_desc.txt"
	cp platform_desc.txt /etc/xocl.txt
fi
echo "INFO: Load the pdi and dtbo using fpgautil"
if [ -f gmio_async_xrt.pdi ] && [ -f gmio_async_xrt.dtbo ]; then
    fpgautil -b gmio_async_xrt.pdi -o gmio_async_xrt.dtbo
else
    echo "WARNING: gmio_async_xrt.pdi or gmio_async_xrt.dtbo not present; zocl may not bind"
fi

./host.exe gmio_async_xrt.xclbin
return_code=$?
if [ $return_code -ne 0 ]; then
	echo "ERROR: Embedded host run failed, RC=$return_code"
fi
echo "INFO: Embedded host run completed."
