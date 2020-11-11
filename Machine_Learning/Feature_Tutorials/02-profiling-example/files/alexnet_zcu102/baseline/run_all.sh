#!/bin/bash

##
##* © Copyright (C) 2016-2020 Xilinx, Inc
##*
##* Licensed under the Apache License, Version 2.0 (the "License"). You may
##* not use this file except in compliance with the License. A copy of the
##* License is located at
##*
##*     http://www.apache.org/licenses/LICENSE-2.0
##*
##* Unless required by applicable law or agreed to in writing, software
##* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
##* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
##* License for the specific language governing permissions and limitations
##* under the License.
##*/

# *******************************************************************************

cd ../
tar -xvf test_images.tar.gz
cd ./baseline

ln -nsf ../test_images ./test_images
ln -nsf ../test_image  ./test_image

# run image classification for prediction accuracy
source ./run_top2_alexnet.sh


# run profiling
source ./run_fps_alexnet.sh
