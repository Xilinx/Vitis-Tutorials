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
Author: Mark Harvey
'''

import os
import sys
import argparse
import numpy as np


# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

# workaround for TF1.15 bug "Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR"
os.environ['TF_FORCE_GPU_ALLOW_GROWTH'] = 'true'

import tensorflow as tf
tf.compat.v1.logging.set_verbosity(tf.compat.v1.logging.ERROR)

from customcnn import customcnn
from datadownload import datadownload


def calc_acc(testdata,testlabels,predictions):
    '''
    Accuracy calculation
    '''
    correct_predictions = 0
    wrong_predictions = 0
    for i in range(len(testdata)):
        if (predictions[i] == np.argmax(testlabels[i])):
           correct_predictions += 1
        else:
            wrong_predictions += 1

    # calculate accuracy
    acc = (correct_predictions/len(testdata)) * 100

    return acc


def train(input_height,input_width,input_chan,epochs,learnrate, \
          batchsize,output_ckpt_path,infer_graph_path,tboard_path):
    
    # Set up directories and files
    INFER_GRAPH_DIR = os.path.dirname(infer_graph_path)
    INFER_GRAPH_FILENAME =os.path.basename(infer_graph_path)

    # dataset download and preparation
    (x_train, y_train), (x_test, y_test) = datadownload()    


    # calculate total number of batches per epoch
    total_batches = int(len(x_train)/batchsize)


    #####################################################
    # Create the Computational graph
    #####################################################

    # define placeholders for the input images, labels, training mode
    images_in = tf.compat.v1.placeholder(tf.float32, shape=[None,input_height,input_width,input_chan], name='images_in')
    labels = tf.compat.v1.placeholder(tf.int32, shape=[None,10], name='labels')
    train = tf.compat.v1.placeholder_with_default(False, shape=None, name='train')

    # build the network, input comes from the 'images_in' placeholder
    # training mode is also driven by placeholder
    logits = customcnn(cnn_in=images_in, is_training=train)

    # softmax cross entropy loss function - needs one-hot encoded labels
    loss = tf.compat.v1.reduce_mean(tf.compat.v1.losses.softmax_cross_entropy(logits=logits, onehot_labels=labels))

    # Adaptive Momentum optimizer - minimize the loss
    update_ops = tf.compat.v1.get_collection(tf.compat.v1.GraphKeys.UPDATE_OPS)
    optimizer = tf.compat.v1.train.AdamOptimizer(learning_rate=learnrate)
    with tf.compat.v1.control_dependencies(update_ops):
        train_op = optimizer.minimize(loss)

    # accuracy calculation during training:
    # Check to see if the predictions matches the labels and then
    # calculate accuracy as mean of the correct predictions
    predicted_logit = tf.compat.v1.argmax(input=logits, axis=1, output_type=tf.int32)
 
    # TensorBoard data collection
    tf.compat.v1.summary.scalar('cross_entropy_loss', loss)
    tf.compat.v1.summary.image('input_images', images_in)    

    # set up saver object
    saver = tf.compat.v1.train.Saver()

    #####################################################
    # Run the graph in a Session
    #####################################################
    # Launch the graph
    with tf.compat.v1.Session() as sess:

        sess.run(tf.compat.v1.initializers.global_variables())

        # TensorBoard writer
        writer = tf.compat.v1.summary.FileWriter(tboard_path, sess.graph)
        tb_summary = tf.compat.v1.summary.merge_all()

        # Training phase with training data
        print("\n----------------------------",flush=True)
        print(" TRAINING STARTED...",flush=True)
        print("----------------------------",flush=True)
        for epoch in range(epochs):

            # process all batches
            for i in range(total_batches):

                # fetch a batch from training dataset
                x_batch, y_batch = x_train[i*batchsize:i*batchsize+batchsize], \
                                   y_train[i*batchsize:i*batchsize+batchsize]

                # Run graph for optimization  - i.e. do the training
                train_feed_dict={images_in: x_batch, labels: y_batch, train: True}
                _, s = sess.run([train_op, tb_summary], feed_dict=train_feed_dict)
                writer.add_summary(s, (((epoch+1)*total_batches)-1))

            # check accuracy at end of epoch, be sure not to run training
            pred = sess.run(predicted_logit, feed_dict={images_in: x_test, labels: y_test, train: False})
            acc = calc_acc(x_test,y_test,pred)
            print (" Epoch", epoch+1, "/", epochs, '- accuracy {:1.2f}'.format(acc),'%',flush=True)

        writer.flush()
        writer.close()

        # save post-training checkpoint, this saves all the parameters of the trained network
        print("\n----------------------------",flush=True)
        print(" SAVING CHECKPOINT & GRAPH...",flush=True)
        print("----------------------------",flush=True)
        saver.save(sess, output_ckpt_path)
        print(' Saved checkpoint to %s' % output_ckpt_path,flush=True)


    #####  SESSION ENDS HERE #############


    #####################################################
    # Write out a graph for inference
    #####################################################
    # we cannot use the training graph for deployment
    # we need to create a new graph with is_training set to False to disable dropout & batch norm
    # this new graph does not have any nodes associated with training (loss, optimizer, etc)

    with tf.compat.v1.Graph().as_default():

      # define placeholders for the input data
      x_1 = tf.compat.v1.placeholder(tf.float32, shape=[None,input_height,input_width,input_chan], name='images_in')

      # call the CNN function with is_training=False
      logits_1 = customcnn(cnn_in=x_1, is_training=False)

      tf.io.write_graph(tf.compat.v1.get_default_graph().as_graph_def(), INFER_GRAPH_DIR, INFER_GRAPH_FILENAME, as_text=False)
      print(' Saved binary inference graph to %s' % infer_graph_path)

    
    print(' Run `tensorboard --logdir=%s --port 6006 --host localhost` to see the results.' % tboard_path,flush=True)

    return


def main():

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()

    ap.add_argument('-ih', '--input_height',
                    type=int,
                    default=28,
                    help='Input data height. Default is 28')                  
    ap.add_argument('-iw', '--input_width',
                    type=int,
                    default=28,
                    help='Input data width. Default is 28')                  
    ap.add_argument('-ic', '--input_chan',
                    type=int,
                    default=1,
                    help='Input data channels. Default is 1')                  
    ap.add_argument('-e', '--epochs',
                    type=int,
                    default=100,
                    help='Number of training epochs. Default is 100')                  
    ap.add_argument('-l', '--learnrate',
                    type=float,
                    default=0.0001,
                    help='Learning rate. Default is 0.0001')
    ap.add_argument('-b', '--batchsize',
                    type=int,
                    default=50,
                    help='Training batchsize. Default is 50')  
    ap.add_argument('-o', '--output_ckpt_path',
                    type=str,
                    default='./chkpt/float_model.ckpt',
                    help='Path and filename of trained checkpoint. Default is ./chkpt/float_model.ckpt')
    ap.add_argument('-ig', '--infer_graph_path',
                    type=str,
                    default='./chkpt/inference_graph.pb',
                    help='Path and filename of inference graph. Default is ./chkpt/inference_graph.pb')        
    ap.add_argument('-t', '--tboard_path',
                    type=str,
                    default='./tb_log',
                    help='Path of TensorBoard logs. Default is ./tb_log')
    ap.add_argument('-g', '--gpu',
                    type=str,
                    default='0',
                    help='IDs of GPU cards to be used. Default is 0')                  
    args = ap.parse_args() 


    print('\n------------------------------------')
    print('Keras version      :',tf.keras.__version__)
    print('TensorFlow version :',tf.__version__)
    print('Python version     :',(sys.version))
    print('------------------------------------')
    print ('Command line options:')
    print (' --input_height    : ', args.input_height)
    print (' --input_width     : ', args.input_width)
    print (' --input_chan      : ', args.input_chan)
    print (' --epochs          : ', args.epochs)
    print (' --batchsize       : ', args.batchsize)
    print (' --learnrate       : ', args.learnrate)
    print (' --output_ckpt_path: ', args.output_ckpt_path)
    print (' --infer_graph_path: ', args.infer_graph_path)
    print (' --tboard_path     : ', args.tboard_path)
    print (' --gpu             : ', args.gpu)
    print('------------------------------------\n')



    os.environ["CUDA_DEVICE_ORDER"]='PCI_BUS_ID'
    
    # indicate which GPU to use
    os.environ["CUDA_VISIBLE_DEVICES"]=args.gpu


    train(args.input_height,args.input_width,args.input_chan,args.epochs,args.learnrate, \
          args.batchsize,args.output_ckpt_path,args.infer_graph_path,args.tboard_path)


if __name__ == '__main__':
  main()

