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

#import tensorflow as tf
import os, shutil
import cv2
import numpy as np
from tqdm import tqdm
import argparse
import rapidjson
import fnmatch
from pycocotools.coco import COCO
from pycocotools.cocoeval import COCOeval
import math

def reverse_coco_category(category_id):
    '''
    convert continuous coco class id (0~79) to discontinuous coco category id
    '''
    if category_id >= 0+1 and category_id <= 10+1:
        category_id = category_id - 1
    elif category_id >= 11+2 and category_id <= 23+2:
        category_id = category_id - 2
    elif category_id >= 24+3 and category_id <= 25+3:
        category_id = category_id - 3
    elif category_id >= 26+5 and category_id <= 39+5:
        category_id = category_id - 5
    elif category_id >= 40+6 and category_id <= 59+6:
        category_id = category_id - 6
    elif category_id == 60+7:
        category_id = category_id - 7
    elif category_id == 61+9:
        category_id = category_id - 9
    elif category_id >= 62+10 and category_id <= 72+10:
        category_id = category_id - 10
    elif category_id >= 73+11 and category_id <= 79+11:
        category_id = category_id - 11
    else:
        raise ValueError('Invalid category id')
    return category_id

def find(pattern, path):
    result = ''
    for root, dirs, files in os.walk(path):
        for name in files:
            if fnmatch.fnmatch(name, pattern):
                result = os.path.join(name)
    return result

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--annotations', type=str, default='/data2/datasets/coco/coco2017/annotations/instances_val2017.json',
                    required=False, help='path to the validation annotations')
    parser.add_argument('--images', type=str, default='/data2/datasets/coco/coco2017/images/val2017/',
                    required=False, help='path to the validation annotations')    
    parser.add_argument('--labels', type=str, default='/data2/datasets/coco/coco2017/labels/val2017/',
                    required=False, help='path to the validation annotations')    
    FLAGS = parser.parse_args()
    cnt = 0
    
    try:
    # Create target Directory
        os.mkdir(FLAGS.labels)
        print("Directory " , FLAGS.labels ,  " Created ") 
    except FileExistsError:
        print("Directory " , FLAGS.labels ,  " already exists, cleaning...")
        shutil.rmtree(FLAGS.labels)
        os.mkdir(FLAGS.labels)

    with open(FLAGS.annotations) as json_file:
        data = rapidjson.load(json_file)
        for i in data['annotations']:
            cnt=cnt+1
            print(cnt)
            bbox=i['bbox']
            category_id=str(reverse_coco_category(i['category_id']))
            left, top, width, height = bbox
            center_x = left + width / 2.0
            center_y = top + height / 2.0
            #find the image in the images directory and read it in to get the height and width
            image_id=str(i['image_id'])
            image_id = image_id.rjust(12, '0')
            image_file_name =image_id+'.jpg'
            filename = os.path.splitext(image_file_name)[0] #remove the extension to get just the filename
            image = cv2.imread((FLAGS.images + image_file_name),cv2.IMREAD_COLOR)
            image_h, image_w, _ = image.shape
            yolo_x = center_x / image_w
            yolo_y = center_y / image_h
            yolo_w = width / image_w
            yolo_h = height / image_h
            yolo_box = str(yolo_x) + ' ' + str(yolo_y) + ' ' + str(yolo_w) + ' ' + str(yolo_h)
            write_line = category_id + ' ' + yolo_box + '\n'
            f=open(FLAGS.labels + filename + ".txt", "a+")
            f.write(write_line)
            f.close()
    print("done")
    