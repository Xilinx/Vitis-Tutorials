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
Make the target folder
Copies images, application code and compiled xmodel to 'target'
'''

'''
Author: Mark Harvey
'''

import torch
import torchvision

import argparse
import os
import shutil
import sys
import cv2
import numpy as np
from tqdm import tqdm

from common import gen_transform


DIVIDER = '-----------------------------------------'

def generate_images(dset_dir, num_images, dest_dir):

  classes = ['zero','one','two','three','four','five','six','seven','eight','nine']

  # MNIST test dataset and dataloader
  test_dataset = torchvision.datasets.MNIST(dset_dir,
                                            train=False, 
                                            download=True,
                                            transform=gen_transform)

  test_loader = torch.utils.data.DataLoader(test_dataset,
                                            batch_size=1, 
                                            shuffle=True)

  # iterate thru' the dataset and create images
  dataiter = iter(test_loader)
  for i in tqdm(range(num_images)):
    image, label = dataiter.next()
    img = image.numpy().squeeze()
    img = (img * 255.).astype(np.uint8)
    idx = label.numpy()
    img_file=os.path.join(dest_dir, classes[idx[0]]+'_'+str(i)+'.png')
    cv2.imwrite(img_file, img)

  return


def make_target(dset_dir,target,num_images,app_dir):

    target_dir = 'target_' + target

    # remove any previous data
    shutil.rmtree(target_dir, ignore_errors=True)    
    os.makedirs(target_dir)

    # copy application code
    print('Copying application code from',app_dir,'...')
    shutil.copy(os.path.join(app_dir, 'app_mt.py'), target_dir)

    # copy compiled model
    model_path = 'compiled_model/CNN_' + target + '.xmodel'
    print('Copying compiled model from',model_path,'...')
    shutil.copy(model_path, target_dir)

    # create images
    dest_dir = target_dir + '/images'
    shutil.rmtree(dest_dir, ignore_errors=True)  
    os.makedirs(dest_dir)
    generate_images(dset_dir, num_images, dest_dir)


    return



def main():

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument('-d', '--dset_dir',   type=str,  default='dataset', help='Path to test & train datasets. Default is dataset')
    ap.add_argument('-t', '--target',     type=str,  default='zcu102', choices=['zcu102','u50','vck190'], help='Target board type (zcu102,u50,vck190). Default is zcu102')
    ap.add_argument('-n', '--num_images', type=int,  default=10000, help='Number of test images. Default is 10000')
    ap.add_argument('-a', '--app_dir',    type=str,  default='application', help='Full path of application code folder. Default is application')
    args = ap.parse_args()  

    print('\n------------------------------------')
    print(sys.version)
    print('------------------------------------')
    print ('Command line options:')
    print (' --dset_dir     : ', args.dset_dir)
    print (' --target       : ', args.target)
    print (' --num_images   : ', args.num_images)
    print (' --app_dir      : ', args.app_dir)
    print('------------------------------------\n')


    make_target(args.dset_dir, args.target, args.num_images, args.app_dir)


if __name__ ==  "__main__":
    main()

