#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
## © Copyright (C) 2016-2020 Xilinx, Inc
##
## Licensed under the Apache License, Version 2.0 (the "License"). You may
## not use this file except in compliance with the License. A copy of the
## License is located at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
## WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
## License for the specific language governing permissions and limitations
## under the License.
'''
# Author: Daniele Bagni, Xilinx Inc
# date 6 May 2021

import cv2
import os
import numpy as np

#from tensorflow.keras.preprocessing.image import img_to_array
from config import cifar10_config as cfg


calib_image_dir  = cfg.CALIB_DIR
calib_image_list = os.path.join(calib_image_dir,"calib_list.txt")
print("script running on folder ", cfg.SCRIPT_DIR)
print("CALIB DIR ", calib_image_dir)

calib_batch_size = 50
line = open(calib_image_list).readlines()
tot_num_images = len(line)

def calib_input(iter):
  assert(int(iter)<=int(tot_num_images/calib_batch_size)),"number of iterations must be <=20"
  images = []
  #print(line)
  for index in range(0, calib_batch_size):
      #print(iter-1, index)
      curline = line[(iter-1) * calib_batch_size + index]
      #print(curline)
      calib_image_name = curline.strip()

      # read image as rgb, returns numpy array (32,32, 3)
      filename = os.path.join(calib_image_dir, calib_image_name)
      image = cv2.imread(filename)
      ##swap channels: from BGR to RGB
      #B, G, R = cv2.split(image)
      #image = cv2.merge([R, G, B])

      #img_array = img_to_array(image, data_format=None)

      # scale the pixel values to range 0 to 1.0
      image2 = cfg.Normalize(image)

      # reshape numpy array to be (32,32,3)
      image2 = image2.reshape((image2.shape[0], image2.shape[1], 3))
      images.append(image2)

  return {"conv2d_1_input": images}


#######################################################

def main():
  calib_input(20)


if __name__ == "__main__":
    main()
