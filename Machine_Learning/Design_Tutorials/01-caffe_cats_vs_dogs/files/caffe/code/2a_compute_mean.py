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


# ##################################################################################################
# USAGE
# python 2a_compute_mean.py
# (optional) -i <WRK_DIR>/cifar10/input/cifar10_jpg/
# ##################################################################################################


import os
import glob
import random
import warnings
warnings.filterwarnings("ignore", message="numpy.dtype size changed")
warnings.filterwarnings("ignore", message="numpy.ufunc size changed")
import numpy as np
import cv2

from config import cats_vs_dogs_config as config

import argparse


# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--inp", default = config.PROJ_JPG_DIR, help="path to the input  jpeg dir")
args = vars(ap.parse_args())

# this is the directory where input JPG images are placed
IMG_DIR = args["inp"]

#Size of images
IMAGE_WIDTH  = 256
IMAGE_HEIGHT = 256

train_data = [img for img in glob.glob(IMG_DIR + "/train/*/*.jpg")]
#valid_data = [img for img in glob.glob(IMG_DIR + "/val/*/*.jpg")]

mean_array = []

for in_idx, img_path in enumerate(train_data):
    img = cv2.imread(img_path, cv2.IMREAD_COLOR)
    mean_array.append(img)


# ##################################################################################################
# compute mean values of the training images
# expected results are
# mean value channel 0:  106.4051
# mean value channel 1:  116.038956
# mean value channel 2:  124.462036


mean_array2 = np.asarray(mean_array, dtype=np.float32)
mean_array3 = np.mean(mean_array2, axis=0)

print(" mean value channel 0: ", np.mean(mean_array3[:,:,0]))
print(" mean value channel 1: ", np.mean(mean_array3[:,:,1]))
print(" mean value channel 2: ", np.mean(mean_array3[:,:,2]))


