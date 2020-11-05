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
tar -xvf test_images.tar
cd ./baseline


ln -nsf ../test ./test_images

source ./run_fps_alexnet.sh 2>&1 | tee ./rpt/logfile_dpu_fps_alexnet.txt

source ./run_top2_alexnet.sh 2>&1 | tee ./rpt/logfile_top2_alexnetBNnoLRN.txt
python3 ./check_dpu_runtime_accuracy.py -i ./rpt/logfile_top2_alexnetBNnoLRN.txt 2>&1 | tee ./rpt/logfile_dpu_top2_alexnet.txt
