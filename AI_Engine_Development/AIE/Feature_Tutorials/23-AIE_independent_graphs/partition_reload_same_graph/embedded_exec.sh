#!/bin/bash
#Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

export XILINX_XRT=/usr

./host.exe pl.xclbin pr0.xclbin pr1.xclbin pr2.xclbin
