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
echo "##########################################################################"
echo "TRAIN & EVAL LeNet on CIFAR10"
echo "##########################################################################"
python code/train_cifar10.py --network LeNet --weights keras_model/cifar10/LeNet --epochs 20 --init_lr 0.01 --batch_size 32 2>&1 | tee rpt/cifar10/1_train_cifar10_LeNet.log


echo " "
echo "##########################################################################"
echo "TRAIN & EVAL miniVggNet  on CIFAR10"
echo "##########################################################################"
python code/train_cifar10.py --network miniVggNet --weights keras_model/cifar10/miniVggNet --epochs 40 --init_lr 0.01 --batch_size 64 2>&1 | tee rpt/cifar10/1_train_cifar10_miniVggNet.log


echo " "
echo "##########################################################################"
echo "TRAIN & EVAL miniGoogleNet on CIFAR10"
echo "##########################################################################"
python code/train_cifar10.py   --network miniGoogleNet --weights keras_model/cifar10/miniGoogleNet --epochs 70 --init_lr 5e-3  --batch_size 128 2>&1 | tee rpt/cifar10/1_train_cifar10_miniGoogleNet.log

echo " "
echo "##########################################################################"
echo "TRAIN & EVAL miniResNet  on CIFAR10"
echo "##########################################################################"
python code/train_cifar10.py  --network miniResNet --weights keras_model/cifar10/miniResNet --epochs 100 --init_lr 0.1 --batch_size 128 2>&1 | tee rpt/cifar10/1_train_cifar10_miniResNet.log
