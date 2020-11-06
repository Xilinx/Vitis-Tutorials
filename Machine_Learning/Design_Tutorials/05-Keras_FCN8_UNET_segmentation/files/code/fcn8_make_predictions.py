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


import cv2, os
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

## Import usual libraries
import tensorflow as tf
from keras.backend.tensorflow_backend import set_session
import keras, sys, time, warnings
from keras.models import *
from keras.layers import *
from datetime import datetime #DB
from keras.utils import plot_model #DB
import gc #DB
from config import fcn_config as cfg
from config import fcn8_cnn as cnn

import pandas as pd
warnings.filterwarnings("ignore")

os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"
config = tf.ConfigProto()
config.gpu_options.per_process_gpu_memory_fraction = 0.85
#config.gpu_options.allow_growth = True
config.gpu_options.visible_device_list = "0"
set_session(tf.Session(config=config))

import argparse #DB
# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-u",  "--upscale", default=False, help="Upscale2D (T) or TransposedConv2D (F) ")
args = vars(ap.parse_args())

UPSCALE = args["upscale"]


HEIGHT = cfg.HEIGHT
WIDTH  = cfg.WIDTH
N_CLASSES = cfg.NUM_CLASSES
EPOCHS = cfg.EPOCHS

######################################################################
# directories
######################################################################

dir_data = cfg.DATASET_DIR
dir_train_img = cfg.dir_train_img
dir_train_seg = cfg.dir_train_seg
dir_test_img  = cfg.dir_test_img
dir_test_seg  = cfg.dir_test_seg
dir_calib_img = cfg.dir_calib_img
dir_calib_seg = cfg.dir_calib_seg
dir_valid_img = cfg.dir_valid_img
dir_valid_seg = cfg.dir_valid_seg

######################################################################
# model
######################################################################

#model = UNET((HEIGHT, WIDTH, 3))

model = cnn.FCN8(nClasses     = N_CLASSES,
             input_height = HEIGHT,
             input_width  = WIDTH,
                 upscale = UPSCALE)


######################################################################
# prepare testing and validation data
######################################################################

# load validation images
valid_images = os.listdir(dir_valid_img)
valid_images.sort()
valid_segmentations  = os.listdir(dir_valid_seg)
valid_segmentations.sort()
X_valid = []
Y_valid = []
for im , seg in zip(valid_images,valid_segmentations) :
    X_valid.append( cnn.NormalizeImageArr(os.path.join(dir_valid_img, im)) )
    Y_valid.append( cnn.LoadSegmentationArr( os.path.join(dir_valid_seg, seg), N_CLASSES , WIDTH, HEIGHT)  )
X_valid, Y_valid = np.array(X_valid) , np.array(Y_valid)
print("\n")
print("validation data (X) (Y) shapes:", X_valid.shape,Y_valid.shape)


# load testing images
test_images = os.listdir(dir_test_img)
test_images.sort()
test_segmentations  = os.listdir(dir_test_seg)
test_segmentations.sort()
X_test = []
Y_test = []
for im , seg in zip(test_images,test_segmentations) :
    X_test.append( cnn.NormalizeImageArr(os.path.join(dir_test_img,im)) )
    Y_test.append( cnn.LoadSegmentationArr(os.path.join(dir_test_seg, seg), N_CLASSES , WIDTH, HEIGHT)  )
X_test, Y_test = np.array(X_test) , np.array(Y_test)
print("testing    data (X) (Y) shapes", X_test.shape,Y_test.shape)
print("\n")

#########################################################################################################
# Calculate intersection over union for each segmentation class

if UPSCALE=="False" :
    model_filename= "../keras_model/fcn8/ep"    + str(EPOCHS) + "_trained_fcn8_"    + str(WIDTH) + "x" + str(HEIGHT) + ".hdf5"
else :
    model_filename= "../keras_model/fcn8ups/ep" + str(EPOCHS) + "_trained_fcn8ups_" + str(WIDTH) + "x" + str(HEIGHT) + ".hdf5"

model = load_model(model_filename) #DB

print("\nnow computing IoU over testing data set:")
y_pred1   = model.predict(X_test)
y_pred1_i = np.argmax(y_pred1, axis=3)
y_test1_i = np.argmax(Y_test, axis=3)
#print(y_test1_i.shape,y_pred1_i.shape)
cnn.IoU(y_test1_i, y_pred1_i)

print("\nnow computing IoU over validation data set:")
y_pred2 = model.predict(X_valid)
y_pred2_i = np.argmax(y_pred2, axis=3)
y_test2_i = np.argmax(Y_valid, axis=3)
#print(y_test2_i.shape,y_pred2_i.shape)
cnn.IoU(y_test2_i,y_pred2_i)


#########################################################################################################
'''
# Visualize the model performance
cnn.visualize_model_performance(X_test, y_pred1_i, y_test1_i, N_CLASSES, UPSCALE)
'''
