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

# USAGE
# python 1_write_cats-vs-dogs_images.py
# (optional) -p /home/danieleb/ML/cats-vs-dogs/input/jpg


# by daniele.bagni@xilinx.com

# ##################################################################################################

# set the matplotlib backend before any other backend, so that figures can be saved in the background
import matplotlib
matplotlib.use("Agg")

import warnings
warnings.filterwarnings("ignore", message="numpy.dtype size changed")
warnings.filterwarnings("ignore", message="numpy.ufunc size changed")
import numpy as np

# import the necessary packages
#from sklearn.preprocessing import LabelBinarizer
#from sklearn.metrics import classification_report
from config import cats_vs_dogs_config as config
from datetime import datetime
import matplotlib.pyplot as plt
import cv2
import os
import argparse
import glob
import sys


# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-p", "--pathname", default=config.PROJ_JPG_DIR, help="path to the dataset")
args = vars(ap.parse_args())

path_root = args["pathname"] # root path name of dataset

if (not os.path.exists(path_root)): # create "path_root" directory if it does not exist
    #os.mkdir(path_root)
    print("ERROR: you need the directory with the jpg files")
    sys.exit(0)



# ##################################################################################################

#Size of images
IMAGE_WIDTH  = 256 #227
IMAGE_HEIGHT = 256 #227

# ##################################################################################################


def resize_img(img, img_width=IMAGE_WIDTH, img_height=IMAGE_HEIGHT):
    img = cv2.resize(img, (img_width, img_height), interpolation = cv2.INTER_CUBIC)
    return img


# ##################################################################################################


labelNames = ["cat", "dog", "others"]

images_path = [img for img in glob.glob(path_root + "/*/*.jpg")]


# ##################################################################################################
print("BUILD THE VALIDATION SET with 4000 images: 2000 per each class")

wrk_dir = path_root + "/val"

if (not os.path.exists(wrk_dir)): # create "val" directory if it does not exist
    os.mkdir(wrk_dir)

f_test = open(wrk_dir+"/validation.txt", "w")  #open file valid.txt"
f_lab  = open(wrk_dir+"/labels.txt", "w") #open file labels.txt"
for s in [0,1,2]:
    string = "%s\n" % labelNames[s]
    f_lab.write(string)
f_lab.close()

counter = [-1,-1, 0]

val_count = 0

for in_idx, img_path in enumerate(images_path):

    #print("DBG: now processing image ", img_path)

    img = cv2.imread(img_path, cv2.IMREAD_COLOR)
    img = resize_img(img, img_width=IMAGE_WIDTH, img_height=IMAGE_HEIGHT)
    image2 = img.astype("int")
    if "/jpg/cats/" in img_path:
        label = 0
        filename = img_path.split("/cats/")[1]
    elif "/jpg/dogs/" in img_path:
        label = 1
        filename = img_path.split("/dogs/")[1]
    else: # other
        label = 2
        filename = "others.jpg"
        print("ERROR: your path name does not contain '/jpg/' ")
        print(img_path)
        sys.exit(0)


    counter[ label ] = counter[ label ] +1;

    if (counter[ label ] <= 10499) : #skip the first 10500 images of each class and take the last 2000
        continue

    val_count = val_count + 1
    string = "%05d" % counter[ label ]

    class_name = labelNames[label]

    path_name = wrk_dir + "/" + class_name

    if (not os.path.exists(path_name)): # create directory if it does not exist
        os.mkdir(path_name) #https://github.com/BVLC/caffe/issues/3698

    path_name = wrk_dir + "/" + class_name + "/" + filename

    string = " %1d" % label
    f_test.write(path_name + string + "\n")

    cv2.imwrite(path_name, image2)

    print(path_name)



f_test.close()


# ##################################################################################################
print("BUILD THE TEST SET with 1000 images of size 227 x 277")


wrk_dir = path_root + "/test"

if (not os.path.exists(wrk_dir)): # create "test" directory if it does not exist
    os.mkdir(wrk_dir)

f_test  = open(wrk_dir+"/test.txt", "w")   #open file test.txt"
f_lab  = open(wrk_dir+"/labels.txt", "w") #open file labels.txt"
for s in [0,1,2]:
    string = "%s\n" % labelNames[s]
    f_lab.write(string)
f_lab.close()

counter = [-1, -1, 0]


