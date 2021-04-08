#!/bin/sh
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

TARGET=ZCU102
NET_NAME=dpu_yolov4 
ARCH=/opt/vitis_ai/compiler/arch/DPUCZDX8G/${TARGET}/arch.json

vai_c_tensorflow --frozen_pb ../yolov4_quantized/quantize_eval_model.pb \
                 --arch ${ARCH} \
		 --output_dir ../yolov4_compiled/ \
		 --net_name ${NET_NAME} \
		 --options "{'mode':'normal','save_kernel':'', 'input_shape':'1,512,512,3'}"


