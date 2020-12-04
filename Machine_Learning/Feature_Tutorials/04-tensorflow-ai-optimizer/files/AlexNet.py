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
AlexNet modified for use on Xilinx DPU:
    - ordering of conv2d, bn & relu set for DPU optimization
'''

'''
Author: Mark Harvey, Xilinx Inc
'''



from random import randint, seed
import os
import numpy as np

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

import tensorflow as tf
tf.compat.v1.logging.set_verbosity(tf.compat.v1.logging.ERROR)




def alexnet(net_in,classes=1000,drop_rate=0.5,is_training=True):
  '''
  net_in      : input tensor
  classes     : number of classes, default is 1000
  drop_rate   : dropout rate for Dropout layers. Default is 0.5
  is_training : enable update of dropout and batchnorm layers during training. Default is True
  '''

  net = tf.compat.v1.layers.conv2d(inputs=net_in, filters=96, kernel_size=(11,11),strides=(4,4))
  net = tf.compat.v1.layers.batch_normalization(inputs=net, fused=True, training=is_training)
  net = tf.nn.relu(net)
  net = tf.compat.v1.layers.max_pooling2d(inputs=net, pool_size=(3,3), strides=(2,2))

  net = tf.compat.v1.layers.conv2d(inputs=net, filters=256, kernel_size=(5,5),strides=(1,1),padding='same')
  net = tf.compat.v1.layers.batch_normalization(inputs=net, fused=True, training=is_training)
  net = tf.nn.relu(net)
  net = tf.compat.v1.layers.max_pooling2d(inputs=net, pool_size=(3,3), strides=(2,2))

  net = tf.compat.v1.layers.conv2d(inputs=net, filters=384, kernel_size=(3,3),strides=(1,1),padding='same')
  net = tf.compat.v1.layers.batch_normalization(inputs=net, fused=True, training=is_training)
  net = tf.nn.relu(net)

  net = tf.compat.v1.layers.conv2d(inputs=net, filters=384, kernel_size=(1,1),strides=(1,1),padding='same')
  net = tf.compat.v1.layers.batch_normalization(inputs=net, fused=True, training=is_training)
  net = tf.nn.relu(net)

  net = tf.compat.v1.layers.conv2d(inputs=net, filters=256, kernel_size=(1,1),strides=(1,1),padding='same')
  net = tf.compat.v1.layers.batch_normalization(inputs=net, fused=True, training=is_training)
  net = tf.nn.relu(net)
  net = tf.compat.v1.layers.max_pooling2d(inputs=net, pool_size=(3,3), strides=(2,2))

  net = tf.layers.flatten(inputs=net)
  net = tf.layers.dense(inputs=net, units=4096, activation=None)
  net = tf.nn.relu(net)
  net = tf.compat.v1.layers.dropout(inputs=net, rate=drop_rate, training=is_training)
  net = tf.layers.dense(inputs=net, units=4096, activation=None)
  net = tf.nn.relu(net)
  net = tf.compat.v1.layers.dropout(inputs=net, rate=drop_rate, training=is_training)

  logits = tf.layers.dense(inputs=net, units=classes, activation=None)


  return logits



def model_fn():

  dataset_dir = './dataset'
  batchsize = 400

  npz_file = np.load(os.path.join(dataset_dir,'testData.npz'))
  x_test = npz_file['x']
  y_test = npz_file['y']

  # generate a batch of images from test set
  # starting from a random index
  seed(127)
  start=randint(0,x_test.shape[0]-batchsize)
  x_test_batch = x_test[start:start+batchsize] 
  y_test_batch = y_test[start:start+batchsize]
  
  # instantiate the network
  logits = alexnet(net_in=tf.convert_to_tensor(x_test_batch),classes=2,drop_rate=0.0,is_training=False)

  # calculate accuracy for the batch
  predictions = tf.argmax(logits, 1)
  labels = tf.argmax(y_test_batch, 1)
  eval_metric_ops = {'accuracy': tf.metrics.accuracy(labels, predictions) }

  return eval_metric_ops

