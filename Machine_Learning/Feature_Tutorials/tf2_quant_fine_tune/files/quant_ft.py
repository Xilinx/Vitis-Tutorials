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
Quantization with fine-tuning
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
from tensorflow_model_optimization.quantization.keras import vitis_quantize
from tensorflow.keras.models import load_model
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.callbacks import ModelCheckpoint,LearningRateScheduler
from tensorflow.keras.losses import SparseCategoricalCrossentropy


from dataset_utils import input_fn

DIVIDER = '-----------------------------------------'



def quant_ft(float_dir,quant_ft_dir,batchsize,tfrec_train,tfrec_val,learnrate,epochs,max_classes):

    '''
    Quantize & fine-tune the floating-point model
    Save to HDF5 file
    '''

    def step_decay(epoch):
        '''
        Learning rate scheduler used by callback
        Reduces learning rate depending on number of epochs
        '''
        lr = learnrate
        '''       
        if epoch > 150:
            lr /= 100
        elif epoch > 5:
            lr /= 10
        '''       
        return lr


    print('\n'+DIVIDER)
    print ('Quantization & Fine-tune')
    print(DIVIDER+'\n')


    # load the floating point trained model
    print(' Loading floating-point model from',float_dir+'/float_model.h5')
    float_model = load_model(float_dir+'/float_model.h5', compile=False)

    # get input dimensions of the floating-point model
    height = float_model.input_shape[1]
    width = float_model.input_shape[2]
    chans = float_model.input_shape[3]
    print(' Input dimensions: height:',height,' width:',width, 'channels:',chans)

    # Quantization-aware training model
    quantizer = vitis_quantize.VitisQuantizer(float_model)
    ft_model = quantizer.get_qat_model()


    '''
    tf.data pipelines
    '''
    train_dataset = input_fn(tfrec_train, batchsize, True)
    test_dataset = input_fn(tfrec_val, batchsize, False)


    '''
    Call backs
    '''
    chkpt_call = ModelCheckpoint(filepath=os.path.join(quant_ft_dir,'quant_ft.h5'), 
                                 monitor='val_accuracy',
                                 verbose=1,
                                 save_best_only=True)
    lr_scheduler_call = LearningRateScheduler(schedule=step_decay,
                                              verbose=1)    
    callbacks_list = [chkpt_call,lr_scheduler_call]


    '''
    Compile model
    Adam optimizer to change weights & biases
    Loss function is sparse categorical crossentropy
    '''
    ft_model.compile(optimizer=Adam(learning_rate=learnrate),
                     loss=SparseCategoricalCrossentropy(from_logits=True),
                     metrics=['accuracy'])

    '''
    Training
    '''
    print('\n'+DIVIDER)
    print(' Training model with training set..')
    print(DIVIDER)

    # make folder for saving trained model checkpoint
    os.makedirs(quant_ft_dir, exist_ok=True)

    # run training
    train_hist=ft_model.fit(train_dataset,
                            epochs=epochs,
                            steps_per_epoch=(1300*max_classes)//batchsize,
                            validation_data=test_dataset,
                            validation_steps=None,
                            callbacks=callbacks_list,
                            verbose=1)

    '''
    Evaluate quantized model
    '''
    print('\n'+DIVIDER)
    print ('Evaluating quantized model..')
    print(DIVIDER+'\n')

    # reload the best checkpoint and evaluate it
    with vitis_quantize.quantize_scope():
        eval_model = load_model(quant_ft_dir+'/quant_ft.h5',compile=False)

    eval_model.compile(loss=SparseCategoricalCrossentropy(from_logits=True),
                       metrics=['accuracy'])

    scores = eval_model.evaluate(test_dataset,
                                 steps=None,
                                 verbose=0)

    print(' Quantized model accuracy: {0:.4f}'.format(scores[1]*100),'%')
    print('\n'+DIVIDER)
    

    return



def main():

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument('-f', '--float_dir',    type=str,   default='float_model',    help='Path to folder containing floating-point model. Default is float_model')
    ap.add_argument('-cf','--quant_ft_dir', type=str,   default='quant_ft_model', help='Path and name of folder for storing fine-tuned model. Default is quant_ft_model')
    ap.add_argument('-tft','--tfrec_train', type=str,   default='tfrec_train', help='Path to folder containing TFRecord files for training. Default is tfrec_train') 
    ap.add_argument('-tfv','--tfrec_val',   type=str,   default='tfrec_val',   help='Path to folder containing TFRecord files for test/validation. Default is tfrec_val') 
    ap.add_argument('-b', '--batchsize',    type=int,   default=100,    help='Training batchsize. Must be an integer. Default is 100.')
    ap.add_argument('-e', '--epochs',       type=int,   default=50 ,    help='number of training epochs. Must be an integer. Default is 50.')
    ap.add_argument('-lr','--learnrate',    type=float, default=0.0001, help='optimizer learning rate. Must be floating-point value. Default is 0.0001')
    ap.add_argument('-mc', '--max_classes', type=int,   default=1000, help='Number of classes to use. Default is 1000')
    args = ap.parse_args()  

    print('\n'+DIVIDER)
    print('TensorFlow version :',tf.__version__)
    print('Keras version      :',tf.keras.__version__)
    print('Python             :',sys.version)
    print(DIVIDER)
    print ('Command line options:')
    print (' --float_dir     :',args.float_dir)
    print (' --quant_ft_dir  :',args.quant_ft_dir)
    print (' --batchsize     :',args.batchsize)
    print (' --tfrec_train   :',args.tfrec_train)
    print (' --tfrec_val     :',args.tfrec_val)
    print (' --learnrate     :',args.learnrate)
    print (' --epochs        :',args.epochs)
    print (' --max_classes   :',args.max_classes)
    print(DIVIDER+'\n')


    quant_ft(args.float_dir,args.quant_ft_dir,args.batchsize,args.tfrec_train,args.tfrec_val,args.learnrate,args.epochs,args.max_classes)


if __name__ ==  "__main__":
    main()

