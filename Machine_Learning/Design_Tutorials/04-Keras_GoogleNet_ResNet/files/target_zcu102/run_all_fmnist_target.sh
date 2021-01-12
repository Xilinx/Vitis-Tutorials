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

# build fmnist test images
bash ./code/build_fmnist_test.sh

: '
## compile CNN application
#cd code
#bash -x ./build_app.sh
#mv code ../run_cnn # change name of the application
#cd ..
'

# now run the fmnist classification with 4 CNNs uing VART C++ APIs
./run_cnn ./fmnist/LeNet/model/LeNet.xmodel                 ./fmnist_test/ ./code/fmnist_labels.txt | tee ./rpt/logfile_fmnist_LeNet.txt
./run_cnn ./fmnist/miniVggNet/model/miniVggNet.xmodel       ./fmnist_test/ ./code/fmnist_labels.txt | tee ./rpt/logfile_fmnist_miniVggNet.txt
./run_cnn ./fmnist/miniGoogleNet/model/miniGoogleNet.xmodel ./fmnist_test/ ./code/fmnist_labels.txt | tee ./rpt/logfile_fmnist_miniGoogleNet.txt
./run_cnn ./fmnist/miniResNet/model/miniResNet.xmodel       ./fmnist_test/ ./code/fmnist_labels.txt | tee ./rpt/logfile_fmnist_miniResNet.txt
# check DPU prediction accuracy
bash -x ./code/check_fmnist_accuracy.sh | tee ./rpt/summary_fmnist_prediction_accuracy_on_dpu.txt

# run multithreading Python VART APIs to get fps
bash -x ./code/fps_fmnist.sh | tee ./rpt/logfile_fmnist_fps.txt

#remove images
rm -r fmnist_test
