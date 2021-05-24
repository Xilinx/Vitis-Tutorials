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



##############################################################################################

from config import cifar10_config as cfg
import glob
import numpy as np

import os
import sys
import shutil
import cv2

from random import seed
from random import random
from random import shuffle #DB
from keras.datasets import cifar10
import gc   #Garbage collector for cleaning deleted data from memory

SCRIPT_DIR = cfg.SCRIPT_DIR
print("This script is located in: ", SCRIPT_DIR)


##############################################################################################
# make the required folders
##############################################################################################
# dataset top level
DATASET_DIR = cfg.DATASET_DIR

# train, validation and test folders
TRAIN_DIR = cfg.TRAIN_DIR
VALID_DIR = cfg.VALID_DIR
TEST_DIR  = cfg.TEST_DIR
CALIB_DIR = os.path.join(SCRIPT_DIR, "./build/dataset/cifar10/calib")

# remove any previous data
dir_list = [DATASET_DIR, TRAIN_DIR, VALID_DIR, TEST_DIR, CALIB_DIR]
for dir in dir_list:
    if (os.path.exists(dir)):
        shutil.rmtree(dir)
    os.makedirs(dir)
    print("Directory" , dir ,  "created ")

dir_list = [TRAIN_DIR, VALID_DIR, TEST_DIR, CALIB_DIR]
for dir in dir_list:
    # create sub-directories
    labeldirs = cfg.labelNames_list
    for labldir in labeldirs:
        newdir = dir + "/" + labldir
        os.makedirs(newdir, exist_ok=True)
        print("subDirectory" , newdir ,  "created ")

IMAGE_LIST_FILE = 'calib_list.txt'

# create file for list of calibration images
f = open(os.path.join(CALIB_DIR, IMAGE_LIST_FILE), 'w')
imgList = list()

#############################################################################################
# Download CIFAR10 Dataset
############################################################################################
# Each image is 32x32x3ch with 8bits x 1ch

cifar10_dataset = cifar10.load_data()

(x_train, y_train), (x_test, y_test) = cifar10_dataset
print("{n} images in original train dataset".format(n=x_train.shape[0]))
print("{n} images in original test  dataset".format(n=x_test.shape[0]))


########################################################################################
# convert in RGB channels (remember that OpenCV works in BGR)
# and fill the "train", "cal" and "test" folders without any classes imbalance
########################################################################################
counter1 = np.array([0,0,0,0,0,0,0,0,0,0], dtype="uint32")

num_train = 0
for i in range (0, x_train.shape[0]):
    class_name = cfg.labelNames_list[int(y_train[i])]
    # store images in TRAIN_DIR
    filename = os.path.join(TRAIN_DIR, class_name+"/"+class_name+"_"+str(i)+ ".png")
    rgb_image = x_train[i].astype("uint8")
    R,G,B = cv2.split(rgb_image)
    bgr_image = cv2.merge([B,G,R])
    #bgr_image=rgb_image
    cv2.imwrite(filename, bgr_image)
    if (i < 1000): #copy first 1000 images into CALIB_DIR too
        filename2= os.path.join(CALIB_DIR, class_name+"/"+class_name+"_"+str(i)+".png")
        local_filename = class_name+"/"+class_name+"_"+str(i)+".png"
        cv2.imwrite(filename2, bgr_image)
        imgList.append(local_filename)
    counter1[ int(y_train[int(i)]) ] = counter1[ int(y_train[int(i)]) ] +1
    num_train = num_train+1


for i in range(0, len(imgList)):
    f.write(imgList[i]+"\n")
f.close()


for i in range (0, x_test.shape[0]):
    class_name = cfg.labelNames_list[int(y_test[i])]
    # store images in TEST_DIR
    filename3=os.path.join(TEST_DIR, class_name+"/"+class_name+'_'+str(i)+'.png')
    rgb_image = x_test[i].astype("uint8")
    R,G,B = cv2.split(rgb_image)
    bgr_image = cv2.merge([B,G,R])
    #bgr_image = rgb_image
    cv2.imwrite(filename3, bgr_image)
    counter1[ int(y_test[int(i)]) ] = counter1[ int(y_test[int(i)]) ] +1

print("classes histogram in train and test dataset: ", counter1)   #DeBuG

#collect garbage to save memory
del rgb_image
gc.collect()

##############################################################################################
# split the test images into validation and test folders
##############################################################################################
# make a list of all files currently in the test folder
imagesList = [img for img in glob.glob(TEST_DIR + "/*/*.png")]

# seed random number generator
seed(1)
# randomly shuffle the list of images
shuffle(imagesList)

NVAL   = cfg.NUM_VAL_IMAGES/cfg.NUM_CLASSES
NTEST  = cfg.NUM_TEST_IMAGES/cfg.NUM_CLASSES

counter = np.array([0,0,0,0,0,0,0,0,0,0], dtype="uint32")
num_val  = 0
num_test = 0

# move the images to their class folders inside train (50000), valid (5000), test (5000)
# we want to be sure that all the folders contain same number of images per each class
for img in imagesList:
    filename = os.path.basename(img)
    classname = filename.split("_")[0]

    # read image with OpenCV
    img_orig = cv2.imread(img)
    label = cfg.labelNames_dict[classname]

    if (counter[ label ] < NTEST): #test images
        dst_dir  = TEST_DIR
        num_test = num_test+1
    else: #if (counter[ label ] >= NTEST and counter[ label ] < (NTEST+NVAL)): # validation images
        dst_dir = VALID_DIR
        num_val = num_val+1

    counter[ label ] = counter[ label ] +1;

    out_filename = os.path.join(dst_dir, cfg.labelNames_list[label]+"/"+filename)
    os.rename(img, out_filename)

print("classes histogram in train and test dataset: ", counter)
print("num images in train folder = ", num_train)
print("num images in val folder   = ", num_val)
print("num images in pred folder  = ", num_test)
print ("\nFINISHED CREATING DATASET\n")
