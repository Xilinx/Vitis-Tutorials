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
# distributed under the License is distributed on an "AS IS" BASIS,_
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
ARCH=/opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json;

vai_c_caffe --prototxt yolov4_quantized/deploy.prototxt --caffemodel yolov4_quantized/deploy.caffemodel --arch ${ARCH} --output_dir yolov4_compiled/ --net_name dpu_yolov4_voc --options "{'mode':'normal','save_kernel':''}";
