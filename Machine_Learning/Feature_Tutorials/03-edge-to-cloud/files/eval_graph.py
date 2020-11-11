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
 Evaluation of frozen/quantized graph
 Author: Mark Harvey
'''

import os
import sys
import argparse
import shutil
import numpy as np
import cv2
from progressbar import ProgressBar

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

# workaround for TF1.15 bug "Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR"
os.environ['TF_FORCE_GPU_ALLOW_GROWTH'] = 'true'

import tensorflow as tf
import tensorflow.contrib.decent_q
from tensorflow.python.platform import gfile

from preprocess import preprocess

DIVIDER = '-----------------------------------------'



def graph_eval(input_graph_def, input_node, output_node, dataset, batchsize):
    
    images = []
    ground_truth = []
       
    for root, dirs, files in os.walk(os.path.join(dataset, 'test')):
        for filename in files:
            class_id,_ = filename.split('.', 1)
            images.append(preprocess(os.path.join(root,filename)))
            ground_truth.append(class_id)
   
    print('Found',len(images),'images and',len(ground_truth),'ground_truth')

    tf.import_graph_def(input_graph_def,name = '')

    # Get input placeholders & tensors
    input_tensor = tf.compat.v1.get_default_graph().get_tensor_by_name(input_node+':0')

    # get output tensors
    predict = tf.compat.v1.get_default_graph().get_tensor_by_name(output_node+':0')
    
    # Create the Computational graph
    with tf.compat.v1.Session() as sess:

        predictions = []
        progress = ProgressBar()

        sess.run(tf.compat.v1.initializers.global_variables())
      
        for i in progress(range(len(images)//batchsize)):

            # make batches of images
            img_batch = images[i*batchsize:i*batchsize+batchsize]

            # run session to get a batch of predictions
            feed_dict={input_tensor: img_batch}            
            pred = sess.run([predict], feed_dict)

            for i in range(len(pred[0])):
                if np.argmax(pred[0][i]) == 1:
                    predictions.append('dog')
                else:
                    predictions.append('cat')            


    # iterate over the list of predictions and compare to ground truth
    correct_predictions = 0
    wrong_predictions = 0
    
    for i in range (len(predictions)):
        if predictions[i] == ground_truth[i]:
            correct_predictions += 1
        else:
            wrong_predictions += 1
    
    # calculate accuracy
    acc = (correct_predictions/len(predictions)) * 100
    
    print('Correct Predictions: ',correct_predictions)
    print('Wrong Predictions  : ',wrong_predictions)
    print('Prediction Accuracy: ',acc,'%')

    return


def run_main():

    ap = argparse.ArgumentParser()
    ap.add_argument('-d', '--dataset',    type=str, default='./dataset', help='root folder of dataset')
    ap.add_argument('-g', '--graph',      type=str, default='./freeze/frozen_graph.pb', help='graph file (.pb) to be evaluated.')
    ap.add_argument('-i', '--input_node', type=str, default='input_1', help='input node.')
    ap.add_argument('-o', '--output_node',type=str, default='flatten_1/Reshape', help='output node.') 
    ap.add_argument('-b', '--batchsize',  type=int, default=1, help='Evaluation batchsize. Default is 1.') 
    args = ap.parse_args()

    print('\n'+DIVIDER)
    print('Keras version      : ',tf.keras.__version__)
    print('TensorFlow version : ',tf.__version__)
    print(sys.version)
    print(DIVIDER)
    print(' Command line options:')
    print ('--dataset     : ',args.dataset)
    print ('--graph       : ',args.graph)
    print ('--input_node  : ',args.input_node)
    print ('--output_node : ',args.output_node)
    print ('--batchsize   : ',args.batchsize)
    print(DIVIDER)

    input_graph_def = tf.Graph().as_graph_def()
    input_graph_def.ParseFromString(tf.io.gfile.GFile(args.graph, "rb").read())

    graph_eval(input_graph_def, args.input_node, args.output_node, args.dataset, args.batchsize)


if __name__ == '__main__':
    run_main()

