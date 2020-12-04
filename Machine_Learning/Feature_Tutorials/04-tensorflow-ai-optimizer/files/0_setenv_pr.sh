#!/bin/bash

# Copyright 2020 Xilinx Inc.
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

# Author: Mark Harvey, Xilinx Inc


# This shell script sets the environment for running the complete flow WITH pruning.



# path to Optimizer license
export XILINXD_LICENSE_FILE=./license/AI_OPTIMIZER.lic


# all files and folders are created under this folder
export BUILD=./build_pr


# set all other environment variables
source ./0_setenv_common.sh

# set input graph & checkpoint for freeze
export FREEZE_IN_GRAPH=${PRUNE_DIR}/${PRUNED_GRAPH}
export FREEZE_IN_CKPT=${TRSF_DIR}/${TRSF_CKPT}

# network name - elf file will include this string
export NET_NAME=alexnet_pr






