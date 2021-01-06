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
export XILINXD_LICENSE_FILE=AI_OPTIMIZER.lic


# all files and folders are created under this folder
export BUILD=./build_pr


# set all other environment variables
source ./0_setenv_common.sh

# folder names
export PRUNE_ANA=${BUILD}/prune_ana
export PRUNE_DIR=${BUILD}/prune
export TRSF_DIR=${BUILD}/transform
export FT_DIR=${BUILD}/ft


# network name - xmodel filename will include this string
export NET_NAME=alexnet_pr


# logs & results filenames
export PRUNE_ANA_LOG=prune_analysis.log
export PRUNE_LOG=prune.log
export EVAL_FT_LOG=eval_tuned_graph.log
export FT_LOG=fine_tune.log
export GEN_DENSE_LOG=generate_dense.log


# fine-tuning epochs
export FT_EPOCHS=200

# checkpoint & graph names
export PRUNE_IN_GRAPH=prune_in_graph.pbtxt
export PRUNE_IN_CKPT=prune_in.ckpt
export PRUNED_GRAPH=pruned_graph.pbtxt
export PRUNED_CKPT=pruned.ckpt
export FT_CKPT=fine_tuned.ckpt
export TRSF_CKPT=transform.ckpt


# set input graph & checkpoint for freeze
export FREEZE_IN_GRAPH=${PRUNE_DIR}/${PRUNED_GRAPH}
export FREEZE_IN_CKPT=${TRSF_DIR}/${TRSF_CKPT}

