#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT



#!/bin/bash

echo ""
date
echo ""
SECONDS=0

cd /mnt/sd-mmcblk0p1
#export XILINX_XRT=/usr
./ps_app.exe --timesteps 1

return_code=$?

if [ $return_code -ne 0 ]; then
        echo "ERROR: Embedded host run failed, RC=$return_code"
else
        echo "INFO: TEST PASSED, RC=0"
fi

duration=$SECONDS

echo ""
echo "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed."
echo ""
date
echo ""
echo "INFO: Embedded host run completed."
echo ""

exit $return_code
