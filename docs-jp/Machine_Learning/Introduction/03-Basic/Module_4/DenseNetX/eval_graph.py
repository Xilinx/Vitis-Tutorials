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


import sys
import os
import argparse
import tensorflow as tf
import numpy as np

from progressbar import ProgressBar

# reduce TensorFlow messages in console
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

# workaround for TF1.15 bug "Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR"
os.environ['TF_FORCE_GPU_ALLOW_GROWTH'] = 'true'


from tensorflow.python.platform import gfile
import tensorflow.contrib.decent_q


from datadownload import datadownload


def graph_eval(input_graph_def, graph, input_node, output_node, batchsize):

    input_graph_def.ParseFromString(tf.io.gfile.GFile(graph, "rb").read())

    # CIFAR-10 dataset    
    (_,_), (x_test,y_test) = datadownload()

    total_batches = int(len(x_test)/batchsize)

    tf.import_graph_def(input_graph_def,name = '')

    # Get input placeholders & tensors
    images_in = tf.compat.v1.get_default_graph().get_tensor_by_name(input_node+':0')
    labels = tf.compat.v1.placeholder(tf.int32,shape = [None,10])

    # get output tensors
    logits = tf.compat.v1.get_default_graph().get_tensor_by_name(output_node+':0')
    predicted_logit = tf.argmax(input=logits, axis=1, output_type=tf.int32)
    ground_truth_label = tf.argmax(labels, 1, output_type=tf.int32)

    # Define the metric and update operations
    tf_metric, tf_metric_update = tf.compat.v1.metrics.accuracy(labels=ground_truth_label,
                                                                predictions=predicted_logit,
                                                                name='acc')

    with tf.compat.v1.Session() as sess:
        progress = ProgressBar()
        
        sess.run(tf.compat.v1.initializers.global_variables())
        sess.run(tf.compat.v1.initializers.local_variables())

        # process all batches
        for i in progress(range(0,total_batches)):

            # fetch a batch from validation dataset
            x_batch, y_batch = x_test[i*batchsize:i*batchsize+batchsize], \
                               y_test[i*batchsize:i*batchsize+batchsize]

            # Run graph for accuracy node
            feed_dict={images_in: x_batch, labels: y_batch}
            acc = sess.run(tf_metric_update, feed_dict)

        print ('Graph accuracy with validation dataset: {:1.4f}'.format(acc))

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
    ap.add_argument('-b', '--batchsize',
                    type=int,
                    default=1,
                    help='Evaluation batchsize, must be integer value. Default is 1')  
    ap.add_argument('--gpu',
                    type=str,
                    default='0',
                    help='gpu device id.')

    args = ap.parse_args()  

    print('\n------------------------------------')
    print('TensorFlow version : ',tf.__version__)
    print(sys.version)
    print('------------------------------------')
    print ('Command line options:')
    print (' --graph      : ', args.graph)
    print (' --input_node : ', args.input_node)
    print (' --output_node: ', args.output_node)
    print (' --batchsize  : ', args.batchsize)
    print (' --gpu        : ', args.gpu)
    print('------------------------------------\n')


    os.environ["CUDA_VISIBLE_DEVICES"] = args.gpu
    input_graph_def = tf.Graph().as_graph_def()
    graph_eval(input_graph_def, args.graph, args.input_node, args.output_node, args.batchsize)


if __name__ ==  "__main__":
    main()
