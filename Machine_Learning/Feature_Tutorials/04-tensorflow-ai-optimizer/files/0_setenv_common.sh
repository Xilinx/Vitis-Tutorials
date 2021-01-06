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
export FREEZE_DIR=${BUILD}/freeze
export QUANT_DIR=${BUILD}/quant
export COMPILE_ZCU102=${BUILD}/compile_zcu102
export COMPILE_U50=${BUILD}/compile_u50
export APP=./application
export TARGET_ZCU102=${BUILD}/target_zcu102
export TARGET_U50=${BUILD}/target_u50


mkdir -p ${BUILD}
mkdir -p ${LOG}


# logs & results filenames
export DSET_LOG=create_dataset.log
export TRAIN_LOG=train.log
export FREEZE_LOG=freeze.log
export EVAL_FR_LOG=eval_frozen_graph.log
export QUANT_LOG=quant.log
export EVAL_Q_LOG=eval_quant_graph.log
export COMP_LOG_ZCU102=compile_zcu102.log
export COMP_LOG_U50=compile_u50.log



# network & training parameters
export INPUT_HEIGHT=224
export INPUT_WIDTH=224
export INPUT_CHAN=3
export INPUT_SHAPE="1,${INPUT_HEIGHT},${INPUT_WIDTH},${INPUT_CHAN}"
export INPUT_SHAPE_Q="?,${INPUT_HEIGHT},${INPUT_WIDTH},${INPUT_CHAN}"
export INPUT_NODES=images_in
export OUTPUT_NODES=dense_2/BiasAdd
export BATCHSIZE=250
export INIT_LR=0.001
export TRAIN_EPOCHS=200

# checkpoint & graph names
export BASE_GRAPH=inference_graph.pbtxt
export BASE_CKPT=trained.ckpt
export FROZEN_PB=frozen_graph.pb

# list of GPUs to use
export CUDA_DEVICE_ORDER=PCI_BUS_ID
export CUDA_VISIBLE_DEVICES="1"

export TF_FORCE_GPU_ALLOW_GROWTH=true
export TF_CPP_MIN_LOG_LEVEL=3
