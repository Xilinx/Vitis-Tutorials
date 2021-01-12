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


import cv2, os
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import sys, time, warnings
from datetime import datetime #DB

import gc #DB
from config import fcn_config as cfg
from config import fcn8_cnn as cnn
from sklearn.utils import shuffle
from config import unet as unet #DB

import pandas as pd
warnings.filterwarnings("ignore")

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
## Import usual libraries
import tensorflow as tf

from tensorflow.keras import optimizers
from tensorflow.keras.backend               import set_session
from tensorflow.keras                       import backend
from tensorflow.keras.utils                 import plot_model #DB
from tensorflow.keras.preprocessing.image   import ImageDataGenerator #DB
from tensorflow.keras.models                import *
from tensorflow.keras.layers                import *
from tensorflow.keras.optimizers            import RMSprop, SGD

# workaround for TF1.15 bug "Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR"
os.environ['TF_FORCE_GPU_ALLOW_GROWTH'] = 'true'

os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"
config = tf.compat.v1.ConfigProto()
config.gpu_options.per_process_gpu_memory_fraction = 0.85
#config.gpu_options.allow_growth = True
config.gpu_options.visible_device_list = "0"
set_session(tf.compat.v1.Session(config=config))

import argparse #DB
# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-m",  "--model", default=False, help="models: 1, 2 or 3")
args = vars(ap.parse_args())
model_type= int(args["model"])
print("UNET MODEL = ", model_type)


HEIGHT = cfg.HEIGHT
WIDTH  = cfg.WIDTH
N_CLASSES = cfg.NUM_CLASSES

BATCH_SIZE = cfg.BATCH_SIZE
EPOCHS = cfg.EPOCHS

######################################################################
# directories
######################################################################

dir_data = cfg.DATASET_DIR
dir_train_img = cfg.dir_train_img
dir_train_seg = cfg.dir_train_seg
dir_test_img  = cfg.dir_test_img
dir_test_seg  = cfg.dir_test_seg
dir_calib_img = cfg.dir_calib_img
dir_calib_seg = cfg.dir_calib_seg
dir_valid_img = cfg.dir_valid_img
dir_valid_seg = cfg.dir_valid_seg


######################################################################
# model a
######################################################################

if (model_type==1):
        model = unet.UNET_v1(N_CLASSES, HEIGHT, WIDTH)
elif (model_type==2):
        model = unet.UNET_v2(N_CLASSES, HEIGHT, WIDTH)
else:
        model = unet.UNET_v3(N_CLASSES, HEIGHT, WIDTH)

model.summary()

# plot the CNN model
plot_model(model, to_file="../rpt/unet_model" +str(model_type) + "_"  + str(WIDTH) + "x" + str(HEIGHT) + ".png", show_shapes=True)

######################################################################
# prepare training and validation data
######################################################################

# load training images
train_images = os.listdir(dir_train_img)
train_images.sort()
train_segmentations  = os.listdir(dir_train_seg)
train_segmentations.sort()
X_train = []
Y_train = []

for im , seg in zip(train_images,train_segmentations) :
	X_train.append( cnn.NormalizeImageArr(os.path.join(dir_train_img,im) ))
	Y_train.append( cnn.LoadSegmentationArr( os.path.join(dir_train_seg,seg) , N_CLASSES , WIDTH, HEIGHT)  )

X_train, Y_train = np.array(X_train), np.array(Y_train)
print(X_train.shape,Y_train.shape)


X_train, Y_train = shuffle(X_train, Y_train)

# load validation images
valid_images = os.listdir(dir_valid_img)
valid_images.sort()
valid_segmentations  = os.listdir(dir_valid_seg)
valid_segmentations.sort()
X_valid = []
Y_valid = []
for im , seg in zip(valid_images,valid_segmentations) :
    X_valid.append( cnn.NormalizeImageArr(os.path.join(dir_valid_img,im)) )
    Y_valid.append( cnn.LoadSegmentationArr( os.path.join(dir_valid_seg,seg) , N_CLASSES , WIDTH, HEIGHT)  )
X_valid, Y_valid = np.array(X_valid) , np.array(Y_valid)
print(X_valid.shape,Y_valid.shape)

X_valid, Y_valid = shuffle(X_valid, Y_valid)


#########################################################################################################
# Training starts here
#########################################################################################################


sgd = SGD(lr=1E-2, decay=5**(-4), momentum=0.9, nesterov=True)

model.compile(loss='categorical_crossentropy',
              optimizer=sgd,
              metrics=['accuracy'])

callbacks_list = []

startTime1 = datetime.now() #DB
hist1 = model.fit(X_train,Y_train, validation_data=(X_valid,Y_valid), batch_size=BATCH_SIZE,epochs=EPOCHS,verbose=2)
endTime1 = datetime.now()
diff1 = endTime1 - startTime1
print("\n")
print("Elapsed time for Keras training (s): ", diff1.total_seconds())
print("\n")


for key in ["loss", "val_loss"]:
    plt.plot(hist1.history[key],label=key)
plt.legend()
plt.savefig("../rpt/unet_model" + str(model_type) + "_training_curves_" + str(WIDTH) + "x" + str(HEIGHT) + ".png")

model.save("../keras_model/unet/ep" + str(EPOCHS) + "_trained_unet_model" + str(model_type) + "_" + str(WIDTH) + "x" + str(HEIGHT) + ".hdf5")

print("\nEnd of UNET training\n")
