########################################
#!/bin/bash

# (c) Copyright 2014 – 2020 Xilinx, Inc. All rights reserved.
#
# This file contains confidential and proprietary information
# of Xilinx, Inc. and is protected under U.S. and
# international copyright and other intellectual property
# laws.
#
# DISCLAIMER
# This disclaimer is not a license and does not grant any
# rights to the materials distributed herewith. Except as
# otherwise provided in a valid license issued to you by
# Xilinx, and to the maximum extent permitted by applicable
# law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
# WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
# AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
# BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
# INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and XILINX INTERNAL
# (2) Xilinx shall not be liable (whether in contract or tort,
# including negligence, or under any other theory of
# liability) for any loss or damage of any kind or nature
# related to, arising under or in connection with these
# materials, including for any direct, or any indirect,
# special, incidental, or consequential loss or damage
# (including loss of data, profits, goodwill, or any type of
# loss or damage suffered as a result of any action brought
# by a third party) even if such damage or loss was
# reasonably foreseeable or Xilinx had been advised of the
# possibility of the same. 
#
# CRITICAL APPLICATIONS
# Xilinx products are not designed or intended to be fail-
# safe, or for use in any application requiring fail-safe
# performance, such as life-support or safety devices or
# systems, Class III medical devices, nuclear facilities,
# applications related to the deployment of airbags, or any
# other applications that could lead to death, personal
# injury, or severe property or environmental damage
# (individually and collectively, "Critical
# Applications"). Customer assumes the sole risk and
# liability of any use of Xilinx products in Critical
# Applications, subject only to applicable laws and
# regulations governing limitations on product liability.
# 
# THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
# PART OF THIS FILE AT ALL TIMES.
#
# 
########################################



export XILINXD_LICENSE_FILE=2100@aiengine


export LM_LICENSE_FILE=2100@aiengine

export BUILD=2021.1_daily_latest
#export BUILD=2021.1_0528_1

export BUILD2=2021.1
export XILINX_LOC=/proj/xbuilds/$BUILD/installs/lin64/Vitis/$BUILD2
export XILINX_VIVADO=/proj/xbuilds/$BUILD/installs/lin64/Vivado/$BUILD2
export XILINX_VITIS=/proj/xbuilds/$BUILD/installs/lin64/Vitis/$BUILD2


export XILINX_VITIS_AIETOOLS=$XILINX_LOC/aietools
export LM_LICENSE_FILE=2100@aiengine
export XILINXD_LICENSE_FILE=2100@aiengine-eng

export sim=xsim

# Update path with XILINX_VITIS_AIETOOLS
export PATH=$XILINX_VITIS_AIETOOLS/bin:$XILINX_LOC/bin:$PATH

# Setup PLATFORM_REPO_PATHS
export PLATFORM_REPO_PATHS=/proj/xbuilds/$BUILD/internal_platforms

# Source XRT, VITIS, and AIE Tools
source $XILINX_VITIS_AIETOOLS/scripts/aietools_env.sh
source $XILINX_LOC/settings64.sh

OSDIST=`lsb_release -i |awk -F: '{print tolower($2)}' | tr -d ' \t'`
OSREL=`lsb_release -r |awk -F: '{print tolower($2)}' |tr -d ' \t'`

if [[ $OSDIST == "centos" ]] || [[ $OSDIST == "redhat"* ]]; then
    source /proj/xbuilds/$BUILD/xbb/xrt/packages/xrt-2.1.0-centos/opt/xilinx/xrt/setup.sh
fi

if [[ $OSDIST == "ubuntu" ]]; then
    dir=xrt-2.1.0-ubuntu${OSREL/./}
    source /proj/xbuilds/$BUILD/xbb/xrt/packages/${dir}/opt/xilinx/xrt/setup.sh
fi

if [ ! -f ~/.Xilinx/Vivado/Vivado_init.tcl ]; then
    echo "Vivado_init.tcl does not exist, creating it."
    touch ~/.Xilinx/Vivado/Vivado_init.tcl
fi

if [ ! -f ~/.Xilinx/HLS_init.tcl ]; then
    echo "HLS_init.tcl does not exist, creating it."
    touch ~/.Xilinx/HLS_init.tcl
fi


if [ ! `grep -q "enable_beta_device" ~/.Xilinx/Vivado/Vivado_init.tcl && echo $?` ]; then
    echo "enable_beta_device xcvc*" >> ~/.Xilinx/Vivado/Vivado_init.tcl
else
    echo "Beta device found!"
fi

if [ ! `grep -q "enable_beta_device"* ~/.Xilinx/HLS_init.tcl && echo $?` ]; then
    echo "enable_beta_device xcvc*" >> ~/.Xilinx/HLS_init.tcl
else
    echo "Beta device found!"
fi

#Python path for XTLM modules
export PYTHONPATH=$PYTHONPATH:$XILINX_VIVADO/data/emulation/hw_em/lib/python:$XILINX_VIVADO/data/emulation/ip_utils/xtlm_ipc/xtlm_ipc_v1_0/python

#Bring in Python3+ with Numpy
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}${LD_LIBRARY_PATH:+:}${XILINX_VITIS}/tps/lnx64/python-3.8.3/lib:${XILINX_VIVADO}/tps/lnx64/gcc-6.2.0/lib64


PATH=$PATH:${XILINX_VITIS}/tps/lnx64/python-3.8.3/bin
