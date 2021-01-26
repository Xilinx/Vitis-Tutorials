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
Training script for dogs-vs-cats tutorial.
'''

'''
Author: Mark Harvey
'''

import os
import shutil
import sys
import argparse


# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL']='3'

import tensorflow as tf
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.callbacks import TensorBoard,ModelCheckpoint,LearningRateScheduler

from dataset_utils import input_fn_trn, input_fn_test
from customcnn import customcnn


DIVIDER = '-----------------------------------------'



def train(input_height,input_width,input_chan,tfrec_dir,batchsize,learnrate,epochs,chkpt_dir,tboard):

    def step_decay(epoch):
        """
        Learning rate scheduler used by callback
        Reduces learning rate depending on number of epochs
        """
        lr = learnrate
        if epoch > 200:
            lr /= 100
        elif epoch > 10:
            lr /= 10
        return lr

        

    '''
    Define the model
    '''
    model = customcnn(input_shape=(input_height, input_width, input_chan),classes=2,filters=[8,16,32,64,128])

    print('\n'+DIVIDER)
    print(' Model Summary')
    print(DIVIDER)
    print(model.summary())
    print("Model Inputs: {ips}".format(ips=(model.inputs)))
    print("Model Outputs: {ops}".format(ops=(model.outputs)))


    '''
    tf.data pipelines
    '''
    # train and test folders
    train_dataset = input_fn_trn(tfrec_dir,batchsize,input_height,input_width)
    test_dataset = input_fn_test(tfrec_dir,batchsize,input_height,input_width)


    '''
    Call backs
    '''
    tb_call = TensorBoard(log_dir=tboard)

    chkpt_call = ModelCheckpoint(filepath=os.path.join(chkpt_dir,'f_model.h5'), 
                                 monitor='val_accuracy',
                                 verbose=1,
                                 save_best_only=True)

    lr_scheduler_call = LearningRateScheduler(schedule=step_decay,
                                              verbose=1)

    callbacks_list = [tb_call, chkpt_call, lr_scheduler_call]


    '''
    Compile model
    Adam optimizer to change weights & biases
    Loss function is categorical crossentropy
    '''
    model.compile(optimizer=Adam(learning_rate=learnrate),
                  loss='sparse_categorical_crossentropy',
                  metrics=['accuracy'])
 

    '''
    Training
    '''

    print('\n'+DIVIDER)
    print(' Training model with training set..')
    print(DIVIDER)

    # make folder for saving trained model checkpoint
    os.makedirs(chkpt_dir, exist_ok = True)


    # run training
    train_history=model.fit(train_dataset,
                            epochs=epochs,
                            steps_per_epoch=17500//batchsize,
                            validation_data=test_dataset,
                            validation_steps=None,
                            callbacks=callbacks_list,
                            verbose=1)

    print("\nTensorBoard can be opened with the command: tensorboard --logdir={dir} --host localhost --port 6006".format(dir=tboard))

    return




def run_main():
    
    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument('-ih','--input_height',type=int,   default=200,  help='Input image height in pixels.')
    ap.add_argument('-iw','--input_width', type=int,   default=250,  help='Input image width in pixels.')
    ap.add_argument('-ic','--input_chan',  type=int,   default=3,    help='Number of input image channels.')
    ap.add_argument('-t', '--tfrec_dir',   type=str,   default='tfrecords',    help='Path of folder containing TFRecord files.')
    ap.add_argument('-b', '--batchsize',   type=int,   default=50,     help='Training batchsize. Must be an integer. Default is 50.')
    ap.add_argument('-e', '--epochs',      type=int,   default=250,    help='number of training epochs. Must be an integer. Default is 100.')
    ap.add_argument('-lr','--learnrate',   type=float, default=0.001,  help='optimizer learning rate. Must be floating-point value. Default is 0.0001')
    ap.add_argument('-cf','--chkpt_dir',   type=str,   default='float_model', help='Path and name of folder for storing Keras checkpoints. Default is float_model')
    ap.add_argument('-tb','--tboard',      type=str,   default='tb_logs', help='path to folder for saving TensorBoard data. Default is tb_logs.')
    args = ap.parse_args()

    print('\n'+DIVIDER)
    print('Keras version      : ',tf.keras.__version__)
    print('TensorFlow version : ',tf.__version__)
    print(sys.version)
    print(DIVIDER)
    print(' Command line options:')
    print ('--input_height : ',args.input_height)
    print ('--input_width  : ',args.input_width)
    print ('--input_chan   : ',args.input_chan)
    print ('--tfrec_dir    : ',args.tfrec_dir)
    print ('--batchsize    : ',args.batchsize)
    print ('--learnrate    : ',args.learnrate)
    print ('--epochs       : ',args.epochs)
    print ('--chkpt_dir    : ',args.chkpt_dir)
    print ('--tboard       : ',args.tboard)
    print(DIVIDER)
    

    train(args.input_height,args.input_width,args.input_chan,args.tfrec_dir, \
          args.batchsize,args.learnrate,args.epochs,args.chkpt_dir,args.tboard)


if __name__ == '__main__':
    run_main()
