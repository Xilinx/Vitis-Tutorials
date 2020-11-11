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

import os

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

# workaround for TF1.15 bug "Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR"
os.environ['TF_FORCE_GPU_ALLOW_GROWTH'] = 'true'


import tensorflow as tf
tf.compat.v1.logging.set_verbosity(tf.compat.v1.logging.ERROR)


from datadownload import datadownload



def customcnn(cnn_in, is_training):

    print('Network input shape: ',cnn_in.shape)

    net = tf.compat.v1.layers.conv2d(inputs=cnn_in, filters=16, kernel_size=5, strides=2, padding='same')
    net = tf.compat.v1.layers.batch_normalization(inputs=net, training=is_training)
    net = tf.nn.relu(net)
    print('                     ',net.shape)

    net = tf.compat.v1.layers.conv2d(inputs=net, filters=32, kernel_size=5, strides=2, padding='same')
    net = tf.compat.v1.layers.batch_normalization(inputs=net, training=is_training)
    net = tf.nn.relu(net)
    print('                     ',net.shape)

    net = tf.compat.v1.layers.conv2d(inputs=net, filters=64, kernel_size=3, strides=2, padding='same')
    net = tf.compat.v1.layers.batch_normalization(inputs=net, training=is_training)
    net = tf.nn.relu(net)
    print('                     ',net.shape)

    h = net.shape[1]
    w = net.shape[2]
    net = tf.compat.v1.layers.conv2d(inputs=net, filters=10, kernel_size=(h,w), strides=(h,w), padding='valid')
    net = tf.compat.v1.layers.batch_normalization(inputs=net, training=is_training)
    print('                     ',net.shape)
    
    logits = tf.compat.v1.layers.flatten(inputs=net)
    print('Network output shape:',logits.shape)

    return logits

