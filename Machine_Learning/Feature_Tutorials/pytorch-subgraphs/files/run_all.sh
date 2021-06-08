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

# Author: Daniele Bagni, Xilinx Inc
# date 7 May 2021



# -------------------------------------------------------------------------------
# make sure text file scripts are in "unix" format


for file in $(find $PWD -name "*.sh"); do
    sed -i 's/\r//g' ${file}
    echo  ${file}
done
for file in $(find $PWD -name "*.py"); do
    sed -i 's/\r//g' ${file}
    echo  ${file}
done


# -------------------------------------------------------------------------------

# conda activate vitis-ai-pytorch

# clean folders
#bash ./clean_all.sh


# -------------------------------------------------------------------------------
# run training
echo " "
echo "-----------------------------------------"
echo "RUN TRAINING."
echo "-----------------------------------------"
python -u train.py --epochs 300  2>&1 | tee ./build/train.log
'
: '
# -------------------------------------------------------------------------------
# quantize & export quantized model
echo " "
echo "-----------------------------------------"
echo "RUN QUANTIZATION."
echo "-----------------------------------------"
python -u quantize.py --quant_mode calib 2>&1 | tee ./build/quant_calib.log
python -u quantize.py --quant_mode test  2>&1 | tee ./build/quant_test.log


# -------------------------------------------------------------------------------
# compile for target boards
source compile.sh vck190
source compile.sh zcu102
source compile.sh zcu104

# -------------------------------------------------------------------------------
# move log files
mv ./build/*.log ./build/log/

: '
# add Xilinx headers
cd ./build/log/
for file in $(find . -name "*.log"); do
  echo ${file}
  cat ../../header.txt ${file} > tmp.txt
  mv tmp.txt ${file}
done
cd ../..
cat ./header.txt ./doc/image/cnn_int8_subgraph_tree.txt > tmp.txt
mv ./tmp.txt ./doc/image/cnn_int8_subgraph_tree.txt
'

# -------------------------------------------------------------------------------
# make target and run functional debug on host
bash ./prepare_target.sh


# -------------------------------------------------------------------------------
# analyze DPU/CPU subgraphs
echo " "
echo "-----------------------------------------"
echo "ANALYZE SUBGRAPHS."
echo "-----------------------------------------"
bash ./analyze_subgraphs.sh zcu102 &> /dev/null
