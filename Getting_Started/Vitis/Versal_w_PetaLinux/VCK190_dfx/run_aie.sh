#!/bin/bash
#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

./app dfx1.xclbin dfx2.xclbin
return_code=$?
if [ $return_code -ne 0 ]; then
echo "ERROR: host run failed, RC=$return_code"
fi
echo "INFO: host run completed."
