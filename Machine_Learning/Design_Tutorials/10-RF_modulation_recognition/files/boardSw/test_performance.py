# Copyright 2020 Xilinx Inc.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from ctypes import *
from typing import List
import cv2
import numpy as np
import xir
import vart
import os
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

global threadnum

#def runRfClassify(dpu,rfIn,rfClass,rfSnr,cnt):
def runRfClassify(runner: "Runner",rfIn, cnt):
    """get tensor"""
    inputTensors = runner.get_input_tensors()
    outputTensors = runner.get_output_tensors()
    input_ndim = tuple(inputTensors[0].dims)
    pre_output_size = int(outputTensors[0].get_data_size() / input_ndim[0])
    output_ndim = tuple(outputTensors[0].dims)
    pre_output_size = int(outputTensors[0].get_data_size() / input_ndim[0])
    softmax = np.empty(pre_output_size)
    batchSize = inputTensors[0].dims[0]
    n_of_samples = len(rfIn)
    count = 0;
    top1_count = 0
    mods = [
    'OOK',      '4ASK',      '8ASK',      'BPSK',   'QPSK',    '8PSK',
    '16PSK',    '32PSK',     '16APSK',    '32APSK', '64APSK',  '128APSK',
    '16QAM',    '32QAM',     '64QAM',     '128QAM', '256QAM',  'AM-SSB-WC',
    'AM-SSB-SC','AM-DSB-WC', 'AM-DSB-SC', 'FM',     'GMSK',    'OQPSK']
	
    while count < cnt:
        runSize = batchSize
        shapeIn = (runSize,) + tuple([inputTensors[0].dims[i] for i in range(inputTensors[0].ndim)][1:])

        """prepare batch input/output """
        outputData = []
        inputData = []
        inputData = [np.empty(input_ndim, dtype=np.float32, order="C")]
        outputData = [np.empty(output_ndim, dtype=np.int8, order="C")]

        """init input image to input buffer """
        for j in range(runSize):
            imageRun = inputData[0]
            imageRun[j, ...] = rfIn[(count + j) % n_of_samples].reshape(input_ndim[1:])

        """run with batch """
        job_id = runner.execute_async(inputData,outputData)
        runner.wait(job_id)

        for j in range(len(outputData)):
            outputData[j] = outputData[j].reshape(runSize, pre_output_size)

        """softmax calculate with batch """
        """Benchmark DPU FPS performance over Vitis AI APIs execute_async() and wait() """
        """Uncomment the following code snippet to include softmax calculation for model’s end-to-end FPS evaluation """
        for j in range(runSize):
           softmax = CPUCalcSoftmax(outputData[0][j], pre_output_size)
           top1 = mods[np.argmax(softmax)]
        count = count + runSize 

"""
 obtain dpu subgrah
"""
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


def main(argv):
    global threadnum

    threadAll = []
    threadnum = int(argv[1])
    i = 0
    g = xir.Graph.deserialize(argv[2])
    subgraphs = get_child_subgraph_dpu(g)
    cnt = int(argv[3])
    assert len(subgraphs) == 1  # only one DPU kernel
    all_dpu_runners = []
    for i in range(int(threadnum)):
        all_dpu_runners.append(vart.Runner.create_runner(subgraphs[0], "run"))
   
    """input files"""
    rfIn=np.load('./rf_input.npy')
    rfIn = rfIn[0:cnt,:,:]    
	
    print("Number of RF Samples is ",(cnt*threadnum))

    """
      The cnt variable is used to control the number of times a single-thread DPU runs.
      Users can modify the value according to actual needs. It is not recommended to use
      too small number when there are few input images, for example:
      1. If users can only provide very few images, e.g. only 1 image, they should set
         a relatively large number such as 360 to measure the average performance;
      2. If users provide a huge dataset, e.g. 50000 images in the directory, they can
         use the variable to control the test time, and no need to run the whole dataset.
    """
    """run with batch """
    time_start = time.time()
    for i in range(int(threadnum)):
        t1 = threading.Thread(target=runRfClassify, args=(all_dpu_runners[i], rfIn, cnt))
        threadAll.append(t1)
    for x in threadAll:
        x.start()
    for x in threadAll:
        x.join()

    del all_dpu_runners

    time_end = time.time()
    timetotal = time_end - time_start
    total_frames = cnt * int(threadnum)
    fps = float(total_frames / timetotal)
    print("FPS=%.2f, total RF frames = %.2f , time=%.6f seconds" %(fps,total_frames, timetotal))


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("usage : python3 test_performance.py <thread_number> <xmodel_file> <number of Rf Frames>")
    else:
        main(sys.argv)

