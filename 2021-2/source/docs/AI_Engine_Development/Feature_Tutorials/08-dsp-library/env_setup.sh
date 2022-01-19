  
#  (c) Copyright 2021 Xilinx, Inc.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License. 

#Setup environment variables 
export PLATFORM_REPO_PATHS=<YOUR-PLATFORMS-DIRECTORY>
export DSPLIB_ROOT=<PATH-TO-DSP-LIBRARY>

#Source setup scripts
source $XILINX_VITIS/settings64.sh

#Print env variables
echo "XILINX_VITIS       : "$XILINX_VITIS
echo "PLATFORM_REPO_PATHS: "$PLATFORM_REPO_PATHS
echo "DSPLIB_ROOT        : "$DSPLIB_ROOT

