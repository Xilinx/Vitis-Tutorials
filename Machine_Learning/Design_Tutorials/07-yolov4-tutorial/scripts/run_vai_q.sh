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
vai_q_caffe quantize -model voc/yolov4_quant.prototxt -keep_fixed_neuron -calib_iter 100 -weights voc/yolov4.caffemodel -sigmoided_layers layer133-conv,layer144-conv,layer155-conv -output_dir yolov4_quantized/ -method 1
