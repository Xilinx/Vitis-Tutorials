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
import cv2
import numpy as np
import runner
import os
import xir
import pathlib
import vart
import threading
import time
import sys
import argparse


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
    sub = []
    root = g.get_root_subgraph()
    sub = [ s for s in root.toposort_child_subgraph()
            if s.has_attr("device") and s.get_attr("device").upper() == "DPU"]
    return sub 


def runDPU(id,start,dpu,img):

    '''get tensor'''
    inputTensors = dpu.get_input_tensors()
    outputTensors = dpu.get_output_tensors()
    shapeIn = tuple(inputTensors[0].dims)
    shapeOut = tuple(outputTensors[0].dims)

    batchSize = shapeIn[0]
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
        outputData = [np.empty(shapeOut, dtype = np.float32, order = 'C')]
        inputData = [np.empty(shapeIn, dtype = np.float32, order = 'C')]
        
        '''init input image to input buffer '''
        for j in range(runSize):
            imageRun = inputData[0]
            imageRun[j, ...] = img[(count+j)% n_of_images].reshape(shapeIn[1:])

        '''run with batch '''
        job_id = dpu.execute_async(inputData,outputData)
        dpu.wait(job_id)

        '''store output vectors '''
        for j in range(runSize):
            out_q[write_index] = outputData[0][j]
            write_index += 1
        count = count + runSize

def app(image_dir,threads,model):

    listimage=os.listdir(image_dir)
    runTotal = len(listimage)

    global out_q
    out_q = [None] * runTotal

    g = xir.Graph.deserialize(model)
    #g = xir.Graph.deserialize('model_dir/densenetx.xmodel')
    subgraphs = get_subgraph (g)
    assert len(subgraphs) == 1 # only one DPU kernel
    all_dpu_runners = []
    for i in range(threads):
        all_dpu_runners.append(runner.Runner(subgraphs[0], "run"))

    ''' preprocess images '''
    print('Pre-processing',runTotal,'images...')
    img = []
    for i in range(runTotal):
        path = os.path.join(image_dir,listimage[i])
        img.append(preprocess_fn(path))

    '''run threads '''
    print('Starting',threads,'threads...')
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

    time1 = time.time()
    for x in threadAll:
        x.start()
    for x in threadAll:
        x.join()
    time2 = time.time()
    timetotal = time2 - time1

    fps = float(runTotal / timetotal)
    print("FPS=%.2f, total frames = %.0f , time=%.4f seconds" %(fps,runTotal, timetotal))


    ''' post-processing '''
    classes = ['airplane','automobile','bird','cat','deer','dog','frog','horse','ship','truck']  
    correct = 0
    wrong = 0
    print('output buffer length:',len(out_q))
    for i in range(len(out_q)):
        argmax = np.argmax((out_q[i]))
        prediction = classes[argmax]
        #prediction = classes[out_q[i]]
        ground_truth, _ = listimage[i].split('_')
        if (ground_truth==prediction):
            correct += 1
        else:
            wrong += 1
    accuracy = correct/len(out_q)
    print('Correct:',correct,'Wrong:',wrong,'Accuracy:', accuracy)





# only used if script is run as 'main' from command line
def main():

  # construct the argument parser and parse the arguments
  ap = argparse.ArgumentParser()  
  ap.add_argument('-d', '--image_dir',
                  type=str,
                  default='images',
                  help='Path to folder of images. Default is images')  
  ap.add_argument('-t', '--threads',
                  type=int,
                  default=1,
                  help='Number of threads. Default is 1')
  ap.add_argument('-m', '--model',
                  type=str,
                  default='model_dir/densenetx.xmodel',
                  help='Path of .elf. Default is model_dir/densenetx.xmodel')

  args = ap.parse_args()  
  
  print ('Command line options:')
  print (' --image_dir : ', args.image_dir)
  print (' --threads   : ', args.threads)
  print (' --model     : ', args.model)

  app(args.image_dir,args.threads,args.model)

if __name__ == '__main__':
  main()

