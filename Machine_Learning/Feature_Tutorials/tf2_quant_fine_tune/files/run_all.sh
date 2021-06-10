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

#-----------------------------------------------
# Run this script from within the Vitis-AI docker
#-----------------------------------------------


# activate the conda Python virtual environment
conda activate vitis-ai-tensorflow2


###########################################
#
# INSTALL PATCHES FOR VITIS_AI 1.3.2 AND 
# vitis-ai-tensorflow2 ONLY!
#
###########################################

# fetch patches
wget --no-clobber https://www.xilinx.com/bin/public/openDownload?filename=unilog-1.3.2-h7b12538_35.tar.bz2 -O unilog-1.3.2-h7b12538_35.tar.bz2
wget --no-clobber https://www.xilinx.com/bin/public/openDownload?filename=target_factory-1.3.2-hf484d3e_35.tar.bz2 -O target_factory-1.3.2-hf484d3e_35.tar.bz2
wget --no-clobber https://www.xilinx.com/bin/public/openDownload?filename=xir-1.3.2-py37h7b12538_47.tar.bz2 -O xir-1.3.2-py37h7b12538_47.tar.bz2
wget --no-clobber https://www.xilinx.com/bin/public/openDownload?filename=xcompiler-1.3.2-py37h7b12538_53.tar.bz2 -O xcompiler-1.3.2-py37h7b12538_53.tar.bz2
wget --no-clobber https://www.xilinx.com/bin/public/openDownload?filename=xnnc-1.3.2-py37_48.tar.bz2 -O xnnc-1.3.2-py37_48.tar.bz2


# install patches
# MUST MAINTAIN THIS INSTALL ORDER!
sudo env PATH=/opt/vitis_ai/conda/bin:$PATH CONDA_PREFIX=/opt/vitis_ai/conda/envs/vitis-ai-tensorflow2 conda install unilog-1.3.2-h7b12538_35.tar.bz2
sudo env PATH=/opt/vitis_ai/conda/bin:$PATH CONDA_PREFIX=/opt/vitis_ai/conda/envs/vitis-ai-tensorflow2 conda install target_factory-1.3.2-hf484d3e_35.tar.bz2
sudo env PATH=/opt/vitis_ai/conda/bin:$PATH CONDA_PREFIX=/opt/vitis_ai/conda/envs/vitis-ai-tensorflow2 conda install xir-1.3.2-py37h7b12538_47.tar.bz2
sudo env PATH=/opt/vitis_ai/conda/bin:$PATH CONDA_PREFIX=/opt/vitis_ai/conda/envs/vitis-ai-tensorflow2 conda install xcompiler-1.3.2-py37h7b12538_53.tar.bz2
sudo env PATH=/opt/vitis_ai/conda/bin:$PATH CONDA_PREFIX=/opt/vitis_ai/conda/envs/vitis-ai-tensorflow2 conda install xnnc-1.3.2-py37_48.tar.bz2


# list of GPUs to use
export CUDA_DEVICE_ORDER=PCI_BUS_ID
export CUDA_VISIBLE_DEVICES="0"

# make folder for log files
rm -rf __pycache__
rm -rf logs
mkdir -p logs

# set number of classes
export CLASSES=10


# make ImageNet validation set TFRecords
python -u make_val_tfrec.py -mc ${CLASSES} 2>&1 | tee logs/make_val_tfrec.log

# make ImageNet training set TFRecords
python -u make_train_tfrec.py -mc ${CLASSES} 2>&1 | tee logs/make_train_tfrec.log

# fine-tune MobileNet model
python -u train.py -mc ${CLASSES} 2>&1 | tee logs/train.log

# Quantization without fine-tuning
python -u quant.py -e 2>&1 | tee logs/quant.log

# Quantization with fine-tuning
python -u quant_ft.py -mc ${CLASSES} 2>&1 | tee logs/quant_ft.log

# compile for chosen targets
source compile.sh zcu102
source compile.sh zcu104
source compile.sh vck190

# create target folders for chosen targets
python -u make_target.py -m compiled_model_zcu102/mobilenet.xmodel -td target_zcu102 2>&1 | tee logs/target_zcu102.log
python -u make_target.py -m compiled_model_zcu104/mobilenet.xmodel -td target_zcu104 2>&1 | tee logs/target_zcu104.log
python -u make_target.py -m compiled_model_vck190/mobilenet.xmodel -td target_vck190 2>&1 | tee logs/target_vck190.log