test_count = -1
for in_idx, img_path in enumerate(images_path):

    img = cv2.imread(img_path, cv2.IMREAD_COLOR)
    img = resize_img(img, img_width=227, img_height=227)
    image2 = img.astype("int")
    if "/jpg/cats/" in img_path:
        label = 0
        filename = img_path.split("/cats/")[1]
    elif "/jpg/dogs/" in img_path:
        label = 1
        filename = img_path.split("/dogs/")[1]
    else: # other
        label = 2
        filename = "others.jpg"
        print("ERROR: your path name does not contain '/jpg/' ")
        sys.exit(0)


    counter[ label ] = counter[ label ] +1;

    if (counter[ label ] <= 9999) or (counter[ label ] > 10499) : #take the images from 10000 to 10500
        continue

    test_count = test_count +1
    string = " %04d" % test_count

    class_name = labelNames[label]

    path_name = wrk_dir + "/" + filename

    f_test.write(path_name + string + "\n")

    cv2.imwrite(path_name, image2)
    #cv2.imshow(labelNames[label], image2)
    #cv2.waitKey(0)

    print(path_name)

f_test.close()
print("Test       set contains ", test_count+1,  " images")


# ##################################################################################################
print("BUILD THE TRAIN IMAGES SET with 20000 images")


wrk_dir = path_root + "/train"

if (not os.path.exists(wrk_dir)): # create "train" directory if it does not exist
    os.mkdir(wrk_dir)

f_test = open(wrk_dir + "/train.txt", "w")   #open file test.txt"
f_lab  = open(wrk_dir + "/labels.txt", "w") #open file labels.txt"
for s in [0,1,2]:
    string = "%s\n" % labelNames[s]
    f_lab.write(string)
f_lab.close()

counter = [-1,-1,0]
train_count = 0

for in_idx, img_path in enumerate(images_path):

    img = cv2.imread(img_path, cv2.IMREAD_COLOR)
    img = resize_img(img, img_width=IMAGE_WIDTH, img_height=IMAGE_HEIGHT)
    image2 = img.astype("int")
    if "/jpg/cats/" in img_path:
        label = 0
        filename = img_path.split("/cats/")[1]
    elif "/jpg/dogs/" in img_path:
        label = 1
        filename = img_path.split("/dogs/")[1]
    else: # other
        print(img_path)
        label = 2
        filename = "others.jpg"
        print("ERROR: your path name does not contain '/jpg/' ")
        sys.exit(0)


    counter[ label ] = counter[ label ] +1;

    if (counter[ label ] > 9999) : #skip images after the first 10000
        continue

    train_count = train_count +1

    string = "%05d" % counter[ label ]

    class_name = labelNames[label]

    path_name = wrk_dir + "/" + class_name

    if (not os.path.exists(path_name)): # create directory if it does not exist
        os.mkdir(path_name)

    path_name = wrk_dir + "/" + class_name + "/" + filename

    string = " %1d" % label
    f_test.write(path_name + string + "\n")

    cv2.imwrite(path_name, image2)
    #cv2.imshow(labelNames[label], image2)
    #cv2.waitKey(0)

    #print(path_name)

f_test.close()

# ##################################################################################################
print("BUILD THE CALIBRATION IMAGES SET with 200 images")


wrk_dir = path_root + "/calib"

if (not os.path.exists(wrk_dir)): # create "calibration" directory if it does not exist
    os.mkdir(wrk_dir)

f_calib = open(wrk_dir + "/calibration.txt", "w")   #open file calibration.txt"
for s in [0,1,2]:
    string = "%s\n" % labelNames[s]

counter = [-1,-1,0]


calib_count = -1
for in_idx, img_path in enumerate(images_path):

    img = cv2.imread(img_path, cv2.IMREAD_COLOR)
    img = resize_img(img, img_width=IMAGE_WIDTH, img_height=IMAGE_HEIGHT)
    image2 = img.astype("int")
    if "/jpg/cats/" in img_path:
        label = 0
        filename = img_path.split("/cats/")[1]
    elif "/jpg/dogs/" in img_path:
        label = 1
        filename = img_path.split("/dogs/")[1]
    else: # other
        label = 2
        filename = "others.jpg"
        print("ERROR: your path name does not contain '/jpg/' ")
        sys.exit(0)


    counter[ label ] = counter[ label ] +1;

    if (counter[ label ] > 99) : #take only the first 100 images per each class
        continue

    calib_count = calib_count + 1
    string = "%05d" % counter[ label]

    class_name = labelNames[ label ]

    path_name = wrk_dir + "/" + class_name

    if (not os.path.exists(path_name)): # create directory if it does not exist
        os.mkdir(path_name)

    path_name = wrk_dir + "/" + class_name + "/" + filename
    string2 = " %1d" % int(calib_count)
    f_calib.write(class_name + "/" + filename + string2 + "\n")

    cv2.imwrite(path_name, image2)
    #cv2.imshow(labelNames[int(testY[int(i)])], image2)
    #cv2.waitKey(0)

    #print(path_name)

f_calib.close()

print("Train      set contains ", train_count, " images")
print("Validation set contains ", val_count,   " images")
print("Calibrationset contains ", calib_count+1, " images")
print("END\n")
