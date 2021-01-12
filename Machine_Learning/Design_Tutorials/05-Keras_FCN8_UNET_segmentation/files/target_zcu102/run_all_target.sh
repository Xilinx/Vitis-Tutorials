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


# build  test images
tar -xvf test.tar.gz
mv ./build/dataset1/ .
rm -r build

# compile CNN application
cd code
bash -x ./build_app.sh
mv code ../run_cnn # change name of the application
cd ..

# now run semantic segmentation with 3 CNNs using VART C++ APIs with single thread
./run_cnn ./fcn8/model/fcn8.xmodel        ./dataset1/img_test/ #| tee ./rpt/logfile_cpp_fcn8.txt
./run_cnn ./fcn8ups/model/fcn8ups.xmodel  ./dataset1/img_test/ #| tee ./rpt/logfile_cpp_fcn8ups.txt
./run_cnn ./unet/v2/model/unet2.xmodel    ./dataset1/img_test/ #| tee ./rpt/logfile_cpp_unet2.txt

# now run semantic segmentation with 3 CNNs using VART C++ APIs with multithreads
bash -x ./code/run_cnn_py_fps.sh 2>&1 | tee ./rpt/logfile_py_fps.txt


#remove images
rm -r dataset1
