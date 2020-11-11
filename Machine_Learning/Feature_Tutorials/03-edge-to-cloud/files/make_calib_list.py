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
Makes a text file with a complete list of all training images
Text file will be used in calibration phase of quantization.
'''

'''
Author: Mark Harvey
'''

import os
import argparse
import random



def make_list(dataset, calib_list_file, num_images):

    TRAIN_DIR = os.path.join(dataset, 'train')

    # create file for list of calibration images
    f = open(calib_list_file, 'w')
        
    # make a list of the files
    imageList = list()
    for (root, name, files) in os.walk(TRAIN_DIR):
        imageList += [os.path.join(root, file) for file in files]

    # shuffle the list
    random.seed(17)
    random.shuffle(imageList)

    for i in range(num_images):
        f.write(imageList[i]+'\n')

    f.close()

    print ('FINISHED GENERATING LIST OF CALIB IMAGES')

    return


def run_main():

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument('-d', '--dataset',
                    type=str,
                    default='./dataset',
    	            help='path to dataset')
    ap.add_argument('-cf', '--calib_list_file',
                    type=str,
                    default='./calib_list.txt',
    	            help='Path & filename of calibration images list')
    ap.add_argument('-n', '--num_images',
                    type=int,
                    default=1000,
    	            help='Number of images to use in calibration')
    args = ap.parse_args()


    print('\n----------------------------------------------')
    print(' Command line options:')
    print ('--dataset:       :', args.dataset)
    print ('--calib_list_file:', args.calib_list_file)
    print ('--num_images     :', args.num_images)
    print('----------------------------------------------')


    make_list(args.dataset, args.calib_list_file, args.num_images)


if __name__ == '__main__':
    run_main()



