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
Utility functions for tf.data pipeline
'''

'''
Author: Mark Harvey, Xilinx Inc
'''
import os

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL']='3'

import tensorflow as tf


def parser(data_record):
    ''' TFRecord parser '''

    feature_dict = {
      'label' : tf.io.FixedLenFeature([], tf.int64),
      'height': tf.io.FixedLenFeature([], tf.int64),
      'width' : tf.io.FixedLenFeature([], tf.int64),
      'chans' : tf.io.FixedLenFeature([], tf.int64),
      'image' : tf.io.FixedLenFeature([], tf.string)
    }
    sample = tf.io.parse_single_example(data_record, feature_dict)
    label = tf.cast(sample['label'], tf.int32)

    h = tf.cast(sample['height'], tf.int32)
    w = tf.cast(sample['width'], tf.int32)
    c = tf.cast(sample['chans'], tf.int32)
    image = tf.io.decode_image(sample['image'], channels=3)
    image = tf.reshape(image,[h,w,3])

    return image, label


def resize_crop(x,y,h,w):
    '''
    Image resize & random crop
    Args:     Image and label
    Returns:  augmented image and unchanged label
    '''
    rh = int(h *1.2)
    rw = int(w *1.2)
    x = tf.image.resize(x, (rh,rw), method='bicubic')
    x = tf.image.random_crop(x, [h, w, 3], seed=42)
    return x,y


def augment(x,y):
    '''
    Image augmentation
    Args:     Image and label
    Returns:  augmented image and unchanged label
    '''
    x = tf.image.random_flip_left_right(x, seed=42)
    x = tf.image.random_brightness(x, 0.1, seed=42)
    x = tf.image.random_contrast(x, 0.9, 1.1, seed=42)
    x = tf.image.random_saturation(x, 0.9, 1.1, seed=42)   
    return x, y


def normalize(x,y):
    '''
    Image normalization
    Args:     Image and label
    Returns:  normalized image and unchanged label
    '''
    # Convert to floating-point & scale to range 0.0 -> 1.0
    x = tf.cast(x, tf.float32) * (1. / 255)
    return x, y



def input_fn_trn(tfrec_dir,batchsize,height,width):
    '''
    Dataset creation and augmentation for training
    '''
    tfrecord_files = tf.data.Dataset.list_files('{}/train_*.tfrecord'.format(tfrec_dir), shuffle=True)
    dataset = tf.data.TFRecordDataset(tfrecord_files)
    dataset = dataset.map(parser, num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.map(lambda x,y: resize_crop(x,y,h=height,w=width), num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.batch(batchsize, drop_remainder=False)
    dataset = dataset.map(augment, num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.map(normalize, num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.prefetch(buffer_size=tf.data.experimental.AUTOTUNE)
    dataset = dataset.repeat()
    return dataset


def input_fn_test(tfrec_dir,batchsize,height,width):
    '''
    Dataset creation and augmentation for test
    '''
    tfrecord_files = tf.data.Dataset.list_files('{}/test_*.tfrecord'.format(tfrec_dir), shuffle=False)
    dataset = tf.data.TFRecordDataset(tfrecord_files)
    dataset = dataset.map(parser, num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.map(lambda x,y: resize_crop(x,y,h=height,w=width), num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.batch(batchsize, drop_remainder=False)
    dataset = dataset.map(normalize, num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.prefetch(buffer_size=tf.data.experimental.AUTOTUNE)
    return dataset


def input_fn_quant(tfrec_dir,batchsize,height,width):
    '''
    Dataset creation and augmentation for quantization
    The TFRecord file(s) must have > 1000 images
    '''
    tfrecord_files = tf.data.Dataset.list_files('{}/test_0.tfrecord'.format(tfrec_dir), shuffle=False)
    dataset = tf.data.TFRecordDataset(tfrecord_files)
    dataset = dataset.map(parser, num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.map(lambda x,y: resize_crop(x,y,h=height,w=width), num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.batch(batchsize, drop_remainder=False)
    dataset = dataset.map(normalize, num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.prefetch(buffer_size=tf.data.experimental.AUTOTUNE)
    return dataset

