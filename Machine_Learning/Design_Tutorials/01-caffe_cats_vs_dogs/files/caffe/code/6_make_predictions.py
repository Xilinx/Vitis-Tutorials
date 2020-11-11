#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
COPYRIGHT

All new contributions:
Copyright [2019] [Xilinx Inc.]
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

All other contributions:
Copyright (c) 2017, Seokju Lee
All rights reserved.

All new contributions compared to the original branch:
Copyright (c) 2017 Shifeng Zhang (CBSR, NLPR, CASIA), Longyin Wen (GE),
Xiao Bian (GE), Zhen Lei (CBSR, NLPR, CASIA), Stan Z. Li (CBSR, NLPR, CASIA).
All rights reserved.

All new contributions compared to the original branch:
Copyright (c) 2015, 2016 Wei Liu (UNC Chapel Hill), Dragomir Anguelov (Zoox),
Dumitru Erhan (Google), Christian Szegedy (Google), Scott Reed (UMich Ann Arbor),
Cheng-Yang Fu (UNC Chapel Hill), Alexander C. Berg (UNC Chapel Hill).
All rights reserved.

## original copyright

All contributions by the University of California:
Copyright (c) 2014, 2015, The Regents of the University of California (Regents)
All rights reserved.

All other contributions:
Copyright (c) 2014, 2015, the respective contributors
All rights reserved.

Caffe uses a shared copyright model: each contributor holds copyright over
their contributions to Caffe. The project versioning records all such
contribution and copyright details. If a contributor wants to further mark
their specific copyright on a particular contribution, they should indicate
their copyright solely in the commit message of the change when it is
committed.

LICENSE

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

CONTRIBUTION AGREEMENT

By contributing to the BVLC/caffe repository through pull-request, comment,
or otherwise, the contributor releases their content to the
license and copyright terms herein.
'''

'''
based on the jupyter notebook code of "Xilinx_Caffe/examples/00-classification.ipynb

modified by: daniele.bagni@xilinx.com
date         11 March 2020
'''

# ##################################################################################################
# USAGE+
# python code/6_make_predictions.py -d ./models/alexnetBNnoLRN/m1/deploy_1_alexnetBNnoLRN.prototxt -w ./models/alexnetBNnoLRN/m1/snapshot_1_alexnetBNnoLRN__iter_12703.caffemodel

# it computes the prediction accuracy for the CNN trainined on CATS cvs DOGS by using 1000 JPEG 227x227x3 images in
# the test directory (not belonging to the trainining or validation LMDB datasets)

# ##################################################################################################

import os
import glob
import sys
import cv2
import caffe
import warnings
warnings.filterwarnings("ignore", message="numpy.dtype size changed")
warnings.filterwarnings("ignore", message="numpy.ufunc size changed")
import numpy as np

from config import cats_vs_dogs_config as config

import argparse

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-d", "--description", required=True, help="description model")
ap.add_argument("-w", "--weights", required=True, help="weights caffemodel")
args = vars(ap.parse_args())

from caffe.proto import caffe_pb2

#caffe.set_mode_cpu() 
caffe.set_mode_gpu()


# ##################################################################################################
#Size of images
IMAGE_WIDTH = 227
IMAGE_HEIGHT = 227
# ##################################################################################################

# mean values of training dataset in BGR format
MEAN_VALUES= (106.4051, 116.038956, 124.462036)

mean_array=np.ones((3,IMAGE_WIDTH,IMAGE_HEIGHT), dtype=np.float32)
mean_array[0,:,:]=mean_array[0,:,:]*MEAN_VALUES[0]
mean_array[1,:,:]=mean_array[1,:,:]*MEAN_VALUES[1]
mean_array[2,:,:]=mean_array[2,:,:]*MEAN_VALUES[2]


# ##################################################################################################
#Read caffemodel architecture and trained model's weights
caffe_description = args["description"]
caffe_model       = args["weights"]

net = caffe.Net(caffe_description, caffe_model, caffe.TEST)

# ##################################################################################################
# Setup Input preprocessing
transformer = caffe.io.Transformer({"data": net.blobs["data"].data.shape}) #"data" is the name of the input blob == net.inputs[0]

transformer.set_mean("data", mean_array)
transformer.set_transpose("data", (2,0,1)) # move image channels to outmost dimension
#transformer.set_raw_scale("data", 255)    # rescslr from [0.1] to [0,255], use only with caffe.io.load_image()
#transformer.set_channel_swap("data", (2,1,0))  # swap from RGB to BGR: you do not need it if you use OpenCV

# reshape the blobs so that they match the image shape.
#net.blobs["data"].reshape(1,3,227,227)

# ##################################################################################################
# Making predictions

#Reading image paths
test_img_paths = [img_path for img_path in glob.glob(config.TEST_DIR+ "/*.jpg")]

NUMEL = len(test_img_paths)

test_ids = np.zeros(([NUMEL,1]))
preds = np.zeros(([NUMEL, 2]))
idx = 0

tot_true  = 0
tot_false = 0
top2_true = 0
top2_false= 0

for img_path in test_img_paths:

    img = cv2.imread(img_path, cv2.IMREAD_COLOR)
    print(img_path)
    #img2 = cv2.resize(img, (128, 128), interpolation = cv2.INTER_CUBIC)
    #cv2.imshow(img_path, img2)
    #cv2.waitKey(0)
    #img = caffe.io.load_image(img_path) # alternative way

    preprocessed_image=transformer.preprocess('data', img)
    net.blobs["data"].data[...] = preprocessed_image #copy the image in the memory allocated for the net
    out = net.forward() # perform classification
    #best_n = net.blobs["prob"].data[0].flatten().argsort()[-1: -6:-1]
    #print("DBG INFO: ", best_n)
    pred_probas = out["prob"] # returns the probabilities of the 10 classes

    # compute top-2: take the last 2 elements [-2:] and reverse them [::-1]
    top2 = pred_probas.argsort()[-2:][::-1]

    if "cat" in img_path:
        label = 0
    elif "dog" in img_path:
        label = 1
    else:
        label = -1 # non existing

    if label in top2 :
        top2_true = top2_true + 1
    else :
        top2_false = top2_false + 1
    #print("DBG INFO ", label, top2)

    test_ids[idx] = label
    preds[idx] = pred_probas
    #print("DBG INFO ", pred_probas)

    print("IMAGE: " + img_path)
    print("PREDICTED: %d" % preds[idx].argmax())
    print("EXPECTED : %d" % test_ids[idx])
    print("-------")

    idx = idx+1


# ##################################################################################################
# SKLEARN REPORT
'''
precision = tp / (tp+fp) = ability of the classifier to not label as positive a sample that is negative
recall    = tp / (tp+fn) = ability of the classifier to find all positive samples
F1-score  = weighter harmonic mean of precision and recall. Best value approaches 1 and worst 0
support   = number of occurrences
'''


from sklearn.preprocessing import LabelBinarizer
from sklearn.metrics import classification_report
lb     = LabelBinarizer()
testY  = lb.fit_transform(test_ids)
labelNames = ["cat", "dog"]

#report=classification_report(testY.argmax(axis=1), preds.argmax(axis=1), target_names=labelNames)
report=classification_report(testY, preds.argmax(axis=1), target_names=labelNames)
print(report)

from sklearn.metrics import accuracy_score
#print("SKLEARN Accuracy = %.2f" % accuracy_score(testY.argmax(axis=1), preds.argmax(axis=1)) )
print("SKLEARN Accuracy = %.2f" % accuracy_score(testY, preds.argmax(axis=1)) )
