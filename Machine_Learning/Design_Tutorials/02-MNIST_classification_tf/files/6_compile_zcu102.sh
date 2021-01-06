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

ARCH=/opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json

compile() {
  vai_c_tensorflow \
    --frozen_pb  ${QUANT}/quantize_eval_model.pb \
    --arch       ${ARCH} \
    --output_dir ${COMPILE_ZCU102} \
    --net_name   ${NET_NAME}
}

echo "-----------------------------------------"
echo "COMPILE ZCU102 STARTED.."
echo "-----------------------------------------"

rm -rf ${COMPILE_ZCU102}
mkdir -p ${COMPILE_ZCU102}
compile 2>&1 | tee ${LOG}/${COMP_LOG_ZCU102}

echo "-----------------------------------------"
echo "COMPILE ZCU102 COMPLETED"
echo "-----------------------------------------"