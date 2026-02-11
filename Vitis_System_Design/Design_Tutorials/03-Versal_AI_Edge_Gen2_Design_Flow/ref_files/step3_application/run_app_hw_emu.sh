#!/bin/bash

# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

fpgautil -b gm2aie.pdi -o gm2aie.dtbo
export XCL_EMULATION_MODE=hw_emu
# Command to launch application. Format: <host_exe_path> <cmd_line_args>
./application ./gm2aie.xclbin
return_code=$?
if [ $return_code -ne 0 ]; then
        echo "ERROR: Embedded host run failed, RC=$return_code"
fi
echo "INFO: Embedded host run completed."
#export XILINX_VITIS=/run/media/mmcblk0p1
#export XILINX_XRT=/usr
#export XCL_EMULATION_MODE=hw_emu
#export XRT_INI_PATH=/run/media/mmcblk0p1/hw_emu_xrt.ini
#export LD_LIBRARY_PATH=/run/media/mmcblk0p1:/tmp:$LD_LIBRARY_PATH


# Command to launch application. Format: <host_exe_path> <cmd_line_args>
