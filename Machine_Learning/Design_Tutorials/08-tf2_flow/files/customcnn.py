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
Simple custom fully-convolutional CNN
Structure is similar to ResNet, includes skip connections.
No pooling or fully-connected/dense layers
'''

'''
Author: Mark Harvey
'''
import os
os.environ['TF_CPP_MIN_LOG_LEVEL']='3'

from tensorflow.keras import backend as K
from tensorflow.keras.models import Model
from tensorflow.keras import regularizers
from tensorflow.keras.layers import Input, Flatten, Conv2D, Dropout, Add
from tensorflow.keras.layers import BatchNormalization, Activation



def cbr(input,filters,kernel_size,strides):
    '''
    Convolution - BatchNorm - ReLU - Dropout
    '''
    net = Conv2D(filters=filters,kernel_size=kernel_size, \
                 kernel_initializer='he_uniform', kernel_regularizer=regularizers.l2(0.01), \
                 strides=strides, padding='same')(input)
    net = BatchNormalization()(net)
    net = Activation('relu')(net)
    net = Dropout(rate=0.2)(net)
    return net


def skip_blk(input,filters,kernel_size=3,strides=1):
    net = cbr(input=input,filters=filters,kernel_size=kernel_size,strides=strides)
    net = cbr(input=net, filters=filters,kernel_size=kernel_size,strides=strides)
    net = cbr(input=net, filters=filters,kernel_size=kernel_size,strides=strides)
    skip = cbr(input=input, filters=filters,kernel_size=kernel_size,strides=strides)
    net = Add()([skip, net])
    net = cbr(input=net,filters=filters,kernel_size=3,strides=strides*2)
    return net


def customcnn(input_shape=(None, None, None),classes=None, filters=[8,16,32,64,128]):
    '''
    Arguments:
      input_shape: tuple of integers indicating height, width, channels
      classes    : integer to set number of classes
      filters    : list of integers - each list element sets the number of filters used in a skip block
    '''

    input_layer = Input(shape=input_shape)
    net = input_layer

    for f in filters:
        net = skip_blk(net,f)
 
    # reduce channels
    f = int((filters[-1]/2 - classes)/2)
    net = skip_blk(net,f)

    # create a conv layer that will reduce feature maps to (1,1,classes)
    h = K.int_shape(net)[1]
    w = K.int_shape(net)[2]

    net = Conv2D(filters=classes,kernel_size=(h,w),kernel_initializer='he_uniform', \
                 kernel_regularizer=regularizers.l2(0.01),strides=w, padding='valid')(net)

    net = Flatten()(net)

    output_layer = Activation('softmax')(net)

     
    return Model(inputs=input_layer, outputs=output_layer)

