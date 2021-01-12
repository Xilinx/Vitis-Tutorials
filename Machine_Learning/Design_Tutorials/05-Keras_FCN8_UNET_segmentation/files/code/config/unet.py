#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Code developed by Harshall Lamba in his post  https://towardsdatascience.com/understanding-semantic-segmentation-with-unet-6be4f42d4b47
under the MIT license https://github.com/hlamba28/UNET-TGS/blob/master/LICENSE
shown again in the following.
'''


'''
MIT License

Copyright (c) 2019 Harshall Lamba

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
/**

* © Copyright (C) 2016-2020 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/
'''

# modified by daniele.bagni@xilinx.com
# date 20 / 11 / 2020

import sys, time, warnings, os

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
## Import usual libraries
import tensorflow as tf
#from tensorflow.keras.backend import set_session
from tensorflow.keras.models import *
from tensorflow.keras.layers import *
from tensorflow.keras.utils import plot_model #DB

warnings.filterwarnings("ignore")


IMAGE_ORDERING =  "channels_last"

#Function to add 2 convolutional layers with the parameters passed to it
def conv2d_block(input_tensor, n_filters, kernel_size = 3, batchnorm = True, activation=True):
    #firt layer
    x = Conv2D(filters = n_filters, kernel_size = (kernel_size, kernel_size),
               kernel_initializer = 'he_normal', padding = 'same', data_format=IMAGE_ORDERING)(input_tensor)
    if batchnorm:
        x = BatchNormalization()(x)
    if activation:
        x = Activation('relu')(x)
    # second layer
    x = Conv2D(filters = n_filters, kernel_size = (kernel_size, kernel_size),
               kernel_initializer = 'he_normal', padding = 'same', data_format=IMAGE_ORDERING)(x)
    if batchnorm:
        x = BatchNormalization()(x)
    if activation:
        x = Activation('relu')(x)

    return x


def UNET_v1( nClasses, input_height, input_width, n_filters=16*4, dropout=0.1,
          batchnorm=True, activation=True):

    ## input_height and width must be devisible by 16 because maxpooling with filter size = (2,2) is operated 4 times,
    ## which makes the input_height and width 2^4 = 16 times smaller
    assert input_height%16 == 0
    assert input_width%16 == 0

    img_input = Input(shape=(input_height,input_width, 3)) ## Assume 224,224,3

    ## Downscaling

    ## Block 1: 64, 64
    c1 = conv2d_block(img_input, n_filters * 1, kernel_size = 3, batchnorm = batchnorm, activation=activation)
    p1 = MaxPooling2D((2, 2))(c1)
    p1 = Dropout(dropout)(p1) # 112x112x64

    ## Block 2: 128, 128
    c2 = conv2d_block(p1, n_filters * 2, kernel_size = 3, batchnorm = batchnorm,  activation=activation)
    p2 = MaxPooling2D((2, 2))(c2)
    p2 = Dropout(dropout)(p2) # 56x56x128

    ## Block 3: 256, 256
    c3 = conv2d_block(p2, n_filters * 4, kernel_size = 3, batchnorm = batchnorm,  activation=activation)
    p3 = MaxPooling2D((2, 2))(c3)
    p3 = Dropout(dropout)(p3) # 28x28x256

    ## Block 4: 512, 512
    c4 = conv2d_block(p3, n_filters * 8, kernel_size = 3, batchnorm = batchnorm,  activation=activation)
    p4 = MaxPooling2D((2, 2))(c4)
    p4 = Dropout(dropout)(p4) # 14x14x512

    ## Block5: 1024, 1024
    c5 = conv2d_block(p4, n_filters = n_filters * 16, kernel_size = 3, batchnorm = batchnorm, activation=activation)
    p5 = Dropout(dropout)(c5) # 14x14x1024

    ## Upscaling

    ## Block6: 512, 512
    up6 = UpSampling2D(size=(2, 2),data_format=IMAGE_ORDERING, interpolation="bilinear")(p5) # 28x28x1024
    u6 = (up6)
    m6 = Concatenate(axis=3)([u6, c4])
    m6 = Dropout(dropout)(m6)
    c6 = conv2d_block(m6, n_filters * 8, kernel_size = 3, batchnorm = batchnorm, activation=activation)

    ## Block7: 256, 256
    up7 = UpSampling2D(size=(2, 2),data_format=IMAGE_ORDERING, interpolation="bilinear")(c6) #56x56x256
    u7 = (up7)
    m7 = Concatenate(axis=3)([u7, c3])
    m7 = Dropout(dropout)(m7)
    c7 = conv2d_block(m7, n_filters * 4, kernel_size = 3, batchnorm = batchnorm, activation=activation)

    ## Block8: 128, 128
    up8 = UpSampling2D(size=(2, 2),data_format=IMAGE_ORDERING, interpolation="bilinear")(c7) # 112x112x128
    u8  = (up8)
    m8  = Concatenate(axis=3)([u8, c2])
    m8  = Dropout(dropout)(m8)
    c8 = conv2d_block(m8, n_filters * 2, kernel_size = 3, batchnorm = batchnorm, activation=activation)

    ## Block9: 64, 64
    up9 = UpSampling2D(size=(2, 2),data_format=IMAGE_ORDERING, interpolation="bilinear")(c8) # 224x224x64
    u9  = (up9)
    m9 = Concatenate(axis=3)([u9, c1])
    m9 = Dropout(dropout)(m9)
    c9 = conv2d_block(m9, n_filters * 1, kernel_size = 3, batchnorm = batchnorm, activation=activation)

    ## Last layers
    c10 = Conv2D(filters=nClasses, kernel_size=3, data_format=IMAGE_ORDERING,
                 activation="relu", padding="same", kernel_initializer="he_normal")(c9)
    c11 = Conv2D(filters=nClasses, kernel_size=1, data_format=IMAGE_ORDERING, activation="sigmoid")(c10)

    model = Model(inputs=img_input, outputs=c10)

    return model




# like UNET_v1 but with a Conv2D layer betweeb UpSampling2D aand Concatenate layers
def UNET_v2( nClasses, input_height, input_width, n_filters=16*4, dropout=0.1,
          batchnorm=True, activation=True):

    ## input_height and width must be devisible by 16 because maxpooling with filter size = (2,2) is operated 4 times,
    ## which makes the input_height and width 2^4 = 16 times smaller
    assert input_height%16 == 0
    assert input_width%16 == 0

    img_input = Input(shape=(input_height,input_width, 3)) ## Assume 224,224,3

    ## Downscaling

    ## Block 1: 64, 64
    c1 = conv2d_block(img_input, n_filters * 1, kernel_size = 3, batchnorm = batchnorm, activation=activation)
    p1 = MaxPooling2D((2, 2))(c1)
    p1 = Dropout(dropout)(p1) # 112x112x64

    ## Block 2: 128, 128
    c2 = conv2d_block(p1, n_filters * 2, kernel_size = 3, batchnorm = batchnorm,  activation=activation)
    p2 = MaxPooling2D((2, 2))(c2)
    p2 = Dropout(dropout)(p2) # 56x56x128

    ## Block 3: 256, 256
    c3 = conv2d_block(p2, n_filters * 4, kernel_size = 3, batchnorm = batchnorm,  activation=activation)
    p3 = MaxPooling2D((2, 2))(c3)
    p3 = Dropout(dropout)(p3) # 28x28x256

    ## Block 4: 512, 512
    c4 = conv2d_block(p3, n_filters * 8, kernel_size = 3, batchnorm = batchnorm,  activation=activation)
    p4 = MaxPooling2D((2, 2))(c4)
    p4 = Dropout(dropout)(p4) # 14x14x512

    ## Block5: 1024, 1024
    c5 = conv2d_block(p4, n_filters = n_filters * 16, kernel_size = 3, batchnorm = batchnorm, activation=activation)
    p5 = Dropout(dropout)(c5) # 14x14x1024

    ## Upscaling

    ## Block6: 512, 512
    up6 = UpSampling2D(size=(2, 2),data_format=IMAGE_ORDERING, interpolation="bilinear")(p5) # 28x28x1024
    u6 = Conv2D(filters=n_filters*8, kernel_size=2, data_format=IMAGE_ORDERING,
                activation="relu", padding="same", kernel_initializer="he_normal")(up6)
    m6 = Concatenate(axis=3)([u6, c4])
    m6 = Dropout(dropout)(m6)
    c6 = conv2d_block(m6, n_filters * 8, kernel_size = 3, batchnorm = batchnorm, activation=activation)

    ## Block7: 256, 256
    up7 = UpSampling2D(size=(2, 2),data_format=IMAGE_ORDERING, interpolation="bilinear")(c6) #56x56x256
    u7 = Conv2D(filters=n_filters*4, kernel_size=2, data_format=IMAGE_ORDERING,
                activation="relu", padding="same", kernel_initializer="he_normal")(up7)
    m7 = Concatenate(axis=3)([u7, c3])
    m7 = Dropout(dropout)(m7)
    c7 = conv2d_block(m7, n_filters * 4, kernel_size = 3, batchnorm = batchnorm, activation=activation)

    ## Block8: 128, 128
    up8 = UpSampling2D(size=(2, 2),data_format=IMAGE_ORDERING, interpolation="bilinear")(c7) # 112x112x128
    u8  = Conv2D(filters=n_filters*2, kernel_size=2, data_format=IMAGE_ORDERING,
                activation="relu", padding="same", kernel_initializer="he_normal")(up8)
    m8  = Concatenate(axis=3)([u8, c2])
    m8  = Dropout(dropout)(m8)
    c8 = conv2d_block(m8, n_filters * 2, kernel_size = 3, batchnorm = batchnorm, activation=activation)

    ## Block9: 64, 64
    up9 = UpSampling2D(size=(2, 2),data_format=IMAGE_ORDERING, interpolation="bilinear")(c8) # 224x224x64
    u9  = Conv2D(filters=n_filters*1, kernel_size=2, data_format=IMAGE_ORDERING,
                activation="relu", padding="same", kernel_initializer="he_normal")(up9)
    m9 = Concatenate(axis=3)([u9, c1])
    m9 = Dropout(dropout)(m9)
    c9 = conv2d_block(m9, n_filters * 1, kernel_size = 3, batchnorm = batchnorm, activation=activation)

    ## Last layers
    c10 = Conv2D(filters=nClasses, kernel_size=3, data_format=IMAGE_ORDERING,
                 activation="relu", padding="same", kernel_initializer="he_normal")(c9)
    c11 = Conv2D(filters=nClasses, kernel_size=1, data_format=IMAGE_ORDERING, activation="sigmoid")(c10)

    model = Model(inputs=img_input, outputs=c10)

    return model


# like UNET_v2 but with a UpSampling2D replaced by Conv2D_transpose layer
def UNET_v3( nClasses, input_height, input_width, n_filters=16*4, dropout=0.1,
          batchnorm=True, activation=True):

    ## input_height and width must be devisible by 16 because maxpooling with filter size = (2,2) is operated 4 times,
    ## which makes the input_height and width 2^4 = 16 times smaller
    assert input_height%16 == 0
    assert input_width%16 == 0

    img_input = Input(shape=(input_height,input_width, 3)) ## Assume 224,224,3

    ## Downscaling

    ## Block 1: 64, 64
    c1 = conv2d_block(img_input, n_filters * 1, kernel_size = 3, batchnorm = batchnorm, activation=activation)
    p1 = MaxPooling2D((2, 2))(c1)
    p1 = Dropout(dropout)(p1) # 112x112x64

    ## Block 2: 128, 128
    c2 = conv2d_block(p1, n_filters * 2, kernel_size = 3, batchnorm = batchnorm,  activation=activation)
    p2 = MaxPooling2D((2, 2))(c2)
    p2 = Dropout(dropout)(p2) # 56x56x128

    ## Block 3: 256, 256
    c3 = conv2d_block(p2, n_filters * 4, kernel_size = 3, batchnorm = batchnorm,  activation=activation)
    p3 = MaxPooling2D((2, 2))(c3)
    p3 = Dropout(dropout)(p3) # 28x28x256

    ## Block 4: 512, 512
    c4 = conv2d_block(p3, n_filters * 8, kernel_size = 3, batchnorm = batchnorm,  activation=activation)
    p4 = MaxPooling2D((2, 2))(c4)
    p4 = Dropout(dropout)(p4) # 14x14x512

    ## Block5: 1024, 1024
    c5 = conv2d_block(p4, n_filters = n_filters * 16, kernel_size = 3, batchnorm = batchnorm, activation=activation)
    p5 = Dropout(dropout)(c5) # 14x14x1024

    ## Upscaling

    ## Block6: 512, 512
    #up6 = UpSampling2D(size=(2, 2),data_format=IMAGE_ORDERING, interpolation="bilinear")(p5) # 28x28x1024
    up6  = Conv2DTranspose( n_filters*8, kernel_size=(3,3), strides=(2,2),padding="same", data_format=IMAGE_ORDERING )(p5)
    #u6 = Conv2D(filters=n_filters*8, kernel_size=2, data_format=IMAGE_ORDERING, activation="relu", padding="same", kernel_initializer="he_normal")(up6)
    u6 = up6
    m6 = Concatenate(axis=3)([u6, c4])
    m6 = Dropout(dropout)(m6)
    c6 = conv2d_block(m6, n_filters * 8, kernel_size = 3, batchnorm = batchnorm, activation=activation)

    ## Block7: 256, 256
    #up7 = UpSampling2D(size=(2, 2),data_format=IMAGE_ORDERING, interpolation="bilinear")(c6) #56x56x256
    up7 = Conv2DTranspose( n_filters*4, kernel_size=(3,3), strides=(2,2),padding="same", data_format=IMAGE_ORDERING )(c6)
    #u7 = Conv2D(filters=n_filters*4, kernel_size=2, data_format=IMAGE_ORDERING, activation="relu", padding="same", kernel_initializer="he_normal")(up7)
    u7 = up7
    m7 = Concatenate(axis=3)([u7, c3])
    m7 = Dropout(dropout)(m7)
    c7 = conv2d_block(m7, n_filters * 4, kernel_size = 3, batchnorm = batchnorm, activation=activation)

    ## Block8: 128, 128
    #up8 = UpSampling2D(size=(2, 2),data_format=IMAGE_ORDERING, interpolation="bilinear")(c7) # 112x112x128
    up8 = Conv2DTranspose( n_filters*2, kernel_size=(3,3), strides=(2,2),padding="same", data_format=IMAGE_ORDERING )(c7)
    #u8  = Conv2D(filters=n_filters*2, kernel_size=2, data_format=IMAGE_ORDERING, activation="relu", padding="same", kernel_initializer="he_normal")(up8)
    u8 = up8
    m8  = Concatenate(axis=3)([u8, c2])
    m8  = Dropout(dropout)(m8)
    c8 = conv2d_block(m8, n_filters * 2, kernel_size = 3, batchnorm = batchnorm, activation=activation)

    ## Block9: 64, 64
    #up9 = UpSampling2D(size=(2, 2),data_format=IMAGE_ORDERING, interpolation="bilinear")(c8) # 224x224x64
    up9 = Conv2DTranspose( n_filters*1, kernel_size=(3,3), strides=(2,2),padding="same", data_format=IMAGE_ORDERING )(c8)
    #u9  = Conv2D(filters=n_filters*1, kernel_size=2, data_format=IMAGE_ORDERING, activation="relu", padding="same", kernel_initializer="he_normal")(up9)
    u9 = up9
    m9 = Concatenate(axis=3)([u9, c1])
    m9 = Dropout(dropout)(m9)
    c9 = conv2d_block(m9, n_filters * 1, kernel_size = 3, batchnorm = batchnorm, activation=activation)

    ## Last layers
    c10 = Conv2D(filters=nClasses, kernel_size=3, data_format=IMAGE_ORDERING,
                 activation="relu", padding="same", kernel_initializer="he_normal")(c9)
    c11 = Conv2D(filters=nClasses, kernel_size=1, data_format=IMAGE_ORDERING, activation="sigmoid")(c10)

    model = Model(inputs=img_input, outputs=c10)

    return model
