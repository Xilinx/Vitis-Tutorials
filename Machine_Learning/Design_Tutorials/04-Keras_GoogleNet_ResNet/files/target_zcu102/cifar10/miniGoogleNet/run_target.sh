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



CNN=miniGoogleNet

ln -nsf ../../cifar10_test ./test

## launch the executable and collect report
./top5_${CNN} 1 2>&1 | tee ./rpt/logfile_top5_${CNN}.txt

## launch python3 script to check top-5 accuracy
python3 ./check_runtime_top5_cifar10.py -i ./rpt/logfile_top5_${CNN}.txt  2>&1 | tee ./rpt/top5_accuracy_cifar10_${CNN}.txt


## launch script to check fps
source ./run_fps_${CNN}.sh 2>&1 | tee ./rpt/fps_cifar10_${CNN}.txt

## clean up files
make clean
rm *~
rm .*~
rm test
rm fps_*
rm tf_main.cc
