'''
 Copyright 2021 Xilinx Inc.
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
Pretrained MobileNet with added classifier head.
'''

'''
Author: Mark Harvey
'''

import os
# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL']='3'

import tensorflow as tf

from tensorflow.keras.models import Model
from tensorflow.keras.layers import Dense,Dropout,Input
from tensorflow.keras.applications.mobilenet import MobileNet


def xfer_model(input_shape=(224,224,3),classes=10):

  # Base model is MobileNet with Imagenet weights
  # Classifier layers are removed
  base_model = MobileNet(input_shape=input_shape,
                         alpha=1.0,
                         depth_multiplier=1,
                         include_top=False,
                         weights='imagenet',
                         pooling='avg')
  
  # Freeze the base_model
  base_model.trainable = False

  input_layer = base_model.input
  net = base_model.output

  # add output classifier head
  net = Dropout(0.001)(net)
  output_layer = Dense(classes)(net)

  return Model(input_layer, output_layer)

