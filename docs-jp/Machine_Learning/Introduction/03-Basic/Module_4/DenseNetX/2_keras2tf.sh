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


TF_CPP_MIN_LOG_LEVEL=3


# convert keras model to frozen graph
keras_2_tf() {
  python keras_2_tf.py \
    --keras_hdf5 ${KERAS}/${K_MODEL} \
    --tf_ckpt    ${TFCKPT_DIR}/${TFCKPT}  
}


freeze() {
  freeze_graph \
    --input_meta_graph  ${TFCKPT_DIR}/${TFCKPT}.meta \
    --input_checkpoint  ${TFCKPT_DIR}/${TFCKPT} \
    --output_graph      ${FREEZE}/${FROZEN_GRAPH} \
    --output_node_names ${OUTPUT_NODE} \
    --input_binary      true
}

echo "-----------------------------------------"
echo "CONVERTING KERAS MODEL TO TF CHECKPOINT.."
echo "-----------------------------------------"

rm -rf ${TFCKPT_DIR}
mkdir -p ${TFCKPT_DIR}
keras_2_tf 2>&1 | tee ${LOG}/${KERAS_LOG}


echo "-----------------------------------------"
echo "FINISHED KERAS MODEL CONVERSION"
echo "-----------------------------------------"


echo "-----------------------------------------"
echo "FREEZING THE GRAPH.."
echo "-----------------------------------------"

rm -rf ${FREEZE}
mkdir -p ${FREEZE}
freeze 2>&1 | tee ${LOG}/${FREEZE_LOG}

echo "-----------------------------------------"
echo "FREEZE GRAPH COMPLETED"
echo "-----------------------------------------"

