#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*
export PATH=$PATH:/sbin
export XILINX_XRT=/usr
export XILINX_VITIS=$PWD
devmem 0x40000000
return_code=$?
echo "$return_code"
if [ $return_code -ne 0 ]; then
echo "ERROR: host run failed, RC=$return_code"
else
echo "INFO: host run completed."
fi
