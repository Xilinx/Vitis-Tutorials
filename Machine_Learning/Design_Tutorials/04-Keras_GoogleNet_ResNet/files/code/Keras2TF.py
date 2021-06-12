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
# Author: Daniele Bagni, Xilinx Inc
# date 6 May 2021

import os, sys, shutil

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

import tensorflow as tf
from tensorflow.keras import backend as K
from tensorflow.keras.models import load_model

import argparse #DB
# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-n",  "--network", default="LeNet",          help="input CNN")
ap.add_argument("-d",  "--dataset", default="fmnist",  help="input dataset")
args = vars(ap.parse_args())
cnn_name = args["network"]
dataset_name = args["dataset"]

if (dataset_name == "cifar10") :
    from config import cifar10_config as cfg #DB
else:
    from config import fashion_mnist_config as cfg #DB


##############################################
# Set up directories
##############################################

KERAS_MODEL_DIR = cfg.KERAS_MODEL_DIR #DB

WEIGHTS_DIR = os.path.join(KERAS_MODEL_DIR, cnn_name)

CHKPT_MODEL_DIR = cfg.CHKPT_MODEL_DIR


# set learning phase for no training: This line must be executed before loading Keras model
K.set_learning_phase(0)

# load weights & architecture into new model
model = load_model(os.path.join(WEIGHTS_DIR,"best_chkpt.hdf5"))

#print the CNN structure
model.summary()

# make list of output node names
output_names=[out.op.name for out in model.outputs]

# set up tensorflow saver object
saver = tf.compat.v1.train.Saver()

# fetch the tensorflow session using the Keras backend
sess = tf.compat.v1.keras.backend.get_session()

## get the tensorflow session graph
#graph_def = sess.graph.as_graph_def()


# Check the input and output name
print ("\n TF input node name:")
print(model.inputs)
print ("\n TF output node name:")
print(model.outputs)

# write out tensorflow checkpoint & inference graph (from MH's "MNIST classification with TensorFlow and Xilinx DNNDK")
save_path = saver.save(sess, os.path.join(CHKPT_MODEL_DIR, cnn_name, "float_model.ckpt"))


print ("\nFINISHED CREATING TF FILES\n")
