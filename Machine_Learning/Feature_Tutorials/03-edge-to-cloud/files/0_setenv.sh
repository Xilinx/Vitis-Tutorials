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


conda activate vitis-ai-tensorflow

# folders
export DSET_ROOT=./dataset
export BUILD=./build
export LOG=${BUILD}/logs
export KERAS=${BUILD}/keras_model
export TB_LOG=${BUILD}/tb_logs
export TFCKPT_DIR=${BUILD}/tf_chkpt
export FREEZE=${BUILD}/freeze
export QUANT=${BUILD}/quantize
export COMPILE_U50=${BUILD}/compile_u50
export COMPILE_ZCU102=${BUILD}/compile_zcu102
export TARGET_U50=${BUILD}/target_u50
export TARGET_ZCU102=${BUILD}/target_zcu102
export APP=./application

# make the necessary folders
mkdir -p ${KERAS}
mkdir -p ${LOG}

# logs & results files
export DSET_LOG=dset.log
export TRAIN_LOG=train.log
export KERAS_LOG=keras_2_tf.log
export FREEZE_LOG=freeze.log
export EVAL_FR_LOG=eval_frozen_graph.log
export QUANT_LOG=quant.log
export EVAL_Q_LOG=eval_quant_graph.log
export COMP_LOG_U50=compile_u50.log
export COMP_LOG_ZCU102=compile_zcu102.log


# TensorFlow files
export FROZEN_GRAPH=frozen_graph.pb
export TFCKPT=tf_float.ckpt

# calibration list file
export CALIB_LIST=calib_list.txt
export CALIB_IMAGES=1000

# network parameters
export INPUT_HEIGHT=200
export INPUT_WIDTH=250
export INPUT_CHAN=3
export INPUT_SHAPE=?,${INPUT_HEIGHT},${INPUT_WIDTH},${INPUT_CHAN}
export INPUT_NODE=input_1
#export OUTPUT_NODE=flatten/Reshape
export OUTPUT_NODE=conv2d_30/BiasAdd
export NET_NAME=customcnn

# training parameters
export EPOCHS=180
export BATCHSIZE=80
export LEARNRATE=0.0002

