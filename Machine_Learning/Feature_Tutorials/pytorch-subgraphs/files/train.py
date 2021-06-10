#!/usr/bin/env python
# coding: utf-8

'''
 Copyright 2021 Xilinx Inc.
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

# Author: Daniele Bagni & Mark Harvey, Xilinx Inc

import numpy as np
import collections
import argparse
import sys
import os
import datetime

import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

from common import *


def train_test(batchsz, lrate, epochs, model_name):

    # check GPU or CPU
    if torch.cuda.is_available():
        device = torch.device('cuda')
    else:
        device = torch.device('cpu')
    print(f"Training on {device} device.")

    # model parameters
    model = CNN().to(device)
    numel_list = [p.numel() for p in model.parameters()]
    sum(numel_list), numel_list

    # training
    train_loader = training_loader(train_data=cifar2_train, batch_size=batchsz, shuffle=True)
    optimizer = optim.SGD(model.parameters(), lr=lrate) #1e-2)
    loss_fn = nn.CrossEntropyLoss()

    startTime1 = datetime.datetime.now() #DB
    training_loop(
        n_epochs = epochs, #100,
        optimizer = optimizer,
        model = model,
        loss_fn = loss_fn,
        train_loader = train_loader,
        dev = device
    )
    endTime1 = datetime.datetime.now()
    diff1 = endTime1 - startTime1
    print("\n")
    print("Elapsed time for training (s): ", diff1.total_seconds())
    print("\n")

    model_summary()

    # save model and reload
    save_path = os.path.join(data_path, model_name)
    print('Trained model written to ', save_path)
    torch.save(model.state_dict(), save_path)
    loaded_model = CNN()  # <1>
    loaded_model.load_state_dict(torch.load(save_path))


    # validation
    train_loader = training_loader(train_data=cifar2_train, batch_size=batchsz, shuffle=False)
    val_loader = validation_loader(val_data=cifar2_val, batch_size=64, shuffle=False)
    all_acc_dict = collections.OrderedDict()
    all_acc_dict["baseline"] = validate(model, train_loader, val_loader, device)


def run_main():

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument('-b', '--batchsize', type=int,  default=64,     help='Training batchsize. Must be an integer. Default is 100')
    ap.add_argument('-e', '--epochs',    type=int,  default=100,    help='Number of training epochs. Must be an integer. Default is 3')
    ap.add_argument('-lr','--learnrate', type=float,default=1e-2,   help='Optimizer learning rate. Must be floating-point value. Default is 0.001')
    ap.add_argument('-m', '--namemodel', type=str,  default='cnn_float.pt', help='name of the floating model')
    args = ap.parse_args()

    print("\n")
    print('PyTorch version : ',torch.__version__)
    print(sys.version)
    print("\n")
    print(' Command line options:')
    print ('--batchsize    : ',args.batchsize)
    print ('--learnrate    : ',args.learnrate)
    print ('--epochs       : ',args.epochs)
    print ('--namemodel    : ',args.namemodel)
    print("\n")

    train_test(args.batchsize, args.learnrate, args.epochs, args.namemodel)



if __name__ == '__main__':
    run_main()
