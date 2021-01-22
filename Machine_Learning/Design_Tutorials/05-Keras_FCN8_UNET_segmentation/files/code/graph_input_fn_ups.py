#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
/**

* © Copyright (C) 2016-2020 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/
'''

# modified by daniele.bagni@xilinx.com
# date 20 / 11 / 2020



import cv2
import os
import numpy as np

## Silence TensorFlow messages
#os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
#import tensorflow as tf
#from tensorflow.keras.preprocessing.image import img_to_array
from config import fcn_config as cfg
from config import fcn8_cnn as cnn


calib_image_dir  = cfg.dir_calib_img
calib_image_list = os.path.join(calib_image_dir,"../calib_list.txt")
print("script running on folder ", cfg.SCRIPT_DIR)
print("CALIB DIR ", calib_image_dir)

calib_batch_size = 10

def calib_input(iter):
  images = []
  line = open(calib_image_list).readlines()
  #print(line)
  for index in range(0, calib_batch_size):
      curline = line[iter*calib_batch_size + index]
      #print("iter= ", iter, "index= ", index, "sum= ", int(iter*calib_batch_size + index), "curline= ", curline)
      calib_image_name = curline.strip()

      image_path = os.path.join(calib_image_dir, calib_image_name)
      image2 = cnn.NormalizeImageArr(image_path)

      #image2 = image2.reshape((image2.shape[0], image2.shape[1], 3))
      images.append(image2)

  return {"input_1": images}


#######################################################

def main():
  calib_input()


if __name__ == "__main__":
    main()
