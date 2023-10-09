#!/bin/bash
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

date

export XILINX_XRT=/usr

./gemm_dsp_xrt.elf a.xclbin

date
