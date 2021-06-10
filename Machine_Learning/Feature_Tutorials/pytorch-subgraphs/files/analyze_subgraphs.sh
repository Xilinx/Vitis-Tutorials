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

# Author: Daniele Bagni & Mark Harvey, Xilinx Inc

if [ $1 = zcu102 ]; then
      TARGET=zcu102
elif [ $1 = u50 ]; then
      TARGET=u50
elif [ $1 = vck190 ]; then
      TARGET=vck190
else
      echo  "Target not found. Valid choices are: zcu102, u50, vck190...exiting"
      exit 1
fi



# analyze DPU/CPU subgraphs
xir    graph ./build/compiled_model/CNN_int_${TARGET}.xmodel 2>&1 | tee ./build/log/cnn_int8_graph_info.txt
xir subgraph ./build/compiled_model/CNN_int_${TARGET}.xmodel 2>&1 | tee ./build/log/cnn_int8_subgraph_tree.txt
xir dump_txt ./build/compiled_model/CNN_int_${TARGET}.xmodel            ./build/log/cnn_int8_dump_xmodel.txt
xir png      ./build/compiled_model/CNN_int_${TARGET}.xmodel            ./build/log/cnn_int8_xmodel.png
xir svg      ./build/compiled_model/CNN_int_${TARGET}.xmodel            ./build/log/cnn_int8_xmodel.svg
