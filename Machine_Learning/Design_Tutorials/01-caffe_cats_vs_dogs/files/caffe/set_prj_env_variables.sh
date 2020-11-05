#!/bin/bash

##
##* © Copyright (C) 2016-2020 Xilinx, Inc
##*
##* Licensed under the Apache License, Version 2.0 (the "License"). You may
##* not use this file except in compliance with the License. A copy of the
##* License is located at
##*
##*     http://www.apache.org/licenses/LICENSE-2.0
##*
##* Unless required by applicable law or agreed to in writing, software
##* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
##* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
##* License for the specific language governing permissions and limitations
##* under the License.
##*/

# *******************************************************************************
ML_DIR=/workspace/tutorials/VAI-Caffe-ML-CATSvsDOGS/files
export ML_DIR
echo "project ML_DIR is" $ML_DIR

# here is the Caffe tool directory
CAFFE_TOOLS_DIR=/workspace/tutorials/caffe-xilinx
export CAFFE_TOOLS_DIR
echo "CAFFE_TOOLS_DIR is" $CAFFE_TOOLS_DIR

#working dir
WORK_DIR=$ML_DIR/caffe
export WORK_DIR

: '
# *******************************************************************************
# VGG variables

VGG_MOD_NUM=3 # model number
export VGG_MOD_NUM
VGG_NUMIT=60000 # number of iterations
export VGG_NUMIT
VGG_NET=miniVggNet
export VGG_NET

# *******************************************************************************
# GOOGLENET variables

GLG_MOD_NUM=3 # model number
export GLG_MOD_NUM
GLG_NUMIT=60000 # number of iterations
export GLG_NUMIT
GLG_NET=miniGoogleNet
export GLG_NET


# *******************************************************************************
# LENET variables

LE_MOD_NUM=6 # model number
export LE_MOD_NUM
LE_NUMIT=20000 # number of iterations
export LE_NUMIT
LE_NET=LeNet
export LE_NET

'

# *******************************************************************************
# ALEXNET variables

ALX_MOD_NUM=2 # model number
export ALX_MOD_NUM
ALX_NUMIT=20000   #20000 # number of iterations
export ALX_NUMIT
ALX_NET=alexnetBNnoLRN
export ALX_NET
