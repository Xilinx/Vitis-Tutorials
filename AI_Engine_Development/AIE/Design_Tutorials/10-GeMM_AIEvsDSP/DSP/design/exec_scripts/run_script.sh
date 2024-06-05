#!/bin/bash
#Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

date

export XILINX_XRT=/usr

./gemm_dsp_xrt.elf a.xclbin

date
