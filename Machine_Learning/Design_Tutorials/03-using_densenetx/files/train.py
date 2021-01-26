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
Trains the DenseNetX model on the CIFAR-10 dataset
'''

'''
Author: Mark Harvey, Xilinx Inc
'''



import numpy as np
import os
import sys
import argparse

from datadownload import datadownload

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

# workaround for TF1.15 bug "Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR"
os.environ['TF_FORCE_GPU_ALLOW_GROWTH'] = 'true'

import tensorflow as tf
from tensorflow.keras.optimizers import RMSprop, SGD
from tensorflow.keras.callbacks import ModelCheckpoint, TensorBoard, LearningRateScheduler, ReduceLROnPlateau
from tensorflow.keras.preprocessing.image import ImageDataGenerator

from DenseNetX import densenetx


DIVIDER = '-----------------------------------------'

def train(input_height,input_width,input_chan,batchsize,learnrate,epochs,keras_hdf5,tboard):
    
    def step_decay(epoch):
        """
        Learning rate scheduler used by callback
        Reduces learning rate depending on number of epochs
        """
        lr = learnrate
        if epoch > 150:
            lr /= 1000
        elif epoch > 120:
            lr /= 100
        elif epoch > 80:
            lr /= 10
        elif epoch > 20:
            lr /= 2
        return lr
    

    # CIFAR10 dataset has 60k images. Training set is 50k, test set is 10k.
    # Each image is 32x32x8bits
    (x_train, y_train), (x_test, y_test) = datadownload()
    print ('Dataset downloaded and pre-processed')

    model = densenetx(input_shape=(input_height,input_width,input_chan),classes=10,theta=0.5,drop_rate=0.2,k=12,convlayers=[16,16,16])


    # prints a layer-by-layer summary of the network
    print('\n'+DIVIDER)
    print(' Model Summary')
    print(DIVIDER)
    print(model.summary())
    print("Model Inputs: {ips}".format(ips=(model.inputs)))
    print("Model Outputs: {ops}".format(ops=(model.outputs)))
    

    '''
    Callbacks
    '''
    chkpt_call = ModelCheckpoint(filepath=keras_hdf5,
                                 monitor='val_acc',
                                 verbose=1,
                                 save_best_only=True)

    tb_call = TensorBoard(log_dir=tboard,
                          batch_size=batchsize,
                          update_freq='epoch')


    lr_scheduler_call = LearningRateScheduler(schedule=step_decay,
                                              verbose=1)

    lr_plateau_call = ReduceLROnPlateau(factor=np.sqrt(0.1),
                                        cooldown=0,
                                        patience=5,
                                        min_lr=0.5e-6)

    callbacks_list = [tb_call, lr_scheduler_call, lr_plateau_call, chkpt_call]



    '''
    Input image pipeline for training, validation
    
     data augmentation for training
       - random rotation
       - random horiz flip
       - random linear shift up and down
    '''
    data_augment = ImageDataGenerator(rotation_range=10,
                                      horizontal_flip=True,
                                      height_shift_range=0.1,
                                      width_shift_range=0.1,
                                      shear_range=0.1,
                                      zoom_range=0.1)

    train_generator = data_augment.flow(x=x_train,
                                        y=y_train,
                                        batch_size=batchsize,
                                        shuffle=True)
                                  
    '''
    Optimizer
    RMSprop used in this example.
    SGD  with Nesterov momentum was used in original paper
    '''
    #opt = SGD(lr=learnrate, momentum=0.9, nesterov=True)
    opt = RMSprop(lr=learnrate)
    
    model.compile(optimizer=opt,
                  loss='categorical_crossentropy',
                  metrics=['accuracy'])


    # calculate number of steps in one training epoch
    train_steps = train_generator.n//train_generator.batch_size

    # run training
    model.fit_generator(generator=train_generator,
                        epochs=epochs,
                        steps_per_epoch=train_steps,
                        validation_data=(x_test, y_test),
                        callbacks=callbacks_list,
                        verbose=1)


    print("\nTensorBoard can be opened with the command: tensorboard --logdir={dir} --host localhost --port 6006".format(dir=tboard))

    print('\n'+DIVIDER)
    print(' Evaluate model accuracy with validation set..')
    print(DIVIDER)

    '''
    Evaluation
    '''

    scores = model.evaluate(x=x_test,y=y_test,batch_size=50, verbose=0)
    print ('Evaluation Loss    : ', scores[0])
    print ('Evaluation Accuracy: ', scores[1])


    return


def run_main():
    
    print('\n'+DIVIDER)
    print('Keras version      : ',tf.keras.__version__)
    print('TensorFlow version : ',tf.__version__)
    print(sys.version)
    print(DIVIDER)

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument('-ih', '--input_height',
                    type=int,
                    default='32',
    	            help='Input image height in pixels.')
    ap.add_argument('-iw', '--input_width',
                    type=int,
                    default='32',
    	            help='Input image width in pixels.')
    ap.add_argument('-ic', '--input_chan',
                    type=int,
                    default='3',
    	            help='Number of input image channels.')
    ap.add_argument('-b', '--batchsize',
                    type=int,
                    default=100,
    	            help='Training batchsize. Must be an integer. Default is 100.')
    ap.add_argument('-e', '--epochs',
                    type=int,
                    default=300,
    	            help='number of training epochs. Must be an integer. Default is 300.')
    ap.add_argument('-lr', '--learnrate',
                    type=float,
                    default=0.001,
    	            help='optimizer initial learning rate. Must be floating-point value. Default is 0.001')
    ap.add_argument('-kh', '--keras_hdf5',
                    type=str,
                    default='./model.hdf5',
    	            help='path of Keras HDF5 file - must include file name. Default is ./model.hdf5')
    ap.add_argument('-tb', '--tboard',
                    type=str,
                    default='./tb_logs',
    	            help='path to folder for saving TensorBoard data. Default is ./tb_logs.')    
    ap.add_argument('-g', '--gpu',
                    type=str,
                    default='0',
    	            help='List of GPUs to use. Default is 0')    
    args = ap.parse_args()


    print(' Command line options:')
    print ('--input_height : ',args.input_height)
    print ('--input_width  : ',args.input_width)
    print ('--input_chan   : ',args.input_chan)
    print ('--batchsize    : ',args.batchsize)
    print ('--learnrate    : ',args.learnrate)
    print ('--epochs       : ',args.epochs)
    print ('--keras_hdf5   : ',args.keras_hdf5)
    print ('--tboard       : ',args.tboard)
    print ('--gpu          : ',args.gpu)
    print(DIVIDER)

    
    os.environ["CUDA_DEVICE_ORDER"]='PCI_BUS_ID'
    
    # indicate which GPU to use
    os.environ["CUDA_VISIBLE_DEVICES"]=args.gpu

    train(args.input_height,args.input_width,args.input_chan,args.batchsize,args.learnrate,args.epochs,args.keras_hdf5,args.tboard)


if __name__ == '__main__':
    run_main()

