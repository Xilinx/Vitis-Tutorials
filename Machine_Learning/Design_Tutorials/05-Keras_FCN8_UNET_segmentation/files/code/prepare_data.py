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

# modified by daniele.bagni@xilinx.com
# date 20 / 11 / 2020


######################################################################
## Import libraries
import cv2, os
import numpy as np
#import matplotlib
#matplotlib.use('qt5agg')
import matplotlib.pyplot as plt
from random import seed
from random import random
from random import shuffle
import sys, time, warnings
import gc
import pandas as pd
from datetime import datetime

from config import fcn_config as cfg
from config import fcn8_cnn as cnn

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
import tensorflow as tf
from tensorflow.keras.backend               import set_session
from tensorflow.keras                       import backend
from tensorflow.keras.models                import *
from tensorflow.keras.layers                import *

warnings.filterwarnings("ignore")

######################################################################

# workaround for TF1.15 bug "Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR"
os.environ['TF_FORCE_GPU_ALLOW_GROWTH'] = 'true'

os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"
config = tf.compat.v1.ConfigProto()
config.gpu_options.per_process_gpu_memory_fraction = 0.85
#config.gpu_options.allow_growth = True
config.gpu_options.visible_device_list = "0"
set_session(tf.compat.v1.Session(config=config))

print("\n")
print("python {}".format(sys.version))
print("keras version {}".format(tf.keras.__version__)); 
print("tensorflow version {}".format(tf.__version__))
print(backend.image_data_format())
print("\n")

######################################################################

HEIGHT = cfg.HEIGHT
WIDTH  = cfg.WIDTH
n_classes = cfg.NUM_CLASSES

input_height , input_width  = HEIGHT, WIDTH
output_height, output_width = HEIGHT, WIDTH


# input directories
dir_data = cfg.dir_data
dir_train_seg_inp = cfg.dir_train_seg_inp
dir_train_img_inp = cfg.dir_train_img_inp
dir_test_seg_inp  = cfg.dir_test_seg_inp
dir_test_img_inp  = cfg.dir_test_img_inp

# output directories
dir_train_img = cfg.dir_train_img
dir_train_seg = cfg.dir_train_seg
dir_test_img  = cfg.dir_test_img
dir_test_seg  = cfg.dir_test_seg
dir_calib_img = cfg.dir_calib_img
dir_calib_seg = cfg.dir_calib_seg
dir_valid_img = cfg.dir_valid_img
dir_valid_seg = cfg.dir_valid_seg

'''
cfg.visualize_legend() # plot Figure 1 of README.md

# plot an image and its segmentation labels for each of the 12 classes
cnn.plot_image_with_classes(dir_train_seg_inp, dir_train_img_inp)

#plot some examples of segmented images with their classes coded in color
cnn.plot_some_images(dir_train_seg_inp, dir_train_img_inp)
'''

######################################################################
# Resize all the images to 224x224 and store in proper folders
######################################################################

train_images = os.listdir(dir_train_img_inp)
train_images.sort()
train_segmentations  = os.listdir(dir_train_seg_inp)
train_segmentations.sort()

# using zip() to map training and segmentation images
zip_mapped = zip(train_images,train_segmentations)
list_mapped=list(set(zip_mapped))

# seed random number generator
seed(1)
# randomly shuffle the list of images
shuffle(list_mapped)

X = []
Y = []
for i in range(len(list_mapped)):
    X.append( cnn.getImageArr(        os.path.join(dir_train_img_inp, list_mapped[i][0]) , input_width , input_height )  )
    Y.append( cnn.getSegmentationArr( os.path.join(dir_train_seg_inp, list_mapped[i][1]), n_classes , output_width , output_height )  )
X, Y = np.array(X) , np.array(Y)

# split between training and validation data
from sklearn.model_selection import train_test_split
X_train, X_valid, Y_train, Y_valid = train_test_split(X, Y, test_size=0.15, random_state=0)

# store effective training and calibration images
trn_count = int(0)
cal_count = int(0)
for i in range(0, len(X_train))  :
    img = X_train[i]
    seg = Y_train[i]
    cv2.imwrite(dir_train_img+"/training_" +str(trn_count)+".png", img)
    cv2.imwrite(dir_train_seg+"/seg_trn_" +str(trn_count)+".png", seg)
    if ( (trn_count % int(3)) == int(0) ) :
        cal_count = cal_count+1
        cv2.imwrite(dir_calib_img+"/training_" +str(trn_count)+".png", img)
        cv2.imwrite(dir_calib_seg+"/seg_trn_" +str(trn_count)+".png", seg)
    trn_count = trn_count + 1
print("\n")
print("written %3d training images" % trn_count)
print("written %3d calibr.  images" % cal_count)

# store effective validation images
val_count=int(0)
for j in range(0, len(X_valid)) :
    img = X_valid[j]
    seg = Y_valid[j]
    cv2.imwrite(dir_valid_img+"/valid_" +str(val_count)+".png", img)
    cv2.imwrite(dir_valid_seg+"/seg_v_" +str(val_count)+".png", seg)
    val_count = val_count+1
print("written %3d valid.   images" % val_count)

# testing images
test_images = os.listdir(dir_test_img_inp)
test_images.sort()
test_segmentations  = os.listdir(dir_test_seg_inp)
test_segmentations.sort()

X_test = []
Y_test = []
for im, seg in zip(test_images,test_segmentations) :
    X_test.append( cnn.getImageArr(        os.path.join(dir_test_img_inp, im) , input_width , input_height )  )
    Y_test.append( cnn.getSegmentationArr( os.path.join(dir_test_seg_inp, seg), n_classes , output_width , output_height )  )
X_test, Y_test = np.array(X_test) , np.array(Y_test)

# store testing images
tst_count=int(0)
for k in range(0, len(X_test)) :
    img = X_test[k]
    seg = Y_test[k]
    cv2.imwrite(dir_test_img+"/testing_" +str(tst_count)+".png", img)
    cv2.imwrite(dir_test_seg+"/seg_tst_" +str(tst_count)+".png", seg)
    tst_count = tst_count+1
print("written %3d test     images" % tst_count)

# write file list with calibration images
file_path = os.path.join(dir_calib_img, "../calib_list.txt")
f = open(file_path, "w")
file_list = os.listdir(dir_calib_img)
file_list.sort()
for i in range(0, len(file_list)) :
    f.write(file_list[i]+"\n")
f.close()
