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


# freeze graph
run_freeze_graph() {
  freeze_graph \
    --input_graph      ${CHKPT_DIR}/${INFER_GRAPH_FILENAME} \
    --input_checkpoint ${CHKPT_DIR}/${CHKPT_FILENAME} \
    --input_binary     true \
    --output_graph     ${FREEZE}/${FROZEN_GRAPH} \
    --output_node_names ${OUTPUT_NODE}
}



freeze() {
  echo "-----------------------------------------"
  echo "FREEZE STARTED.."
  echo "-----------------------------------------"

  rm -rf ${FREEZE}
  mkdir -p ${FREEZE}

  run_freeze_graph 2>&1 | tee ${LOG}/${FREEZE_LOG}

  echo "-----------------------------------------"
  echo "FREEZE COMPLETED"
  echo "-----------------------------------------"
}

freeze
