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

'''
Author: Mark Harvey, Xilinx Inc
'''

from ctypes import *
from typing import List
import cv2
import numpy as np
import vart
import os
import pathlib
import xir
import threading
import time
import sys
import argparse


divider = '-----------------------------------------------'


def preprocess_fn(image_path):
    '''
    Image pre-processing.
    Rearranges from BGR to RGB then normalizes to range 0:1
    input arg: path of image file
    return: numpy array
    '''
    image = cv2.imread(image_path)
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    image = image/255.0
    return image


def get_child_subgraph_dpu(graph: "Graph") -> List["Subgraph"]:
    '''
    interrogate model file to return subgraphs
    Returns a list of subgraph objects
    '''
    assert graph is not None, "'graph' should not be None."
    root_subgraph = graph.get_root_subgraph()
    assert (root_subgraph is not None), "Failed to get root subgraph of input Graph object."
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

    '''
    DPU execution - called in thread from app function.

    Arguments:
    id: integer to identify thread - not currently used
    start: Start index for writes to out_q.
    dpu: runner
    img: list of pre-processed images to pass into DPU
    '''

    ''' input/output tensor information
    get_input_tensors() and get_output_tensors() return lists of tensors objects.
    The lists will contain one element for each input or output of the network.
    The shape of each tensor object is (batch,height,width,channels)
    For Zynq DPUCZDX8G DPU, batchsize is always 1 and this code can be simplified.
    '''
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

        '''store output vectors in global results list '''
        for j in range(runSize):
            out_q[write_index] = np.argmax(outputData[0][j])
            write_index += 1
        count = count + runSize
    
    return

def app(image_dir,threads,model):

    '''
    main application function
    '''

    listimage=os.listdir(image_dir)
    runTotal = len(listimage[:2500])
    print('Found',len(listimage),'images - processing',runTotal,'of them')

    ''' global list that all threads can write results to '''
    global out_q
    out_q = [None] * runTotal


    ''' get a list of subgraphs from the compiled model file '''
    g = xir.Graph.deserialize(model)
    subgraphs = get_child_subgraph_dpu(g)
    print('Found',len(subgraphs),'subgraphs in',model)


    ''' create dpu runners
    Each thread receives a dpu runner.
    Each dpu runner executes a subgraph
    '''
    all_dpu_runners = []
    for i in range(threads):
        all_dpu_runners.append(vart.Runner.create_runner(subgraphs[0], "run"))
        

    ''' preprocess images '''
    print('Pre-processing',runTotal,'images...')
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
        if (i==threads-1):
            end = len(img)
        else:
            end = start+(len(img)//threads)
        in_q = img[start:end]
        t1 = threading.Thread(target=runDPU, args=(i,start,all_dpu_runners[i], in_q))
        threadAll.append(t1)
        start=end

    '''run threads '''
    print('Starting',threads,'threads...')
    time1 = time.time()
    for x in threadAll:
        x.start()
    for x in threadAll:
        x.join()
    time2 = time.time()
    threads_time = time2 - time1


    ''' post-processing '''
    classes = ['dog','cat']  
    correct = 0
    wrong = 0
    for i in range(len(out_q)):
        prediction = classes[out_q[i]]
        ground_truth, _ = listimage[i].split('.',1)
        if (ground_truth==prediction):
            correct += 1
        else:
            wrong += 1
    accuracy = correct/len(out_q)
    print (divider)
    print('Correct:%d, Wrong:%d, Accuracy:%.4f' %(correct,wrong,accuracy))
    
    print (divider)
    fps = float(runTotal / threads_time)
    print('FPS: %.2f, total frames: %.0f, total time: %.3f seconds' %(fps,runTotal,threads_time))
    print (divider)

    return



# only used if script is run as 'main' from command line
def main():

  # construct the argument parser and parse the arguments
  ap = argparse.ArgumentParser()  
  ap.add_argument('-d', '--image_dir',type=str,default='images',   help='Path to folder of images. Default is images')  
  ap.add_argument('-t', '--threads',  type=int,default=1,          help='Number of threads. Default is 1')
  ap.add_argument('-m', '--model',    type=str,default='model_dir/alexnet_np.xmodel',  help='Path of folder containing .xmodel. Default is model_dir/alexnet_np.xmodel')

  args = ap.parse_args()  
  print (divider)
  print ('Command line options:')
  print (' --image_dir : ', args.image_dir)
  print (' --threads   : ', args.threads)
  print (' --model     : ', args.model)
  print (divider)

  app(args.image_dir,args.threads,args.model)

if __name__ == '__main__':
  main()

