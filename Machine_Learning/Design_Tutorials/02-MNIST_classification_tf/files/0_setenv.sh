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

# Author: Mark Harvey

# activate the conda virtual environment
conda activate vitis-ai-tensorflow

# folders
export BUILD=./build
export LOG=${BUILD}/logs
export TB_LOG=${BUILD}/tb_logs
export CHKPT_DIR=${BUILD}/chkpts
export FREEZE=${BUILD}/freeze
export QUANT=${BUILD}/quantize
export COMPILE_U50=${BUILD}/compile_u50
export COMPILE_ZCU102=${BUILD}/compile_zcu102
export TARGET_U50=${BUILD}/target_u50
export TARGET_ZCU102=${BUILD}/target_zcu102
export APP=./application

# make folders
mkdir -p ${BUILD}
mkdir -p ${LOG}
mkdir -p ${TB_LOG}
mkdir -p ${CHKPT_DIR}

# checkpoints & graphs filenames
export CHKPT_FILENAME=float_model.ckpt
export INFER_GRAPH_FILENAME=inference_graph.pb
export FROZEN_GRAPH=frozen_graph.pb

# logs & results files
export TRAIN_LOG=train.log
export FREEZE_LOG=freeze.log
export EVAL_FR_LOG=eval_frozen_graph.log
export EVAL_Q_LOG=eval_quant_graph.log
export QUANT_LOG=quant.log
export COMP_LOG_U50=compile_u50.log
export COMP_LOG_ZCU102=compile_zcu102.log

# training parameters
export EPOCHS=5
export LEARNRATE=0.001
export BATCHSIZE=100

# list of GPUs to use
export CUDA_DEVICE_ORDER=PCI_BUS_ID
export CUDA_VISIBLE_DEVICES="0"

export TF_FORCE_GPU_ALLOW_GROWTH=true

# network parameters
export INPUT_HEIGHT=28
export INPUT_WIDTH=28
export INPUT_CHAN=1
export INPUT_SHAPE=?,${INPUT_HEIGHT},${INPUT_WIDTH},${INPUT_CHAN}
export INPUT_NODE=images_in
export OUTPUT_NODE=conv2d_3/BiasAdd
export NET_NAME=customcnn


# number of images used in calibration
export CALIB_IMAGES=1000

