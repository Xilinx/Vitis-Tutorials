#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
/**

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

# Author: daniele.bagni@xilinx.com
# Date:   11 Feb 2020


import os
import numpy as np
import cv2
import sys

###############################################################################
# project folders
###############################################################################

def get_script_directory():
    path = os.getcwd()
    return path

# get current directory
SCRIPT_DIR = get_script_directory()
print("fcn_config.py runs from ", SCRIPT_DIR)

# dataset top level folder
DATASET_DIR = os.path.join(SCRIPT_DIR, "../build/dataset1")
# train, validation, test and calibration folders
SEG_TRAIN_DIR = os.path.join(DATASET_DIR, "annotations_prepped_train")
IMG_TRAIN_DIR = os.path.join(DATASET_DIR, "images_prepped_train")
SEG_TEST_DIR  = os.path.join(DATASET_DIR, "annotations_prepped_test")
IMG_TEST_DIR  = os.path.join(DATASET_DIR, "images_prepped_test")

# input directories
dir_data = DATASET_DIR
dir_train_seg_inp = SEG_TRAIN_DIR
dir_train_img_inp = IMG_TRAIN_DIR
dir_test_seg_inp  = SEG_TEST_DIR
dir_test_img_inp  = IMG_TEST_DIR

# output directories
dir_train_img = os.path.join(DATASET_DIR, "img_train")
dir_train_seg = os.path.join(DATASET_DIR, "seg_train")
dir_test_img  = os.path.join(DATASET_DIR, "img_test")
dir_test_seg  = os.path.join(DATASET_DIR, "seg_test")
dir_calib_img = os.path.join(DATASET_DIR, "img_calib")
dir_calib_seg = os.path.join(DATASET_DIR, "seg_calib")
dir_valid_img = os.path.join(DATASET_DIR, "img_valid")
dir_valid_seg = os.path.join(DATASET_DIR, "seg_valid")


# Augmented images folder
#AUG_IMG_DIR = os.path.join(SCRIPT_DIR,'aug_img/cifar10')

# Keras model folder
KERAS_MODEL_DIR = os.path.join(SCRIPT_DIR, "../keras_model")

# TF checkpoints folder
CHKPT_MODEL_DIR = os.path.join(SCRIPT_DIR, "../build/tf_chkpts")

# TensorBoard folder
TB_LOG_DIR = os.path.join(SCRIPT_DIR, "../build/tb_logs")


###############################################################################
# global variables
###############################################################################

#Size of images
WIDTH  = 224 #1088 #448 #224
HEIGHT = 224 #1920 #448 #224

#normalization factor
NORM_FACTOR = 127.5

#number of classes
NUM_CLASSES = 12

# names of classes
CLASS_NAMES = ("Sky",
               "Wall",
               "Pole",
               "Road",
               "Sidewalk",
               "Vegetation",
               "Sign",
               "Fence",
               "vehicle",
               "Pedestrian",
               "Bicyclist",
               "miscellanea")

BATCH_SIZE = 32
EPOCHS = 200


#######################################################################################################

# colors for segmented classes
colorB = [128, 232, 70, 156, 153, 153,  30,   0,  35, 152, 180,  60,   0, 142, 70, 100, 100, 230,  32]
colorG = [ 64,  35, 70, 102, 153, 153, 170, 220, 142, 251, 130,  20,   0,   0,  0,  60,  80,   0,  11]
colorR = [128, 244, 70, 102, 190, 153, 250, 220, 107, 152,  70, 220, 255,   0,  0,   0,   0,   0, 119]
CLASS_COLOR = list()
for i in range(0, 19):
    CLASS_COLOR.append([colorR[i], colorG[i], colorB[i]])
COLORS = np.array(CLASS_COLOR, dtype="float32")

#######################################################################################################
def visualize_legend():
    # initialize the legend visualization
    legend = np.zeros( ((NUM_CLASSES * 25) + 25, 300, 3), dtype="uint8")
    # loop over the class names + colors
    for (i, (className, color)) in enumerate(zip(CLASS_NAMES, COLORS)):
        # draw the class name + color on the legend
        color = [int(c) for c in color]
        cv2.putText(legend, className, (5, (i * 25) + 17), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
        cv2.rectangle(legend, (100, (i * 25)), (300, (i * 25) + 25), tuple(color), -1)
        B,G,R = cv2.split(legend)
        legend_rgb = cv2.merge((R,G,B))

    cv2.imshow("BGR Legend", legend)
    cv2.imshow("RGB Legend", legend_rgb)
    cv2.imwrite("legend_rgb.png", legend_rgb)
    cv2.imwrite("legend_bgrb.png", legend)
    cv2.waitKey(0)

#######################################################################################################
