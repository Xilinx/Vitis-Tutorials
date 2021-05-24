#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
# Original code from https://fairyonice.github.io/Learn-about-Fully-Convolutional-Networks-for-semantic-segmentation.html
# developed under the license https://github.com/FairyOnIce/FairyOnIce.github.io/blob/master/LICENSE here reproduced:

MIT License

Copyright (c) 2020 yumi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
'''

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
# date 10 May 2021


## Import usual libraries
import cv2, os
import numpy as np
import tensorflow as tf
import sys, time, warnings

# Silence TensorFlow messages
#0 = all messages are logged (default behavior)
#1 = INFO messages are not printed
#2 = INFO and WARNING messages are not printed
#3 = INFO, WARNING, and ERROR messages are not printed
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
from tensorflow.keras.models import Model
from tensorflow.keras.layers import Conv2D,BatchNormalization,Add,Dropout,MaxPooling2D,Input
from tensorflow.keras.layers import Activation,UpSampling2D, Conv2DTranspose

import matplotlib.pyplot as plt

warnings.filterwarnings("ignore")

from config import fcn_config as cfg #DB


def NormalizeImageArr( path ):
    img1 = cv2.imread(path, 1)
    img = cv2.resize(img1, (cfg.WIDTH, cfg.HEIGHT))
    img = img.astype(np.float32)
    img = img/cfg.NORM_FACTOR - 1.0
    return img

def LoadSegmentationArr( path , nClasses,  width , height ):
    seg_labels = np.zeros((  height , width  , nClasses ))
    img1 = cv2.imread(path, 1)
    img = cv2.resize(img1, (cfg.WIDTH, cfg.HEIGHT))
    img = img[:, : , 0]
    for c in range(nClasses):
        seg_labels[: , : , c ] = (img == c ).astype(int)
    return seg_labels

def give_color_to_seg_img(seg,n_classes):
    if len(seg.shape)==3:
        seg = seg[:,:,0]
    seg_img = np.zeros( (seg.shape[0],seg.shape[1],3) ).astype('float')
    #colors = sns.color_palette("hls", n_classes) #DB
    colors = cfg.COLORS #DB
    for c in range(n_classes):
        segc = (seg == c)
        seg_img[:,:,0] += (segc*( colors[c][0]/255.0 ))
        seg_img[:,:,1] += (segc*( colors[c][1]/255.0 ))
        seg_img[:,:,2] += (segc*( colors[c][2]/255.0 ))

    return(seg_img)

def IoU(Yi,y_predi):
    ## mean Intersection over Union
    ## Mean IoU = TP/(FN + TP + FP)
    IoUs = []
    Nclass = int(np.max(Yi)) + 1
    for c in range(Nclass):
        TP = np.sum( (Yi == c)&(y_predi == c))
        FP = np.sum( (Yi != c)&(y_predi == c))
        FN = np.sum( (Yi == c)&(y_predi != c))
        IoU = TP/float(TP + FP + FN)
        #print("class {:02.0f}: #TP={:7.0f}, #FP={:7.0f}, #FN={:7.0f}, IoU={:4.3f}".format(c,TP,FP,FN,IoU))
        print("class (%2d) %12.12s: #TP=%7.0f, #FP=%7.0f, #FN=%7.0f, IoU=%4.3f" % (c, cfg.CLASS_NAMES[c],TP,FP,FN,IoU))
        IoUs.append(IoU)
    mIoU = np.mean(IoUs)
    print("_________________")
    print("Mean IoU: {:4.3f}".format(mIoU))
    return


def get_images_and_labels(dir_img, dir_seg, n_classes, width, height ):
    images = os.listdir(dir_img)
    images.sort()
    segmentations  = os.listdir(dir_seg)
    segmentations.sort()
    X = [] # list of input images
    Y = [] # list of ground truth (gt)
    I = [] # List of image Filenames
    S = [] # List of gt filenames
    for im , seg in zip(images,segmentations) :
        img_filename = os.path.join(dir_img, im)
        seg_filename = os.path.join(dir_seg, seg)
        S.append(seg_filename)
        I.append(img_filename)
        X.append( NormalizeImageArr(img_filename)  )
        Y.append( LoadSegmentationArr( seg_filename, n_classes , width , height )  )
    X, Y = np.array(X) , np.array(Y)
    print("X tensor shape: ", X.shape, " Y tensor shape: ", Y.shape)
    return X, Y, I, S

def get_images(dir_img):
    images = os.listdir(dir_img)
    images.sort()
    X = []
    for im in images :
        img_filename = os.path.join(dir_img, im)
        X.append( NormalizeImageArr(img_filename)  )
    X = np.array(X)
    return X

#########################################################################################################
# Visualize the model performance

def visualize_model_performance(X_test, y_pred1_i, y_test1_i, n_classes, UPSCALE):

    for k in range(5):

        i = 10*k
        img_is  = (X_test[i] + 1)*(255.0/2)
        seg = y_pred1_i[i]
        segtest = y_test1_i[i]

        fig = plt.figure(figsize=(10,30))
        ax = fig.add_subplot(1,3,1)
        ax.imshow(img_is/255.0)
        ax.set_title("original")

        ax = fig.add_subplot(1,3,2)
        ax.imshow(give_color_to_seg_img(seg,n_classes))
        ax.set_title("predicted class")

        ax = fig.add_subplot(1,3,3)
        ax.imshow(give_color_to_seg_img(segtest,n_classes))
        ax.set_title("true class")

        if UPSCALE=="False" :
            plt.savefig("../rpt/fcn8_model_performance_image_" + str(i) + ".png")
        else :
            plt.savefig("../rpt/fcn8ups_model_performance_image_" + str(i) + ".png")

    plt.show()



def getImageArr( path , width , height ):
    img = cv2.imread(path, 1)
    img = cv2.resize(img, ( width, height ))
    return img

def getSegmentationArr( path , nClasses ,  width , height  ):
    img = cv2.imread(path, 1)
    img = cv2.resize(img, ( width, height ))
    img = img[:, : , 0]
    return img

######################################################################
# plot an image and its 12 classes
######################################################################
import seaborn as sns

def plot_image_with_classes(dir_train_seg_inp, dir_train_img_inp):

    # seaborn has white grid by default so I will get rid of this.
    sns.set_style("whitegrid", {'axes.grid' : False})
    ldseg = np.array(os.listdir(dir_train_seg_inp))

    ## pick the first image file
    fnm = "0016E5_01860.png" #"0001TP_008040.png" #ldseg[0]
    print(fnm)

    ## read in the original image and segmentation labels
    seg2 = cv2.imread(os.path.join(dir_train_seg_inp, fnm) ) # (360, 480, 3)
    img_is = cv2.imread(os.path.join(dir_train_img_inp, fnm) )
    print("seg.shape={}, img_is.shape={}".format(seg2.shape,img_is.shape))

    ## Check the number of labels
    mi, ma = np.min(seg2), np.max(seg2)
    n_classes = ma - mi + 1
    print("minimum seg = {}, maximum seg = {}, Total number of segmentation classes = {}".format(mi,ma, n_classes))

    # plot the image
    fig = plt.figure(figsize=(5,5))
    ax = fig.add_subplot(1,1,1)
    ax.imshow(img_is)
    ax.set_title("original image")
    plt.savefig("../rpt/original_image.png")
    plt.show()

    # plot the classes
    fig = plt.figure(figsize=(15,10))

    for kk in range(mi,ma+1):
        k  = kk #new_classes[kk]
        ax = fig.add_subplot(3,n_classes/3,k+1)
        seg_image = (seg2 == k)*cfg.COLORS[k]/255.0
        ax.imshow(seg_image)
        #ax.set_title("label = {}".format(k))
        ax.set_title("label = {}".format(cfg.CLASS_NAMES[k]))

    plt.savefig("../rpt/segmentation_classes.png")
    plt.show()

######################################################################
# Plot some images with classes coded in colors
######################################################################


def plot_some_images(dir_train_seg_inp, dir_train_img_inp):

    ldseg = np.array(os.listdir(dir_train_seg_inp))
    input_height , input_width  = cfg.HEIGHT, cfg.WIDTH
    output_height, output_width = cfg.HEIGHT, cfg.WIDTH

    for fnm in ldseg[np.random.choice(len(ldseg),30,replace=False)]:
        fnm = fnm.split(".")[0]
        seg_filename = os.path.join(dir_train_seg_inp, fnm) + ".png"
        seg3 = cv2.imread(seg_filename) # (360, 480, 3)
        img_filename = os.path.join(dir_train_img_inp, fnm) + ".png"
        img_is = cv2.imread(img_filename)
        seg_img = give_color_to_seg_img(seg3,cfg.NUM_CLASSES)
        print(img_filename, seg_filename)

        fig = plt.figure(figsize=(20,40))
        ax = fig.add_subplot(1,4,1)
        ax.imshow(seg_img)

        ax = fig.add_subplot(1,4,2)
        ax.imshow(img_is/255.0)
        ax.set_title("original image {}".format(img_is.shape[:2]))
        ax = fig.add_subplot(1,4,3)
        ax.imshow(cv2.resize(seg_img,(input_width, input_height)))
        ax = fig.add_subplot(1,4,4)
        tmp_img = cv2.resize(img_is,(output_width, output_height))
        ax.imshow(tmp_img/255.0)
        ax.set_title("resized to {}".format((output_width, output_height)))

    plt.savefig("../rpt/segmentation_example_" + fnm + ".png")
    plt.show()


########################################################################################
# FCN8 CNN based on VGG
########################################################################################
VGG_Weights_path = "../keras_model/vgg16_weights_tf_dim_ordering_tf_kernels_notop.h5"

def FCN8( nClasses ,  input_height, input_width, upscale="False"):
    ## input_height and width must be devisible by 32 because maxpooling with filter size = (2,2) is operated 5 times,
    ## which makes the input_height and width 2^5 = 32 times smaller
    assert input_height%32 == 0
    assert input_width%32 == 0
    IMAGE_ORDERING =  "channels_last"

    img_input = Input(shape=(input_height,input_width, 3)) ## Assume 224,224,3

    ## Block 1: 64, 64
    x = Conv2D(64, (3, 3), activation='relu', padding='same', name='block1_conv1', data_format=IMAGE_ORDERING )(img_input)
    x = Conv2D(64, (3, 3), activation='relu', padding='same', name='block1_conv2', data_format=IMAGE_ORDERING )(x)
    x = MaxPooling2D((2, 2), strides=(2, 2), name='block1_pool', data_format=IMAGE_ORDERING )(x)
    f1 = x

    # Block 2: 128, 128
    x = Conv2D(128, (3, 3), activation='relu', padding='same', name='block2_conv1', data_format=IMAGE_ORDERING )(x)
    x = Conv2D(128, (3, 3), activation='relu', padding='same', name='block2_conv2', data_format=IMAGE_ORDERING )(x)
    x = MaxPooling2D((2, 2), strides=(2, 2), name='block2_pool', data_format=IMAGE_ORDERING )(x)
    f2 = x
# Author: daniele.bagni@xilinx.com
# Date:   11 Feb 2020
    # Block 3: 256, 256, 256
    x = Conv2D(256, (3, 3), activation='relu', padding='same', name='block3_conv1', data_format=IMAGE_ORDERING )(x)
    x = Conv2D(256, (3, 3), activation='relu', padding='same', name='block3_conv2', data_format=IMAGE_ORDERING )(x)
    x = Conv2D(256, (3, 3), activation='relu', padding='same', name='block3_conv3', data_format=IMAGE_ORDERING )(x)
    x = MaxPooling2D((2, 2), strides=(2, 2), name='block3_pool', data_format=IMAGE_ORDERING )(x)
    pool3 = x

    # Block 4: 512, 512, 512
    x = Conv2D(512, (3, 3), activation='relu', padding='same', name='block4_conv1', data_format=IMAGE_ORDERING )(x)
    x = Conv2D(512, (3, 3), activation='relu', padding='same', name='block4_conv2', data_format=IMAGE_ORDERING )(x)
    x = Conv2D(512, (3, 3), activation='relu', padding='same', name='block4_conv3', data_format=IMAGE_ORDERING )(x)
    pool4 = MaxPooling2D((2, 2), strides=(2, 2), name='block4_pool', data_format=IMAGE_ORDERING )(x)## (None, 14, 14, 512)

    # Block 5: 512, 512, 512
    x = Conv2D(512, (3, 3), activation='relu', padding='same', name='block5_conv1', data_format=IMAGE_ORDERING )(pool4)
    x = Conv2D(512, (3, 3), activation='relu', padding='same', name='block5_conv2', data_format=IMAGE_ORDERING )(x)
    x = Conv2D(512, (3, 3), activation='relu', padding='same', name='block5_conv3', data_format=IMAGE_ORDERING )(x)
    pool5 = MaxPooling2D((2, 2), strides=(2, 2), name='block5_pool', data_format=IMAGE_ORDERING )(x)## (None, 7, 7, 512)

    vgg  = Model(  img_input , pool5  )
    vgg.load_weights(VGG_Weights_path) ## loading VGG weights for the encoder parts of FCN8

    ## 4 times upsamping for pool4 layer
    if upscale=="True" :
        conv7_4a=( Conv2D( nClasses , ( 7 , 7 ) ,  activation='relu', padding='same', name="conv7_4a", data_format=IMAGE_ORDERING))(pool5)
        conv7_4b=( Conv2D( nClasses , ( 1 , 1 ) ,  activation='relu', padding='same', name="conv7_4b", data_format=IMAGE_ORDERING))(conv7_4a)
        #conv7_4 = UpSampling2D(size=(4,4), data_format=IMAGE_ORDERING, interpolation="bilinear")(conv7_4b)
        conv7_4 = UpSampling2D(size=(4,4), data_format=IMAGE_ORDERING, interpolation="nearest")(conv7_4b)
    else :
        n = 512 #4096
        o =     ( Conv2D( n , ( 7 , 7 ) , activation='relu' , padding='same', name="conv6", data_format=IMAGE_ORDERING))(pool5)
        conv7 = ( Conv2D( n , ( 1 , 1 ) , activation='relu' , padding='same', name="conv7", data_format=IMAGE_ORDERING))(o)
        conv7_4 = Conv2DTranspose( nClasses , kernel_size=(4,4) ,  strides=(4,4) , use_bias=False, data_format=IMAGE_ORDERING )(conv7)


    ## 2 times upsampling for pool411
    pool411 = ( Conv2D( nClasses , ( 1 , 1 ) , activation='relu' , padding='same', name="pool4_11", data_format=IMAGE_ORDERING))(pool4)
    if upscale == "True" :
        pool411_b = ( Conv2D( nClasses , ( 1 , 1 ) ,  activation='relu', padding='same', name="pool411_b", data_format=IMAGE_ORDERING))(pool411)
        #pool411_2 = (UpSampling2D(size=(2,2), data_format=IMAGE_ORDERING, interpolation="bilinear"))(pool411_b)
        pool411_2 = (UpSampling2D(size=(2,2), data_format=IMAGE_ORDERING, interpolation="nearest"))(pool411_b)

    else:
        pool411_2 = (Conv2DTranspose( nClasses , kernel_size=(2,2) ,  strides=(2,2) , use_bias=False, data_format=IMAGE_ORDERING ))(pool411)


    pool311 = ( Conv2D( nClasses , ( 1 , 1 ) , activation='relu' , padding='same', name="pool3_11", data_format=IMAGE_ORDERING))(pool3)

    add_layer = Add(name="add_layer")([pool411_2, pool311, conv7_4 ])
    o = add_layer

    # you cannot replace this last Conv2DTranspose layer with an UpSampling2D layer: the CNN will not converge!
    o = Conv2DTranspose( nClasses , kernel_size=(8,8) ,  strides=(8,8) , use_bias=False, data_format=IMAGE_ORDERING )(o)

    o = (Activation("softmax"))(o)

    model = Model(img_input, o)

    return model
