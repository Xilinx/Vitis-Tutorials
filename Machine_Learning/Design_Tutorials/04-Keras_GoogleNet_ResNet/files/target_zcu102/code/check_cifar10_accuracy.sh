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

# check DPU prediction top1_accuracy
python3 ./code/src/check_runtime_top5_cifar10.py -i ./rpt/logfile_cifar10_LeNet.txt
python3 ./code/src/check_runtime_top5_cifar10.py -i ./rpt/logfile_cifar10_miniVggNet.txt
python3 ./code/src/check_runtime_top5_cifar10.py -i ./rpt/logfile_cifar10_miniGoogleNet.txt
python3 ./code/src/check_runtime_top5_cifar10.py -i ./rpt/logfile_cifar10_miniResNet.txt
