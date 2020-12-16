#  © Copyright 2020 Xilinx, Inc.
#  Licensed under the Apache License, Version 2.0 (the 'License');
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  
#       http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an 'AS IS' BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
                    


#Setup environment variables 
export XILINX_XRT=/proj/xbuilds/2020.2_daily_latest/xbb/xrt/packages/xrt-2.1.0-centos/opt/xilinx/xrt
export COMMSLIB_ROOT=$PWD/design/aie_src/commslib

#INTERNAL ONLY: WILL BE REMOVED 
export LM_LICENSE_FILE=2100@aiengine
export PLATFORM_REPO_PATHS=/proj/xbuilds/2020.2_daily_latest/internal_platforms

#Source setup scripts
source /proj/xbuilds/2020.2_daily_latest/installs/lin64/Vitis/2020.2/settings64.sh
source $XILINX_XRT/setup.sh

#Print env variables
echo "XILINX_VITIS       : "$XILINX_VITIS
echo "XILINX_VIVADO      : "$XILINX_VIVADO
echo "XILINX_HSL         : "$XILINX_HLS
echo "XILINX_XRT         : "$XILINX_XRT
echo "PLATFORM_REPO_PATHS: "$PLATFORM_REPO_PATHS
echo "COMMSLIB_ROOT      : "$COMMSLIB_ROOT

#INTERNAL ONLY: WILL BE REMOVED 
#Create Vivado_int.tcl and HLS_init.tcl if they do not exist already
touch ~/.Xilinx/Vivado/Vivado_init.tcl
touch ~/.Xilinx/HLS_init.tcl 

#INTERNAL ONLY: WILL BE REMOVED 
#Append enable_beta_device xcvc* if it is not already there
if grep -q enable_beta_device ~/.Xilinx/Vivado/Vivado_init.tcl; then
    echo "Beta device found!"
else
    echo "enable_beta_device xcvc*"
    echo "enable_beta_device xcvc*" >> ~/.Xilinx/Vivado/Vivado_init.tcl
fi 

if grep -q enable_beta_device ~/.Xilinx/HLS_init.tcl; then
    echo "Beta device found!"
else
    echo "enable_beta_device xcvc*"
    echo "enable_beta_device xcvc*" >> ~/.Xilinx/HLS_init.tcl
fi 
