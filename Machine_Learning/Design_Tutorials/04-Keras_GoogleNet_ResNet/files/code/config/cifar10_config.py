#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
## © Copyright (C) 2016-2020 Xilinx, Inc
##
## Licensed under the Apache License, Version 2.0 (the "License"). You may
## not use this file except in compliance with the License. A copy of the
## License is located at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
## WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
## License for the specific language governing permissions and limitations
## under the License.
'''
# Author: Daniele Bagni, Xilinx Inc
# date 6 May 2021

import os
import numpy as np

###############################################################################
# project folders
###############################################################################

def get_script_directory():
    path = os.getcwd()
    return path

# get current directory
SCRIPT_DIR = get_script_directory()

# dataset top level folder
DATASET_DIR = os.path.join(SCRIPT_DIR, "./build/dataset/cifar10")
# train, validation, test and calibration folders
TRAIN_DIR = os.path.join(DATASET_DIR, "train")
VALID_DIR = os.path.join(DATASET_DIR, "valid")
TEST_DIR  = os.path.join(DATASET_DIR, "test")
CALIB_DIR = os.path.join(SCRIPT_DIR, "../build/dataset/cifar10/calib")

# Augmented images folder
#AUG_IMG_DIR = os.path.join(SCRIPT_DIR,'aug_img/cifar10')

# Keras model folder
KERAS_MODEL_DIR = os.path.join(SCRIPT_DIR, "keras_model/cifar10")

# TF checkpoints folder
CHKPT_MODEL_DIR = os.path.join(SCRIPT_DIR, "./build/tf_chkpts/cifar10")

# TensorBoard folder
TB_LOG_DIR = os.path.join(SCRIPT_DIR, "./build/tb_logs/cifar10")

###############################################################################
# global variables
###############################################################################

# since we do not have validation data or access to the testing labels we need
# to take a number of images from the training data and use them instead
NUM_CLASSES      =    10
NUM_VAL_IMAGES   =  5000
NUM_TEST_IMAGES  =  5000
NUM_TRAIN_IMAGES = 50000

#Size of images
IMAGE_WIDTH  = 32
IMAGE_HEIGHT = 32

#normalization factor to scale image 0-255 values to 0-1 #DB
NORM_FACTOR = 255.0 # could be also 256.0

# label names for the FASHION MNIST dataset
labelNames_dict = { "airplane" : 0, "automobile" : 1, "bird" : 2, "cat" : 3, "deer" : 4, "dog" : 5,
                    "frog" : 6, "horse" : 7, "ship" : 8, "truck" : 9}
labelNames_list = ["airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "horse", "ship", "truck"]



###############################################################################
# global functions
###############################################################################

'''
import cv2

_R_MEAN = 0
_G_MEAN = 0
_B_MEAN = 0

MEANS = np.array([_B_MEAN,_G_MEAN,_R_MEAN],np.dtype(np.int32))

def mean_image_subtraction(image, means):
  B, G, R = cv2.split(image)
  B = B - means[0]
  G = G - means[1]
  R = R - means[2]
  image = cv2.merge([R, G, B])
  return image
'''

def Normalize(x_test):
    x_test  = np.asarray(x_test)
    x_test = x_test.astype(np.float32)
    x_test = x_test/NORM_FACTOR
    x_test = x_test -0.5
    out_x_test = x_test *2
    return out_x_test


def ScaleTo1(x_test):
    x_test  = np.asarray(x_test)
    x_test = x_test.astype(np.float32)
    our_x_test = x_test/NORM_FACTOR
    return out_x_test
