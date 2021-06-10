#!/usr/bin/env python
# coding: utf-8

"""
Copyright 2021 Xilinx Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

"""
Author:    Daniele Bagni & Jon Cory
date:      4 May 2021
"""

from ctypes import *
from typing import List
import cv2
import numpy as np
import xir
import vart
import os
import math
#import threading
import time
import sys
import queue
#from hashlib import md5
import argparse


DEBUG = False #True
PRINT_IMAGES = False #True

BUF_SIZE = 10
imgQ = queue.Queue(BUF_SIZE)
outQ = queue.Queue(BUF_SIZE)

cifar2_classes = ["automobile", "truck"]


def CPUCalcArgmax(data):
    '''
    returns index of highest value in data
    '''
    val = np.argmax(data)
    return val


def preprocess_fn(image_path):
    '''
    Image pre-processing.
    Opens image as grayscale then normalizes to range 0:1
    input arg: path of image file
    return: numpy array
    '''
    image = cv2.imread(image_path)
    #image = image.reshape(32,32,3)
    data = np.asarray( image, dtype="float32" )
    data = data/255.0
    return data

def Sigmoid1(xx):
    x = np.asarray( xx, dtype="float32")
    t = 1 / (1 + np.exp(-x))
    if DEBUG:
        print("SIGM1 inp shape ", x.shape)
        np.save('sigm1_data_inp.bin', x[0])
        #print("SIGM1 inp: ", x)
        #print("SIGM1 out: ", t)
        np.save('sigm1_data_out.bin', t[0])
    return t

def Sigmoid2(xx):
    x = np.asarray( xx, dtype="float32")
    t = 1 / (1 + np.exp(-x))
    if DEBUG:
        print("SIGM2 inp shape ", x.shape)
        np.save('sigm2_data_inp.bin', x)
        #print("SIGM2 inp: ", x)
        #print("SIGM2 out: ", t)
        np.save('sigm2_data_out.bin', t)
    return t

def Tanh(xx):
    x = np.asarray( xx, dtype="float32" )
    t=(np.exp(x)-np.exp(-x))/(np.exp(x)+np.exp(-x))
    if DEBUG:
        print("TANH inp shape ", x.shape)
        np.save('tanh_data_inp.bin', x[0])
        #print("TANH inp: ", x)
        np.save('tanh_data_out.bin', t[0])
        #print("TANH out: ", t)
    return t


def Linear(xx):
    fc2_w = np.load("fc2_weight.bin.npy")
    fc2_b = np.load("fc2_bias.bin.npy")
    x = np.asarray( xx, dtype="float32" )
    w = np.asarray( fc2_w, dtype="float32" )
    b = np.asarray( fc2_b, dtype="float32" )
    #dot product
    wt = np.transpose(w,(1,0))
    y = np.dot(x, wt) + b
    if DEBUG:
        np.save('cnn_out.bin', y)
    return y

"""
# yet to be checked
def LeakyReLU(K, x):
    data = np.asarray( x, dtype="float32" )
    print("LKRE inp shape ", data.shape)
    size  = data[0].size
    shape = data[0].shape
    d = data[0].reshape([1, size])
    print("LKRE inp: ", d)
    pos_index = (d >= 0);
    neg_index = (d <  0); Linear(out6)
    coef = np.float32(K)
    y_n = d[np.ix_(neg_index)]*coef
    y_p = d[np.ix_(pos_index)]
    y = [y_n, y_p]
    print("LKRE out: ", y)
    x[0] = y.reshape([shape])
    return x
