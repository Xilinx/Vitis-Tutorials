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
Quantize the floating-point model
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

import tensorflow as tf
from tensorflow_model_optimization.quantization.keras import vitis_quantize
from tensorflow.keras.models import load_model
from tensorflow.keras.optimizers import Adam

from dataset_utils import input_fn_test, input_fn_quant

DIVIDER = '-----------------------------------------'



def quant_model(float_model,quant_model,batchsize,tfrec_dir,evaluate):
    '''
    Quantize the floating-point model
    Save to HDF5 file
    '''

    # make folder for saving quantized model
    head_tail = os.path.split(quant_model) 
    os.makedirs(head_tail[0], exist_ok = True)

    # load the floating point trained model
    float_model = load_model(float_model)

    # get input dimensions of the floating-point model
    height = float_model.input_shape[1]
    width = float_model.input_shape[2]

    # make TFRecord dataset and image processing pipeline
    quant_dataset = input_fn_quant(tfrec_dir, batchsize, height, width)

    # run quantization
    quantizer = vitis_quantize.VitisQuantizer(float_model)
    quantized_model = quantizer.quantize_model(calib_dataset=quant_dataset)

    # saved quantized model
    quantized_model.save(quant_model)
    print('Saved quantized model to',quant_model)


    if (evaluate):
        '''
        Evaluate quantized model
        '''
        print('\n'+DIVIDER)
        print ('Evaluating quantized model..')
        print(DIVIDER+'\n')

        test_dataset = input_fn_test(tfrec_dir, batchsize, height, width)

        quantized_model.compile(optimizer=Adam(),
                                loss='sparse_categorical_crossentropy',
                                metrics=['accuracy'])

        scores = quantized_model.evaluate(test_dataset,
                                          verbose=0)

        print('Quantized model accuracy: {0:.4f}'.format(scores[1]*100),'%')
        print('\n'+DIVIDER)

    return



def main():

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument('-m', '--float_model',  type=str, default='float_model/f_model.h5', help='Full path of floating-point model. Default is float_model/k_model.h5')
    ap.add_argument('-q', '--quant_model',  type=str, default='quant_model/q_model.h5', help='Full path of quantized model. Default is quant_model/q_model.h5')
    ap.add_argument('-b', '--batchsize',    type=int, default=50,                       help='Batchsize for quantization. Default is 50')
    ap.add_argument('-tfdir', '--tfrec_dir',type=str, default='tfrecords',              help='Full path to folder containing TFRecord files. Default is tfrecords')
    ap.add_argument('-e', '--evaluate',     action='store_true', help='Evaluate floating-point model if set. Default is no evaluation.')
    args = ap.parse_args()  

    print('\n------------------------------------')
    print('TensorFlow version : ',tf.__version__)
    print(sys.version)
    print('------------------------------------')
    print ('Command line options:')
    print (' --float_model  : ', args.float_model)
    print (' --quant_model  : ', args.quant_model)
    print (' --batchsize    : ', args.batchsize)
    print (' --tfrec_dir    : ', args.tfrec_dir)
    print (' --evaluate     : ', args.evaluate)
    print('------------------------------------\n')


    quant_model(args.float_model, args.quant_model, args.batchsize, args.tfrec_dir, args.evaluate)


if __name__ ==  "__main__":
    main()
