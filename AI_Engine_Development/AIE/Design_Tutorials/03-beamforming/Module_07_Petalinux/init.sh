

#Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

SCRIPTPATH4=$(dirname $BASH_SOURCE)
echo ${SCRIPTPATH4}
cp ${SCRIPTPATH4}/platform_desc.txt /etc/xocl.txt
export XILINX_XRT=/usr
