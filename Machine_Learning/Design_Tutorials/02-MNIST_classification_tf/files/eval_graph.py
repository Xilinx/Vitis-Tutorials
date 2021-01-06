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

'''
Author: Mark Harvey
'''

import sys
import os
import argparse
import numpy as np


# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

# workaround for TF1.15 bug "Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR"
os.environ['TF_FORCE_GPU_ALLOW_GROWTH'] = 'true'

import tensorflow as tf
tf.compat.v1.logging.set_verbosity(tf.compat.v1.logging.ERROR)
from tensorflow.python.platform import gfile
from datadownload import datadownload

import tensorflow.contrib.decent_q


def calc_acc(testdata,testlabels,predictions):
    '''
    Accuracy calculation
    '''
    correct_predictions = 0
    wrong_predictions = 0

    for i in range(len(testdata)):
        if (predictions[i] == np.argmax(testlabels[i])):
           correct_predictions += 1
        else:
            wrong_predictions += 1

    # calculate accuracy
    acc = (correct_predictions/len(testdata)) * 100

    return acc




def graph_eval(input_graph_def, graph, input_node, output_node):

    input_graph_def.ParseFromString(tf.io.gfile.GFile(graph, "rb").read())

    # CIFAR-10 dataset    
    _, (x_test,y_test) = datadownload()

    tf.import_graph_def(input_graph_def,name = '')

    # Get input placeholders & tensors
    images_in = tf.compat.v1.get_default_graph().get_tensor_by_name(input_node+':0')
    labels = tf.compat.v1.placeholder(tf.int32, shape=[None,10], name='labels')

    # get output tensors
    logits = tf.compat.v1.get_default_graph().get_tensor_by_name(output_node+':0')
    predicted_logit = tf.compat.v1.argmax(input=logits, axis=1, output_type=tf.int32)

    with tf.compat.v1.Session() as sess:
        
        sess.run(tf.compat.v1.initializers.global_variables())

        # Run graph to get predictions
        pred = sess.run(predicted_logit, feed_dict={images_in: x_test, labels: y_test})
    
    # iterate over the list of predictions and compare to ground truth
    acc = calc_acc(x_test,y_test,pred)

    print('Graph accuracy: {:1.2f}'.format(acc),'%',flush=True)
    
    return



def main():

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()

    ap.add_argument('--graph',
                    type=str,
                    default='./quantize_results/quantize_eval_model.pb',
                    help='graph file (.pb) to be evaluated.')
    ap.add_argument('--input_node',
                    type=str,
                    default='Reshape',
                    help='input node.')
    ap.add_argument('--output_node',
                    type=str,
                    default='dense_1/BiasAdd',
                    help='output node.')
    ap.add_argument('-g', '--gpu',
                    type=str,
                    default='0',
                    help='IDs of GPU cards to be used. Default is 0')
    args = ap.parse_args()  

    print('\n------------------------------------')
    print('Keras version      : ',tf.keras.__version__)
    print('TensorFlow version : ',tf.__version__)
    print(sys.version)
    print('------------------------------------')
    print ('Command line options:')
    print (' --graph      : ', args.graph)
    print (' --input_node : ', args.input_node)
    print (' --output_node: ', args.output_node)
    print (' --gpu        : ', args.gpu)
    print('------------------------------------\n')


    os.environ["CUDA_DEVICE_ORDER"]='PCI_BUS_ID'
    
    # indicate which GPU to use
    os.environ["CUDA_VISIBLE_DEVICES"]=args.gpu

    input_graph_def = tf.Graph().as_graph_def()
    graph_eval(input_graph_def, args.graph, args.input_node, args.output_node)


if __name__ ==  "__main__":
    main()
