#!/bin/bash
echo model     : $0
echo inputfile : $1
echo width     : $2
echo height    : $3
echo num-buff  : $4



if [ "$#" -ne 5 ]; then
  echo "Illegal number of parameters"
  echo "Usage : run_model.sh <modelname> <inputfile> <width> <height> <num-buffers>"
  echo "Ex : ./run_model.sh resnet50 ~/Videos/test_model_224_224.bgr 224 224 1000"
  echo "Ex : ./run_model.sh resnet18 ~/Videos/test_model_224_224.bgr 224 224 1000"
  echo "Ex : ./run_model.sh densebox_320_320 ~/Videos/densebox_320_320.bgr 320 320 1000"
  echo "Ex : ./run_model.sh yolov3_voc_tf ~/Videos/yolov3_416_416.bgr 416 416 1000"
  echo "Ex : ./run_model.sh yolov3_voc ~/Videos/yolov3_416_416.bgr 416 416 1000"
  echo "Ex : ./run_model.sh ssd_adas_pruned_0_95 ~/Videos/traffic4_480_360.bgr 480 360 1000"
  echo "Ex : ./run_model.sh refinedet_pruned_0_96 ~/Videos/refinedet_480_360.bgr 480 360 1"
  echo "Ex : ./run_model.sh yolov3_adas_pruned_0_9 ~/Videos/yolov3_adas_pruned_0_9_512_256.bgr 512 256 1000"
  echo "Ex : ./run_model.sh resnet_v1_50_tf ~/Videos/test_model_224_224.bgr 224 224 1000"
  echo "Ex : ./run_model.sh ssd_mobilenet_v2 ~/Videos/traffic4_480_360.bgr 480 360 1000"
  echo "Ex : ./run_model.sh ssd_mobilenet_v2_coco_tf ~/Videos/traffic4_300_300.bgr 300 300 200"
  echo "Ex : ./run_model.sh yolov2_voc ~/Videos/yolov2_448_448.bgr 448 448 100"
  echo "Ex : ./run_model.sh yolov2_voc_pruned_0_77 ~/Videos/yolov2_448_448.bgr 448 448 100"
  exit -1;
fi

buffer_size=$(($3*$4*3))
echo $buffer_size

pipe="filesrc location=$2 blocksize=$buffer_size num-buffers=$5 ! videoparse width=$3 height=$4 framerate=30/1 format=16 ! ivas_xfilter name="kernel1" kernels-config="./json_files/kernel_$1.json" ! fakesink"

pipe="gst-launch-1.0 "$pipe
echo $pipe
$pipe

#
# Copyright (C) 2020 - 2021 Xilinx, Inc.  All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software
# is furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
# KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
# EVENT SHALL XILINX BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
# OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE. Except as contained in this notice, the name of the Xilinx shall
# not be used in advertising or otherwise to promote the sale, use or other
# dealings in this Software without prior written authorization from Xilinx.
#
