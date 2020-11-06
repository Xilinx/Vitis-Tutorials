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

# Author: Mark Harvey


source ./0_setenv.sh
source ./1_train.sh
source ./2_freeze.sh
source ./3_eval_frozen_graph.sh
source ./4_quant.sh
source ./5_eval_quant_graph.sh
source ./6_compile_zcu102.sh
source ./6_compile_u50.sh
source ./7_make_target_zcu102.sh
source ./7_make_target_u50.sh


echo "--------------------------------------------"
echo " TOOL FLOW COMPLETED"
echo "--------------------------------------------"
