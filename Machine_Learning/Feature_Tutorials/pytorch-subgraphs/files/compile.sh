#!/bin/bash

# Copyright 2021 Xilinx Inc.
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
# modified by Daniele Bagni


if [ $1 = zcu102 ]; then
      ARCH=/opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json
      TARGET=zcu102
      echo "-----------------------------------------"
      echo "COMPILING MODEL FOR ZCU102.."
      echo "-----------------------------------------"
elif [ $1 = zcu104 ]; then
      ARCH=/opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json
      TARGET=zcu104
      echo "-----------------------------------------"
      echo "COMPILING MODEL FOR ZCU104.."
      echo "-----------------------------------------"

elif [ $1 = vck190 ]; then
      ARCH=/opt/vitis_ai/compiler/arch/DPUCVDX8G/VCK190/arch.json
      TARGET=vck190
      echo "-----------------------------------------"
      echo "COMPILING MODEL FOR VCK190.."
      echo "-----------------------------------------"
else
      echo  "Target not found. Valid choices are: zcu104, zcu102, vck190...exiting"
      exit 1
fi

# Depth-Wise Convolution enabled
compile_dw() {
  vai_c_xir \
  --xmodel      ./build/quantized_model/CNN_int.xmodel \
  --arch        /opt/vitis_ai/compiler/arch/DPUCVDX8G/VCK190/arch_dw.json \
  --net_name    CNN_int_${TARGET}_dw \
  --output_dir  ./build/compiled_model
}

# no Depth-Wise Convolution
compile() {
  vai_c_xir \
  --xmodel      ./build/quantized_model/CNN_int.xmodel \
  --arch        $ARCH \
  --net_name    CNN_int_${TARGET} \
  --output_dir  ./build/compiled_model
}

compile    2>&1 | tee ./build/compile_${TARGET}.log

if [ $1 = vck190 ]; then
  compile_dw 2>&1 | tee ./build/compile_${TARGET}_dw.log
fi

echo "-----------------------------------------"
echo "MODEL COMPILED"
echo "-----------------------------------------"
