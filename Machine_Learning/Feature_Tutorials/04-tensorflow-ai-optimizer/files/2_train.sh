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

# runs initial training and writes out a trained, float-pt checkpoint
train() {
  python -u train_ft.py \
          --target_acc    1.0 \
          --epochs        ${TRAIN_EPOCHS} \
          --batchsize     ${BATCHSIZE} \
          --init_lr       ${INIT_LR} \
          --output_ckpt   ${TRAIN_DIR}/${BASE_CKPT} \
          --tboard_logs   ${TB_DIR} \
          --input_height  ${INPUT_HEIGHT} \
          --input_width   ${INPUT_WIDTH} \
          --input_chan    ${INPUT_CHAN} \
          --gpu           ${CUDA_VISIBLE_DEVICES}
}


echo "-----------------------------------------"
echo "TRAINING STARTED.."
echo "-----------------------------------------"

conda activate vitis-ai-tensorflow

rm -rf ${TRAIN_DIR}
mkdir -p ${TRAIN_DIR}
rm -rf ${TB_DIR}
mkdir -p ${TB_DIR}

train 2>&1 | tee ${LOG}/${TRAIN_LOG}

echo "-----------------------------------------"
echo "TRAINING FINISHED"
echo "-----------------------------------------"

