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
Author: Mark Harvey
'''

'''
Converts Keras datasets in numpy array format to image files
in .png or .jpg formats.

Note that the MNIST and Fashion MNIST arrays are 2D arrays (single-channel).


Command line arguments:

--dataset: Choice of Keras dataset to download and convert.
         : Only possible valid choices are 'cifar10', 'cifar100', 'mnist', 'fashion_mnist'.
         : Default is 'mnist'.

--subset : Convert training or test subset.
         : Only possible valid choices are 'train' or 'test'.
         : Default is 'test'.

--image_dir: Path to folder for saving the images and images list file.
           : If it doesn't already exist, it will be created.
           : Default is 'image_dir'.

--label_list: Name of labels list file.
            : Will be written to folder specified by --image_dir.
            : Default is '' which causes the file to not be generated.

--image_list: Name of images list file.
            : Will be written to folder specified by --image_dir.
            : Default is '' which causes the file to not be generated.

--image_format: Specifies image file format.
              : Only possible valid choices are 'png','jpg' or 'bmp'.
              : Default is png

--max_images: Number of images to generate.
            : The conversion will start from the first element of the dataset.
            : Default is 1000
'''

import os
import argparse
import cv2
import tensorflow as tf

from tensorflow.keras.datasets import cifar10, cifar100, mnist, fashion_mnist



def gen_images(dataset, subset, image_dir, image_list, label_list, max_images, image_format):

  one_chan = False
  
  # make the calibration images folder if it doesn't exist
  if not os.path.isdir(image_dir):
    os.makedirs(image_dir)

  # Fetch the Keras dataset
  if (dataset=='fashion_mnist'):
    (x_train, y_train), (x_test, y_test) = fashion_mnist.load_data()
    one_chan = True
    classes = ['T-shirt_top','Trouser','Pullover','Dress','Coat','Sandal','Shirt','Sneaker','Bag','Ankle boot']
  elif (dataset=='cifar100'):
    (x_train, y_train), (x_test, y_test) = cifar100.load_data()
    classes = ['apple', 'aquarium_fish', 'baby', 'bear', 'beaver', 'bed', 'bee', 'beetle', \
           'bicycle', 'bottle', 'bowl', 'boy', 'bridge', 'bus', 'butterfly', 'camel', \
           'can', 'castle', 'caterpillar', 'cattle', 'chair', 'chimpanzee', 'clock', \
           'cloud', 'cockroach', 'couch', 'crab', 'crocodile', 'cup', 'dinosaur', \
           'dolphin', 'elephant', 'flatfish', 'forest', 'fox', 'girl', 'hamster', \
           'house', 'kangaroo', 'keyboard', 'lamp', 'lawn_mower', 'leopard', 'lion', \
           'lizard', 'lobster', 'man', 'maple_tree', 'motorcycle', 'mountain', 'mouse', \
           'mushroom', 'oak_tree', 'orange', 'orchid', 'otter', 'palm_tree', 'pear', \
           'pickup_truck', 'pine_tree', 'plain', 'plate', 'poppy', 'porcupine', \
           'possum', 'rabbit', 'raccoon', 'ray', 'road', 'rocket', 'rose', \
           'sea', 'seal', 'shark', 'shrew', 'skunk', 'skyscraper', 'snail', 'snake', \
           'spider', 'squirrel', 'streetcar', 'sunflower', 'sweet_pepper', 'table', \
           'tank', 'telephone', 'television', 'tiger', 'tractor', 'train', 'trout', \
           'tulip', 'turtle', 'wardrobe', 'whale', 'willow_tree', 'wolf', 'woman', 'worm']
  elif (dataset=='cifar10'):
    (x_train, y_train), (x_test, y_test) = cifar10.load_data()
    classes = ['airplane','automobile','bird','cat','deer','dog','frog','horse','ship','truck']
  else:
    (x_train, y_train), (x_test, y_test) = mnist.load_data()
    one_chan = True
    classes = ['zero','one','two','three','four','five','six','seven','eight','nine']

  # create file for list of images & labels
  if image_list != '':
    fi = open(os.path.join(image_dir, image_list), 'w')
  if label_list != '':
    fl = open(os.path.join(image_dir, label_list), 'w')

  # which subset?
  if (subset=='train'):
    data_array = x_train
    label_array = y_train
  else:
    data_array = x_test
    label_array = y_test


  # Convert numpy arrays of dataset subset into image files.
  for i in range(len(data_array[:max_images])):

    img_file=os.path.join(image_dir, classes[int(label_array[i])]+'_'+str(i)+'.'+image_format)

    if (one_chan == True):
      img = cv2.cvtColor(data_array[i], cv2.COLOR_GRAY2BGR)
    else:
      img = cv2.cvtColor(data_array[i], cv2.COLOR_RGB2BGR)

    # imwrite assumes BGR format
    cv2.imwrite(img_file, img)
   
    # write image file name to image list
    #fi.write('image_'+str(i)+'.'+image_format+'\n')

    # use this line if complete path of image file is to be written to image list
    if image_list != '':
      fi.write(img_file+'\n')

    # write label into list
    if label_list != '':
      fl.write(str(label_array[i]).strip('[]') +'\n')

  if image_list != '':
    fi.close()
  if label_list != '':
    fl.close()

  return


# only used if script is run as 'main' from command lime
def main():
  # construct the argument parser and parse the arguments
  ap = argparse.ArgumentParser()
  
  ap.add_argument('-d', '--dataset',
                  type=str,
                  default='mnist',
                  choices=['cifar10','cifar100','mnist','fashion_mnist'],
                  help='Dataset - valid choices are cifar10, cifar100, mnist, fashion_mnist. Default is mnist') 
  ap.add_argument('-s', '--subset',
                  type=str,
                  default='test',
                  choices=['train','test'],
                  help='Convert training or test subset - valid choices are train, test. Default is test') 
  ap.add_argument('-dir', '--image_dir',
                  type=str,
                  default='image_dir',
                  help='Path to folder for saving images and images list file. Default is image_dir')  
  ap.add_argument('-li', '--image_list',
                  type=str,
                  default='',
                  help='Name of images list file. Default is not to produce the file')  
  ap.add_argument('-ll', '--label_list',
                  type=str,
                  default='',
                  help='Name of labels list file. Default is not to produce the file')  
  ap.add_argument('-f', '--image_format',
                  type=str,
                  default='png',
                  choices=['png','jpg','bmp'],
                  help='Image file format - valid choices are png, jpg, bmp. Default is png')  
  ap.add_argument('-m', '--max_images',
                  type=int,
                  default=1000,
                  help='Number of images to generate. Default is 1000')
  
  args = ap.parse_args()  
  
  print ('Command line options:')
  print (' --dataset      : ', args.dataset)
  print (' --subset       : ', args.subset)
  print (' --image_dir    : ', args.image_dir)
  print (' --image_list   : ', args.image_list)
  print (' --label_list   : ', args.label_list)
  print (' --image_format : ', args.image_format)
  print (' --max_images   : ', args.max_images)


  gen_images(args.dataset, args.subset, args.image_dir, args.image_list, args.label_list, args.max_images, args.image_format)


if __name__ == '__main__':
  main()
