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
if [ -f krnl_aieml.pdi ] && [ -f vek280.dtbo ]; then
    fpgautil -b krnl_aieml.pdi -o vek280.dtbo
else
    echo "WARNING: krnl_aieml.pdi or vek280.dtbo not present; zocl may not bind"
fi

./aieml_system krnl_aieml.xclbin
return_code=$?
if [ $return_code -ne 0 ]; then
	echo "ERROR: Embedded host run failed, RC=$return_code"
fi
echo "INFO: Embedded host run completed."
