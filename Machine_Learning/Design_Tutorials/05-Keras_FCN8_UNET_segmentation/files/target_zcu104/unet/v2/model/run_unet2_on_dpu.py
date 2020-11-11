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

##################################################################
# Evaluation of UNETv2 CNN directly from DPU
#################################################################

import numpy as np
import cv2
import os
import sys

from config import dpu_fcn_config as cfg

# where are we?
def get_script_directory():
    path = os.getcwd()
    return path


#################################################################

# DPU Kernel Name
KERNEL_CONV="unet2"

# input & output node names
CONV_INPUT_NODE = "conv2d_1_convolution"
CONV_OUTPUT_NODE= "conv2d_23_convolution"

SCRIPT_DIR = get_script_directory()

IMG_TEST_DIR  = cfg.dir_test_img
SEG_TEST_DIR  = cfg.dir_test_seg

USE_DPU     = True

if USE_DPU:
    from dnndk import n2cube
    #from common import dputils


#################################################################

# Compute softmax values for each sets of scores in x.
def softmax(x) :
    e_x = np.exp(x - np.max(x))
    prediction = e_x / e_x.sum()
    return prediction

#################################################################


def dpu_IoU(Yi,y_predi):
    IoUs = []
    Nclass = int(np.max(Yi)) + 1
    for c in range(Nclass):
        TP = np.sum( (Yi == c)&(y_predi == c))
        FP = np.sum( (Yi != c)&(y_predi == c))
        FN = np.sum( (Yi == c)&(y_predi != c))
        IoU = TP/float(TP + FP + FN)
        print("class (%2d) %12.12s: #TP=%7.0f, #FP=%7.0f, #FN=%7.0f, IoU=%4.3f" % (c, cfg.CLASS_NAMES[c],TP,FP,FN,IoU))
        IoUs.append(IoU)
    mIoU = np.mean(IoUs)
    print("_________________")
    print("DPU Mean IoU: {:4.3f}".format(mIoU))
    return


def dpu_get_data(dir_img, dir_seg, n_classes, width, height ):
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
        # Normalize Images
        img = cv2.imread(img_filename, 1)
        img = img.astype(np.float32)
        img = img/cfg.NORM_FACTOR - 1.0
        X.append(img)
        # Load Segmented images 
        seg_labels = np.zeros((  height , width  , n_classes ))
        img = cv2.imread(seg_filename, 1)
        img = img[:, : , 0]
        for c in range(n_classes):
            seg_labels[: , : , c ] = (img == c ).astype(int)
        Y.append(seg_labels)
    X, Y = np.array(X) , np.array(Y)
    print("X tensor shape: ", X.shape, " Y tensor shape: ", Y.shape)
    return X, Y, I, S

#################################################################


def main():

    print("STARTING UNETv2 on DPU...")

    if USE_DPU:
        # Attach to DPU driver
        n2cube.dpuOpen()

        # Load DPU Kernel and create a task
        kernel = n2cube.dpuLoadKernel(KERNEL_CONV)
        task = n2cube.dpuCreateTask(kernel, 0)

    # load and preprocess images and load segmentation labels
    assert os.path.isdir(IMG_TEST_DIR)
    #print(IMG_TEST_DIR)
    x_test, y_test, img_file, seg_file = dpu_get_data(IMG_TEST_DIR, SEG_TEST_DIR, cfg.NUM_CLASSES, cfg.WIDTH, cfg.HEIGHT)

    y_pred = []
    # process all images
    for i in range(len(x_test)):

        # opened image as BGR, convert it to RGB
        #B,G,R  = cv2.split(x_test[i])
        #imageRun = cv2.merge((R,G,B))
        imageRun=x_test[i]
        imageRun=imageRun.reshape((imageRun.shape[0]*imageRun.shape[1]*imageRun.shape[2]))
        input_len=len(imageRun)

        if USE_DPU:
            # load pre-processed image as DPU input
            n2cube.dpuSetInputTensorInHWCFP32(task,CONV_INPUT_NODE,imageRun,input_len)
            dpu_in = n2cube.dpuGetInputTensor(task,CONV_INPUT_NODE)
            ti_scale = n2cube.dpuGetTensorScale(dpu_in)
            ti_h     = n2cube.dpuGetTensorHeight(dpu_in)
            ti_w     = n2cube.dpuGetTensorWidth(dpu_in)
            ti_sz    = n2cube.dpuGetTensorSize(dpu_in)
            ti_ch    = n2cube.dpuGetTensorChannel(dpu_in)
            if (i==0):
                print("Input  tensor=%3d ch=%3d H=%3d W=%3d Size=%6d scale=%4d" % (i, ti_ch, ti_h, ti_w, ti_sz, ti_scale))
            # run DPU task
            n2cube.dpuRunTask(task)

            # get output tensor address
            dpu_out = n2cube.dpuGetOutputTensorAddress(task, CONV_OUTPUT_NODE)

            # get number of channels in output tensor
            to_ch = n2cube.dpuGetOutputTensorChannel(task, CONV_OUTPUT_NODE)
            # get size in bytes of output tensor
            to_sz = n2cube.dpuGetOutputTensorSize(task, CONV_OUTPUT_NODE)
            # get width output tensor
            to_w  = n2cube.dpuGetOutputTensorWidth(task, CONV_OUTPUT_NODE)
            # get height output tensor
            to_h  = n2cube.dpuGetOutputTensorHeight(task, CONV_OUTPUT_NODE)
            # get output tensor scale
            to_scale  = n2cube.dpuGetOutputTensorScale(task, CONV_OUTPUT_NODE)

            softmax = np.zeros(to_sz,dtype=np.float32)

            if (i==0):
                print("Output tensor=%3d ch=%3d H=%3d W=%3d Size=%6d" % (i, to_ch, to_h, to_w, to_sz))
                print("Output tensor scaling factor", to_scale)

            softmax =  n2cube.dpuRunSoftmax(dpu_out, to_ch, to_sz//to_ch, to_scale)

            prediction = softmax.reshape((to_h,to_w,to_ch))

            y_pred.append(prediction)
            if (i==0):
                print("prediction shape: ", prediction.shape)


    # Calculate intersection over union for each segmentation class
    y_pred = np.asarray(y_pred)
    y_test = np.asarray(y_test)
    print("y_pred shape: ", y_pred.shape)
    print("y_test shape: ", y_test.shape)

    y_predi = np.argmax(y_pred, axis=3)
    y_testi = np.argmax(y_test, axis=3)
    print("shape of y_testi and y_predi ", y_testi.shape, y_predi.shape)

    dpu_IoU(y_testi,y_predi)

    # print results
    print ("Processed",len(x_test),"images")
    print ("FINISHED")

    if USE_DPU:
        # Destroy DPU Kernel & detach
        n2cube.dpuDestroyKernel(kernel)
        n2cube.dpuClose()

if __name__ == "__main__":
    main()
