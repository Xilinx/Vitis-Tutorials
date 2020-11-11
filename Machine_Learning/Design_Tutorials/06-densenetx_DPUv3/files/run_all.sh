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


source ./0_setenv.sh

# either run training from scratch or copy pretrained checkpoint

# training from scratch
#source ./1_train.sh

# unzip & copy pretrained checkpoint
unzip -o pretrained/k_model.zip -d ./build/keras_model

source ./2_keras2tf.sh
source ./3_eval_frozen.sh
source ./4_quant.sh
source ./5_eval_quant.sh
source ./6_compile.sh
source ./7_make_target.sh


# run the application - requires a U50 to be connected
# run from inside Vitis-AI docker container
source ./U50_overlay.sh
cd ./build/target
/usr/bin/python3 app_mt.py -t 4 -m model_dir/densenetx.xmodel

