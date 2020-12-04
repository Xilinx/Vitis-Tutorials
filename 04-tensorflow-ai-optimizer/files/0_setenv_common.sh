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

# This script is called by 0_setenv_np.sh and 0_setenv_pr.sh

# folder names
export DSET_ROOT=./dataset
export LOG=${BUILD}/logs
export TRAIN_DIR=${BUILD}/train
export TB_DIR=${BUILD}/tb_logs
export PRUNE_ANA=${BUILD}/prune_ana
export PRUNE_DIR=${BUILD}/prune
export TRSF_DIR=${BUILD}/transform
export FREEZE_DIR=${BUILD}/freeze
export FT_DIR=${BUILD}/ft
export QUANT_DIR=${BUILD}/quant
export COMPILE_ZCU102=${BUILD}/compile_zcu102
export APP=./application
export TARGET_ZCU102=${BUILD}/target_zcu102


mkdir -p ${BUILD}
mkdir -p ${LOG}


# logs & results filenames
export DSET_LOG=create_dataset.log
export TRAIN_LOG=train.log
export PRUNE_ANA_LOG=prune_analysis.log
export PRUNE_LOG=prune.log
export EVAL_FT_LOG=eval_tuned_graph.log
export FT_LOG=fine_tune.log
export GEN_DENSE_LOG=generate_dense.log
export FREEZE_LOG=freeze.log
export EVAL_FR_LOG=eval_frozen_graph.log
export QUANT_LOG=quant.log
export EVAL_Q_LOG=eval_quant_graph.log
export COMP_LOG_ZCU102=compile_zcu102.log



# network & training parameters
export INPUT_HEIGHT=224
export INPUT_WIDTH=224
export INPUT_CHAN=3
export INPUT_SHAPE="1,${INPUT_HEIGHT},${INPUT_WIDTH},${INPUT_CHAN}"
export INPUT_SHAPE_Q="?,${INPUT_HEIGHT},${INPUT_WIDTH},${INPUT_CHAN}"
export INPUT_NODES=images_in
export OUTPUT_NODES=dense_2/BiasAdd
export BATCHSIZE=200
export INIT_LR=0.001
# training epochs
export TRAIN_EPOCHS=200
# fine-tuning epochs
export FT_EPOCHS=80

# checkpoint & graph names
export BASE_GRAPH=inference_graph.pbtxt
export BASE_CKPT=trained.ckpt
export PRUNE_IN_GRAPH=prune_in_graph.pbtxt
export PRUNE_IN_CKPT=prune_in.ckpt
export PRUNED_GRAPH=pruned_graph.pbtxt
export PRUNED_CKPT=pruned.ckpt
export FT_CKPT=fine_tuned.ckpt
export TRSF_CKPT=transform.ckpt
export FROZEN_PB=frozen_graph.pb

# list of GPUs to use
export CUDA_DEVICE_ORDER=PCI_BUS_ID
export CUDA_VISIBLE_DEVICES="0"

export TF_FORCE_GPU_ALLOW_GROWTH=true

