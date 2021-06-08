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
Train MobileNet
'''

'''
Author: Mark Harvey
'''


import argparse
import os
import shutil
import sys

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL']='3'
os.environ['TF_FORCE_GPU_ALLOW_GROWTH']='true'

import tensorflow as tf
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.callbacks import ModelCheckpoint,LearningRateScheduler,TensorBoard
from tensorflow.keras.applications.mobilenet import MobileNet
from tensorflow.keras.losses import SparseCategoricalCrossentropy
from tensorflow.keras.models import load_model

from dataset_utils import input_fn
from xfer_model import xfer_model

DIVIDER = '-----------------------------------------'



def train(float_dir,tfrec_train,tfrec_val,batchsize,learnrate,epochs,max_classes):

    '''
    Train MobileNet
    '''


    def step_decay(epoch):
        '''
        Learning rate scheduler used by callback
        Reduces learning rate depending on number of epochs
        '''
        lr = learnrate        
        if epoch > 150:
            lr /= 1000
        elif epoch > 10:
            lr /= 100
        elif epoch > 5:
            lr /= 10        
        return lr


    print('\n'+DIVIDER)
    print ('Training')
    print(DIVIDER+'\n')

    '''
    Create floating-point MobileNet model
    '''
    print(DIVIDER)
    print('Make & train floating-point model..')
    
    model = xfer_model(input_shape=(224,224,3),classes=max_classes)

    # make directory to hold TFRecords
    shutil.rmtree(float_dir, ignore_errors=True)
    os.makedirs(float_dir)
    print('Directory',float_dir,'created')


    # print model summary to a file
    from contextlib import redirect_stdout
    with open(float_dir+'/model_summary.txt', 'w') as f:
      with redirect_stdout(f):
          model.summary()
    print(' Printed model summary to',float_dir+'/model_summary.txt')

    '''
    tf.data pipelines
    '''
    train_dataset = input_fn(tfrec_train, batchsize, True)
    test_dataset = input_fn(tfrec_val, batchsize, False)


    '''
    Callbacks
    '''
    chkpt_call = ModelCheckpoint(filepath=os.path.join(float_dir,'float_model.h5'), 
                                 monitor='val_accuracy',
                                 verbose=1,
                                 save_best_only=True)
    tb_call = TensorBoard(log_dir='tb_logs',
                          update_freq='epoch')                          
    lr_scheduler_call = LearningRateScheduler(schedule=step_decay,
                                              verbose=1)

    callbacks_list = [chkpt_call,lr_scheduler_call,tb_call]


    '''
    Compile model
    Adam optimizer to change weights & biases
    Loss function is sparse categorical crossentropy
    '''
    model.compile(optimizer=Adam(learning_rate=learnrate),
                  loss=SparseCategoricalCrossentropy(from_logits=True),
                  metrics=['accuracy'])

    '''
    Training
    '''
    print('\n'+DIVIDER)
    print(' Training model with training set..')
    print(DIVIDER)

    # make folder for saving trained model checkpoint
    os.makedirs(float_dir, exist_ok=True)

    # clean out TensorBoard logs
    shutil.rmtree('tb_logs', ignore_errors=True)
    os.makedirs('tb_logs')


    # run training
    train_hist=model.fit(train_dataset,
                         epochs=epochs,
                         steps_per_epoch=(max_classes*1300)//batchsize,
                         validation_data=test_dataset,
                         validation_steps=None,
                         callbacks=callbacks_list,
                         verbose=1)

    print('\nTensorBoard can be opened with the command: tensorboard --logdir=tb_logs --host localhost --port 6006')

    '''
    Evaluate best checkpoint
    '''
    print('\n'+DIVIDER)
    print ('Evaluating best checkpoint..')
    print(DIVIDER+'\n')

    # reload the best checkpoint and evaluate it
    eval_model = load_model(float_dir+'/float_model.h5',compile=False)

    eval_model.compile(loss=SparseCategoricalCrossentropy(from_logits=True),
                       metrics=['accuracy'])

    scores = eval_model.evaluate(test_dataset,
                                 steps=None,
                                 verbose=0)

    print(' Floating-point model accuracy: {0:.4f}'.format(scores[1]*100),'%')
    print('\n'+DIVIDER)    

    return



def main():

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument('-cf', '--float_dir',   type=str,   default='float_model', help='Path and name of folder for storing Keras checkpoints. Default is float_model')
    ap.add_argument('-tft','--tfrec_train', type=str,   default='tfrec_train', help='Path to folder containing TFRecord files for training. Default is tfrec_train') 
    ap.add_argument('-tfv','--tfrec_val',   type=str,   default='tfrec_val',   help='Path to folder containing TFRecord files for test/validation. Default is tfrec_val') 
    ap.add_argument('-b',  '--batchsize',   type=int,   default=100,   help='Training batchsize. Must be an integer. Default is 100.')
    ap.add_argument('-e',  '--epochs',      type=int,   default=80,   help='number of training epochs. Must be an integer. Default is 80.')
    ap.add_argument('-lr', '--learnrate',   type=float, default=0.01, help='optimizer learning rate. Must be floating-point value. Default is 0.01')
    ap.add_argument('-mc', '--max_classes', type=int,   default=1000, help='Number of classes to use. Default is 1000')
    args = ap.parse_args()  

    print('\n'+DIVIDER)
    print('TensorFlow version :',tf.__version__)
    print('Keras version      :',tf.keras.__version__)
    print('Python             :',sys.version)
    print(DIVIDER)
    print ('Command line options:')
    print (' --float_dir     :',args.float_dir)
    print (' --tfrec_train   :',args.tfrec_train)
    print (' --tfrec_val     :',args.tfrec_val)
    print (' --batchsize     :',args.batchsize)
    print (' --learnrate     :',args.learnrate)
    print (' --epochs        :',args.epochs)
    print (' --max_classes   :',args.max_classes)
    print(DIVIDER+'\n')


    train(args.float_dir,args.tfrec_train,args.tfrec_val,args.batchsize,args.learnrate,args.epochs,args.max_classes)


if __name__ ==  "__main__":
    main()

