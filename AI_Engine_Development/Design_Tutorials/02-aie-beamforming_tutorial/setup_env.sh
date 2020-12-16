# © Copyright 2020 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#!/bin/bash
export TUTORIAL_PATH=/group/siapps/dharap/tutorials/all_tutorials/Tutorials/AI_Engine
export BEAMFORMING=$TUTORIAL_PATH/beamforming
export BUILD=2020.2_daily_latest
export BUILD2=2020.2
export XILINX_LOC=/proj/xbuilds/$BUILD/installs/lin64/Vitis/$BUILD2
export XILINX_VITIS_AIETOOLS=$XILINX_LOC/aietools
export LM_LICENSE_FILE=2100@aiengine
export PATH_TO_BSP=/proj/petalinux/2020.2/petalinux-v2020.2_daily_latest_bsps/release
source /proj/petalinux/2020.2/petalinux-v2020.2_daily_latest/tool/petalinux-v2020.2-final/settings.sh

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

echo $BEAMFORMING
echo $BUILD
echo $BUILD2
echo $sim

