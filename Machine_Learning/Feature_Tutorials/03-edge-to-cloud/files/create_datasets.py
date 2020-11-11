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

The script will create a folder structure compatible with Keras's 
ImageGenerator.flow_from_directory():

dataset
   |_dogs-vs-cats
       |_test
       |   |_cats
       |   |_dogs
       |_train
       |   |_cats
       |   |_dogs
       |_valid
           |_cats
           |_dogs

'''

'''
Author: Mark Harvey
'''


import os
import argparse
import zipfile
import random
import cv2
import shutil

DIVIDER = '-----------------------------------------'


# delete list of directories
def delete_create_dir(dir_list):
    for dir in dir_list: 
        if (dir != '' and os.path.exists(dir)):
            shutil.rmtree(dir)
        if (dir != ''):
            os.makedirs(dir)
    return


def resize_maintain_aspect(image, target_w, target_h):
    orig_h = image.shape[0]
    orig_w = image.shape[1]

    #landscape
    if (orig_w > orig_h):
        new_h = target_h
        new_w = int(orig_w * (target_h/orig_h))
    # portrait
    elif (orig_h > orig_w):
        new_h = int(orig_h * (target_w/orig_w))
        new_w = target_w   
    # square
    elif (target_h > target_w):
        new_h = target_h
        new_w = int(orig_w * (target_h/orig_h))
    else:
        new_h = int(orig_h * (target_w/orig_w))
        new_w = target_w

    if new_w < target_w:
        new_h = int(new_h*(target_w/new_w))
        new_w = target_w

    # resize the image
    resized = cv2.resize(image, (new_w,new_h), interpolation = cv2.INTER_CUBIC)

    # return the resized image
    return resized


def center_crop(image,out_height,out_width):
  image_height, image_width = image.shape[:2]
  offset_height = (image_height - out_height) // 2
  offset_width = (image_width - out_width) // 2
  image = image[offset_height:offset_height+out_height, offset_width:offset_width+out_width,:]
  return image



def create_datasets(dataset_dir,overwrite,input_height,input_width):


    if (overwrite=='true') or not(os.path.exists(dataset_dir)):    
    
        # train, validation and test folders
        TRAIN_DIR = os.path.join(dataset_dir, 'train')
        VALID_DIR = os.path.join(dataset_dir, 'valid')
        TEST_DIR = os.path.join(dataset_dir, 'test')

        # class folders
        TRAIN_CAT_DIR = os.path.join(TRAIN_DIR, 'cat')
        TRAIN_DOG_DIR = os.path.join(TRAIN_DIR, 'dog')
        VALID_CAT_DIR = os.path.join(VALID_DIR, 'cat')
        VALID_DOG_DIR = os.path.join(VALID_DIR, 'dog')
        TEST_CAT_DIR = os.path.join(TEST_DIR, 'cat')
        TEST_DOG_DIR = os.path.join(TEST_DIR, 'dog')

        # remove any previous data
        dir_list = [dataset_dir]
        delete_create_dir(dir_list)
    
    
        # unzip the dogs-vs-cats archive that was downloaded from Kaggle
        zip_ref = zipfile.ZipFile('./dogs-vs-cats.zip', 'r')
        zip_ref.extractall(dataset_dir)
        zip_ref.close()

        # unzip train archive (inside the dogs-vs-cats archive)
        zip_ref = zipfile.ZipFile(os.path.join(dataset_dir, 'train.zip'), 'r')
        zip_ref.extractall(dataset_dir)
        zip_ref.close()
    
        print('\nUnzipped dataset..\n')
    
        # make all necessary folders
        dir_list = [VALID_DIR,TEST_DIR,TRAIN_CAT_DIR,TRAIN_DOG_DIR, \
                    VALID_CAT_DIR,VALID_DOG_DIR,TEST_CAT_DIR,TEST_DOG_DIR]

        for dir in dir_list: 
            os.makedirs(dir)
            print("Directory " , dir ,  "created ")
    
        # remove un-needed files
        os.remove(os.path.join(dataset_dir, 'sampleSubmission.csv'))
        os.remove(os.path.join(dataset_dir, 'test1.zip'))
        os.remove(os.path.join(dataset_dir, 'train.zip'))
    
    
        # make a list of all files currently in the train folder
        imageList = list()
        for (root, name, files) in os.walk(TRAIN_DIR):
            imageList += [os.path.join(root, file) for file in files]

        # shuffle the list of images
        random.shuffle(imageList)

        # open each image, resize and save
        for img in imageList:
            image = cv2.imread(img)
            scale_height = input_height + 20
            scale_width = input_width + 20
            image = resize_maintain_aspect(image,scale_width,scale_height)

            if (image.shape[0] != 220 and image.shape[1] != 270):
                print ('ERROR_0:',img, image.shape)               

            image = center_crop(image,input_height,input_width)

            if (image.shape[0] != 200 or image.shape[1] != 250):
                print ('ERROR_1:',img, image.shape)

            cv2.imwrite(img, image)


        # make lists of images according to their class
        catImages=list()
        dogImages=list()

        for img in imageList:
            filename = os.path.basename(img)
            class_name,_ = filename.split('.',1)
            if class_name == 'cat':
                catImages.append(img)
            else:
                dogImages.append(img)


        # define train, valid, test split as 70:20:10
        testImages = int(len(dogImages) * 0.1)
        validImages = int(len(dogImages) * 0.3)


        # move the images to their class folders inside train, valid, test
        for i in range(0,testImages):
            filename_d = os.path.basename(dogImages[i])
            filename_c = os.path.basename(catImages[i])
            os.rename(dogImages[i], os.path.join(TEST_DOG_DIR, filename_d))
            os.rename(catImages[i], os.path.join(TEST_CAT_DIR, filename_c))

        for i in range(testImages,validImages):
            filename_d = os.path.basename(dogImages[i])
            filename_c = os.path.basename(catImages[i])
            os.rename(dogImages[i], os.path.join(VALID_DOG_DIR, filename_d))
            os.rename(catImages[i], os.path.join(VALID_CAT_DIR, filename_c))

        for i in range(validImages,len(dogImages)):
            filename_d = os.path.basename(dogImages[i])
            filename_c = os.path.basename(catImages[i])
            os.rename(dogImages[i], os.path.join(TRAIN_DOG_DIR, filename_d))
            os.rename(catImages[i], os.path.join(TRAIN_CAT_DIR, filename_c))


        # run a check on number of files in each class folder
        dir_list = [TEST_DOG_DIR,TEST_CAT_DIR,VALID_DOG_DIR,VALID_CAT_DIR,TRAIN_DOG_DIR,TRAIN_CAT_DIR]
        for dir in dir_list: 
            file_count = sum([len(files) for root,dir,files in os.walk(dir)])
            print('Number of Files in', dir,': ',file_count)
    
    
        print('\nDATASET PREPARATION COMPLETED')
        print(DIVIDER,'\n')

    else:
        print('Dataset already exists and --overwrite option is',overwrite)
        print(DIVIDER,'\n')

    return
    



def run_main():

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()

    ap.add_argument('-d', '--dataset_dir',
                    type=str,
                    default='./dataset',
    	            help='path to dataset root folder')
    ap.add_argument('-o', '--overwrite',
                    type=str,
                    default='true',
                    choices=['true','false'],
    	            help='overwrite existing dataset - true or false.')
    ap.add_argument('-ih', '--input_height',
                    type=int,
                    default='200',
    	            help='Input image height in pixels.')
    ap.add_argument('-iw', '--input_width',
                    type=int,
                    default='250',
    	            help='Input image width in pixels.')

    args = ap.parse_args()

    print('\n'+DIVIDER)
    print('DATASET PREPARATION STARTED..')

    print('Command line options:')
    print (' --dataset_dir  : ',args.dataset_dir)
    print (' --overwrite    : ',args.overwrite)
    print (' --input_height : ',args.input_height)
    print (' --input_width  : ',args.input_width)


    create_datasets(args.dataset_dir,args.overwrite,args.input_height,args.input_width)

if __name__ == '__main__':
    run_main()



