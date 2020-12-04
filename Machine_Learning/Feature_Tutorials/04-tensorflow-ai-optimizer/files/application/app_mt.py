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
import cv2
import numpy as np
import runner
import os
import xir.graph
import pathlib
import xir.subgraph
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

def get_subgraph (g):
    '''
    interrogate model file to return subgraphs
    Returns a list of subgraph objects
    '''
    sub = []
    root = g.get_root_subgraph()
    sub = [ s for s in root.children
            if s.metadata.get_attr_str ("device") == "DPU"]
    return sub 


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
    For Edge DPU, batchsize is always 1.
    '''
    inputTensors = dpu.get_input_tensors()
    outputTensors = dpu.get_output_tensors()
    #print('Input tensor :',inputTensors[0].name,inputTensors[0].shape)
    #print('Output Tensor:',outputTensors[0].name,outputTensors[0].shape)

    outputSize = outputTensors[0].dims[1]*outputTensors[0].dims[2]*outputTensors[0].dims[3]
    shapeIn = inputTensors[0].shape
    shapeOut = outputTensors[0].shape

    for i in range(len(img)):

        '''prepare lists of np arrays to hold input & output tensors '''
        inputData = []
        inputData.append(img[i].reshape(shapeIn))
        outputData = []
        outputData.append(np.empty((shapeOut), dtype = np.float32, order = 'C'))

        '''start DPU, wait until it finishes '''
        job_id = dpu.execute_async(inputData,outputData)
        dpu.wait(job_id)

        ''' output data shape is currently (batch,height,width,channels)
        so flatten it into (batch,height*width*channels)'''
        outputData[0] = outputData[0].reshape(1, outputSize)

        ''' store results in global lists '''
        out_q[start+i] = outputData[0][0]
    
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
    g = xir.graph.Graph.deserialize(pathlib.Path(model))
    subgraphs = get_subgraph (g)
    print('Found',len(subgraphs),'subgraphs in',model)

    ''' preprocess images '''
    print('Pre-processing',runTotal,'images...')
    img = []
    for i in range(runTotal):
        path = os.path.join(image_dir,listimage[i])
        img.append(preprocess_fn(path))


    ''' create dpu runners
    Each thread receives a dpu runner.
    Each dpu runner executes a subgraph
    '''
    all_dpu_runners = []
    for i in range(threads):
        all_dpu_runners.append(runner.Runner(subgraphs[0], "run"))
        

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
        argmax = np.argmax((out_q[i]))
        prediction = classes[argmax]
        ground_truth, _ = listimage[i].split('.',1)
        if (ground_truth==prediction):
            correct += 1
        else:
            wrong += 1
    accuracy = correct/len(out_q)
    print (divider)
    print('Correct:',correct,'Wrong:',wrong,'Accuracy:', accuracy)
    
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
  ap.add_argument('-m', '--model',    type=str,default='model_dir/dpu_alexnet_np.elf',  help='Path of folder with .elf or .xmodel. Default is model_dir/dpu_alexnet_np.elf')

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

