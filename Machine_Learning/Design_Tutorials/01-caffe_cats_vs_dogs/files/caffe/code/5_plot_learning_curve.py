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

from config import cats_vs_dogs_config as config

import os
import sys
import pandas as pd

import matplotlib
matplotlib.use('Agg')
import matplotlib.pylab as plt
plt.style.use('ggplot')

import argparse
# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-lf", "--log_file",   required=True, help="logfile")
ap.add_argument("-od", "--output_dir", required=True, help="out directory")
ap.add_argument("-im", "--img_name",   required=True, help="out image name")
args = vars(ap.parse_args())
LOGFILE = args["log_file"]
OUTDIR  = args["output_dir"]
IMGNAME = args["img_name"]

CAFFE_TOOLS_DIR = config.CAFFE_TOOLS_DIR

# ****************************************************************************************
# Generating training and test logs

command = "python " + CAFFE_TOOLS_DIR + "/tools/extra/parse_log.py " + LOGFILE + " " + OUTDIR
os.system(command)
#Read training and test logs
train_log_path = LOGFILE + '.train'
test_log_path  = LOGFILE + '.test'
# get frames of data in Pandas
train_log = pd.read_csv(train_log_path, sep=",")
test_log  = pd.read_csv(test_log_path, sep=",")

# ****************************************************************************************
# Making learning curve

fig, ax1 = plt.subplots()
#Plotting training and test losses
train_loss, = ax1.plot(train_log['NumIters'], train_log['loss'], color='red',  alpha=.5)
test_loss, = ax1.plot(test_log['NumIters'], test_log['loss'], linewidth=2, color='green')
ax1.set_ylim(ymin=0, ymax=1)
ax1.set_xlabel('Iterations', fontsize=15)
ax1.set_ylabel('Loss', fontsize=15)
ax1.tick_params(labelsize=15)
#Plotting test accuracy
ax2 = ax1.twinx()
test_accuracy, = ax2.plot(test_log['NumIters'], test_log['accuracy'], linewidth=2, color='blue')
ax2.set_ylim(ymin=0, ymax=1)
ax2.set_ylabel('Accuracy', fontsize=15)
ax2.tick_params(labelsize=15)
#Adding legend
plt.legend([train_loss, test_loss, test_accuracy], ['Training Loss', 'Test Loss', 'Test Accuracy'],  bbox_to_anchor=(1, 0.8))
plt.title('Training Curve', fontsize=18)
#Saving learning curve
plt.savefig(OUTDIR+"/"+IMGNAME)
