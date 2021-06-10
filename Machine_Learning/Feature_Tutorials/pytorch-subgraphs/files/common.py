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


import datetime
from tqdm import tqdm
import numpy as np
import os
import cv2

import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision
from torchvision import datasets, transforms
from torchsummary import summary

#######################################################################

data_path = './build/data/'

#######################################################################
# CIFAR10 dataset

class_names = ['airplane','automobile','bird','cat','deer',
               'dog','frog','horse','ship','truck']

cifar10 = datasets.CIFAR10(
    data_path, train=True, download=True,
    transform=transforms.Compose([
        transforms.ToTensor() # this also normalizes data from 0 to 1
    ]))

cifar10_val = datasets.CIFAR10(
    data_path, train=False, download=True,
    transform=transforms.Compose([
        transforms.ToTensor() # this also normalizes data from 0 to 1
    ]))


#######################################################################
# CIFAR2 dataset

#labels_map = {0: 0, 2: 1}
#class_names = ['airplane', 'bird']

labels_map = {1: 0, 9: 1}
class_names = ['automobile', 'truck']

cifar2_train = []
cifar2_val   = []
for i, l in cifar10:
    #if l in [0, 2]:
    if l in [1, 9]:
        cifar2_train.append([i, labels_map[l]])
for i, l in cifar10_val:
    #if l in [0, 2]:
    if l in [1, 9]:
        cifar2_val.append([i, labels_map[l]])

#######################################################################
# CNN

def preprocess_fn(image_path):
    '''
    Image pre-processing.
    Opens image as grayscale then normalizes to range 0:1
    input arg: path of image file
    return: numpy array
    '''
    image = cv2.imread(image_path)
    #image = image.reshape(32,32,3)
    data = np.asarray( image, dtype="float32" )
    data = data/255.0
    return data

class CNN(nn.Module):
    def __init__(self):
        super(CNN,self).__init__()
        self.conv1 = nn.Conv2d(3, 16, kernel_size=3, padding=1)
        self.act1  = nn.Tanh()
        self.pool1 = nn.MaxPool2d(2)
        self.conv2 = nn.Conv2d(16, 8, kernel_size=3, padding=1)
        self.act2  = nn.Sigmoid()
        self.pool2 = nn.MaxPool2d(2)
        self.fc1   = nn.Linear(8 * 8 * 8, 32)
        self.act3  = nn.Sigmoid()
        self.fc2   = nn.Linear(32, 2)
    def forward(self, x):
        inp2=self.conv1(x) # tanh input
        out2 = self.act1(inp2)
        out = self.pool1(out2)
        inp4=self.conv2(out) #sigmoid1 input
        out4 = self.act2(inp4)
        out = self.pool2(out4)
        out = out.view(-1, 8 * 8 * 8) # <1>
        inp6 = self.fc1(out) #sigmoid2 input
        out6 = self.act3(inp6)
        out = self.fc2(out6)
        return out


class CNN_DEBUG(nn.Module):
    def __init__(self):
        super(CNN_DEBUG,self).__init__()
        self.conv1 = nn.Conv2d(3, 16, kernel_size=3, padding=1)
        self.act1  = nn.Tanh()
        self.pool1 = nn.MaxPool2d(2)
        self.conv2 = nn.Conv2d(16, 8, kernel_size=3, padding=1)
        self.act2  = nn.Sigmoid()
        self.pool2 = nn.MaxPool2d(2)
        self.fc1   = nn.Linear(8 * 8 * 8, 32)
        self.act3  = nn.Sigmoid()
        self.fc2   = nn.Linear(32, 2)

    def forward(self, x):
        inp2=self.conv1(x) # tanh input

        print("TANH inp shape ", inp2.shape)
        x = inp2.detach().numpy()
        x = np.asarray(x, dtype="float32")
        xx = np.transpose(x[0],(1,2,0))
        np.save('ref_tanh_data_inp.bin', xx)
        out2 = self.act1(inp2)
        x = out2.detach().numpy()
        x = np.asarray(x, dtype="float32")
        xx = np.transpose(x[0],(1,2,0))
        np.save('ref_tanh_data_out.bin', xx)
        print("TANH out shape ", out2.shape)

        out = self.pool1(out2)
        inp4=self.conv2(out) #sigmoid1 input

        print("SIGM1 inp shape ", inp4.shape)
        x = inp4.detach().numpy()
        x = np.asarray( x, dtype="float32")
        xx = np.transpose(x[0],(1,2,0))
        np.save('ref_sigm1_data_inp.bin', xx)
        out4 = self.act2(inp4)
        x = out4.detach().numpy()
        x = np.asarray( x, dtype="float32")
        xx = np.transpose(x[0],(1,2,0))
        np.save('ref_sigm1_data_out.bin', xx)
        print("SIGM1 out shape ", out4.shape)

        out = self.pool2(out4)
        out = out.view(-1, 8 * 8 * 8) # <1>
        inp6 = self.fc1(out) #sigmoid2 input

        print("SIGM2 inp shape ", inp6.shape)
        x = inp6.detach().numpy()
        x = np.asarray( x, dtype="float32")
        np.save('ref_sigm2_data_inp.bin', x)
        out6 = self.act3(inp6)
        x = out6.detach().numpy()
        x = np.asarray( x, dtype="float32")
        np.save('ref_sigm2_data_out.bin', x)
        print("SIGM2 out shape ", out6.shape)

        out = self.fc2(out6)
        x = out.detach().numpy()
        x = np.asarray( x, dtype="float32")
        np.save('ref_cnn_out.bin', x)
        print("CNN out shape ", out.shape)

        return out


