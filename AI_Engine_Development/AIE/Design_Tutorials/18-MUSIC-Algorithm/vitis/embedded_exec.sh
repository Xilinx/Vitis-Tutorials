#!/bin/bash
#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Vivek Kanchanapalli
#
# Run the MUSIC Algorithm host application on the VCK190 board.
#
# Usage:
#   ./embedded_exec.sh [port]
#
#   port : TCP port the host application listens on for MATLAB client
#          connections. Default: 8888
#
# The MATLAB client (matlab/HIL/MatlabClient/TcpIp/getIpAddr.m) must be
# configured with the board IP address and the same port number before
# running sendSnapshots.m on the host PC.
#
# Example:
#   ./embedded_exec.sh           # listen on default port 8888
#   ./embedded_exec.sh 9000      # listen on port 9000

export XILINX_XRT=/usr

PORT=${1:-8888}
XCLBIN=binary_container_1.xclbin

if [ ! -f "$XCLBIN" ]; then
    echo "ERROR: $XCLBIN not found in $(pwd)"
    echo "       Run this script from the directory containing $XCLBIN"
    exit 1
fi

echo "INFO: Starting MUSIC Algorithm host application"
echo "INFO: xclbin : $XCLBIN"
echo "INFO: TCP port: $PORT"
echo "INFO: Configure MATLAB client getIpAddr.m with board IP and port $PORT"
echo ""

./host_app -c "$XCLBIN" -p "$PORT"
return_code=$?

if [ $return_code -ne 0 ]; then
    echo ""
    echo "ERROR: host_app exited with error code $return_code"
    exit $return_code
fi

echo ""
echo "INFO: host_app completed successfully."
