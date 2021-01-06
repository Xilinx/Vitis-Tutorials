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
Description:
    Training or fine-tuning of AlexNet on dogs-vs-cats dataset.
    Training must leave the --input_ckpt at default value.
    Fine-tuning must start from a previously trained checkpoint.
'''


'''
Author: Mark Harvey
'''

import os
import sys
import numpy as np
from AlexNet import alexnet
import argparse
import random

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

# workaround for TF1.15 bug "Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR"
os.environ['TF_FORCE_GPU_ALLOW_GROWTH'] = 'true'

import tensorflow as tf
tf.compat.v1.logging.set_verbosity(tf.compat.v1.logging.ERROR)



def train(target_acc,dataset_dir,input_ckpt,epochs,batchsize,init_lr,output_ckpt, \
          tboard_logs,input_height,input_width,input_chan):

    # if an input checkpoint is specified, we are doing pruning fine-tune 
    if (input_ckpt!=''):
        tf.set_pruning_mode()
        print('Doing fine-tuning', flush=True)

    # unpack dataset from npz files
    npz_file = np.load(os.path.join(dataset_dir,'trainData.npz'))
    x_train = npz_file['x']
    y_train = npz_file['y']

    npz_file = np.load(os.path.join(dataset_dir,'testData.npz'))
    x_test = npz_file['x']
    y_test = npz_file['y']

    train_batches = int(len(x_train)/batchsize)
    test_batches = int(len(x_test)/batchsize)

    print('Train batches:',train_batches,flush=True)
    print('Test batches:',test_batches,flush=True)


    # define placeholders for training mode and droput rate
    images_in = tf.compat.v1.placeholder(tf.float32, shape=[None,input_height,input_width,input_chan], name='images_in')
    labels_ph = tf.compat.v1.placeholder(tf.float32, shape=[None,2], name='labels_ph')
    train_ph = tf.compat.v1.placeholder(tf.bool, shape=None, name='train_ph')
    drop_ph = tf.compat.v1.placeholder(tf.float32, shape=None, name='drop_ph')
    
    # build the network, input comes from the 'images_in' placeholder
    logits = alexnet(images_in, classes=2, drop_rate=drop_ph, is_training=train_ph)
  
    # softmax cross entropy loss function - needs one-hot encoded labels
    loss = tf.reduce_mean(tf.compat.v1.losses.softmax_cross_entropy(logits=logits, onehot_labels=labels_ph),name='loss')

    # global step
    global_step=tf.compat.v1.train.get_or_create_global_step()
    
    # stepped learning rate - divides the learning rate by 10 at each step
    # number of steps is defined by n
    n = 2
    decay_steps = int((epochs/n)*(len(x_train)/batchsize))
    decay_lr = tf.compat.v1.train.exponential_decay(learning_rate=init_lr,
                                                    global_step=global_step,
                                                    decay_steps=decay_steps,
                                                    decay_rate=0.1,
                                                    staircase=True,
                                                    name='decay_lr')
    
    # Adaptive Momentum optimizer - minimize the loss
    update_ops = tf.compat.v1.get_collection(tf.compat.v1.GraphKeys.UPDATE_OPS)
    optimizer = tf.compat.v1.train.AdamOptimizer(learning_rate=decay_lr)
    with tf.control_dependencies(update_ops):
        train_op = optimizer.minimize(loss, global_step=global_step)

    predicted_logit = tf.argmax(input=logits, axis=1, output_type=tf.int32,name='predicted_logit')
    ground_truth = tf.argmax(input=labels_ph, axis=1, output_type=tf.int32, name='ground_truth')

    # Accuracy
    tf_acc, tf_acc_update = tf.compat.v1.metrics.accuracy(ground_truth, predicted_logit, name='accuracy')
    acc_var = tf.compat.v1.get_collection(tf.compat.v1.GraphKeys.LOCAL_VARIABLES, scope='accuracy')
    acc_var_initializer = tf.compat.v1.variables_initializer(var_list=acc_var)

    # TensorBoard data collection
    tf.compat.v1.summary.scalar('loss', loss)
    tf.compat.v1.summary.scalar('decay_lr', decay_lr)
    tf.compat.v1.summary.image('images_in', images_in)

    # set up saver object
    saver = tf.compat.v1.train.Saver()

    # Run the graph in a Session
    with tf.compat.v1.Session() as sess:
        best_epoch = 0
        best_acc = 0    
        sess.run(tf.compat.v1.initializers.global_variables())

        # TensorBoard writer
        writer = tf.compat.v1.summary.FileWriter(tboard_logs, sess.graph)
        tb_summary = tf.compat.v1.summary.merge_all()

        # if input checkpoint specified, restore it
        if (input_ckpt!=''):
            saver.restore(sess, input_ckpt)
   
        # Training and test over specified number of epochs
        for epoch in range(epochs):
            print('\nEpoch', epoch+1, '/', epochs,':', flush=True)   

            # Training for 1 epoch
            for i in range(train_batches):    
                x_batch, y_batch = x_train[i*batchsize:(i+1)*batchsize], y_train[i*batchsize:(i+1)*batchsize]    
                # random left-right flip
                for j in range(batchsize):
                    if (random.randint(0,1)==1):
                        np.fliplr(x_batch[j])

                # Run graph for optimization - i.e. do the training
                _, s, dlr = sess.run([train_op,tb_summary,decay_lr], feed_dict={images_in: x_batch, labels_ph: y_batch, train_ph: True, drop_ph: 0.5})
            writer.add_summary(s, global_step=(((epoch+1)*train_batches)-1) )

            # test after every epoch            
            sess.run(acc_var_initializer)
            for i in range(test_batches):   
                # fetch a batch from test dataset
                x_batch, y_batch = x_test[i*batchsize:(i+1)*batchsize], y_test[i*batchsize:(i+1)*batchsize]
    
                # Run graph for accuracy
                sess.run([tf_acc_update], feed_dict={images_in: x_batch, labels_ph: y_batch, train_ph: False, drop_ph: 0.0})

            print (' - Learning rate:', dlr, flush=True)
            score = sess.run(tf_acc)
            print (' - Accuracy with test set:', score, flush=True)

            # save checkpoint if accuracy improved
            if (score > best_acc):
                saver.save(sess, output_ckpt)
                print(' - Saved checkpoint to %s' % output_ckpt, flush=True)
                best_acc = score
                best_epoch = epoch+1

            # exit if target accuracy reached
            if (best_acc >= target_acc):
                print('Accuracy of',best_acc,'is better than target accuracy of',target_acc,'..exiting train/fine-tune',flush=True)
                break

    print('\nBest epoch:',best_epoch,' Accuracy:',best_acc, flush=True)
    writer.flush()
    writer.close()
    print('Run `tensorboard --logdir=%s --port 6006 --host localhost` to see the results.' % tboard_logs)

    return best_acc, best_epoch


def main():
    
    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser() 
    ap.add_argument('-a', '--target_acc',  type=float,default=1.0,           help='Target accuracy. Default is 1.0 (100%)')
    ap.add_argument('-d', '--dataset_dir', type=str,  default='dataset',     help='path to dataset root folder. Default is dataset')
    ap.add_argument('-i', '--input_ckpt',  type=str,  default='',            help='Path of input checkpoint. Default is no checkpoint')  
    ap.add_argument('-e', '--epochs',      type=int,  default=1,             help='Number of training epochs, must be integer value. Default is 1') 
    ap.add_argument('-b', '--batchsize',   type=int,  default=100,           help='Training batchsize, must be integer value. Default is 100') 
    ap.add_argument('-il','--init_lr',     type=float,default=0.001,         help='Initial learning rate, must be floating-point value. Default is 0.001')
    ap.add_argument('-o', '--output_ckpt', type=str,  default='output.ckpt', help='Path of output checkpoint. Default is output.ckpt')
    ap.add_argument('-tb','--tboard_logs', type=str,  default='tb_logs',     help='Path of TensorBoard log files. Default is tb_logs')
    ap.add_argument('-ih','--input_height',type=int,  default=224,           help='Input height. Default is 224') 
    ap.add_argument('-iw','--input_width', type=int,  default=224,           help='Input width. Default is 224') 
    ap.add_argument('-ic','--input_chan',  type=int,  default=3,             help='Input channels. Default is 3')
    ap.add_argument('-g', '--gpu',         type=str,  default='0',           help='String value to select which CUDA GPU devices to use. Default is 0')
    args = ap.parse_args()
  
    print('\n------------------------------------')
    print('Keras version      : ',tf.keras.__version__)
    print('TensorFlow version : ',tf.__version__)
    print(sys.version)
    print('------------------------------------')
    print ('Command line options:')
    print (' --target_acc   : ',args.target_acc)
    print (' --dataset_dir  : ',args.dataset_dir)
    print (' --input_ckpt   : ',args.input_ckpt)
    print (' --epochs       : ',args.epochs)
    print (' --batchsize    : ',args.batchsize)
    print (' --init_lr      : ',args.init_lr)
    print (' --output_ckpt  : ',args.output_ckpt)
    print (' --tboard_logs  : ',args.tboard_logs)
    print (' --input_height : ',args.input_height)
    print (' --input_width  : ',args.input_width)
    print (' --input_chan   : ',args.input_chan) 
    print (' --gpu          : ',args.gpu)
    print('------------------------------------\n')


    os.environ["CUDA_DEVICE_ORDER"]='PCI_BUS_ID'
    
    # indicate which GPU to use
    os.environ["CUDA_VISIBLE_DEVICES"]=args.gpu
 
  
    _,_ = train(args.target_acc, args.dataset_dir, args.input_ckpt, args.epochs, \
                args.batchsize, args.init_lr, args.output_ckpt, args.tboard_logs, \
                args.input_height, args.input_width, args.input_chan )


if __name__ == '__main__':
    main()

