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
import sys
import argparse
from google.protobuf import text_format
from AlexNet import alexnet


# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

# workaround for TF1.15 bug "Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR"
os.environ['TF_FORCE_GPU_ALLOW_GROWTH'] = 'true'


import tensorflow as tf
tf.compat.v1.logging.set_verbosity(tf.compat.v1.logging.ERROR)

from tensorflow.python.platform import gfile


def export(output_file, input_nodes):
  
  with tf.Graph().as_default() as graph:
    input_tensor = tf.compat.v1.placeholder(name=input_nodes,
                                            dtype=tf.float32,
                                            shape=[None,224,224,3])
    network = alexnet(net_in=input_tensor,classes=2,drop_rate=0.0,is_training=False)

    graph_def = graph.as_graph_def()

    with gfile.GFile(output_file, 'w') as f:
      f.write(text_format.MessageToString(graph_def))

  return


def main():
  # construct the argument parser and parse the arguments
  ap = argparse.ArgumentParser()  
  ap.add_argument('-o', '--output_file', type=str, default='', help='Name of graph file. Default is inference_graph.pbtxt')
  ap.add_argument('-i', '--input_nodes', type=str, default='', help='Names of graph input nodes. Default is empty string') 
  args = ap.parse_args()

  print('\n------------------------------------')
  print('TensorFlow version : ',tf.__version__)
  print(sys.version)
  print('------------------------------------')    
  print ('Command line options:')
  print (' --output_file : ', args.output_file)
  print (' --input_nodes : ', args.input_nodes)
  print('------------------------------------\n')
    
  export(args.output_file, args.input_nodes)

if __name__ == '__main__':
  main()
