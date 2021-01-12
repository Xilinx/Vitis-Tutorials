'''
Copyright 2020 Xilinx Inc.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
'''

from ctypes import *
from typing import List
import cv2
import numpy as np
import vart
import xir
import os
import pathlib
import xir
import threading
import time
import sys
import argparse
import math


def get_script_directory():
    path = os.getcwd()
    return path

def CPUCalcArgmax(data):
    '''
    returns index of highest value in data
    '''
    return np.argmax(data)

def CPUCalcSoftmax(data,size):
    '''
    Calculate softmax
    data: data to be calculated
    size: data size
    return: softamx result
    '''
    #print("\n data.shape ", data.shape)
    #print("\n data.size  ", size)
    sum=0.0
    result = [0 for i in range(size)]
    for i in range(size):
        result[i] = math.exp(data[i])
        sum +=result[i]
    for i in range(size):
        result[i] /=sum
    return result



def NormalizeImageArr( path ):
    img = cv2.imread(path, 1)
    img = img.astype(np.float32)
    img = img/127.5 - 1.0
    return img

def LoadSegmentationArr( path , nClasses,  width , height ):
    seg_labels = np.zeros((  height , width  , nClasses ))
    img = cv2.imread(path, 1)
    img = img[:, : , 0]
    for c in range(nClasses):
        seg_labels[: , : , c ] = (img == c ).astype(int)
    return seg_labels

def IoU(Yi,y_predi):
    ## mean Intersection over Union
    ## Mean IoU = TP/(FN + TP + FP)
    CLASS_NAMES = ("Sky",
                   "Wall",
                   "Pole",
                   "Road",
                   "Sidewalk",
                   "Vegetation",
                   "Sign",
                   "Fence",
                   "vehicle",
                   "Pedestrian",
                   "Bicyclist",
                   "miscellanea")
    IoUs = []
    Nclass = int(np.max(Yi)) + 1
    for c in range(Nclass):
        TP = np.sum( (Yi == c)&(y_predi == c))
        FP = np.sum( (Yi != c)&(y_predi == c))
        FN = np.sum( (Yi == c)&(y_predi != c))
        IoU = TP/float(TP + FP + FN)
        #print("class {:02.0f}: #TP={:7.0f}, #FP={:7.0f}, #FN={:7.0f}, IoU={:4.3f}".format(c,TP,FP,FN,IoU))
        print("class (%2d) %12.12s: #TP=%7.0f, #FP=%7.0f, #FN=%7.0f, IoU=%4.3f" % (c, CLASS_NAMES[c],TP,FP,FN,IoU))
        IoUs.append(IoU)
    mIoU = np.mean(IoUs)
    print("_________________")
    print("Mean IoU: {:4.3f}".format(mIoU))
    return


def get_child_subgraph_dpu(graph: "Graph") -> List["Subgraph"]:
    assert graph is not None, "'graph' should not be None."
    root_subgraph = graph.get_root_subgraph()
    assert (
        root_subgraph is not None
    ), "Failed to get root subgraph of input Graph object."
    if root_subgraph.is_leaf:
        return []
    child_subgraphs = root_subgraph.toposort_child_subgraph()
    assert child_subgraphs is not None and len(child_subgraphs) > 0
    return [
        cs
        for cs in child_subgraphs
        if cs.has_attr("device") and cs.get_attr("device").upper() == "DPU"
    ]


def runDPU(id,start,dpu,img):

    '''get tensor'''
    inputTensors = dpu.get_input_tensors()
    outputTensors = dpu.get_output_tensors()
    input_ndim = tuple(inputTensors[0].dims)
    output_ndim = tuple(outputTensors[0].dims)
    print("\nrunDPU-  INPUT DIM: ",  input_ndim)
    print("\nrunDPU- OUTPUT DIM: ", output_ndim)

    batchSize = input_ndim[0]
    print()
    n_of_images = len(img)
    count = 0
    write_index = start
    print("\nrunDPU- batchSize: ", batchSize)
    print("\nrunDPU- # images : ", n_of_images)

    while count < n_of_images:
        if (count+batchSize<=n_of_images):
            runSize = batchSize
        else:
            runSize=n_of_images-count
        if count==0:
            print("\nrunDPU- # runSize : ", runSize)

        '''prepare batch input/output '''
        outputData = []
        inputData = []
        inputData = [np.empty(input_ndim, dtype=np.float32, order="C")]
        outputData = [np.empty(output_ndim, dtype=np.float32, order="C")]

        '''init input image to input buffer '''
        for j in range(runSize):
            imageRun = inputData[0]
            imageRun[j, ...] = img[(count + j) % n_of_images].reshape(input_ndim[1:])

        '''run with batch '''
        job_id = dpu.execute_async(inputData,outputData)
        dpu.wait(job_id)

        '''store output vectors '''
        for j in range(runSize):
            out_q[write_index] = outputData[0][j] #CPUCalcSoftmax(outputData[0], 12*224*224) #CPUCalcArgmax(outputData[0][j])
            write_index += 1

        count = count + runSize

    print("\nrunDPU: write_index : ", write_index)

