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


# train, evaluate and save trained keras model
train() {
  python train.py \
    --input_height ${INPUT_HEIGHT} \
    --input_width  ${INPUT_WIDTH} \
    --input_chan   ${INPUT_CHAN} \
    --epochs       ${EPOCHS} \
    --learnrate    ${LEARNRATE} \
    --batchsize    ${BATCHSIZE} \
    --tboard       ${TB_LOG} \
    --keras_hdf5   ${KERAS}/${K_MODEL}
}

echo "-----------------------------------------"
echo "TRAINING STARTED"
echo "-----------------------------------------"

rm -rf ${KERAS}
mkdir -p ${KERAS}
train 2>&1 | tee ${LOG}/${TRAIN_LOG}

echo "-----------------------------------------"
echo "TRAINING FINISHED"
echo "-----------------------------------------"
