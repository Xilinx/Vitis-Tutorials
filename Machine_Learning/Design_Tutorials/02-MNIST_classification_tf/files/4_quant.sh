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

run_quant() {

  echo "Making list of calibration images.."

  # create image files for calibration
  python generate_images.py  \
      --dataset=mnist \
      --image_dir=${QUANT}/images \
      --image_format=jpg \
      --image_list=calib_list.txt \
      --max_images=${CALIB_IMAGES}

  # log the quantizer version being used
  vai_q_tensorflow --version
  
  # quantize
  vai_q_tensorflow quantize \
    --input_frozen_graph ${FREEZE}/${FROZEN_GRAPH} \
		--input_fn           image_input_fn.calib_input \
		--output_dir         ${QUANT} \
	  --input_nodes        ${INPUT_NODE} \
		--output_nodes       ${OUTPUT_NODE} \
		--input_shapes       ${INPUT_SHAPE} \
		--calib_iter         100 \
    --gpu                ${CUDA_VISIBLE_DEVICES}
}


quant() {

  echo "-----------------------------------------"
  echo "QUANTIZE STARTED.."
  echo "-----------------------------------------"

  rm -rf ${QUANT} 
  mkdir -p ${QUANT} 

  run_quant 2>&1 | tee ${LOG}/${QUANT_LOG}

  rm -rf ${QUANT}/images
  
  echo "-----------------------------------------"
  echo "QUANTIZED COMPLETED"
  echo "-----------------------------------------"
}

quant
