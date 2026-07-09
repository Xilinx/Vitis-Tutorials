#!/bin/bash

#/*
#Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier:MIT
#*/

# The Linux DT loaded by the PLM during boot does not contain a
# `xlnx,zocl-versal` compatible node (only the PL/AIE region nodes). Without
# zocl registering a device, XRT's xrt::device(0) throws "No such device".
# We apply the auto-generated zocl overlay at runtime via fpgautil so zocl
# can bind and create the device node before the host application runs.
if [ -f vck190.pdi ] && [ -f vck190.dtbo ]; then
    echo "INFO: Loading PDI and applying zocl device-tree overlay via fpgautil..."
    fpgautil -b vck190.pdi -o vck190.dtbo || echo "WARNING: fpgautil failed (rc=$?)"
elif [ -f vck190.dtbo ]; then
    echo "INFO: Applying zocl device-tree overlay via fpgautil..."
    fpgautil -o vck190.dtbo || echo "WARNING: fpgautil overlay apply failed (rc=$?)"
else
    echo "WARNING: vck190.dtbo not present; XRT may fail to find device."
fi

export XCL_EMULATION_MODE=hw_emu
# Command to launch application. Format: <host_exe_path> <cmd_line_args>
./application ./vck190.xclbin
return_code=$?
if [ $return_code -ne 0 ]; then
        echo "ERROR: Embedded host run failed, RC=$return_code"
fi
echo "INFO: Embedded host run completed."
