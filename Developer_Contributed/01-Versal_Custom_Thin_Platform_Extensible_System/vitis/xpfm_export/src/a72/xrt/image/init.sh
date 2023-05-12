# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

SCRIPTPATH4=$(dirname $BASH_SOURCE)
echo ${SCRIPTPATH4}
cp ${SCRIPTPATH4}/platform_desc.txt /etc/xocl.txt
export XILINX_XRT=/usr
