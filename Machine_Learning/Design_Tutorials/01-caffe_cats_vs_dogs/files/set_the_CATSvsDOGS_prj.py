#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
**

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

# USAGE
# python set_the_CATSvsDOGS_prj.py -i WORK_DIR

# Assumptions:
# 1) you are placed in WORK_DIR and the "train.zip" file is placed there too: WORK_DIR==$ML_DIR
# 2) caffe/code subdirectories are already there

# by daniele.bagni@xilinx.com

# ##################################################################################################


# import the necessary packages
import os
import argparse
import glob
import sys


# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--work_dir", required=True, help="path to the working dir")
args = vars(ap.parse_args())

path_root = args["work_dir"] # root path name of dataset


##########################################################################################

if (not os.path.exists(path_root)):
    print("ERROR: the ", path_root, "  directory does not exist!")
    sys.exit(0)

## check the kaggle_dogs_vs_cats.zip  file is there
#if (not os.path.exists(path_root + "/kaggle_dogs_vs_cats.zip ")):
#    print("ERROR: missing train.zip archive !")
#    sys.exit(0)

# create directory for input images
if (not os.path.exists(path_root + "/input")):
    os.mkdir(path_root + "/input")

## unzip the kaggle_dogs_vs_cats.zip file
os.system("unzip kaggle_dogs_vs_cats.zip -d" + path_root + "/input")

# rename "/input/train" as "input/jpg"
os.system("mv "    + path_root + "/input/kaggle_dogs_vs_cats/train " + path_root + "/input/jpg")
os.system("rm -r " + path_root + "/input/kaggle_dogs_vs_cats")

# create subdirectories "input/jpg/cats" and "input/jpg/dogs"
os.mkdir(path_root + "/input/jpg/cats")
os.mkdir(path_root + "/input/jpg/dogs")

# move all "cat.*.jpg" files into "input/jpg/cats" a
os.system("mv " + path_root + "/input/jpg/cat.*.jpg " + path_root + "/input/jpg/cats")
os.system("mv " + path_root + "/input/jpg/dog.*.jpg " + path_root + "/input/jpg/dogs")


##########################################################################################

## launch python script 1
#command1 = "python " + path_root + "/caffe/code/1_write_cats-vs-dogs_images.py -p " + path_root + "/input/jpg"
#os.system(command1)


## launch python script 2a
#command2 = "python " + path_root + "/caffe/code/2a_create_lmdb.py -i " + path_root + "/input/jpg/ -o " + path_root + "/input/lmdb"
#os.system(command2)



print(" END")
