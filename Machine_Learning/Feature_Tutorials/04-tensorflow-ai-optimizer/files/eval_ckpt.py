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


import os
import argparse
import sys
import numpy as np


# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

# workaround for TF1.15 bug "Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR"
os.environ['TF_FORCE_GPU_ALLOW_GROWTH'] = 'true'


import tensorflow as tf
tf.compat.v1.logging.set_verbosity(tf.compat.v1.logging.ERROR)

from AlexNet import alexnet

try:
    import tensorflow.contrib.decent_q
    print('Imported tensorflow.contrib.decent_q')
except ImportError:
    print('Did not import tensorflow.contrib.decent_q')



def eval_ckpt(dataset_dir,input_ckpt,batchsize):

  # unpack dataset
  npz_file = np.load(os.path.join(dataset_dir,'testData.npz'))
  x_test = npz_file['x']
  y_test = npz_file['y']

  test_batches = int(len(x_test)/batchsize)
   
  # define placeholders for the input images, labels, training mode and droput rate
  images_in = tf.compat.v1.placeholder(tf.float32, shape=[None,224,224,3], name='images_in')
  labels = tf.compat.v1.placeholder(tf.uint8, shape=[None,2], name='labels')

  # build the network, input comes from the 'images_in' placeholder
  logits = alexnet(images_in,classes=2,drop_rate=0.0,is_training=False)

  predicted_logit = tf.argmax(input=logits, axis=1, output_type=tf.int32,name='predicted_logit')
  ground_truth = tf.compat.v1.argmax(labels, 1, output_type=tf.int32, name='ground_truth')
  
  # Define the metric and update operations
  tf_acc, tf_acc_update = tf.compat.v1.metrics.accuracy(ground_truth, predicted_logit, name='accuracy')
  acc_var = tf.compat.v1.get_collection(tf.compat.v1.GraphKeys.LOCAL_VARIABLES, scope="accuracy")
  acc_var_initializer = tf.compat.v1.variables_initializer(var_list=acc_var)


  # create saver object
  saver = tf.compat.v1.train.Saver()

  with tf.compat.v1.Session() as sess:

    sess.run(tf.compat.v1.initializers.global_variables())
    sess.run(acc_var_initializer)

    # restore checkpoint to be evaluated
    saver.restore(sess, input_ckpt)
 
    for i in range(test_batches):
        
      # fetch a batch from test dataset
      x_batch, y_batch = x_test[i*batchsize:(i+1)*batchsize], y_test[i*batchsize:(i+1)*batchsize]
  
      # Run graph for accuracy node
      sess.run(tf_acc_update, feed_dict={images_in: x_batch, labels: y_batch})
   
    score = sess.run(tf_acc)
    print ('Checkpoint accuracy with test dataset:', score)
    
  return


def main():

  # construct the argument parser and parse the arguments
  ap = argparse.ArgumentParser()  
  ap.add_argument('-d', '--dataset_dir', type=str, default='./dataset', help='path to dataset root folder')
  ap.add_argument('-i', '--input_ckpt',  type=str, default='input.ckpt', help='Path of input checkpoint. Default is input.ckpt') 
  ap.add_argument('-b', '--batchsize',   type=int, default=1,            help='Evaluation batchsize, must be integer value. Default is 1')  
  ap.add_argument('-g', '--gpu',         type=str, default='0',          help='String value to select which CUDA GPU devices to use')
  args = ap.parse_args()

  print('\n------------------------------------')
  print('TensorFlow version : ',tf.__version__)
  print(sys.version)
  print('------------------------------------')
  print ('Command line options:')
  print (' --dataset_dir : ',args.dataset_dir)
  print (' --input_ckpt  : ',args.input_ckpt)
  print (' --batchsize   : ',args.batchsize)
  print (' --gpu         : ',args.gpu)
  print('------------------------------------\n')

  os.environ["CUDA_DEVICE_ORDER"]='PCI_BUS_ID'
    
  # indicate which GPU to use
  os.environ["CUDA_VISIBLE_DEVICES"]=args.gpu


  eval_ckpt(args.dataset_dir,args.input_ckpt,args.batchsize)


if __name__ == '__main__':
  main()

