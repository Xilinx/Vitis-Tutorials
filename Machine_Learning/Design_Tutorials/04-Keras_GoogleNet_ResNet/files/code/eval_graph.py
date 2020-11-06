#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
## © Copyright (C) 2016-2020 Xilinx, Inc
##
## Licensed under the Apache License, Version 2.0 (the "License"). You may
## not use this file except in compliance with the License. A copy of the
## License is located at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
## WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
## License for the specific language governing permissions and limitations
## under the License.
'''


##################################################################
# Evaluation of frozen/quantized graph
#################################################################

'''
TESTED WITH PYTHON 3.6

Author: Mark Harvey (mark.harvey@xilinx.com)
Date:   28 May 2019

Modified by Daniele Bagni (daniele.bagni@xilinx.com)
Date:   27 Aug 2019
'''

import os
import sys
import glob
import argparse
import shutil
import tensorflow as tf
import numpy as np
import cv2
import gc # memory garbage collector #DB

import tensorflow.contrib.decent_q
from tensorflow.python.platform import gfile

from config import fashion_mnist_config as cfg #DB

#DB
DATAS_DIR = cfg.DATASET_DIR
TEST_DIR  = os.path.join(DATAS_DIR, "test")
print("\n eval_graph.py runs from ", DATAS_DIR)


def graph_eval(input_graph_def, input_node, output_node):

    #Reading image paths
    test_img_paths = [img_path for img_path in glob.glob(TEST_DIR+"/*/*.png")]
    NUMEL = len(test_img_paths)
    assert (NUMEL  > 0 )

    y_test= np.zeros((NUMEL,1),      dtype="uint8")
    x_test= np.zeros((NUMEL,cfg.IMAGE_HEIGHT,cfg.IMAGE_WIDTH,3),dtype="uint8")

    i = 0
    for img_path in test_img_paths:
        img = cv2.imread(img_path, cv2.IMREAD_COLOR)

        filename = os.path.basename(img_path)
        class_name = filename.split("_")[0]
        label = cfg.labelNames_dict[class_name]

        #print("filename: ", img_path)
        #print("classname: ", class_name)

        x_test[i] = img
        y_test[i] = int(label)
        i = i + 1

    '''
    #normalize
    x_test = x_test.astype(np.float32)
    x_test = x_test/cfg.NORM_FACTOR
    x_test = x_test -0.5
    x_test = x_test *2
    '''
    x_test = cfg.Normalize(x_test)
    #print(x_test[0])

    #collect garbage to save memory #DB
    #del img
    #del test_img_paths
    #del img_path
    #gc.collect()

    x_test = np.reshape(x_test, [-1, cfg.IMAGE_HEIGHT,cfg.IMAGE_WIDTH, 3])
    y_test = tf.keras.utils.to_categorical(y_test, num_classes=cfg.NUM_CLASSES)

    tf.import_graph_def(input_graph_def,name = '')

    # Get input placeholders & tensors
    images_in = tf.get_default_graph().get_tensor_by_name(input_node+':0')
    labels = tf.placeholder(tf.int32,shape = [None,cfg.NUM_CLASSES])

    # get output tensors
    logits = tf.get_default_graph().get_tensor_by_name(output_node+':0')

    # top 5 and top 1 accuracy
    in_top5 = tf.nn.in_top_k(predictions=logits, targets=tf.argmax(labels, 1), k=5)
    in_top1 = tf.nn.in_top_k(predictions=logits, targets=tf.argmax(labels, 1), k=1)
    top5_acc = tf.reduce_mean(tf.cast(in_top5, tf.float32))
    top1_acc = tf.reduce_mean(tf.cast(in_top1, tf.float32))

    # Create the Computational graph
    with tf.Session() as sess:

        sess.run(tf.initializers.global_variables())

        feed_dict={images_in: x_test, labels: y_test}
        t5_acc,t1_acc = sess.run([top5_acc,top1_acc], feed_dict)
        #print(dir(x_test))
        #print(max(x_test[0]))
        #print(min(x_test[0]))
        print (' Top 1 accuracy with test dataset: {:1.4f}'.format(t1_acc))
        print (' Top 5 accuracy with test dataset: {:1.4f}'.format(t5_acc))

    print ('FINISHED!')
    return


def main(unused_argv):
    os.environ["CUDA_VISIBLE_DEVICES"] = FLAGS.gpu
    input_graph_def = tf.Graph().as_graph_def()
    input_graph_def.ParseFromString(tf.gfile.GFile(FLAGS.graph, "rb").read())
    graph_eval(input_graph_def, FLAGS.input_node, FLAGS.output_node)


if __name__ ==  "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--graph', type=str,
                        default='./freeze/frozen_graph.pb',
                        help='graph file (.pb) to be evaluated.')
    parser.add_argument('--input_node', type=str,
                        default='images_in',
                        help='input node.')
    parser.add_argument('--output_node', type=str,
                        default='dense_1/BiasAdd',
                        help='output node.')
    parser.add_argument('--class_num', type=int,
                        default=cfg.NUM_CLASSES,
                        help='number of classes.')
    parser.add_argument('--gpu', type=str,
                        default='0',
                        help='gpu device id.')
    FLAGS, unparsed = parser.parse_known_args()
    tf.app.run(main=main, argv=[sys.argv[0]] + unparsed)
