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

'''
############################################################################################
#
# The MIT License (MIT)
# 
# IDC Classifier CaffeNet Trainer
# Copyright (C) 2018 Adam Milton-Barker (AdamMiltonBarker.com)
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# Title:         IDC Classifier CaffeNet Trainer
# Description:   Sorts the data for the IDC Classifier training.
# Configuration: required/confs.json
# Last Modified: 2018-08-09
#
# Example Usage:
#
#   $ python3.5 Train.py
#
############################################################################################

import os, glob, random, cv2, caffe, lmdb, json

import numpy as np

from caffe.proto import caffe_pb2
from components.caffe import CaffeHelper

class Trainer():
    
    def __init__(self):
                
        self._confs = {}
        self.trainData = []
        self.classNames = []
        self.trainSize = 0
        self.validationSize = 0
        self.testSize = 0
        self.labels = None
        self.trainLMDB = None

        self.CaffeHelper = CaffeHelper()
        
        with open('required/confs.json') as confs:
            self._confs = json.loads(confs.read())
        
        os.system('rm -rf  ' + self._confs["ClassifierSettings"]["trainLMDB"])
        os.system('rm -rf  ' + self._confs["ClassifierSettings"]["validationLMDB"])

    def sortData(self):

        self.labels = open(self._confs["ClassifierSettings"]["labels"],"w")
        self.labels.write("classes\n")

        for dirName in os.listdir(self._confs["ClassifierSettings"]["dataset_dir"]):

            path = os.path.join(self._confs["ClassifierSettings"]["dataset_dir"], dirName)

            if os.path.isdir(path) and dirName is not ".ipynb_checkpoints":

                self.classNames.append(path)
                self.labels.write(dirName+"\n")
        
        self.labels.close()

        for directory in self.classNames:

            for filename in os.listdir(directory):

                if filename.endswith('.jpg') or filename.endswith('.jpeg') or filename.endswith('.png') or filename.endswith('.gif'):

                    path = os.path.join(directory, filename)
                    self.trainData.append(path)

                else:

                    continue

    def createLMDB(self):

        print("CREATING LMDB")

        self.trainSize = int((len(self.trainData) * self._confs["ClassifierSettings"]["trainCut"]) / 100)
        self.validationSize = int((len(self.trainData) * self._confs["ClassifierSettings"]["validationCut"]) / 100)
        self.testSize = int((len(self.trainData) * self._confs["ClassifierSettings"]["testCut"]) / 100)
        
        self.trainingData = self.trainData[:self.trainSize]
        self.validationData = self.trainData[:self.validationSize]
        self.testData = self.trainData[:self.testSize]

        print("")
        print("Data Size:       "+str(len(self.trainData)))
        print("Training Size:   "+str(len(self.trainingData)))
        print("Validation Size: "+str(len(self.validationData)))
        print("Test Size:       "+str(len(self.testData)))
        print("")
        print("CREATING TRAINING LMDB ")
        print("PLEASE WAIT THIS MAY TAKE A WHILE ")
        print("")

        random.shuffle(self.trainingData)
        
        trainer = lmdb.open(
            self._confs["ClassifierSettings"]["trainLMDB"], 
            map_size=int(1e12))

        with trainer.begin(write=True) as i:

            count = 0
            for data in self.trainingData:

                label = os.path.basename(os.path.dirname(data))

                i.put(
                    '{:08}'.format(count).encode('ascii'), 
                    self.CaffeHelper.createDatum(
                        cv2.resize(
                            self.CaffeHelper.transform(
                                cv2.imread(data, cv2.IMREAD_COLOR)
                            ), 
                            (self._confs["ClassifierSettings"]["imageHeight"], self._confs["ClassifierSettings"]["imageWidth"])), 
                        label
                    ).SerializeToString())
                    
                count = count + 1

        trainer.close()

        print("DATA COUNT: "+str(count))
        print("")
        print("CREATING VALIDATION LMDB ")
        print("PLEASE WAIT THIS MAY TAKE A WHILE ")
        print("")

        random.shuffle(self.validationData)
        
        validator = lmdb.open(
            self._confs["ClassifierSettings"]["validationLMDB"], 
            map_size=int(1e12))

        with validator.begin(write=True) as i:

            count = 0
            for data in self.validationData:

                label = os.path.basename(os.path.dirname(data))

                i.put(
                    '{:08}'.format(count).encode('ascii'), 
                    self.CaffeHelper.createDatum(
                        cv2.resize(
                            self.CaffeHelper.transform(
                                cv2.imread(data, cv2.IMREAD_COLOR)
                            ), 
                            (self._confs["ClassifierSettings"]["imageHeight"], self._confs["ClassifierSettings"]["imageWidth"])), 
                        label
                    ).SerializeToString())
                    
                count = count + 1

        validator.close()

        print("DATA COUNT: "+str(count))

    def computeMean(self):

        print("COMPUTING MEAN")

        os.system('compute_image_mean -backend=lmdb  ' + self._confs["ClassifierSettings"]["trainLMDB"] + ' ' + self._confs["ClassifierSettings"]["proto"])

        
Trainer = Trainer()
Trainer.sortData()
Trainer.createLMDB()
Trainer.computeMean()
'''

