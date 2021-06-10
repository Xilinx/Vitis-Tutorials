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
Author: Mark Harvey, Xilinx Inc
'''

import os
# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL']='3'

import tensorflow as tf
from tensorflow.keras.applications.mobilenet import preprocess_input



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


def resize_centercrop(x,y):
    '''
    Image resizing & center cropping to 224,224
    Args:     Image and label
    Returns:  resized image and unchanged label
    '''

    # short-side resized, aspect ratio maintained
    shape_x = tf.shape(x)
    min_dim = tf.minimum(shape_x[0],shape_x[1])

    sf = tf.divide(256.0, tf.cast(min_dim,dtype=tf.float32))

    height = tf.cast( shape_x[0],dtype=tf.float32)
    width = tf.cast( shape_x[1],dtype=tf.float32)

    new_height = tf.cast((height * sf), dtype=tf.int32)
    new_width = tf.cast((width * sf), dtype=tf.int32)

    x = tf.image.resize(x, (new_height,new_width), method='bicubic')

    # center crop to 224,224
    shape_x = tf.shape(x)
    height = shape_x[0]
    width = shape_x[1]

    offset_y = (height - 224)//2
    offset_x = (width - 224)//2
    x = tf.image.crop_to_bounding_box(x, offset_y, offset_x, 224, 224)
    
    return x, y


def resize_randomcrop(x,y):
    '''
    Image resizing & random cropping to 224,224
    Args:     Image and label
    Returns:  resized image and unchanged label
    '''

    # short-side resized, aspect ratio maintained
    shape_x = tf.shape(x)
    min_dim = tf.minimum(shape_x[0],shape_x[1])

    sf = tf.divide(256.0, tf.cast(min_dim,dtype=tf.float32))

    height = tf.cast( shape_x[0],dtype=tf.float32)
    width = tf.cast( shape_x[1],dtype=tf.float32)

    new_height = tf.cast((height * sf), dtype=tf.int32)
    new_width = tf.cast((width * sf), dtype=tf.int32)

    x = tf.image.resize(x, (new_height,new_width), method='bicubic')

    x = tf.image.random_crop(x, (224,224,3), seed=42)
    
    return x, y


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


def preprocess(x,y):
    '''
    Image preprocessing with Keras utility
    '''
    x = preprocess_input(x, data_format=None)
    return x, y


def input_fn(tfrec_dir, batchsize, is_training):
    '''
    Dataset pipeline
    '''
    if is_training:
        tfrecord_files = tf.data.Dataset.list_files('{}/*.tfrecord'.format(tfrec_dir), shuffle=True)
    else:
        tfrecord_files = tf.data.Dataset.list_files('{}/*.tfrecord'.format(tfrec_dir), shuffle=False)
    dataset = tf.data.TFRecordDataset(tfrecord_files)
    dataset = dataset.map(parser, num_parallel_calls=tf.data.experimental.AUTOTUNE)
    if is_training:
        dataset = dataset.map(lambda x,y: resize_randomcrop(x,y), num_parallel_calls=tf.data.experimental.AUTOTUNE)
        dataset = dataset.map(augment, num_parallel_calls=tf.data.experimental.AUTOTUNE)
    else:
        dataset = dataset.map(lambda x,y: resize_centercrop(x,y), num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.batch(batchsize, drop_remainder=False)
    if is_training:
        dataset = dataset.shuffle(buffer_size=13000) 
    dataset = dataset.map(lambda x,y: preprocess(x,y), num_parallel_calls=tf.data.experimental.AUTOTUNE)
    dataset = dataset.prefetch(buffer_size=tf.data.experimental.AUTOTUNE)
    if is_training:
        dataset = dataset.repeat()
    return dataset


