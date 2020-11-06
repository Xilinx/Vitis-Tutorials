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



# organize Fashion-MNIST data
python code/fashion_mnist_generate_images.py 2>&1 | tee rpt/fmnist/0_fashion_mnist_generate_images.log

# organize CIFAR10  data
python code/cifar10_generate_images.py 2>&1 | tee rpt/cifar10/0_cifar10_generate_images.log
