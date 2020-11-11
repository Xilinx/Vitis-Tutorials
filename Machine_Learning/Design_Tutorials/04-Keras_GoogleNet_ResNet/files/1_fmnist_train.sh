#!/bin/bash

## © Copyright (C) 2016-2020 Xilinx, Inc
##
## Licensed under the Apache License, Version 2.0 (the "License"). You may
## not use this file except in compliance with the License. A copy of the
## License is located at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
## WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
## License for the specific language governing permissions and limitations
## under the License.



echo " "
echo "##################################################################################"
echo "TRAIN & EVAL LeNet on fashion MNIST"
echo "##################################################################################"
python code/train_fashion_mnist.py --network LeNet --weights keras_model/fmnist/LeNet --epochs 5 --init_lr 0.01 --batch_size 32 2>&1 | tee rpt/fmnist/1_train_fashion_mnist_LeNet.log

echo " "
echo "####################################################################################"
echo "TRAIN & EVAL miniVggNet on fashion MNIST"
echo "####################################################################################"
python code/train_fashion_mnist.py --network miniVggNet --weights keras_model/fmnist/miniVggNet --epochs 25 --init_lr 0.01 --batch_size 64 2>&1 | tee rpt/fmnist/1_train_fashion_mnist_miniVggNet.log

echo " "
echo "######################################################################################"
echo "TRAIN & EVAL miniGoogleNet on fashion MNIST"
echo "######################################################################################"
python code/train_fashion_mnist.py --network miniGoogleNet --weights keras_model/fmnist/miniGoogleNet --epochs 70 --init_lr 5e-3  --batch_size 128 2>&1 | tee rpt/fmnist/1_train_fashion_mnist_miniGoogleNet.log

echo " "
echo "######################################################################################"
echo "TRAIN & EVAL miniResNet on fashion MNIST"
echo "######################################################################################"
python code/train_fashion_mnist.py --network miniResNet --weights keras_model/fmnist/miniResNet --epochs 100 --init_lr 0.1 --batch_size 128 2>&1 | tee rpt/fmnist/1_train_fashion_mnist_miniResNet.log
