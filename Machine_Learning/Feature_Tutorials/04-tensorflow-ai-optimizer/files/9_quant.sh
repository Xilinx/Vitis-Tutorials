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

# quantize
quantize() {

  # log the quantizer version being used
  vai_q_tensorflow --version

  vai_q_tensorflow quantize \
    --input_frozen_graph ${FREEZE_DIR}/${FROZEN_PB} \
		--input_fn           image_input_fn.calib_input \
		--output_dir         ${QUANT_DIR} \
	  --input_nodes        ${INPUT_NODES} \
		--output_nodes       ${OUTPUT_NODES} \
		--input_shapes       ${INPUT_SHAPE_Q} \
		--calib_iter         100 \
    --gpu                ${CUDA_VISIBLE_DEVICES}
}


echo "-----------------------------------------"
echo "QUANTIZATION STARTED.."
echo "-----------------------------------------"

conda activate vitis-ai-tensorflow

rm -rf ${QUANT_DIR}
mkdir -p ${QUANT_DIR}

quantize 2>&1 | tee ${LOG}/${QUANT_LOG}

echo "-----------------------------------------"
echo "QUANTIZATION FINISHED"
echo "-----------------------------------------"

