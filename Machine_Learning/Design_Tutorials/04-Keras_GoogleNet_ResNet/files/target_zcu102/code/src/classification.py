'''
Copyright 2019 Xilinx Inc.

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
# code inspired by VAI 1.3 VART  inception_v1_mt_py

from ctypes import *
from typing import List
import cv2
import numpy as np
import vart
import xir
import pathlib
import os
import math
import threading
import time
import sys
import argparse

#######################################################################################################
#Size of images
IMAGE_WIDTH  = 32
IMAGE_HEIGHT = 32
#normalization factor
NORM_FACTOR = 255.0
#number of classes
NUM_CLASSES = 10

cifar10_classes = ["airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "horse", "ship", "truck"]
fmnist_classes = ["top", "trouser", "pullover", "dress", "coat", "sandal", "shirt", "sneaker", "bag", "ankleBoot"]

def normalize(image):
  image=image/255.0
  image=image-0.5
  image=image*2
  return image

def preprocess_fn(image_path, crop_height = 32, crop_width = 32):
    image = cv2.imread(image_path) #BGR
    image = normalize(image)
    return image
#######################################################################################################

def get_script_directory():
    path = os.getcwd()
    return path

def CPUCalcSoftmax(data,size):
    '''
    Calculate softmax
    data: data to be calculated
    size: data size
    return: softamx result
    '''
    sum=0.0
    result = [0 for i in range(size)]
    for i in range(size):
        result[i] = math.exp(data[i])
        sum +=result[i]
    for i in range(size):
        result[i] /=sum
    return result


def TopK(datain,size,filePath):
    '''
    Get topk results according to its probability
    datain: data result of softmax
    filePath: filePath in witch that records the infotmation of kinds
    '''
    cnt=[i for i in range(size) ]
    pair=zip(datain,cnt)
    pair=sorted(pair,reverse=True)
    softmax_new,cnt_new=zip(*pair)
    fp=open(filePath, "r")
    data1=fp.readlines()
    fp.close()
    for i in range(2):
        flag=0
        for line in data1:
            if flag==cnt_new[i]:
                print("Top[%d] %f %s" %(i, (softmax_new[i]),(line.strip)("\n")))
            flag=flag+1

#######################################################################################################

SCRIPT_DIR = get_script_directory()


def CPUCalcArgmax(data):
    '''
    returns index of highest value in data
    '''
    return np.argmax(data)

#######################################################################################################

def runDPU(id,start,dpu:"Runner",img):
    '''get tensor'''
    inputTensors = dpu.get_input_tensors()
    outputTensors = dpu.get_output_tensors()
    input_ndim = tuple(inputTensors[0].dims)
    output_ndim = tuple(outputTensors[0].dims)
    batchSize = input_ndim[0]
    n_of_images = len(img)
    count = 0
    write_index = start
    while count < n_of_images:
        if (count+batchSize<=n_of_images):
            runSize = batchSize
        else:
            runSize=n_of_images-count
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
            out_q[write_index] = CPUCalcArgmax(outputData[0][j])
            write_index += 1

        count = count + runSize


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


def app(image_dir,threads,model, classes_type):
    '''
    main application function
    '''
    listimage=os.listdir(image_dir)
    #print(listimage)
    runTotal = len(listimage)
    print('Found',len(listimage),'images - processing',runTotal,'of them')

    ''' global list that all threads can write results to '''
    global out_q
    out_q = [None] * runTotal
    all_dpu_runners = []

    ''' get a list of subgraphs from the compiled model file '''
    g = xir.Graph.deserialize(model) #(pathlib.Path(model))
    subgraphs = get_child_subgraph_dpu(g)
    assert len(subgraphs) == 1  # only one DPU kernel
    print('Found',len(subgraphs),'subgraphs in',model)
    for i in range(threads):
        # make a runner for each thread, give it a subgraph to execute
        all_dpu_runners.append(vart.Runner.create_runner(subgraphs[0], "run"))

    ''' preprocess images '''
    img = []
    for i in range(runTotal):
        path = os.path.join(image_dir,listimage[i])
        img.append(preprocess_fn(path))

    ''' create threads
    Each thread receives a section of the preprocessed images list as input and
    will write results into the corresponding section of the global out_q list.
    '''
    threadAll = []
    start=0
    for i in range(threads):
        # divide the list of preprocessed images across the threads
        if (i==threads-1):
            end = len(img)
        else:
            end = start+(len(img)//threads)
        in_q = img[start:end]

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
    print(" ")
    print("FPS=%.2f, total frames = %.0f , time=%.4f seconds" %(fps,runTotal, timetotal))
    print(" ")
    del all_dpu_runners

    ''' post-processing '''
    '''
    if ((classes_type=="FMNIST") or (classes_type=="fmnist")):
        classes = fmnist_classes
    else:
        classes = cifar10_classes # default value
    correct = 0
    wrong = 0
    for i in range(len(out_q)):
        #print("image number ", i)
        #inp_img  = (in_q[i] + 0.5) * 255.0
        #cv2.imshow("Image", np.uint8(inp_img));
        #cv2.waitKey(10);
        prediction = classes[out_q[i]]
        ground_truth, _ = listimage[i].split("_", 1)
        if (ground_truth==prediction):
            correct += 1
        else:
            wrong += 1
    accuracy = correct/len(out_q)
    print('Correct:',correct,'Wrong:',wrong,'Accuracy:', accuracy)
    '''

#######################################################################################################

# only used if script is run as 'main' from command line
def main():

  # construct the argument parser and parse the arguments
  ap = argparse.ArgumentParser()
  ap.add_argument('-d', '--images',  type=str,            help='Path to folder of test images')
  ap.add_argument('-t', '--threads', type=int, default=1, help='Number of threads. Default is 1')
  ap.add_argument('-m', '--model',   type=str,            help='Path of elf file')
  ap.add_argument('-c', '--classes', type=str, default="CIFAR10", help='CIFAR10 or FMIST. Default is CIFAR10')


  args = ap.parse_args()

  print ('Command line options:')
  print (' --images  : ', args.images)
  print (' --threads : ', args.threads)
  print (' --model   : ', args.model)
  print (' --classes : ', args.classes)

  app(args.images,args.threads,args.model, args.classes)

#######################################################################################################

if __name__ == '__main__':
    main()