"""

def fix2float(fix_point, value):
    return value.astype(np.float32) * np.exp2(fix_point, dtype=np.float32)


def float2fix(fix_point, value):
    return value.astype(np.float32) / np.exp2(fix_point, dtype=np.float32)


def execute_async(dpu, tensor_buffers_dict):
    input_tensor_buffers = [
        tensor_buffers_dict[t.name] for t in dpu.get_input_tensors()
    ]
    output_tensor_buffers = [
        tensor_buffers_dict[t.name] for t in dpu.get_output_tensors()
    ]
    jid = dpu.execute_async(input_tensor_buffers, output_tensor_buffers)
    return dpu.wait(jid)

def DEBUG_runDPU(dpu_1, dpu_3, dpu_5):
    print("Start DPU DEBUG with 1 input image")
    # get DPU input/output tensors
    inputTensor_1  = dpu_1.get_input_tensors()
    outputTensor_1 = dpu_1.get_output_tensors()
    inputTensor_3  = dpu_3.get_input_tensors()
    outputTensor_3 = dpu_3.get_output_tensors()
    inputTensor_5  = dpu_5.get_input_tensors()
    outputTensor_5 = dpu_5.get_output_tensors()
    input_1_ndim  = tuple(inputTensor_1[0].dims)
    input_3_ndim  = tuple(inputTensor_3[0].dims)
    input_5_ndim  = tuple(inputTensor_5[0].dims)
    output_1_ndim = tuple(outputTensor_1[0].dims)
    output_3_ndim = tuple(outputTensor_3[0].dims)
    output_5_ndim = tuple(outputTensor_5[0].dims)
    batchSize = input_1_ndim[0]

    out1 = np.zeros([batchSize, 32, 32, 16], dtype='float32')
    out3 = np.zeros([batchSize, 16, 16,  8], dtype='float32')
    out5 = np.zeros([batchSize, 32        ], dtype='float32')

    if DEBUG :
        print(" inputTensor1={}\n".format( inputTensor_1[0]))
        print("outputTensor1={}\n".format(outputTensor_1[0]))
        print(" inputTensor3={}\n".format( inputTensor_3[0]))
        print("outputTensor3={}\n".format(outputTensor_3[0]))
        print(" inputTensor5={}\n".format( inputTensor_5[0]))
        print("outputTensor5={}\n".format(outputTensor_5[0]))

    if not imgQ.empty():
        img_org = imgQ.get()
        # run DPU
        execute_async(
            dpu_1, {
                "CNN__input_0_fix": img_org,
                "CNN__CNN_Conv2d_conv1__18_fix": out1
            })
        inp2 = out1.copy()
        out2 = Tanh(inp2)
        print("out2 shape ", out2.shape)
        # run DPU
        execute_async(
            dpu_3, {
                "CNN__CNN_Tanh_act1__19_fix": out2,
                "CNN__CNN_Conv2d_conv2__35_fix": out3
            })
        inp4 = out3.copy()
        out4 = Sigmoid1(inp4)
        print("out4 shape ", out4.shape)
        # run DPU
        execute_async(
            dpu_5, {
                "CNN__CNN_Sigmoid_act2__36_fix": out4,
                "CNN__CNN_Linear_fc1__48_fix":   out5
            })
        inp6 = out5.copy()
        out6 = Sigmoid2(inp6)
        print("out6 shape ", out6.shape)
        cnn_out = Linear(out6)
        prediction_index = CPUCalcArgmax(cnn_out) #(outputData[0][j])
        print("DEBUG DONE")


def runDPU(dpu_1, dpu_3, dpu_5, img):
    # get DPU input/output tensors
    inputTensor_1  = dpu_1.get_input_tensors()
    outputTensor_1 = dpu_1.get_output_tensors()
    inputTensor_3  = dpu_3.get_input_tensors()
    outputTensor_3 = dpu_3.get_output_tensors()
    inputTensor_5  = dpu_5.get_input_tensors()
    outputTensor_5 = dpu_5.get_output_tensors()
    input_1_ndim  = tuple(inputTensor_1[0].dims)
    input_3_ndim  = tuple(inputTensor_3[0].dims)
    input_5_ndim  = tuple(inputTensor_5[0].dims)
    output_1_ndim = tuple(outputTensor_1[0].dims)
    output_3_ndim = tuple(outputTensor_3[0].dims)
    output_5_ndim = tuple(outputTensor_5[0].dims)
    batchSize = input_1_ndim[0]

    out1 = np.zeros([batchSize, 32, 32, 16], dtype='float32')
    out3 = np.zeros([batchSize, 16, 16,  8], dtype='float32')
    out5 = np.zeros([batchSize, 32        ], dtype='float32')

    n_of_images = len(img)
    count = 0
    write_index = 0
    while count < n_of_images:
        if (count+batchSize<=n_of_images):
            runSize = batchSize
        else:
            runSize=n_of_images-count
        '''prepare batch input/output '''
        outputData = []
        inputData = []
        inputData = [np.empty(input_1_ndim, dtype=np.float32, order="C")]
        #outputData = [np.empty(output_5_ndim, dtype=np.float32, order="C")]
        '''init input image to input buffer '''
        for j in range(runSize):
            imageRun = inputData[0]
            imageRun[j, ...] = img[(count + j) % n_of_images].reshape(input_1_ndim[1:])
        '''run with batch '''
        # run DPU
        execute_async(
            dpu_1, {
                "CNN__input_0_fix": inputData[0],
                "CNN__CNN_Conv2d_conv1__18_fix": out1
            })
        inp2 = out1.copy()
        out2 = Tanh(inp2)
        #print("out2 shape ", out2.shape)
        # run DPU
        execute_async(
            dpu_3, {
                "CNN__CNN_Tanh_act1__19_fix": out2,
                "CNN__CNN_Conv2d_conv2__35_fix": out3
            })
        inp4 = out3.copy()
        out4 = Sigmoid1(inp4)
        #print("out4 shape ", out4.shape)
        # run DPU
        execute_async(
            dpu_5, {
                "CNN__CNN_Sigmoid_act2__36_fix": out4,
                "CNN__CNN_Linear_fc1__48_fix":   out5
            })
        inp6 = out5.copy()
        out6 = Sigmoid2(inp6)
        #print("out6 shape ", out6.shape)
        cnn_out = Linear(out6)
        '''store output vectors '''
        for j in range(runSize):
            out_q[write_index] = CPUCalcArgmax(cnn_out[j]) #(outputData[0][j])
            write_index += 1
        count = count + runSize


#def app(images_dir,threads,model_name):
def app(images_dir,model_name):

    images_list=os.listdir(images_dir)
    runTotal = len(images_list)
    print('Found',len(images_list),'images - processing',runTotal,'of them')

    ''' global list that all threads can write results to '''
    global out_q
    out_q = [None] * runTotal

    ''' get a list of subgraphs from the compiled model file '''
    g = xir.Graph.deserialize(model_name)
    subgraphs = g.get_root_subgraph().toposort_child_subgraph()
    dpu_subgraph0 = subgraphs[0]
    dpu_subgraph1 = subgraphs[1]
    dpu_subgraph3 = subgraphs[3]
    dpu_subgraph5 = subgraphs[5]
    if DEBUG:
        print("dpu_subgraph0 = " + dpu_subgraph0.get_name()) #dpu_subgraph0=subgraph_CNN__input_0
        print("dpu_subgraph1 = " + dpu_subgraph1.get_name()) #dpu_subgraph1 = subgraph_CNN__CNN_Conv2d_conv1__18
        print("dpu_subgraph3 = " + dpu_subgraph3.get_name()) #dpu_subgraph3 = subgraph_CNN__CNN_Conv2d_conv2__35
        print("dpu_subgraph5 = " + dpu_subgraph5.get_name()) #dpu_subgraph5 = subgraph_CNN__CNN_Linear_fc1__48(TransferMatMulToConv2d)
    dpu_1 = vart.Runner.create_runner(dpu_subgraph1, "run")
    dpu_3 = vart.Runner.create_runner(dpu_subgraph3, "run")
    dpu_5 = vart.Runner.create_runner(dpu_subgraph5, "run")

    ''' DEBUG with 1 input image '''
    if DEBUG:
        dbg_img = []
        path = "./test/img.png"
        dbg_img.append(preprocess_fn(path))
        imgQ.put(dbg_img[0])
        DEBUG_runDPU(dpu_1, dpu_3, dpu_5)
        return

    ''' Pre Processing images '''
    print("Pre-processing ",runTotal," images")
    img = []
    for i in range(runTotal):
        path = os.path.join(images_dir,images_list[i])
        img.append(preprocess_fn(path))

    ''' DPU execution '''
    print("run DPU")
    start=0
    end = len(img)
    in_q = img[start:end]
    time1 = time.time()
    runDPU(dpu_1, dpu_3, dpu_5, img)
    time2 = time.time()
    timetotal = time2 - time1
    fps = float(runTotal / timetotal)
    print(" ")
    print("FPS=%.2f, total frames = %.0f , time=%.4f seconds" %(fps,runTotal, timetotal))
    print(" ")

    ''' Post Processing '''
    print("Post-processing")
    classes = cifar2_classes
    correct = 0
    wrong = 0
    for i in range(len(out_q)):
        prediction = classes[out_q[i]]
        ground_truth, _ = images_list[i].split("_", 1)
        if PRINT_IMAGES:
            print("image number ", i, ": ", images_list[i])
            inp_img  = in_q[i] * 255.0
            cv2.imshow(images_list[i], np.uint8(inp_img));
            cv2.waitKey(1000);
            print("predicted: ", prediction, " ground Truth ", ground_truth)
        if (ground_truth==prediction):
            correct += 1
        else:
            wrong += 1
    accuracy = correct/len(out_q)
    print("Correct: ",correct," Wrong: ",wrong," Accuracy: ", accuracy)
    return



# only used if script is run as 'main' from command line
def main():

  # construct the argument parser and parse the arguments
  ap = argparse.ArgumentParser()
  ap.add_argument('-d', '--images_dir', type=str, default='../test_images', help='Path to folder of images. Default is images')
  #ap.add_argument('-t', '--threads',    type=int, default=1,        help='Number of threads. Default is 1')
  ap.add_argument('-m', '--model',      type=str, default='./CNN_int_vck190_dw.xmodel', help='Path of xmodel. Default is CNN_zcu102.xmodel')

  args = ap.parse_args()
  print("\n")
  print ('Command line options:')
  print (' --images_dir : ', args.images_dir)
  #print (' --threads    : ', args.threads)
  print (' --model      : ', args.model)
  print("\n")

  #app(args.images_dir,args.threads,args.model)
  app(args.images_dir,args.model)



if __name__ == '__main__':
  main()
