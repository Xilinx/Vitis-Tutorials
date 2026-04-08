#!/bin/bash
#Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

export XILINX_XRT=/usr

./host.exe pl.xclbin pr0.xclbin pr0_2.xclbin pr1.xclbin pr1_2.xclbin pr2.xclbin
