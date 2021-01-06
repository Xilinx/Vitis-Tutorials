 
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
This script will do data wrangling on the dogs-vs-cats dataset.
The dataset must be downloaded from https://www.kaggle.com/c/dogs-vs-cats/data
 - this will require a Kaggle account.
The downloaded 'dogs-vs-cats.zip' archive should be placed in the same folder 
as this script, then this script should be run.
'''

'''
Author: Mark Harvey, Xilinx Inc
'''


import os
import argparse
import zipfile
import random
import cv2
import shutil
import numpy as np
from tqdm import tqdm

DIVIDER = '-----------------------------------------'



def center_crop(image,target_h,target_w):
  '''
  Center crop images to size out_height x out_width
  '''
  image_height, image_width = image.shape[:2]
  offset_height = (image_height - target_h) // 2
  offset_width = (image_width - target_w) // 2
  image = image[offset_height:offset_height+target_h, offset_width:offset_width+target_w,:]
  return image


def resize_maintain_aspect(image,target_h,target_w):
  '''
  Resize image but maintain aspect ratio
  '''
  image_height, image_width = image.shape[:2]
  if image_height > image_width:
    new_width = target_w
    new_height = int(image_height*(target_w/image_width))
  else:
    new_height = target_h
    new_width = int(image_width*(target_h/image_height))

  image = cv2.resize(image,(new_width,new_height),interpolation=cv2.INTER_CUBIC)
  return image



def img_to_npz(listImages,image_height,image_width,output_file,save_image):
  '''
  Converts a list of images to a single compressed numpy file.
  Images are resized and cropped to image_height x image_width, then 
  are normalized so that all pixels are floting-point numbers.
  Labels are derived from the image filenames and then 1-hot encoded before
  being packed into the numpy file.
  '''

  # make data array for images
  x = np.ndarray(shape=(len(listImages),image_height,image_width,3), dtype=np.float32, order='C')

  # make labels array for labels
  y = np.ndarray(shape=(len(listImages),2), dtype=np.float32, order='C')

  for i in tqdm(range(len(listImages))):

    # open image to numpy array
    img = cv2.imread(listImages[i])

    # resize
    img = resize_maintain_aspect(img,image_height,image_width)

    # center crop
    img = center_crop(img,image_height,image_width)

    if save_image == True:
      cv2.imwrite(listImages[i], img) 

    # switch to RGB from BGR
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

    # normalize then write into data array
    x[i] = (img/255.0).astype(np.float32)

    # make one-hot labels and write into labels array
    filename = os.path.basename(listImages[i])
    class_name,_ = filename.split('.',1)
    if class_name == 'dog':
      label = 0
    else:
      label = 1

    label_1hot = np.zeros(2,dtype=np.float32,order='C')
    np.put(label_1hot, label, 1.0)
    y[i] = label_1hot
 
  print(' Saving numpy file, this may take some time...',flush=True)
  np.savez_compressed(output_file, x=x, y=y)
  print(' Saved to',output_file+'.npz',flush=True)

  return



def create_datasets(dataset_dir,input_height,input_width):
  '''
  Convert images and labels into numpy arrays
  '''

  # remove any previous data
  shutil.rmtree(os.path.join(dataset_dir))
  
  # unzip the dogs-vs-cats archive that was downloaded from Kaggle
  zip_ref = zipfile.ZipFile('./dogs-vs-cats.zip', 'r')
  zip_ref.extractall(dataset_dir)
  zip_ref.close()

  # unzip train archive (inside the dogs-vs-cats archive)
  zip_ref = zipfile.ZipFile(os.path.join(dataset_dir, 'train.zip'), 'r')
  zip_ref.extractall(dataset_dir)
  zip_ref.close()
  
  print('\nUnzipped dataset..\n',flush=True)
  
  # remove un-needed files
  os.remove(os.path.join(dataset_dir, 'sampleSubmission.csv'))
  os.remove(os.path.join(dataset_dir, 'test1.zip'))
  os.remove(os.path.join(dataset_dir, 'train.zip'))    
  
  # make a list of all files currently in the train folder
  imageList = list()
  for (root, name, files) in os.walk(os.path.join(dataset_dir, 'train')):
      imageList += [os.path.join(root, file) for file in files]

  # separate images into 'cat and 'dog' classes
  catImages = []
  dogImages = []        
  for img in imageList:
    filename = os.path.basename(img)
    class_name,_ = filename.split('.',1)
    if class_name == 'cat':
        catImages.append(img)
    else:
        dogImages.append(img)

  print('Found',len(catImages),'cat images and',len(dogImages),'dog images.',flush=True)

  # define 80:20 train/test split
  split = int(len(dogImages) * 0.8)
  
  print('Converting train data to npz format..',flush=True)
  trainData = catImages[:split] + dogImages[:split]
  random.shuffle(trainData)
  img_to_npz(trainData,input_height,input_width,os.path.join(dataset_dir,'trainData'),False)
  
  print('Converting test data to npz format..',flush=True)
  testData = catImages[split:] + dogImages[split:]
  random.shuffle(testData)
  img_to_npz(testData,input_height,input_width,os.path.join(dataset_dir,'testData'),True)

  # move the test images into a separate folder - will be used later when creating files to run on target board
  os.mkdir(os.path.join(dataset_dir,'test_images'))
  for filename in testData:
    shutil.move(filename, os.path.join(dataset_dir,'test_images',os.path.basename(filename)))

  # delete remaining images
  shutil.rmtree(os.path.join(dataset_dir,'train'))

  return
    

def run_main():

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument('-d',  '--dataset_dir', type=str, default='./dataset', help='path to dataset root folder.')
    ap.add_argument('-ih', '--input_height',type=int, default=224,  help='Input image height in pixels.')
    ap.add_argument('-iw', '--input_width', type=int, default=224,  help='Input image width in pixels.')
    args = ap.parse_args()

    print('\n'+DIVIDER)
    print('Command line options:')
    print (' --dataset_dir  : ',args.dataset_dir)
    print (' --input_height : ',args.input_height)
    print (' --input_width  : ',args.input_width)
    print(DIVIDER,flush=True)


    create_datasets(args.dataset_dir,args.input_height,args.input_width)

if __name__ == '__main__':
    run_main()


