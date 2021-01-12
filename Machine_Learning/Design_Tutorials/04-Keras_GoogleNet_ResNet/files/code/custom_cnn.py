#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''

MIT License

Copyright (c) 2018 Adrian Rosebrock, PyImageSearch.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
'''

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

'''
History:
- All the 4 CNNs of this file are taken from the "Deep Learning for Computer Vision with Python (Starter Bundle and Practitioner Bundle)"" books by Adrian Rosebrock's PyImageSearch.

- I have got written permission from the author - Adrian Rosebrock to freely use (reproduce and modify) them, see file "PyImageSearch_permission.md"

- All CNNs of this file were rewritten to remove the Sequential model, as I need to force the input layer name for the quantization process.

- In some cases I swapped the sequence of layers "CONV-> RELU -> BN" with "CONV-> BN -> RELU"
'''


# import the necessary packages
from tensorflow.keras.models import Model
from tensorflow.keras.layers import Input
from tensorflow.keras.layers import add
from tensorflow.keras.regularizers import l2
from tensorflow.keras.layers import BatchNormalization
from tensorflow.keras.layers import Conv2D
from tensorflow.keras.layers import MaxPooling2D
from tensorflow.keras.layers import AveragePooling2D
from tensorflow.keras.layers import ZeroPadding2D
from tensorflow.keras.layers import Dense
from tensorflow.keras.layers import Activation
from tensorflow.keras.layers import Flatten
from tensorflow.keras.layers import Dropout
from tensorflow.keras.layers import Dense
from tensorflow.keras.layers import concatenate
from tensorflow.keras import backend as K


class ConvNetFactory:
        def __init__(self):
                pass

        @staticmethod
        def build(name, *args, **kargs):
                # define the network (i.e., string => function) mappings
                mappings = {"LeNet"        : ConvNetFactory.LeNet,
                            "miniVggNet"   : ConvNetFactory.miniVggNet,
                            "miniGoogleNet": ConvNetFactory.miniGoogleNet,
                            "miniResNet"   : ConvNetFactory.miniResNet}
                # grab the builder function from the mappings dictionary
                builder = mappings.get(name, None)
                # if the builder is None, then there is not a function that can be used to build to the network, so return None
                if builder is None:
                        return None
                # otherwise, build the network architecture
                return builder(*args, **kargs)

        @staticmethod
        def LeNet(width, height, depth, classes=10, reg=0.0001, bnEps=2e-5, bnMom=0.9, dropout=False, BN=True, FC=True, **kargs):
                # initialize the input shape to be "channels last" and the channels dimension itself
                inputShape = (height, width, depth)
                chanDim = -1
                # if we are using "channels first", update the input shape and channels dimension
                if K.image_data_format() == "channels_first":
                        inputShape = (depth, height, width)
                        chanDim = 1
                inputs = Input(shape=inputShape, name="conv2d_1_input")
                # define the first set of CONV => ACTIVATION => POOL layer
                x = Conv2D(20, (5, 5), padding="same", kernel_regularizer=l2(reg))(inputs)
                if BN:
                        x = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(x)
                x = Activation("relu")(x)
                x = MaxPooling2D(pool_size=(2, 2))(x)
                if dropout:
                        x = Dropout(0.25)(x)

                x = Conv2D(50, (5, 5), padding="same", kernel_regularizer=l2(reg))(inputs)
                if BN:
                        x = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(x)
                x = Activation("relu")(x)
                x = MaxPooling2D(pool_size=(2, 2))(x)
                if dropout:
                        x = Dropout(0.25)(x)
                if FC:
                        #define the first FC => ACTIVATION layers
                        x = Flatten()(x)
                        x = Dense(500, kernel_regularizer=l2(reg))(x)
                        x = Activation("relu")(x)
                        # define the second FC layer
                        x = Dense(classes, kernel_regularizer= l2(reg))(x)
                        # lastly, define the soft-max classifier
                        x = Activation("softmax")(x)
                else: # CONV layers to replace the FC layers
                        kernel_size = (width//2, height//2)
                        x = Conv2D(500, kernel_size)(x) # input data has size 14x14 #DB
                        x = Conv2D(500, (1, 1))(x)
                        x = Activation("relu")(x)
                        x = Conv2D(classes, (1, 1))(x)
                        x = Flatten()(x)
                        x = Activation("softmax")(x)

                # create the model
                model = Model(inputs, x, name="LeNet")
                return model

        @staticmethod
        def miniVggNet(width, height, depth, classes=10, reg=0.0001, bnEps=2e-5, bnMom=0.9,dropout=False, BN=True, FC=True, **kargs):
                # initialize the input shape to be "channels last" and the
                # channels dimension itself
                inputShape = (height, width, depth)
                chanDim = -1

                # if we are using "channels first", update the input shape and channels dimension
                if K.image_data_format() == "channels_first":
                        inputShape = (depth, height, width)
                        chanDim = 1

                # set the inpu			"minivggnet": cnn.MiniVGGNet}t and apply BN
                inputs = Input(shape=inputShape, name="conv2d_1_input")

                # apply a single CONV layer
                x = Conv2D(32, (3, 3), use_bias=False, padding="same", kernel_regularizer=l2(reg))(inputs)
                x = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(x)
                x = Activation("relu")(x)

                x = Conv2D(32, (3, 3), use_bias=False, padding="same", kernel_regularizer=l2(reg))(x)
                x = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(x)
                x = Activation("relu")(x)

                x = MaxPooling2D(pool_size=(2, 2))(x)
                x = Dropout(0.25)(x)

                # apply a single CONV layer
                x = Conv2D(64, (3, 3), use_bias=False, padding="same", kernel_regularizer=l2(reg))(x)
                x = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(x)
                x = Activation("relu")(x)

                x = Conv2D(64, (3, 3), use_bias=False, padding="same", kernel_regularizer=l2(reg))(x)
                x = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(x)
                x = Activation("relu")(x)

                x = MaxPooling2D(pool_size=(2, 2))(x)
                x = Dropout(0.25)(x)

                if FC:
                        x = Flatten()(x)
                        x = Dense(512, kernel_regularizer=l2(reg))(x)
                        x = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(x)
                        x = Activation("relu")(x)
                        x = Dropout(0.50)(x)
                        x = Dense(classes, kernel_regularizer= l2(reg))(x)
                        x = Activation("softmax")(x)
                else: #replace FC layers with CONV layers
                        kernel_size = (width//4, height//4)
                        x = Conv2D(512, kernel_size)(x) # input data has size 7x7 #DB
                        x = Conv2D(512, (1, 1))(x)
                        x = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(x)
                        x = Activation("relu")(x)
                        x = Conv2D(classes, (1, 1))(x)
                        x = Flatten()(x)
                        x = Activation("softmax")(x)


                # create the model
                model = Model(inputs, x, name="miniVggNet")
                return model

        # miniResNet
        @staticmethod
        def residual_module(data, K, stride, chanDim, red=False, reg=0.0001, bnEps=2e-5, bnMom=0.9):
                # the shortcut branch of the ResNet module should be initialized as the input (identity) data
                shortcut = data
                # the first block of the ResNet module are the 1x1 CONVs
                bn1 = BatchNormalization(axis=chanDim, epsilon=bnEps,momentum=bnMom)(data)
                act1 = Activation("relu")(bn1)
                conv1 = Conv2D(int(K * 0.25), (1, 1), use_bias=False, kernel_regularizer=l2(reg))(act1)
                # the second block of the ResNet module are the 3x3 CONVs
                bn2 = BatchNormalization(axis=chanDim, epsilon=bnEps,momentum=bnMom)(conv1)
                act2 = Activation("relu")(bn2)
                conv2 = Conv2D(int(K * 0.25), (3, 3), strides=stride, padding="same", use_bias=False, kernel_regularizer=l2(reg))(act2)
                # the third block of the ResNet module is another set of 1x1 CONVs
                bn3 = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(conv2)
                act3 = Activation("relu")(bn3)
                conv3 = Conv2D(K, (1, 1), use_bias=False, kernel_regularizer=l2(reg))(act3)
                # if we are to reduce the spatial size, apply a CONV layer to the shortcut
                if red:
                        shortcut = Conv2D(K, (1, 1), strides=stride, use_bias=False, kernel_regularizer=l2(reg))(act1)
                # add together the shortcut and the final CONV
                x = add([conv3, shortcut])
                # return the addition as the output of the ResNet module
                return x

        @staticmethod
        def miniResNet(width, height, depth, classes, stages, filters, reg=0.0001, bnEps=2e-5, bnMom=0.9, dataset="cifar"):
                # initialize the input shape to be "channels last" and the channels dimension itself
                inputShape = (height, width, depth)
                chanDim = -1
                # if we are using "channels first", update the input shape and channels dimension
                if K.image_data_format() == "channels_first":
                        inputShape = (depth, height, width)
                        chanDim = 1
                # set the input and apply BN
                inputs = Input(shape=inputShape, name="conv2d_1_input")
                x = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(inputs)
                # check if we are utilizing the CIFAR dataset
                if dataset == "cifar":
                        # apply a single CONV layer
                        x = Conv2D(filters[0], (3, 3), use_bias=False, padding="same", kernel_regularizer=l2(reg))(x)
                # check to see if we are using the Tiny ImageNet dataset
                elif dataset == "tiny_imagenet" :
                        # apply CONV => BN => ACT => POOL to reduce spatial size
                        x = Conv2D(filters[0], (5, 5), use_bias=False, padding="same", kernel_regularizer=l2(reg))(x)
                        x = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(x)
                        x = Activation("relu")(x)
                        x = ZeroPadding2D((1, 1))(x)
                        x = MaxPooling2D((3, 3), strides=(2, 2))(x)
                # loop over the number of stages
                for i in range(0, len(stages)):
                        # initialize the stride, then apply a residual module used to reduce the spatial size of the input volume
                        stride = (1, 1) if i == 0 else (2, 2)
                        x = ConvNetFactory.residual_module(x, filters[i + 1], stride, chanDim, red=True, bnEps=bnEps, bnMom=bnMom)
                        # loop over the number of layers in the stage
                        for j in range(0, stages[i] - 1):
                                # apply a ResNet module
                                x = ConvNetFactory.residual_module(x, filters[i + 1],(1, 1), chanDim, bnEps=bnEps, bnMom=bnMom)
                # apply BN => ACT => POOL
                x = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(x)
                x = Activation("relu")(x)
                x = AveragePooling2D((8, 8))(x)
                # softmax classifier
                x = Flatten()(x)
                x = Dense(classes, kernel_regularizer=l2(reg))(x)
                x = Activation("softmax")(x)
                # create the model
                model = Model(inputs, x, name="resnet")
                # return the constructed network architecture
                return model

        @staticmethod
        def new_residual_module(data, K, stride, chanDim, red=False, reg=0.0001, bnEps=2e-5, bnMom=0.9):
                shortcut = data
                conv1 = Conv2D(int(K * 0.25), (1, 1), use_bias=False, kernel_regularizer=l2(reg))(shortcut)
                bn1 = BatchNormalization(axis=chanDim, epsilon=bnEps,momentum=bnMom)(conv1)
                act2 = Activation("relu")(bn1)
                conv2 = Conv2D(int(K * 0.25), (3, 3), strides=stride, padding="same", use_bias=False, kernel_regularizer=l2(reg))(act2)
                bn2 = BatchNormalization(axis=chanDim, epsilon=bnEps,momentum=bnMom)(conv2)
                act3 = Activation("relu")(bn2)
                conv3 = Conv2D(K, (1, 1), use_bias=False, kernel_regularizer=l2(reg))(act3)
                bn3 = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(conv3)
                if red:
                        shortcut = Conv2D(K, (1, 1), strides=stride, use_bias=False,kernel_regularizer=l2(reg))(data)
                        shortcut = BatchNormalization(axis=chanDim, epsilon=bnEps, momentum=bnMom)(shortcut)
                # add together the shortcut and the final CONV
                x = add([bn3, shortcut])
                x = Activation("relu")(x)
                # return the addition as the output of the ResNet module
                return x


        @staticmethod
        def new_miniResNet(width, height, depth, classes, stages, filters, reg=0.0001, bnEps=2e-5, bnMom=0.9, dataset="cifar"):
                # initialize the input shape to be "channels last" and the channels dimension itself
                inputShape = (height, width, depth)
                chanDim = -1
                # if we are using "channels first", update the input shape and channels dimension
                if K.image_data_format() == "channels_first":
                        inputShape = (depth, height, width)
                        chanDim = 1

                inputs = Input(shape=inputShape, name="conv2d_1_input")
                x = Conv2D(filters[0], (3, 3), use_bias=False, padding="same", kernel_regularizer=l2(reg))(inputs)
                x = BatchNormalization(axis=chanDim, epsilon=bnEps,momentum=bnMom)(x)
                x = Activation("relu")(x)
                # loop over the number of stages
                for i in range(0, len(stages)):
                        # initialize the stride, then apply a residual module
                        # used to reduce the spatial size of the input volume
                        stride = (1, 1) if i == 0 else (2, 2)
                        x = ConvNetFactory.new_residual_module(x, filters[i + 1], stride, chanDim, red=True, bnEps=bnEps, bnMom=bnMom)
                        # loop over the number of layers in the stage
                        for j in range(0, stages[i] - 1):
                                # apply a ResNet module
                                x = ConvNetFactory.new_residual_module(x, filters[i + 1],(1, 1), chanDim, bnEps=bnEps, bnMom=bnMom)
                # apply BN => ACT => POOL
                x = Conv2D(filters[3], (3, 3), use_bias=False, padding="same", kernel_regularizer=l2(reg))(x)
                x = BatchNormalization(axis=chanDim, epsilon=bnEps,momentum=bnMom)(x)
                x = Activation("relu")(x)
                x = AveragePooling2D((8, 8))(x)
                # softmax classifier
                x = Flatten()(x)
                x = Dense(classes, kernel_regularizer=l2(reg))(x)
                x = Activation("softmax")(x)
                # create the model
                model = Model(inputs, x, name="miniResNet")
                # return the constructed network architecture
                return model

        # miniGoogleNet
        @staticmethod
        def conv_module(x, K, kX, kY, stride, chanDim, padding="same"):
                # define a CONV => BN => RELU pattern
                x = Conv2D(K, (kX, kY), strides=stride, padding=padding)(x)
                x = BatchNormalization(axis=chanDim)(x)
                x = Activation("relu")(x)
                # return the block
                return x

        @staticmethod
        def inception_module(x, numK1x1, numK3x3, chanDim):
                # define two CONV modules, then concatenate across the
                # channel dimension
                conv_1x1 = ConvNetFactory.conv_module(x, numK1x1, 1, 1, (1, 1), chanDim)
                conv_3x3 = ConvNetFactory.conv_module(x, numK3x3, 3, 3, (1, 1), chanDim)
                x = concatenate([conv_1x1, conv_3x3], axis=chanDim)
                # return the block
                return x

        @staticmethod
        def downsample_module(x, K, chanDim):
                # define the CONV module and POOL, then concatenate
                # across the channel dimensions
                conv_3x3 = ConvNetFactory.conv_module(x, K, 3, 3, (2, 2), chanDim, padding="valid")
                pool = MaxPooling2D((3, 3), strides=(2, 2))(x)
                x = concatenate([conv_3x3, pool], axis=chanDim)
                # return the block
                return x

        @staticmethod
        def miniGoogleNet(width, height, depth, classes,  FC=True):
                # initialize the input shape to be "channels last" and the
                # channels dimension itself
                inputShape = (height, width, depth)
                chanDim = -1
                # if we are using "channels first", update the input shape
                # and channels dimension
                if K.image_data_format() == "channels_first":
                        inputShape = (depth, height, width)
                        chanDim = 1
                # define the model input and first CONV module
                inputs = Input(shape=inputShape, name="conv2d_1_input")
                x = ConvNetFactory.conv_module(inputs, 96, 3, 3, (1, 1),chanDim)
                # two Inception modules followed by a downsample module
                x = ConvNetFactory.inception_module(x, 32, 32, chanDim)
                x = ConvNetFactory.inception_module(x, 32, 48, chanDim)
                x = ConvNetFactory.downsample_module(x, 80, chanDim)
                # four Inception modules followed by a downsample module
                x = ConvNetFactory.inception_module(x, 112, 48, chanDim)
                x = ConvNetFactory.inception_module(x,  96, 64, chanDim)
                x = ConvNetFactory.inception_module(x,  80, 80, chanDim)
                x = ConvNetFactory.inception_module(x,  48, 96, chanDim)
                x = ConvNetFactory.downsample_module(x, 96, chanDim)
                # two Inception modules followed by global POOL and dropout
                x = ConvNetFactory.inception_module(x, 176, 160, chanDim)
                x = ConvNetFactory.inception_module(x, 176, 160, chanDim)
                x = AveragePooling2D((7, 7))(x)
                x = Dropout(0.5)(x)
                # softmax classifier
                x = Flatten()(x)
                x = Dense(classes)(x)
                x = Activation("softmax")(x)
                # create the model
                model = Model(inputs, x, name="miniGoogleNet")
                # return the constructed network architecture
                return model
