#!/bin/bash
#/*
#Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
#Copyright (C) 2022-2026 Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/
# Script to run the application on VCK190 board with segmented configuration

set -e

XCLBIN_FILE="gm2aie.xclbin"
PDI_FILE="gm2aie.pdi"
DTBO_FILE="gm2aie.dtbo"
APP_FILE="application"

echo "INFO: Running application on VCK190 with segmented configuration"

# Check if files exist
if [ ! -f "$XCLBIN_FILE" ]; then
    echo "ERROR: XCLBIN file not found: $XCLBIN_FILE"
    exit 1
fi

if [ ! -f "$PDI_FILE" ]; then
    echo "ERROR: PDI file not found: $PDI_FILE"
    exit 1
fi

if [ ! -f "$APP_FILE" ]; then
    echo "ERROR: Application file not found: $APP_FILE"
    exit 1
fi

# Load PL design (second segment) using fpgautil
if [ -f "$DTBO_FILE" ]; then
    echo "INFO: Loading PL design segment..."
    fpgautil -b $PDI_FILE -o $DTBO_FILE
    if [ $? -ne 0 ]; then
        echo "ERROR: Failed to load PL design"
        exit 1
    fi
    echo "INFO: PL design loaded successfully"
else
    echo "WARNING: DTBO file not found, attempting to load PDI without overlay"
    fpgautil -b $PDI_FILE
    if [ $? -ne 0 ]; then
        echo "ERROR: Failed to load PL design"
        exit 1
    fi
fi

# Run application
echo "INFO: Running application..."
./$APP_FILE $XCLBIN_FILE
return_code=$?

if [ $return_code -ne 0 ]; then
    echo "ERROR: Application run failed, return code: $return_code"
    exit $return_code
fi

echo "INFO: Application completed successfully"
exit 0

