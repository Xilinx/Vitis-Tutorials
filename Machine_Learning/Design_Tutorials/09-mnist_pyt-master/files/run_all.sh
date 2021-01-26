#!/bin/bash

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

conda activate vitis-ai-pytorch

# run training
python -u train.py 2>&1 | tee train.log


# quantize & export quantized model
python -u quantize.py --quant_mode calib 2>&1 | tee quant_calib.log
python -u quantize.py --quant_mode test  2>&1 | tee quant_test.log


# compile for target boards
source compile.sh zcu102
source compile.sh u50
source compile.sh vck190

# make target folders
python -u target.py --target zcu102
python -u target.py --target vck190
python -u target.py --target u50
