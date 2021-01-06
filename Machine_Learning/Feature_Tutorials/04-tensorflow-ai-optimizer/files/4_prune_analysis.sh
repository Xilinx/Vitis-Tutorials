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


# pruning analysis
# writes a file named .ana to the --workspace folder
prune_analysis() {
  vai_p_tensorflow \
    --action            ana \
    --input_graph       ${TRAIN_DIR}/${BASE_GRAPH} \
    --input_ckpt        ${TRAIN_DIR}/${BASE_CKPT}  \
    --eval_fn_path      AlexNet.py \
    --target            "accuracy" \
    --max_num_batches   25 \
    --workspace         ${PRUNE_ANA} \
    --input_nodes       ${INPUT_NODES} \
    --input_node_shapes ${INPUT_SHAPE} \
    --output_nodes      ${OUTPUT_NODES} \
    --gpu               ${CUDA_VISIBLE_DEVICES}
}

echo "-----------------------------------------"
echo "PRUNING ANALYSIS STARTED"
echo "-----------------------------------------"

TF_CPP_MIN_LOG_LEVEL=0

# activate the TF pruning conda environment
conda activate vitis-ai-optimizer_tensorflow

# clear any previous results
rm -rf ${PRUNE_ANA}
mkdir -p ${PRUNE_ANA}

prune_analysis 2>&1 | tee ${LOG}/${PRUNE_ANA_LOG}

echo "-----------------------------------------"
echo "PRUNING ANALYSIS FINISHED"
echo "-----------------------------------------"

