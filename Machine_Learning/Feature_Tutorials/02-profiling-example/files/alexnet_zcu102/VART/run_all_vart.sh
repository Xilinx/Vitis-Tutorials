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

cd VART

# setup softlinks
ln -nsf ../test_images ./test_images

# running alexnet for image classification of dogs and cats
echo "running alexnet for image classification of dogs and cats"
./alexnet ../baseline/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf


# running Python multithreading
echo "running Python multithreading"

cd src
# baseline
python3 alexnet.py 1 ../../baseline/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
python3 alexnet.py 2 ../../baseline/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
python3 alexnet.py 3 ../../baseline/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
python3 alexnet.py 4 ../../baseline/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
python3 alexnet.py 5 ../../baseline/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
python3 alexnet.py 6 ../../baseline/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
# pruned
python3 alexnet.py 1 ../../pruned/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
python3 alexnet.py 2 ../../pruned/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
python3 alexnet.py 3 ../../pruned/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
python3 alexnet.py 4 ../../pruned/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
python3 alexnet.py 5 ../../pruned/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
python3 alexnet.py 6 ../../pruned/model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
