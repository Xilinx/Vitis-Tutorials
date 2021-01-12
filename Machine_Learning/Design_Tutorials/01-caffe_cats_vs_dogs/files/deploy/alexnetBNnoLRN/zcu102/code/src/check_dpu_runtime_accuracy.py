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
# python3 ~/ML/cats-vs-dogs/caffe/code/check_dpu_runtime_accuracy.py -i ~/ML/cats-vs-dogs/deephi//quantiz/zcu102/rpt/logfile_top2_alexnetBNnoLRN.txt

# It checks the top-1 and top-2 accuracy obtained at runtime by DeePhi DPU, by analysis of the related logfile


# by daniele.bagni@xilinx.com

# ##################################################################################################

import warnings
warnings.filterwarnings("ignore", message="numpy.dtype size changed")
warnings.filterwarnings("ignore", message="numpy.ufunc size changed")
import numpy as np

from datetime import datetime
import os
import sys
import argparse

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--file", required=True, help="input logfile")
ap.add_argument("-n", "--numel", default="1000", help="number of test images")
args = vars(ap.parse_args())

logfile = args["file"] # root path name of dataset

try:
    f = open(logfile, "r")
except IOError:
    print("cannot open ", logfile)
else:
    lines = f.readlines()
    tot_lines = len(lines)
    print(logfile, " has ", tot_lines, " lines")

#f.seek(0)
f.close()

# ##################################################################################################

NUMEL =  int(args["numel"]) #1000

# initialize the label names for the CATS-vs-DOGS dataset
labelNames = {"cat" : 0, "dog" : 1 }

# ##################################################################################################

top1_true  = 0
top1_false = 0
img_count  = 0
false_pred = 0

test_ids = np.zeros(([NUMEL,1]))
preds    = np.zeros(([NUMEL, 1]))
idx = 0

for ln in range(0, tot_lines):

    if "Image" in lines[ln]:

        top2_lines = lines[ln:ln+3]

        filename= top2_lines[0].split("Image :")[1]
        s2 = filename.index(".")
        class_name = filename[: s2].strip()
        #print("DBG: found class ", class_name, " in line ", ln, ": ", lines[ln])

        predicted = top2_lines[1].split("name = ")[1].strip()

        if class_name in top2_lines[1]:
            top1_true += 1
        elif class_name in top2_lines[2]:
            top1_false +=1
        else:
            print("ERROR: no class detected\n")
            sys.exit(0)


        test_ids[idx] = labelNames[class_name] # ground truth
        preds[idx]    = labelNames[predicted ] # actual prediction

        if (predicted != class_name) :
            print("LINE: ", top2_lines[0].split(":")[1].strip())
            print("PREDICTED: ", preds[idx], predicted)
            print("EXPECTED : ", test_ids[idx], class_name)
            for k in range(1, 3):
                print(top2_lines[k].strip())
            print("\n")


        img_count +=1
        idx += 1

        if ( idx == (NUMEL-1) ):
            break


    else:
        continue



assert (top1_true+top1_false)  == img_count, "ERROR: top1 true+false not equal to the number of images"

print("number of total images predicted ", img_count)
print("number of top1 false predictions ", top1_false)
print("number of top1 right predictions ", top1_true)

top1_accuracy = float(top1_true)/(top1_true+top1_false)
print("top1 accuracy = %.2f" % top1_accuracy)
