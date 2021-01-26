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

# Author: Mark Harvey, Xilinx Inc

# activate the python virtual environment
conda activate vitis-ai-tensorflow2

# convert dataset to TfRecords
# this only needs to be run once
python -u images_to_tfrec.py 2>&1 | tee tfrec.log



# If you want to skip training, run this..
mkdir -p float_model
cp -f pretrained/f_model.h5 float_model/.

# ...otherwise run this:
#python -u train.py 2>&1 | tee train.log


# quantize & evaluate
python -u quantize.py --evaluate 2>&1 | tee quantize.log


# compile for selected target board
source compile.sh zcu102
#source compile.sh u50


# make target folder
python -u target.py 2>&1 | tee target.log


