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
Creates JPEG image files from Keras dataset in numpy array format
'''

import argparse
import os

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
import tensorflow as tf
from tensorflow.keras.datasets import cifar10



def gen_images(image_dir,calib_list,max_images,dataset):
    
    (x_train,y_train), (x_test,y_test) = cifar10.load_data()
    classes = ['airplane','automobile','bird','cat','deer','dog','frog','horse','ship','truck']

    # create file for list of calibration images
    # folder specified by args.calib_dir must exist
    if not (calib_list==''):
        f = open(os.path.join(image_dir, calib_list), 'w')
    
    array_img = tf.compat.v1.placeholder(tf.uint8)
    op = tf.io.encode_jpeg(array_img, format='rgb', quality=100)
    init = tf.compat.v1.global_variables_initializer()

    if dataset=='train':
        x_feed = x_train
        y_feed = y_train
    else:
        x_feed = x_test
        y_feed = y_test

    with tf.compat.v1.Session() as sess:
        sess.run(init)

        for i in range(len(x_test[:max_images])):
            jpg_tensor = sess.run(op, feed_dict={array_img: x_feed[i]})
            img_file=os.path.join(image_dir, classes[int(y_feed[i])]+'_'+str(i)+'.jpg')
            with open(img_file, 'wb') as fd:
                fd.write(jpg_tensor)
            if not (calib_list==''):
                f.write(img_file+'\n')

    if not (calib_list==''):
        f.close()
    print('Calib images generated')
    return


# only used if script is run as 'main' from command line
def main():

  # construct the argument parser and parse the arguments
  ap = argparse.ArgumentParser()  
  ap.add_argument('-dir', '--image_dir',
                  type=str,
                  default='images',
                  help='Path to folder for saving images and images list file. Default is images')  
  ap.add_argument('-l', '--calib_list',
                  type=str,
                  default='',
                  help='Name of images list file. Default is empty string so that no file is generated.')  
  ap.add_argument('-m', '--max_images',
                  type=int,
                  default=1,
                  help='Number of images to generate. Default is 1')
  ap.add_argument('-d', '--dataset',
                  type=str,
                  default='train',
                  choices=['train','test'],
                  help='Use train or test dataset. Default is train')
  args = ap.parse_args()  
  
  print ('Command line options:')
  print (' --image_dir    : ', args.image_dir)
  print (' --calib_list   : ', args.calib_list)
  print (' --dataset      : ', args.dataset)
  print (' --max_images   : ', args.max_images)

  gen_images(args.image_dir,args.calib_list,args.max_images,args.dataset)


if __name__ == '__main__':
  main()



