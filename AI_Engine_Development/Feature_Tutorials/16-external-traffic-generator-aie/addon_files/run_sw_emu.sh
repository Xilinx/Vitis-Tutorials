
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


#!/bin/bash

export XILINX_XRT=/usr
export XCL_EMULATION_MODE=sw_emu
export LD_LIBRARY_PATH=/mnt/sd*1:/tmp:$LD_LIBRARY_PATH

./host.exe a.xclbin
