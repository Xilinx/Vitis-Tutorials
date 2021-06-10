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
Convert ImageNet validation set into TFRecords
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


def _create_images_labels(labels_file, max_classes):
  ''' create lists of image filenames and their labels '''  
  f= open(labels_file,'r')
  linesList = f.readlines()
  f.close()
  labels_list=[]
  fileNames_list=[]
  for line in linesList:
    fileName, label = line.split()
    if int(label.strip()) < max_classes:
      labels_list.append(int(label.strip()))
      fileNames_list.append(fileName.strip())
  return labels_list, fileNames_list
  

def write_tfrec(tfrec_filename, image_dir, img_list, label_list):
  ''' write TFRecord file '''

  with tf.io.TFRecordWriter(tfrec_filename) as writer:

    for i in range(len(img_list)):
      filePath = os.path.join(image_dir, img_list[i])

      # read the JPEG source file into a tf.string
      image = tf.io.read_file(filePath)

      # get the shape of the image from the JPEG file header
      image_shape = tf.io.extract_jpeg_shape(image, output_type=tf.dtypes.int32, name=None)

      # features dictionary
      feature_dict = {
        'label' : _int64_feature(int(label_list[i])),
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

  return



def make_tfrec(image_dir, img_shard, tfrec_base, tfrec_dir, max_classes):

  # make directory to hold validation set images
  os.makedirs(image_dir, exist_ok=True)
  print('Directory',image_dir,'created',flush=True)

  # untar validation set images
  tar = tarfile.open('ILSVRC2012_img_val.tar')
  tar.extractall(path=image_dir)
  tar.close()
  print ('Extracted ILSVRC2012_img_val.tar in',image_dir)


  # fetch file with ground truth labels
  url = 'http://dl.caffe.berkeleyvision.org/caffe_ilsvrc12.tar.gz'
  r = requests.get(url)
  with open("caffe_ilsvrc12.tar.gz", 'wb') as f:
    f.write(r.content)

  # untar val.txt
  tar = tarfile.open('caffe_ilsvrc12.tar.gz')
  tar.extract('val.txt')
  tar.close()
  print ('Extracted val.txt from caffe_ilsvrc12.tar.gz')

  # make tfrecords directory
  shutil.rmtree(tfrec_dir, ignore_errors=True)
  os.makedirs(tfrec_dir)
  print('Directory',tfrec_dir,'created',flush=True)

  # make lists of images and their labels
  all_labels, all_images = _create_images_labels('val.txt', max_classes)
  print('Creating TFRecords for',len(all_labels),'images..')

  # calculate how many shards we will generate and number of images in last shard
  last_shard, num_shards = _calc_num_shards(all_images, img_shard)
  print (num_shards,'TFRecord files will be created.')
  if (last_shard>0):
    print ('Last TFRecord file will have',last_shard,'images.')


  # create TFRecord files (shards)
  start = 0

  for i in tqdm(range(num_shards)):

    tfrec_filename = tfrec_base+'_'+str(i)+'.tfrecord'
    write_path = os.path.join(tfrec_dir, tfrec_filename)

    if (i == num_shards-1):
      write_tfrec(write_path, image_dir, all_images[start:], all_labels[start:])
    else:
      end = start + img_shard
      write_tfrec(write_path, image_dir, all_images[start:end], all_labels[start:end])
      start = end


  # delete images to save space
  shutil.rmtree(image_dir)

  return



# only used if script is run as 'main' from command line
def main():
  # construct the argument parser and parse the arguments
  ap = argparse.ArgumentParser()
  ap.add_argument('-dir', '--image_dir',  type=str, default='val_images',help='Path to folder that contains images. Default is val_images')
  ap.add_argument('-s',   '--img_shard',  type=int, default=5000,        help='Number of images per shard. Default is 5000') 
  ap.add_argument('-tfb', '--tfrec_base', type=str, default='data',      help='Base file name for TFRecord files. Default is data') 
  ap.add_argument('-tfd', '--tfrec_dir',  type=str, default='tfrec_val', help='Path to folder for saving TFRecord files. Default is tfrec_val')  
  ap.add_argument('-mc',  '--max_classes',type=int, default=1000,        help='Number of classes to use. Default is 1000')  
  args = ap.parse_args()  
  
  print ('\n'+_divider)
  print('TensorFlow version :',tf.__version__)
  print('Keras version      :',tf.keras.__version__)
  print('Python             :',sys.version)
  print (_divider)
  print ('Command line options:')
  print (' --image_dir  : ', args.image_dir)
  print (' --img_shard  : ', args.img_shard)
  print (' --tfrec_base : ', args.tfrec_base)
  print (' --tfrec_dir  : ', args.tfrec_dir)
  print (' --max_classes: ', args.max_classes)
  print (_divider+'\n')


  make_tfrec(args.image_dir, args.img_shard, args.tfrec_base, args.tfrec_dir, args.max_classes)


if __name__ == '__main__':
  main()


