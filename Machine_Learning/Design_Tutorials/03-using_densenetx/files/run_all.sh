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


source 0_setenv.sh

# Uncomment the next line if you want to train from scratch
#source 1_train.sh

# unzip & copy pretrained checkpoint
# Comment this line if running training from scratch
unzip -o pretrained/k_model.zip -d ${KERAS}

# convert Keras model to Tensorflow frozen graph
source 2_keras2tf.sh

# Evaluate frozen graph
source 3_eval_frozen.sh

# Quantize
source 4_quant.sh

# Evaluate quantized model
source 5_eval_quant.sh

# compile for ZCU102
source 6_compile.sh zcu102

# compile for U50
source 6_compile.sh u50


source 7_make_target_zcu102.sh
source 7_make_target_vck190.sh


