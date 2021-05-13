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

# reformat data for evaluation function
sed -i 's/images\//''/g' yolov4_quantized/caffe_result_quant.txt ;
sed -i 's/.jpg//g' yolov4_quantized/caffe_result_quant.txt ;
python  scripts/evaluation.py -mode detection -detection_use_07_metric True  -gt_file voc/2007_test_gt.txt -result_file yolov4_quantized/caffe_result_quant.txt -detection_iou 0.5 -detection_thresh 0.005; 
