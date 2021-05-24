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
Make the target folder
Creates images, copies application code and compiled xmodel to 'target'
'''

'''
Author: Mark Harvey
'''


import argparse
import os
import shutil
import sys
import cv2
from tqdm import tqdm

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL']='3'

import tensorflow as tf

from dataset_utils import parser, resize_centercrop

DIVIDER = '-----------------------------------------'



def input_fn(tfrec_dir, batchsize):
    '''
    Dataset pipeline
    '''
    tfrecord_files = tf.data.Dataset.list_files('{}/*.tfrecord'.format(tfrec_dir), shuffle=False)
    dataset = tf.data.TFRecordDataset(tfrecord_files)
    dataset = dataset.map(parser, num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.map(lambda x,y: resize_centercrop(x,y), num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.batch(batchsize, drop_remainder=False)
    return dataset



def make_target(target_dir,tfrec_dir,num_images,app_dir,model):

    # remove any previous data
    shutil.rmtree(target_dir, ignore_errors=True)    
    os.makedirs(target_dir)
    os.makedirs(target_dir+'/images')

    # make the dataset
    target_dataset = input_fn(tfrec_dir,1)

    '''
    # extract images & labels from TFRecords
    # save as JPEG image files
    # the label will be built into the JPEG filename
    '''
    i = 0
    for tfr in tqdm(target_dataset):

        label = tfr[1][0].numpy()

        # reshape image to remove batch dimension
        img = tf.reshape(tfr[0], [tfr[0].shape[1],tfr[0].shape[2],tfr[0].shape[3]] )

        # JPEG encode
        img = tf.cast(img, tf.uint8)
        img = tf.io.encode_jpeg(img)

        # save as file
        filepath =  os.path.join(target_dir, 'images', str(label)+'_image'+str(i)+'.jpg' )
        tf.io.write_file(filepath, img)

        i += 1


    # copy application code
    print('Copying application code from',app_dir,'...')
    shutil.copy(os.path.join(app_dir, 'app_mt.py'), target_dir)

    # copy compiled model
    print('Copying compiled model from',model,'...')
    shutil.copy(model, target_dir)


    return



def main():

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument('-td','--target_dir', type=str,  default='target',     help='Full path of target folder. Default is target')
    ap.add_argument('-t', '--tfrec_dir',  type=str,  default='tfrec_val',  help='Path to folder for reading TFRecord files. Default is tfrec_val') 
    ap.add_argument('-n', '--num_images', type=int,  default=1000, help='Number of test images. Default is 1000')
    ap.add_argument('-a', '--app_dir',    type=str,  default='application', help='Full path of application code folder. Default is application')
    ap.add_argument('-m', '--model',      type=str,  default='compiled_model/mobilenet.xmodel', help='Full path of compiled model.Default is compiled_model/mobilenet.xmodel')
    args = ap.parse_args()  

    print('\n------------------------------------')
    print(sys.version)
    print('------------------------------------')
    print ('Command line options:')
    print (' --target_dir   : ', args.target_dir)
    print (' --tfrec_dir    : ', args.tfrec_dir)
    print (' --num_images   : ', args.num_images)
    print (' --app_dir      : ', args.app_dir)
    print (' --model        : ', args.model)
    print('------------------------------------\n')


    make_target(args.target_dir,args.tfrec_dir,args.num_images,args.app_dir,args.model)


if __name__ ==  "__main__":
    main()
  
