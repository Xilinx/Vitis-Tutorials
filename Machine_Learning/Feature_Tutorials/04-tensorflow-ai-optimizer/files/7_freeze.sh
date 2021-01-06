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

# freeze graph
run_freeze_graph() {
  freeze_graph \
    --input_graph       ${FREEZE_IN_GRAPH} \
    --input_checkpoint  ${FREEZE_IN_CKPT} \
    --input_binary      false  \
    --output_graph      ${FREEZE_DIR}/${FROZEN_PB} \
    --output_node_names ${OUTPUT_NODES}
}

echo "-----------------------------------------"
echo "FREEZING THE GRAPH.."
echo "-----------------------------------------"

# activate the TF  conda environment
#conda activate vitis-ai-tensorflow
conda activate vitis-ai-optimizer_tensorflow

rm -rf ${FREEZE_DIR}
mkdir -p ${FREEZE_DIR}

run_freeze_graph 2>&1 | tee ${LOG}/${FREEZE_LOG}

echo "-----------------------------------------"
echo "GRAPH FROZEN"
echo "-----------------------------------------"

