#!/bin/bash

#/*
#Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier:MIT
#*/

# The Linux DT loaded by the PLM during boot does not contain a
# `xlnx,zocl-versal` compatible node (only the PL/AIE region nodes).
# Without zocl registering a device, XRT's xrt::device(0) throws
# "No such device". We apply the auto-generated zocl overlay at runtime via
# fpgautil so zocl can bind and create the device node before the host
# application runs.
export XCL_EMULATION_MODE=hw_emu
echo "INFO: Load the pdi and dtbo using fpgautil"
if [ -f gmio_async_xrt.dtbo ] && [ -f gmio_async_xrt.pdi ]; then
    fpgautil -b gmio_async_xrt.pdi -o gmio_async_xrt.dtbo
else
    echo "WARNING: gmio_async_xrt.pdi or gmio_async_xrt.dtbo not present; zocl may not bind"
fi

if [ -f platform_desc.txt ]; then
    echo "INFO: Copying the file platform_desc.txt"
    cp platform_desc.txt /etc/xocl.txt
fi

./host.exe gmio_async_xrt.xclbin
return_code=$?
if [ $return_code -ne 0 ]; then
        echo "ERROR: Embedded host run failed, RC=$return_code"
fi
echo "INFO: Embedded host run completed."