'''
original code from https://github.com/BreastCancerAI/IDC-Detection-System/blob/master/Caffe/CaffeNet/Trainer.py

modified by daniele.bagni@xilinx.com

date 11 March 2020
'''

# ##################################################################################################
# USAGE
# python 2a_create_lmdb.py -i <WRK_DIR>/cats-vs-dogs/input/jpg/ -o <WRK_DIR>/cats-vs-dogs/input/lmdb

# it reads the CIFAR10 JPG images and creates 2 LMDB databases:
# train_lmdb (20000 images in LMDB) and  val_lmdb (4000 images in LMDB) to be used during the training

# ##################################################################################################

import os
import glob
import random
import warnings
warnings.filterwarnings("ignore", message="numpy.dtype size changed")
warnings.filterwarnings("ignore", message="numpy.ufunc size changed")
import numpy as np
import cv2
import sys
import matplotlib.pyplot as plt
import matplotlib.cm as cm

from config import cats_vs_dogs_config as config

import caffe
from caffe.proto import caffe_pb2
import lmdb

import argparse


# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--inp", default = config.PROJ_JPG_DIR, help="path to the input  jpeg dir")
ap.add_argument("-o", "--out", default = config.LMDB_DIR, help="path to the output lmdb dir")
args = vars(ap.parse_args())

# this is the directory where input JPG images are placed
IMG_DIR = args["inp"]   
# this is the directory where lmdb will be placed
WORK_DIR= args["out"]   

if (not os.path.exists(WORK_DIR)): # create "WORK_DIR" directory if it does not exist
    os.mkdir(WORK_DIR)


#Size of images
IMAGE_WIDTH  = 256 #227
IMAGE_HEIGHT = 256 #227

# ##################################################################################################

def createDatum(img, label):
    datum = caffe_pb2.Datum()
    datum.channels = 3
    datum.height = IMAGE_HEIGHT
    datum.width = IMAGE_WIDTH
    #rollaxis(img, 2) converts img from (32,32,3) to (3,32,32)
    datum.data = np.rollaxis(img, 2).tostring() 
    datum.label = int(label)
    return datum

# ##################################################################################################

def resize_img(img, img_width=IMAGE_WIDTH, img_height=IMAGE_HEIGHT):
    img = cv2.resize(img, (img_width, img_height), interpolation = cv2.INTER_CUBIC)
    return img

# ##################################################################################################

train_lmdb = WORK_DIR + '/train_lmdb'
valid_lmdb = WORK_DIR + '/valid_lmdb'

os.system('rm -rf  ' + train_lmdb)
os.system('rm -rf  ' + valid_lmdb)

if (not os.path.exists(train_lmdb)): # create directory if it does not exist
        os.mkdir(train_lmdb)
if (not os.path.exists(valid_lmdb)): # create directory if it does not exist
        os.mkdir(valid_lmdb)        

train_data = [img for img in glob.glob(IMG_DIR + "/train/*/*.jpg")]
valid_data = [img for img in glob.glob(IMG_DIR + "/val/*/*.jpg")]

# ##################################################################################################

print("\nCreating train_lmdb\n")

#Shuffle train_data
random.seed(48)
random.shuffle(train_data)

num_train_images = 0
train_db = lmdb.open(train_lmdb, map_size=int(1e12))
with train_db.begin(write=True) as i:
    for idx, img_path in enumerate(train_data):

        num_train_images = 1 + num_train_images
        img = cv2.imread(img_path, cv2.IMREAD_COLOR)
        filename = img_path.split("/jpg/")[1]
        if "cat" in filename:
            label = 0
        elif "dog" in filename:
            label = 1
        else:
            print("ERROR: there is a class which is not part of the dataset")
            sys.exit(0)
            
        datum = createDatum(img, label)
        string="{:0>5d}".format(idx)
        i.put(string.encode(), datum.SerializeToString())        

train_db.close()

# ##################################################################################################
# we create the validation LMDB
print("\nCreating valid_lmdb")
random.seed(48)
random.shuffle(valid_data)

num_valid_images = 0
valid_db = lmdb.open(valid_lmdb, map_size=int(1e12))
with valid_db.begin(write=True) as i:
    for idx, img_path in enumerate(valid_data):

        num_valid_images = 1 + num_valid_images
        img = cv2.imread(img_path, cv2.IMREAD_COLOR)
        filename = img_path.split("/jpg/")[1]
        if "cat" in filename:
            label = 0
        elif "dog" in filename:
            label = 1
        else:
            print("ERROR: there is a class which is not part of the dataset")
            sys.exit(0)        

        datum = createDatum(img, label)
        string="{:0>5d}".format(idx)
        i.put(string.encode(), datum.SerializeToString())                
        #'{:0>5d}'.format(idx) + ':' + img_path
valid_db.close()


print("\nFinished processing all images")
print("\n Number of images in LMDB training   dataset ",  num_train_images)
print("\n Number of images in LMDB validation dataset ",  num_valid_images)

