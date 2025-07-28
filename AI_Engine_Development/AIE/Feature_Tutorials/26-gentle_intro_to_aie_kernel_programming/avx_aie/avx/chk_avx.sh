####################################################################
# Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
####################################################################

#! /usr/bin/env bash

echo
echo "Checking for AVX capabilities on this machine by looking at /proc/cpuinfo..."
echo

cat /proc/cpuinfo | grep -nw "avx" > /dev/null
ret_val=$?
if [[ $ret_val -eq 0 ]]; then
    echo "This machine supports AVX!"
else
    echo "This machine does NOT support AVX!"
fi

echo
