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



# quantize
quantize() {
  
  echo "Making calibration images.."  

  python tf_gen_images.py  \
      --dataset=train \
      --image_dir=${QUANT}/images \
      --calib_list=calib_list.txt \
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
		--calib_iter         10
}

echo "-----------------------------------------"
echo "QUANTIZE STARTED.."
echo "-----------------------------------------"

rm -rf ${QUANT} 
mkdir -p ${QUANT}/images
quantize 2>&1 | tee ${LOG}/${QUANT_LOG}
rm -rf ${QUANT}/images

echo "-----------------------------------------"
echo "QUANTIZE COMPLETED"
echo "-----------------------------------------"
