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

#Pascal 2007 VOC Citation:
#@misc{pascal-voc-2007, author = "Everingham, M. and Van~Gool, L. and Williams, C. K. I. and Winn, J. and Zisserman, A.", title = "The {PASCAL} {V}isual {O}bject {C}lasses {C}hallenge 2007 {(VOC2007)} {R}esults", howpublished = "http://www.pascal-network.org/challenges/VOC/voc2007/workshop/index.html"}

cd voc;
#download voc data images
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtest_06-Nov-2007.tar;
tar -xvf VOCtest_06-Nov-2007.tar;
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtrainval_06-Nov-2007.tar;
tar -xvf VOCtrainval_06-Nov-2007.tar;
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2012/VOCtrainval_11-May-2012.tar;
tar -xvf VOCtrainval_11-May-2012.tar;
python ../scripts/voc_label.py;
rm -f *.tar;
cd ..;
ln -s voc/VOCdevkit/VOC2007/JPEGImages images;

#download ground truth file from Xilinx model Zoo
wget -O tf_yolov3_voc_416_416_1.0.zip  https://www.xilinx.com/bin/public/openDownload?filename=tf_yolov3_voc_416_416_1.0.zip;
unzip -j -d voc tf_yolov3_voc_416_416_1.0.zip tf_yolov3_voc_416_416_65.63G/test_code/voc_data/2007_test_gt.txt;
unzip -j -d voc tf_yolov3_voc_416_416_1.0.zip tf_yolov3_voc_416_416_65.63G/test_code/voc_data/2007_test.txt;
rm tf_yolov3_voc_416_416_1.0.zip;


