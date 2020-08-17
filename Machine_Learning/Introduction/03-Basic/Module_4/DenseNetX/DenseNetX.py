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
DenseNetX is an implementation of the DenseNet-BC architecture
specifically for Xilinx DPU.

Modified for use on Xilinx DPU:
    - ordering of conv2d, bn & relu set for DPU optimization


Reference: Densely Connected Convolutional Networks
         : https://arxiv.org/abs/1608.06993
'''

from tensorflow.keras import backend as K
from tensorflow.keras.models import Model
from tensorflow.keras.regularizers import l2
from tensorflow.keras.layers import Conv2D,BatchNormalization,Dense,Dropout,MaxPooling2D,Input
from tensorflow.keras.layers import Activation,Concatenate,AveragePooling2D,GlobalAveragePooling2D



def conv_block(net_in, k, drop_rate, weight_decay):
    '''
    Used in dense_block
    Includes Bottleneck layer
    '''
    # 1x1 convolution
    net = Conv2D(4 * k, 1, use_bias=False, kernel_initializer='he_uniform',kernel_regularizer=l2(weight_decay))(net_in)
    net = BatchNormalization()(net)
    net = Activation('relu')(net)
    net = Dropout(rate=drop_rate)(net)

    # 3x3 convolution
    net = Conv2D(k, 3, use_bias=False, kernel_initializer='he_uniform',kernel_regularizer=l2(weight_decay),padding='same')(net)
    net = BatchNormalization()(net)
    net = Activation('relu')(net)
    net = Dropout(rate=drop_rate)(net)

    # concatenate
    net = Concatenate()([net_in, net])
    return net




def dense_block(net_in, num_blocks,k,drop_rate,weight_decay):
    '''
    Dense block
    '''
    net = net_in
    for _ in range(0,num_blocks):
        net = conv_block(net,k,drop_rate,weight_decay)
    return net



def transition_layer(net_in,theta,drop_rate,weight_decay):
    '''
    Transition layer with compression
    '''

    chan =  K.int_shape(net_in)[-1] 

    # 1 x 1 convolution
    net = Conv2D(int(chan*theta),1,use_bias=False,kernel_initializer='he_uniform',kernel_regularizer=l2(weight_decay))(net_in)
    net = BatchNormalization()(net)
    net = Activation('relu')(net)
    net = Dropout(rate=drop_rate)(net)

    # 2 x 2 average pooling
    net = AveragePooling2D(2, 2)(net)
    return net



def densenetx(input_shape=(224,224,3),classes=1000,k=32,drop_rate=0.2,theta=0.5,weight_decay=1e-4,convlayers=[6,12,24,16]):

    '''
    input_shape : Must be a Python tuple (height, width, channels), default is (224,224,3)
    classes     : number of classes, default is 1000
    k           : Growth rate, default is 32
    drop_rate   : dropout rate for Dropout layers. Default is 0.2
    theta       : Compression factor. Float value, must be > 0 and <= 1.0 Default is 0.5
    weight_decay: Penalty factor for L2 kernel regularizer for convolution layers, Default = 1e-4
    convlayers  : Number of 1x1 and 3x3 conv layers in each dense block.
                : Must be declared as a list with length = number of required denseblks
                : Default is [6,12,24,16] which produces 4 dense blocks and 3 transition layers.
    '''


    # error checking
    if theta <= 0.0 or theta > 1.0:
        raise Exception('Compression factor must be > 0 and <= 1.0')
    if type(input_shape) is not tuple:
        raise Exception('input_shape must be a Python tuple (height,width,channels).')
    if type(convlayers) is not list:
        raise Exception('convlayers must be a Python list.')

    input_layer = Input(shape=input_shape)


    # Use this for CIFAR-10, CIFAR-100
    # first convolutional layer + BN + ReLU (Imagenet style)
    net = Conv2D((2*k),3,strides=1,use_bias=False,kernel_initializer='he_uniform',kernel_regularizer=l2(weight_decay),padding='same')(input_layer)
    net = BatchNormalization()(net)
    net = Activation('relu')(net)


    ''' 
    # Use this for IMAGENET
    # first convolutional layer + BN + ReLU
    net = Conv2D((2*k), 7, strides=2, use_bias=False)(input_layer)
    net = BatchNormalization()(net)
    net = Activation('relu')(net)

    # max pooling layer
    net = MaxPooling2D(3, 2)(net)
    '''

    # dense blocks & transition layers
    for i in range(0,len(convlayers)-1):
        net = dense_block(net, convlayers[i],k,drop_rate,weight_decay)
        net = transition_layer(net,theta,drop_rate,weight_decay)
    net = dense_block(net, convlayers[-1],k,drop_rate,weight_decay)

    # Global average Pooling 2D
    net = GlobalAveragePooling2D()(net)
    net = Dense(classes, kernel_initializer='he_normal')(net)
    output_layer = Activation('softmax')(net)
    
    return Model(inputs=input_layer, outputs=output_layer)

