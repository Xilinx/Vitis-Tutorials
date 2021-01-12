#!/bin/bash

## © Copyright (C) 2016-2020 Xilinx, Inc
##
## Licensed under the Apache License, Version 2.0 (the "License"). You may
## not use this file except in compliance with the License. A copy of the
## License is located at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
## WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
## License for the specific language governing permissions and limitations
## under the License.

# build cifar10 test images
bash ./code/build_cifar10_test.sh

# compile CNN application
cd code
bash -x ./build_app.sh
mv code ../run_cnn # change name of the application
cd ..

# now run the cifar10 classification with 4 CNNs uing VART C++ APIs
./run_cnn ./cifar10/LeNet/model/LeNet.xmodel                 ./cifar10_test/ ./code/cifar10_labels.txt | tee ./rpt/logfile_cifar10_LeNet.txt
./run_cnn ./cifar10/miniVggNet/model/miniVggNet.xmodel       ./cifar10_test/ ./code/cifar10_labels.txt | tee ./rpt/logfile_cifar10_miniVggNet.txt
./run_cnn ./cifar10/miniGoogleNet/model/miniGoogleNet.xmodel ./cifar10_test/ ./code/cifar10_labels.txt | tee ./rpt/logfile_cifar10_miniGoogleNet.txt
./run_cnn ./cifar10/miniResNet/model/miniResNet.xmodel       ./cifar10_test/ ./code/cifar10_labels.txt | tee ./rpt/logfile_cifar10_miniResNet.txt

# check DPU prediction accuracy
bash -x ./code/check_cifar10_accuracy.sh | tee ./rpt/summary_cifar10_prediction_accuracy_on_dpu.txt

## run multithreading Python VART APIs to get fps
bash -x ./code/fps_cifar10.sh | tee ./rpt/logfile_cifar10_fps.txt


#remove images
rm -r cifar10_test
