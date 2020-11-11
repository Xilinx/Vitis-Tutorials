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
# python 3_read_lmdb.py <WRK_DIR>/input/lmdb/valid_lmdb <WRK_DIR>/input/lmdb/train_lmdb

# it reads LMDB databases just created with previous script 2b_create_lmdb.py
# just to debug it

# by daniele.bagni@xilinx.com

# ##################################################################################################

import lmdb
import warnings
warnings.filterwarnings("ignore", message="numpy.dtype size changed")
warnings.filterwarnings("ignore", message="numpy.ufunc size changed")
import numpy as np

from config import cats_vs_dogs_config as config

import sys
import cv2
import caffe
from caffe.proto import caffe_pb2

import matplotlib.pyplot as plt
import matplotlib.cm as cm

valid_lmdb = config.VALID_DIR #sys.argv[1] 
train_lmdb = config.TRAIN_DIR #sys.argv[2]


# ##################################################################################################
print("Now testing VALIDATION LMDB")
lmdb_env = lmdb.open(valid_lmdb)

lmdb_txn = lmdb_env.begin()
lmdb_cursor = lmdb_txn.cursor()
datum = caffe_pb2.Datum()

count1 = 0 
for key, value in lmdb_cursor:
    #print(key, value)
    datum.ParseFromString(value)

    label = datum.label
    data = caffe.io.datum_to_array(datum)
    count1 = count1 +1
    
    '''
    if (count1 % 1000 == 0):
        #CxHxW to HxWxC in cv2
        image = np.transpose(data, (1,2,0))
        #cv2.imshow('VALID', image)
        arr = np.array(image)
        plt.imshow(arr, cmap=cm.hsv)
        plt.show()
        cv2.waitKey(1)
    '''
    

    #print('{},{}'.format(key, label))
  
# ##################################################################################################
print("Now testing TRAINING LMDB")
lmdb_env = lmdb.open(train_lmdb)

lmdb_txn = lmdb_env.begin()
lmdb_cursor = lmdb_txn.cursor()
datum = caffe_pb2.Datum()

count3 = 0 
for key, value in lmdb_cursor:
    #print(key, value)
    datum.ParseFromString(value)

    label = datum.label
    data = caffe.io.datum_to_array(datum)
    count3 = count3 +1


    #import pdb; pdb.set_trace()

    '''
    if (count3 % 1000 == 0):
        #CxHxW to HxWxC in cv2
        image = np.transpose(data, (1,2,0))
        #cv2.imshow('TRAIN', image)
        arr = np.array(image)
        plt.imshow(arr, cmap=cm.hsv)
        plt.show()
        cv2.waitKey(1)
    '''
    #print('{},{}'.format(key, label))


# ##################################################################################################

print("number of images in the VALID database = %d" % count1)
print("number of images in the TRAIN database = %d" % count3)



