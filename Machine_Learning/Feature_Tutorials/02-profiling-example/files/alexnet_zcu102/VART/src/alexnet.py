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

from ctypes import *
import cv2
import numpy as np
import runner
import xir.graph
import pathlib
import xir.subgraph
import os
import input_fn
import math
import threading
import time
import sys

'''
Calculate softmax
data: data to be calculated
size: data size
return: softamx result
'''
def CPUCalcSoftmax(data,size):
    sum=0.0
    result = [0 for i in range(size)]
    for i in range(size):
        result[i] = math.exp(data[i])
        sum +=result[i]
    for i in range(size):
        result[i] /=sum
    return result

def get_script_directory():
    path = os.getcwd()
    return path

'''
Get topk results according to its probability
datain: data result of softmax
filePath: filePath in witch that records the infotmation of kinds
'''
def TopK(datain,size,filePath):

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


SCRIPT_DIR = get_script_directory()
test_image_dir  = SCRIPT_DIR + "/../../test_image/"
IMAGE_WIDTH = 227
IMAGE_HEIGHT = 227

global threadnum
threadnum = 0


'''
run AlexNet  with batch
dpu: dpu runner
img: imagelist to be run
cnt: threadnum
'''
def runAlexNet(dpu,img,cnt):
    """get tensor"""
    inputTensors = dpu.get_input_tensors()
    outputTensors = dpu.get_output_tensors()
    outputHeight = outputTensors[0].dims[1]
    outputWidth = outputTensors[0].dims[2]
    outputChannel = outputTensors[0].dims[3]
    outputSize = outputHeight*outputWidth*outputChannel
    softmax = np.empty(outputSize)
    batchSize = inputTensors[0].dims[0]
    n_of_images = len(img)
    #print(n_of_images)
    count = 0
    while count < cnt:
        runSize = batchSize
        shapeIn = (runSize,) + tuple([inputTensors[0].dims[i] for i in range(inputTensors[0].ndim)][1:])

        """prepare batch input/output """
        outputData = []
        inputData = []
        outputData.append(np.empty((runSize,outputHeight,outputWidth,outputChannel), dtype = np.float32, order = 'C'))
        inputData.append(np.empty((shapeIn), dtype = np.float32, order = 'C'))

        """init input image to input buffer """
        for j in range(runSize):
            imageRun = inputData[0]
            imageRun[j,...] = img[(count+j)% n_of_images].reshape(inputTensors[0].dims[1],inputTensors[0].dims[2],inputTensors[0].dims[3])

        """run with batch """
        job_id = dpu.execute_async(inputData,outputData)
        dpu.wait(job_id)

        for j in range(len(outputData)):
            outputData[j] = outputData[j].reshape(runSize, outputSize)

        """softmax calculate with batch """
        """Benchmark DPU FPS performance over Vitis AI APIs execute_async() and wait() """
        """Uncomment the following code snippet to include softmax calculation for model’s end-to-end FPS evaluation """
        #for j in range(runSize):
        #    softmax = CPUCalcSoftmax(outputData[0][j], outputSize)

        count = count + runSize

def get_subgraph (g):
    sub = []
    root = g.get_root_subgraph()
    sub = [ s for s in root.children
            if s.metadata.get_attr_str ("device") == "DPU"]
    return sub
def main(argv):
    global threadnum

    listimage = []
    list_files=os.listdir(test_image_dir)
    for filename in list_files:
        if "jpg" in filename:
            listimage.append(filename)
        #else:
        #    print(filename)
    
    
    #print(listimage)
    threadAll = []
    threadnum = int(argv[1])
    i = 0
    global runTotall
    runTotall = len(listimage)
    g = xir.graph.Graph.deserialize(pathlib.Path(argv[2]))
    subgraphs = get_subgraph (g)
    assert len(subgraphs) == 1 # only one DPU kernel
    all_dpu_runners = [];
    for i in range(int(threadnum)):
        all_dpu_runners.append(runner.Runner(subgraphs[0], "run"));
    """image list to be run """
    img = []
    for i in range(runTotall):
        path = os.path.join(test_image_dir,listimage[i])
        img.append(input_fn.preprocess_fn(path))

    cnt = 360 ;
    """run with batch """
    time_start = time.time()
    for i in range(int(threadnum)):
        t1 = threading.Thread(target=runAlexNet, args=(all_dpu_runners[i], img, cnt))
        threadAll.append(t1)
    for x in threadAll:
        x.start()
    for x in threadAll:
        x.join()

    time_end = time.time()
    timetotal = time_end - time_start
    total_frames =  cnt*int(threadnum)
    fps = float(total_frames / timetotal)
    #print("FPS=%.2f, total frames = %.2f , time=%.6f seconds" %(fps,total_frames, timetotal))
    print("FPS=%.2f, time=%.6f seconds" %(fps, timetotal))

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("please input thread number and elf file path.")
    else :
        main(sys.argv)
