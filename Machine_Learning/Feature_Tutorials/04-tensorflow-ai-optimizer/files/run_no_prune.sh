#!/bin/bash
set -e

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

# This script will run all steps necessary to create the baseline (unpruned) design

source ./0_setenv_np.sh
source ./1_create_dataset.sh
source ./2_train.sh
source ./3_export_inf.sh

source ./7_freeze.sh
source ./8_eval_frozen.sh

source ./9_quant.sh
source ./10_eval_quant.sh

source ./11_compile_zcu102.sh
source ./12_make_target_zcu102.sh

source ./11_compile_u50.sh
source ./12_make_target_u50.sh

