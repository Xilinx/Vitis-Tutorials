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
CIFAR10 example using Keras
Runs for 5 epochs only

Demonstrates the 3 methods for saving Keras models

Method 1 - as a HDF5 checkpoint during training
Method 2 - as an HDF5 model (identical results to method #1)
Method 3 - as separate weights (HDF5) and architecture (JSON) files
'''

'''
Author: Mark Harvey
'''

import os
import sys
import shutil
import numpy as np
import keras
from keras import datasets, utils, layers, models, optimizers
from keras.callbacks import ModelCheckpoint



##############################################
# Set up directories
##############################################
# Returns the directory the current script (or interpreter) is running in
def get_script_directory():
    path = os.path.realpath(sys.argv[0])
    if os.path.isdir(path):
        return path
    else:
        return os.path.dirname(path)


print('\n------------------------------------')
print('Keras version      :',keras.__version__)
print('Python version     :',(sys.version))
print('------------------------------------')


SCRIPT_DIR = get_script_directory()
print('This script is located in: ', SCRIPT_DIR)

METHOD1_DIR = os.path.join(SCRIPT_DIR, 'method1')
METHOD2_DIR = os.path.join(SCRIPT_DIR, 'method2')
METHOD3_DIR = os.path.join(SCRIPT_DIR, 'method3')


# create a directory for saving if it doesn't already exist
# delete it and recreate if it already exists
if (os.path.exists(METHOD1_DIR)):
    shutil.rmtree(METHOD1_DIR)
os.makedirs(METHOD1_DIR)
print("Directory " , METHOD1_DIR ,  "created ")


if (os.path.exists(METHOD2_DIR)):
    shutil.rmtree(METHOD2_DIR)
os.makedirs(METHOD2_DIR)
print("Directory " , METHOD2_DIR ,  "created ")


if (os.path.exists(METHOD3_DIR)):
    shutil.rmtree(METHOD3_DIR)
os.makedirs(METHOD3_DIR)
print("Directory " , METHOD3_DIR ,  "created ")



#####################################################
# Hyperparameters
#####################################################
BATCHSIZE = 25
EPOCHS = 5
LEARN_RATE = 0.0001
DECAY_RATE = 1e-6


##############################################
# Preparation of input dataset
##############################################
# CIFAR10 datset has 60k images. Training set is 50k, test set is 10k.
# Each image is 32x32 pixels RGB
(X_train, Y_train), (X_test, Y_test) = datasets.cifar10.load_data()

# Scale image data from range 0:255 to range 0:1
X_train = X_train / 255.0
X_test = X_test / 255.0


# one-hot encode the labels
Y_train = utils.to_categorical(Y_train)
Y_test = utils.to_categorical(Y_test)



##############################################
# Simple custom CNN Keras functional model
##############################################
inputs = layers.Input(shape=(32, 32, 3))
net = layers.Conv2D(32, kernel_size=(3, 3), padding='same')(inputs)
net = layers.Activation('relu')(net)
net = layers.BatchNormalization()(net)
net = layers.MaxPooling2D(pool_size=(2,2))(net)

net = layers.Conv2D(64, kernel_size=(3, 3), padding='same')(net)
net = layers.Activation('relu')(net)
net = layers.BatchNormalization()(net)
net = layers.MaxPooling2D(pool_size=(2,2))(net)

net = layers.Flatten()(net)
net = layers.Dropout(0.4)(net)
net = layers.Dense(512)(net)
net = layers.Activation('relu')(net)

net = layers.Dropout(0.4)(net)
net = layers.Dense(10)(net)
prediction = layers.Activation('softmax')(net)

model = models.Model(inputs=inputs, outputs=prediction)


##############################################
# Compile model
##############################################
model.compile(loss='categorical_crossentropy', 
              optimizer=optimizers.Adam(lr=LEARN_RATE, decay=DECAY_RATE),
              metrics=['accuracy']
              )

##############################################
# Method #1 - save a checkpoint during training 
##############################################
# set up the checkpoint callback
chkpt_callback = ModelCheckpoint(os.path.join(METHOD1_DIR,'keras_chkpt.h5'),
                                 monitor='val_accuracy',
                                 verbose=1,
                                 save_best_only=True,
                                 mode='max')

callback_list = [chkpt_callback]

# Train model with training set
model.fit(X_train,
          Y_train,
          batch_size=BATCHSIZE,
          shuffle=True,
          epochs=EPOCHS,
          validation_data=(X_test, Y_test),
          callbacks = callback_list
          )

##############################################
# Evaluate model accuracy with test set
##############################################
scores = model.evaluate(X_test, 
                        Y_test,
                        batch_size=BATCHSIZE
                        )

print('Loss: %.3f' % scores[0])
print('Accuracy: %.3f' % scores[1])



##############################################
# Method #2 - as single HDF5 file
##############################################
# save weights, model architecture & optimizer to an HDF5 format file
model.save(os.path.join(METHOD2_DIR,'k_complete_model.h5'))


##############################################
# Method #3 - as JSON and HDF5 files
##############################################
# save just the weights (no architecture) to an HDF5 format file
model.save_weights(os.path.join(METHOD3_DIR,'k_model_weights.h5'))

# save just the architecture (no weights) to a JSON file
with open(os.path.join(METHOD3_DIR,'k_model_architecture.json'), 'w') as f:
    f.write(model.to_json())


print ('FINISHED!')