#######################################################################
# FUNCTIONS FOR TRAINING AND VALIDATION

def training_loader(train_data, batch_size, shuffle):
    return torch.utils.data.DataLoader(train_data, batch_size, shuffle)


def validation_loader(val_data, batch_size, shuffle):
    return torch.utils.data.DataLoader(val_data, batch_size, shuffle)


def training_loop(n_epochs, optimizer, model, loss_fn, train_loader, dev):
    for epoch in range(1, n_epochs + 1):
        loss_train = 0.0
        for imgs, labels in train_loader:
            imgs = imgs.to(device=dev)
            labels = labels.to(device=dev)
            outputs = model(imgs)
            loss = loss_fn(outputs, labels)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
            loss_train += loss.item()
        if epoch == 1 or epoch % 10 == 0:
            print('{0} Epoch {1:3d}, Training loss {2:.6f}'.format(
                datetime.datetime.now(), epoch,
                loss_train / len(train_loader)))
    return


def validate(model, train_loader, val_loader, dev):
    accdict = {}
    for name, loader in [("train", train_loader), ("val  ", val_loader)]:
        correct = 0
        total = 0
        with torch.no_grad():
            for imgs, labels in loader:
                imgs = imgs.to(device=dev)
                labels = labels.to(device=dev)
                outputs = model(imgs)
                _, predicted = torch.max(outputs, dim=1) # <1>
                total += labels.shape[0]
                correct += int((predicted == labels).sum())
        print("Accuracy {0}: {1:.2f}(%)".format(name , 100*(correct/total)))
        accdict[name] = correct / total
    return accdict


def model_summary():
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu") # PyTorch v0.4.0
    model = CNN().to(device)
    summary(model, (3, 32, 32))

#######################################################################
# FUNCTION TO DEBUG THE CNN SUBGRAPHS PARTITIONING

def RUN_CNN_DEBUG(images_dir):

    dev = torch.device('cpu')
    print(f"Training on {dev} device.")

    images_list=os.listdir(images_dir)
    runTotal = len(images_list)
    img = []
    for i in range(runTotal):
        path = os.path.join(images_dir,images_list[i])
        img.append(preprocess_fn(path))
        if i == 0 :
            print(path)

    loaded_model = CNN_DEBUG().to(dev)  # <1>
    save_path = os.path.join(data_path, "cnn_float.pt")
    loaded_model.load_state_dict(torch.load(save_path, map_location=torch.device('cpu')))
    print(loaded_model)
    model_dict = l = loaded_model.state_dict()

    # get the weights of the last FC layer
    weight = model_dict['fc2.weight'].numpy()
    bias   = model_dict['fc2.bias'].numpy()
    np.set_printoptions(precision=16)
    print("fc2 weights = ...")
    print(weight)
    print("fc2 bias    = ", bias)
    np.save("fc2_weight.bin.npy", weight)
    np.save("fc2_bias.bin.npy",   bias)

    # Create the preprocessing transformation here
    transform = transforms.ToTensor()
    # Transform
    input = transform(img[0])
    # unsqueeze batch dimension, in case you are dealing with a single image
    input = input.unsqueeze(0)
    #Set model to eval
    loaded_model.eval()
    # Get prediction
    output = loaded_model(input)

    return loaded_model


#######################################################################
# FUNCTION TO GENERATE TEST IMAGES

def generate_images(dest_dir):
    num_images = len(cifar2_val)
    for i in range(num_images):
        image, label = cifar2_val[i]
        img = image.numpy().squeeze()
        img = (img * 255.).astype(np.uint8)
        R = img[0, :, :]
        G = img[1, :, :]
        B = img[2, :, :]
        bgr_img = cv2.merge([B,G,R])
        img_file=os.path.join(dest_dir, class_names[label] + '_' + str(i) + '.png')
        cv2.imwrite(img_file, bgr_img)
    print("\n generated ", num_images, " validation images")
    return
