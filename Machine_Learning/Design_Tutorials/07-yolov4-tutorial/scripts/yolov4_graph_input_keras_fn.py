# Copyright 2020 Xilinx Inc.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import cv2
from sklearn import preprocessing
import glob
import numpy as np
import os
import sys
import shutil

from PIL import Image


calib_image_dir = "/workspace/yolov4/scripts/val2017/"
calib_image_list = "/workspace/yolov4/scripts/tf_calib.txt"
calib_batch_size = 10

#normalization factor to scale image 0-255 values to 0-1 #DB
NORM_FACTOR = 255.0 # could be also 256.0




def ScaleTo1(x_test):
    x_test  = np.asarray(x_test)
    x_test = x_test.astype(np.float32)
    new_x_test = x_test / NORM_FACTOR
    return new_x_test



def eval_input(iter, eval_image_dir, eval_image_list, class_num, eval_batch_size):
  images = []
  labels = []
  line = open(eval_image_list).readlines()
  for index in range(0, eval_batch_size):
    curline = line[iter * eval_batch_size + index]
    [image_name, label_id] = curline.split(' ')
    image = cv2.imread(eval_image_dir + image_name)
    image = central_crop(image, 224, 224)
    image = mean_image_subtraction(image, MEANS)
    images.append(image)
    labels.append(int(label_id))
  lb = preprocessing.LabelBinarizer()
  lb.fit(range(0, class_num))
  labels = lb.transform(labels)
  return {"input": images, "labels": labels}




def letterbox_image(image, size):
    '''resize image with unchanged aspect ratio using padding'''
    iw, ih = image.size
    w, h = size
    scale = min(w/iw, h/ih)
    nw = int(iw*scale)
    nh = int(ih*scale)

    image = image.resize((nw,nh), Image.BICUBIC)
    new_image = Image.new('RGB', size, (128,128,128))
    new_image.paste(image, ((w-nw)//2, (h-nh)//2))
    return new_image





def calib_input(iter):
  images = []
  line = open(calib_image_list).readlines()
  for index in range(0, calib_batch_size):
    curline = line[iter * calib_batch_size + index]
    calib_image_name = curline.strip()
    filename = os.path.join(calib_image_dir, calib_image_name)
    image = cv2.imread(filename)
    size = 512,512
    custom_image = cv2.resize(image, size, interpolation=cv2.INTER_NEAREST)
    image2 = np.array(custom_image)
    image2 = ScaleTo1(image2)
    images.append(image2)
    print("Iteration number : {} and index number {} and  file name  {} ".format(iter, index, filename))
  return {"image_input": images}


#######################################################

def main():
  calib_input(0)
  calib_input(1)


if __name__ == "__main__":
    main()


