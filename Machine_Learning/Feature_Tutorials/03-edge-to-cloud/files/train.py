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
Training script for dogs-vs-cats tutorial.
'''

'''
Author: Mark Harvey
'''

import numpy as np
import os
import sys
import argparse


os.environ["CUDA_DEVICE_ORDER"]='PCI_BUS_ID'
 
# indicate which GPU to use
os.environ["CUDA_VISIBLE_DEVICES"]='1'

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL']='3'

# workaround for TF1.15 bug "Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR"
os.environ['TF_FORCE_GPU_ALLOW_GROWTH']='true'


import tensorflow as tf
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.losses import categorical_crossentropy
from tensorflow.keras.callbacks import TensorBoard,ModelCheckpoint,LearningRateScheduler,ReduceLROnPlateau
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.models import load_model

from customcnn import customcnn


DIVIDER = '-----------------------------------------'



def train(input_height,input_width,input_chan,dataset,batchsize,learnrate,epochs,chkpt_dir,aug,tboard):


    def step_decay(epoch):
        """
        Learning rate scheduler used by callback
        Reduces learning rate depending on number of epochs
        """
        lr = learnrate
        if epoch > 150:
            lr /= 1000
        elif epoch > 120:
            lr /= 100
        elif epoch > 80:
            lr /= 10
        elif epoch > 40:
            lr /= 4
        return lr


    '''
    Set up directories & files
    '''
    TRAIN_DIR = os.path.join(dataset, 'train')
    VALID_DIR = os.path.join(dataset, 'valid')
    TEST_DIR = os.path.join(dataset, 'test')


    '''
    Define the model
    '''
    model = customcnn(input_shape=(input_height, input_width, input_chan),classes=2,filters=[8,16,32,64,128])

    print('\n'+DIVIDER)
    print(' Model Summary')
    print(DIVIDER)
    print(model.summary())
    print("Model Inputs: {ips}".format(ips=(model.inputs)))
    print("Model Outputs: {ops}".format(ops=(model.outputs)))


    '''
    Input image pipeline for training, validation, prediction

    data augmentation for training & validation
      - pixel data is rescaled from 0:225 to 0:1.0
      - random rotation of 5° max
      - random horiz flip (images are flipped along vertical axis)
      - random linear shift up and down by (200*01) pixels
    '''

    # data generation for training
    datagen_t = ImageDataGenerator(rotation_range=8,
                                    horizontal_flip=True,
                                    height_shift_range=0.1,
                                    width_shift_range=0.1,
                                    rescale=1/255
                                    )

    # data generation for validation & prediction - only rescaling
    datagen_vp = ImageDataGenerator(rescale=1/255)


    # train generator takes images from the specified directory, applies
    # a resize (if required) with bilinear interpolation.
    train_generator = datagen_t.flow_from_directory(TRAIN_DIR,
                                                     target_size=(input_height, input_width),
                                                     interpolation='bilinear',
                                                     batch_size=batchsize,
                                                     class_mode='categorical',
                                                     color_mode='rgb',
                                                     shuffle=True,
                                                     seed=42
                                                     )

    validation_generator = datagen_vp.flow_from_directory(VALID_DIR,
                                                          target_size=(input_height, input_width),
                                                          interpolation='bilinear',
                                                          batch_size=batchsize,
                                                          class_mode='categorical',
                                                          color_mode='rgb',
                                                          shuffle=True,
                                                          save_to_dir=aug
                                                          )


    prediction_generator = datagen_vp.flow_from_directory(TEST_DIR,
                                                         target_size=(input_height, input_width),
                                                         interpolation='bilinear',
                                                         batch_size=1,
                                                         class_mode='categorical',
                                                         color_mode='rgb',
                                                         shuffle=False)

    '''
    Call backs
    '''

    tb_call = TensorBoard(log_dir=tboard,
                          batch_size=batchsize)

    chkpt_call = ModelCheckpoint(filepath=os.path.join(chkpt_dir,'k_model.h5'), 
                                 monitor='val_acc',
                                 verbose=1,
                                 save_best_only=True)


    lr_scheduler_call = LearningRateScheduler(schedule=step_decay,
                                              verbose=1)

    lr_plateau_call = ReduceLROnPlateau(factor=np.sqrt(0.1),
                                        cooldown=0,
                                        patience=5,
                                        min_lr=0.5e-6)

    callbacks_list = [tb_call, lr_scheduler_call, lr_plateau_call, chkpt_call]


    '''
    Compile model
    Adam optimizer to change weights & biases
    Loss function is categorical crossentropy
    '''

    model.compile(optimizer=Adam(lr=learnrate),
                  loss='categorical_crossentropy',
                  metrics=['accuracy'])


    '''
    Training
    '''

    print('\n'+DIVIDER)
    print(' Training model with training set..')
    print(DIVIDER)

    # calculate number of steps in one training epoch
    train_steps = train_generator.n//train_generator.batch_size

    # calculate number of steps in one validation epoch
    val_steps = validation_generator.n//validation_generator.batch_size

    # run training
    train_history=model.fit_generator(generator=train_generator,
                                      epochs=epochs,
                                      steps_per_epoch=train_steps,
                                      validation_data=validation_generator,
                                      validation_steps=val_steps,
                                      callbacks=callbacks_list,
                                      shuffle=True,
                                      verbose=1,
                                      initial_epoch=0,
                                      max_queue_size=50,
                                      workers=8,
                                      use_multiprocessing=True)



    print("\nTensorBoard can be opened with the command: tensorboard --logdir={dir} --host localhost --port 6006".format(dir=tboard))



    '''
    Evaluation
    '''

    print('\n'+DIVIDER)
    print(' Evaluate model accuracy with validation set..')
    print(DIVIDER)
    scores = model.evaluate_generator(generator=validation_generator,
                                      max_queue_size=10,
                                      steps=val_steps,
                                      verbose=1)

    print ('Evaluation Loss    : ', scores[0])
    print ('Evaluation Accuracy: ', scores[1])



    '''
    Predictions
    '''

    print('\n'+DIVIDER)
    print(' Test predictions accuracy with test dataset..')
    print(DIVIDER)

    # reset the generator before using it for predictions
    prediction_generator.reset()

    # get a list of image filenames used in prediction
    filenames = prediction_generator.filenames

    # predict generator returns a list of all predictions
    pred = model.predict_generator(generator=prediction_generator,
                                   steps=prediction_generator.n,
                                   verbose=1)

    # the .class_indices attribute returns a dictionary with keys = classes
    labels = (prediction_generator.class_indices)

    # make a new dictionary with keys & values swapped 
    labels = dict((v,k) for k,v in labels.items())

    # use the 'labels dictionary to create a list of predictions as strings
    # predictions is a list of softmax outputs
    predictions = []
    for i in range(len(pred)):
        predictions.append(labels[np.argmax(pred[i])])

    # iterate over the list of predictions and compare to ground truth labels
    # ground truth labels are derived from prediction filenames.
    correct_predictions = 0
    wrong_predictions = 0

    for i in range(len(predictions)):

        # ground truth is first part of filename (i.e. the class folder)
        ground_truth = os.path.split(filenames[i])

        # compare prediction to ground truth
        if predictions[i] == ground_truth[0]:
            correct_predictions += 1
        else:
            wrong_predictions += 1

    # calculate absolute accuracy
    acc = (correct_predictions/len(predictions)) * 100

    print('Correct Predictions: ',correct_predictions)
    print('Wrong Predictions  : ',wrong_predictions)
    print('Prediction Accuracy: ',acc,'%')


    return




def run_main():
    
    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument('-ih', '--input_height',
                    type=int,
                    default='200',
    	            help='Input image height in pixels.')
    ap.add_argument('-iw', '--input_width',
                    type=int,
                    default='250',
    	            help='Input image width in pixels.')
    ap.add_argument('-ic', '--input_chan',
                    type=int,
                    default='3',
    	            help='Number of input image channels.')
    ap.add_argument('-d', '--dataset',
                    type=str,
                    default='./dataset',
    	            help='path to dataset root')
    ap.add_argument('-b', '--batchsize',
                    type=int,
                    default=25,
    	            help='Training batchsize. Must be an integer. Default is 25.')
    ap.add_argument('-e', '--epochs',
                    type=int,
                    default=100,
    	            help='number of training epochs. Must be an integer. Default is 100.')
    ap.add_argument('-lr', '--learnrate',
                    type=float,
                    default=0.0001,
    	            help='optimizer learning rate. Must be floating-point value. Default is 0.0001')
    ap.add_argument('-cf', '--chkpt_dir',
                    type=str,
                    default='keras_model',
    	            help='Path and name of folder for storing Keras checkpoints. Default is keras_model')
    ap.add_argument('-a', '--aug',
                    type=str,
                    default='',
    	            help='path to folder for saving augmented images. Default is empty string to indicate no saving of images')
    ap.add_argument('-tb', '--tboard',
                    type=str,
                    default='./tb_logs',
    	            help='path to folder for saving TensorBoard data. Default is ./tb_logs.')
    args = ap.parse_args()

    print('\n'+DIVIDER)
    print('Keras version      : ',tf.keras.__version__)
    print('TensorFlow version : ',tf.__version__)
    print(sys.version)
    print(DIVIDER)
    print(' Command line options:')
    print ('--input_height : ',args.input_height)
    print ('--input_width  : ',args.input_width)
    print ('--input_chan   : ',args.input_chan)
    print ('--dataset      : ',args.dataset)
    print ('--batchsize    : ',args.batchsize)
    print ('--learnrate    : ',args.learnrate)
    print ('--epochs       : ',args.epochs)
    print ('--chkpt_dir    : ',args.chkpt_dir)
    print ('--aug          : ',args.aug)
    print ('--tboard       : ',args.tboard)
    print(DIVIDER)
    

    train(args.input_height,args.input_width,args.input_chan,args.dataset, \
          args.batchsize,args.learnrate,args.epochs,args.chkpt_dir,args.aug,args.tboard)


if __name__ == '__main__':
    run_main()

