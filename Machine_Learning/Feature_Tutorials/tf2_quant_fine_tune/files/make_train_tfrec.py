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
Convert ImageNet training set into TFRecords
'''

'''
Author: Mark Harvey, Xilinx Inc
'''


import os
import sys
import argparse
import shutil
from tqdm import tqdm
import tarfile
import requests


# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL']='3'
import tensorflow as tf


_divider = '-------------------------------------'


def _bytes_feature(value):
  '''Returns a bytes_list from a string / byte'''
  if isinstance(value, type(tf.constant(0))):
    value = value.numpy() # BytesList won't unpack a string from an EagerTensor.
  return tf.train.Feature(bytes_list=tf.train.BytesList(value=[value]))


def _float_feature(value):
  '''Returns a float_list from a float / double'''
  return tf.train.Feature(float_list=tf.train.FloatList(value=[value]))


def _int64_feature(value):
  ''' Returns an int64_list from a bool / enum / int / uint '''
  return tf.train.Feature(int64_list=tf.train.Int64List(value=[value]))


def _calc_num_shards(img_list, img_shard):
  ''' calculate number of shards'''
  last_shard =  len(img_list) % img_shard
  if last_shard != 0:
    num_shards =  (len(img_list) // img_shard) + 1
  else:
    num_shards =  (len(img_list) // img_shard)
  return last_shard, num_shards


def _create_images_labels(labels_file):
  ''' create lists of image filenames and their labels '''  
  f= open(labels_file,'r')
  linesList = f.readlines()
  f.close()
  labels_list=[]
  fileNames_list=[]
  for line in linesList:
    fileName, label = line.split()
    labels_list.append(int(label.strip()))
    fileNames_list.append(fileName.strip())
  return labels_list, fileNames_list
  

def write_tfrec(tfrec_filename, image_dir, label):
  ''' write TFRecord file '''

  with tf.io.TFRecordWriter(tfrec_filename) as writer:

    img_list = os.listdir('train_images')

    for i in range(len(img_list)):
      filePath = os.path.join(image_dir, img_list[i])

      try:
        # read the JPEG source file into a tf.string
        image = tf.io.read_file(filePath)

        # get the shape of the image from the JPEG file header
        image_shape = tf.io.extract_jpeg_shape(image, output_type=tf.dtypes.int32, name=None)

        # features dictionary
        feature_dict = {
          'label' : _int64_feature(label),
          'height': _int64_feature(image_shape[0]),
          'width' : _int64_feature(image_shape[1]),
          'chans' : _int64_feature(image_shape[2]),
          'image' : _bytes_feature(image)
        }

        # Create Features object
        features = tf.train.Features(feature = feature_dict)

        # create Example object
        tf_example = tf.train.Example(features=features)

        # serialize Example object into TFRecord file
        writer.write(tf_example.SerializeToString())

      except:
        print('Ignoring', filePath)

  return


def make_tfrec(tfrec_base, tfrec_dir, max_classes):

  # make directory to hold training set images
  os.makedirs('train_images', exist_ok=True)
  print('Directory train_images created')


  # make directory to hold TFRecords
  shutil.rmtree(tfrec_dir, ignore_errors=True)
  os.makedirs(tfrec_dir)
  print('Directory',tfrec_dir,'created')

 
  # make tarfile object
  tar = tarfile.open('ILSVRC2012_img_train.tar')
  class_tars = tar.getnames()

  total_images = 0

  for i in tqdm(range(max_classes)):
    tar.extract(class_tars[i])

    # untar class images
    class_tar = tarfile.open(class_tars[i])
    class_tar.extractall(path='train_images')
    class_tar.close()
    os.remove(class_tars[i])

    # create TFRecords file
    tfrec_filename = tfrec_base+'_'+str(i)+'.tfrecord'
    write_path = os.path.join(tfrec_dir, tfrec_filename)
    write_tfrec(write_path, 'train_images', i)

    # delete images to save space
    files = os.listdir('train_images')
    total_images += len(files)
    for f in files:
      os.remove('train_images/'+f)

  tar.close()
  shutil.rmtree('train_images')
  print('Total images:',total_images)

  return


# only used if script is run as 'main' from command line
def main():
  # construct the argument parser and parse the arguments
  ap = argparse.ArgumentParser()
  ap.add_argument('-tfb', '--tfrec_base', type=str, default='data',        help='Base file name for TFRecord files. Default is data') 
  ap.add_argument('-tfd', '--tfrec_dir',  type=str, default='tfrec_train', help='Path to folder for saving TFRecord files. Default is tfrec_train')  
  ap.add_argument('-mc',  '--max_classes',type=int, default=1000, help='Number of classes to use. Default is 1000')  
  args = ap.parse_args()  
  
  print ('\n'+_divider)
  print('TensorFlow version :',tf.__version__)
  print('Keras version      :',tf.keras.__version__)
  print('Python             :',sys.version)
  print (_divider)
  print ('Command line options:')
  print (' --tfrec_base  :',args.tfrec_base)
  print (' --tfrec_dir   :',args.tfrec_dir)
  print (' --max_classes :',args.max_classes)
  print (_divider+'\n')


  make_tfrec(args.tfrec_base, args.tfrec_dir, args.max_classes)


if __name__ == '__main__':
  main()

