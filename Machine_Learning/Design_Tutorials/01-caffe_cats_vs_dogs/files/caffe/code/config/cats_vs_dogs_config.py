#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
**

* © Copyright (C) 2016-2020 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/
'''

import os

PROJ_DIR      = os.environ['ML_DIR']     # working dir
WORK_DIR      = PROJ_DIR + "/caffe"      # caffe dir

# environmental variables: $CAFFE_ROOT and $CAFFE_TOOLS_DIR
#CAFFE_ROOT     = os.environ['CAFFE_ROOT']  # where your Caffe root is placed
CAFFE_TOOLS_DIR= os.environ['CAFFE_TOOLS_DIR']


# project folders
PROJ_JPG_DIR    = PROJ_DIR + "/input/jpg" # where plain JPEG images are placed
INPUT_DIR       = PROJ_DIR + "/input"     # input image and databases main directory
LMDB_DIR        = INPUT_DIR + "/lmdb"                 # where validation and training LMDB databases are placed
VALID_DIR       = LMDB_DIR + "/valid_lmdb"  # i.e. "/home/danieleb/ML/cats-vs-dogs/input/lmdb/valid_lmdb"
TRAIN_DIR       = LMDB_DIR + "/train_lmdb"  # i.e. "/home/danieleb/ML/cats-vs-dogs/input/lmdb/train_lmdb"
TEST_DIR        = PROJ_JPG_DIR + "/test" 

#project file for mean values
#MEAN_FILE = INPUT_DIR + "/mean.binaryproto" # i.e. "/home/danieleb/ML/cats-vs-dogs/input/cifar10_mean.binaryproto"
