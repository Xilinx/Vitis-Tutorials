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

# transform
generate_dense_ckpt() {
  input_checkpoint=$1
  output_checkpoint=$2
  vai_p_tensorflow \
    --action      transform \
    --input_ckpt  $input_checkpoint \
    --output_ckpt $output_checkpoint \
    --gpu         ${CUDA_VISIBLE_DEVICES}
}


echo "-----------------------------------------"
echo "GENERATING DENSE CHECKPOINT.."
echo "-----------------------------------------"

# activate the TF pruning conda environment
conda activate vitis-ai-optimizer_tensorflow

generate_dense_ckpt ${FT_DIR}/${FT_CKPT} ${TRSF_DIR}/${TRSF_CKPT} 2>&1 | tee ${LOG}/${GEN_DENSE_LOG}

echo "-----------------------------------------"
echo "DENSE CHECKPOINT GENERATION FINISHED"
echo "-----------------------------------------"

