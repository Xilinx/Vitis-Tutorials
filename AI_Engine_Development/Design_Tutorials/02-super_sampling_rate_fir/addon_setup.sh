#
# Copyright 2022 Xilinx, Inc.
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
#
#!/bin/bash


# =========================================================
# Platform Selection...
# =========================================================
export PFM_NAME=xilinx_vck190_base_202210_1

# =========================================================
# Do not change anything below this line unless necessary
# =========================================================

export TOOLS_VERSION=`basename $XILINX_VITIS`

# Python setup (libraries for external traffic generators)
export PYTHONPATH=${PWD}/Utils:${PYTHONPATH}
export PATH=${PWD}/Utils:$PATH



#Print env variables
echo "Tools Version      : "$TOOLS_VERSION
echo "XILINX_VITIS       : "$XILINX_VITIS