def app(image_dir,threads,model, compute_miou):

    # load testing images
    print("\nAPP- loading segmentation images and preprocessing test images")
    test_images = os.listdir(image_dir)
    test_images.sort()
    #print("\nAPP- TEST IMAGES = ", test_images)
    #print("\nAPP- LEN TEST IMAGES = ", len(test_images))

    runTotal = len(test_images)
    dir_test_seg = image_dir + "../seg_test"
    test_segmentations  = os.listdir(dir_test_seg)
    test_segmentations.sort()
    #print("\nAPP- SEG IMAGES = ", test_segmentations)
    #print("\nAPP- LEN SEGM IMAGES = ", len(test_segmentations))

    X_test = []
    Y_test = []
    for im , seg in zip(test_images,test_segmentations) :
        X_test.append(NormalizeImageArr(  os.path.join(image_dir, im)) )
        Y_test.append(LoadSegmentationArr(os.path.join(dir_test_seg, seg), 12, 224, 224))
    X_test = np.array(X_test)
    Y_test = np.array(Y_test)
    #print("\nAPP- testing    data (X) (Y) shapes", X_test.shape,Y_test.shape)
    #print("\n")

    global out_q
    out_q = np.zeros((runTotal,224,224,12),dtype=np.float32)
    all_dpu_runners = []
    #print("\nAPP- out_q: ", np.array(out_q).shape)

    g = xir.Graph.deserialize(model)
    subgraphs = get_child_subgraph_dpu(g)
    assert len(subgraphs) == 1  # only one DPU kernel
    print('\nAPP- Found',len(subgraphs),'subgraphs in',model)
    for i in range(threads):
        all_dpu_runners.append(vart.Runner.create_runner(subgraphs[0], "run"))

    '''run threads '''
    print('\nAPP- Starting',threads,'threads...')
    threadAll = []
    start=0
    for i in range(threads):
        if (i==threads-1):
            end = len(X_test)
        else:
            end = start+(len(X_test)//threads)
        in_q = X_test[start:end]

        t1 = threading.Thread(target=runDPU, args=(i,start,all_dpu_runners[i], in_q))
        threadAll.append(t1)
        start=end

    time1 = time.time()
    for x in threadAll:
        x.start()
    for x in threadAll:
        x.join()
    time2 = time.time()
    timetotal = time2 - time1

    fps = float(runTotal / timetotal)
    print("\nAPP- FPS=%.2f, total frames = %.0f , time=%.4f seconds\n" %(fps,runTotal, timetotal))

    ''' put post-processing here if you have one '''
    if (compute_miou==1):
        y_pred1 = np.zeros((runTotal,224,224,12),dtype=np.float32)
        #print("out_q.shape : ", out_q.shape)
        for i in range( len(X_test) ):
            #print("i= ", i)
            tmp = np.reshape(out_q[i], 224*224*12)
            y_pred = CPUCalcSoftmax(tmp, 224*224*12)
            y_pred1[i] = np.reshape(y_pred, (224,224,12))
        print("\n\nAPP- now computing IoU over testing data set:")
        #np.save("dpu_out_q.npy", out_q)
        y_pred1_i = np.argmax(y_pred1, axis=3)
        #print(y_pred1_i.shape)
        y_test1_i = np.argmax(Y_test, axis=3)
        #print(y_test1_i.shape)
        IoU(y_test1_i, y_pred1_i)


# only used if script is run as 'main' from command line
def main():

  # construct the argument parser and parse the arguments
  ap = argparse.ArgumentParser()
  ap.add_argument('-d', '--images',  type=str,            help='Path to folder of images.')
  ap.add_argument('-t', '--threads', type=int, default=1, help='Number of threads. Default is 1')
  ap.add_argument('-m', '--model',   type=str,            help='Path of xmodel')
  ap.add_argument('-i', '--miou',    type=int, default=0, help='Compute Mean IoU: 1(YES)/0(NO). Default is 0')

  args = ap.parse_args()

  print ('Command line options:')
  print (' --images  : ', args.images)
  print (' --threads : ', args.threads)
  print (' --model   : ', args.model)
  print (' --miou    : ', args.miou)

  app(args.images,args.threads,args.model, args.miou)

if __name__ == '__main__':
  main()
