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


conda activate vitis-ai-tensorflow

# folders
export BUILD=./build
export APP=./application
export LOG=${BUILD}/logs
export TB_LOG=${BUILD}/tb_logs
export KERAS=${BUILD}/keras_model
export TFCKPT_DIR=${BUILD}/tf_chkpt
export FREEZE=${BUILD}/freeze
export QUANT=${BUILD}/quantize


# make the necessary folders
mkdir -p ${KERAS}
mkdir -p ${LOG}

# logs & results files
export TRAIN_LOG=train.log
export KERAS_LOG=keras_2_tf.log
export FREEZE_LOG=freeze.log
export EVAL_FR_LOG=eval_frozen_graph.log
export QUANT_LOG=quant.log
export EVAL_Q_LOG=eval_quant_graph.log


# Keras checkpoint file
export K_MODEL=k_model.h5

# TensorFlow files
export FROZEN_GRAPH=frozen_graph.pb
export TFCKPT=tf_float.ckpt

# calibration list file
export CALIB_LIST=calib_list.txt
export CALIB_IMAGES=1000

# network parameters
export INPUT_HEIGHT=32
export INPUT_WIDTH=32
export INPUT_CHAN=3
export INPUT_SHAPE=?,${INPUT_HEIGHT},${INPUT_WIDTH},${INPUT_CHAN}
export INPUT_NODE=input_1
export OUTPUT_NODE=dense/BiasAdd
export NET_NAME=densenetx

# training parameters
export EPOCHS=200
export BATCHSIZE=150
export LEARNRATE=0.001

# list of GPUs to use
export CUDA_DEVICE_ORDER=PCI_BUS_ID
export CUDA_VISIBLE_DEVICES="0"

export TF_FORCE_GPU_ALLOW_GROWTH=true

