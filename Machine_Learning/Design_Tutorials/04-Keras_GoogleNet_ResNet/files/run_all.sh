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

# author daniele.bagni@xilinx.com
# date: 6 May 2021


: '
# -------------------------------------------------------------------------------
#dos2unix conversion
for file in $(find $PWD -name "*.sh"); do
    sed -i 's/\r//g' ${file}
    echo  ${file}
done
'

: '
# -------------------------------------------------------------------------------
# organize Fashion-MNIST data
rm -rf dataset
python code/fmnist_generate_images.py    2>&1 | tee fmnist_generate_images.log
# organize CIFAR10  data
python code/cifar10_generate_images.py   2>&1 | tee cifar10_generate_images.log
'
: '
# -------------------------------------------------------------------------------
# clean directories
#keras_model
rm -rf ./keras_model
mkdir  ./keras_model
mkdir  ./keras_model/cifar10 ./keras_model/fmnist
mkdir  ./keras_model/cifar10/LeNet ./keras_model/cifar10/miniGoogleNet ./keras_model/cifar10/miniResNet ./keras_model/cifar10/miniVggNet
mkdir  ./keras_model/fmnist/LeNet ./keras_model/fmnist/miniGoogleNet ./keras_model/fmnist/miniResNet ./keras_model/fmnist/miniVggNet

#compile
rm -rf ./build/compile
mkdir ./build/compile
mkdir ./build/compile/cifar10 ./build/compile/fmnist
mkdir ./build/compile/cifar10/LeNet ./build/compile/cifar10/miniGoogleNet ./build/compile/cifar10/miniResNet ./build/compile/cifar10/miniVggNet
mkdir ./build/compile/fmnist/LeNet ./build/compile/fmnist/miniGoogleNet ./build/compile/fmnist/miniResNet ./build/compile/fmnist/miniVggNet
#freeze
rm -rf ./build/freeze/
mkdir ./build/freeze
mkdir ./build/freeze/cifar10 ./build/freeze/fmnist
mkdir ./build/freeze/cifar10/LeNet ./build/freeze/cifar10/miniGoogleNet ./build/freeze/cifar10/miniResNet ./build/freeze/cifar10/miniVggNet
mkdir ./build/freeze/fmnist/LeNet ./build/freeze/fmnist/miniGoogleNet ./build/freeze/fmnist/miniResNet ./build/freeze/fmnist/miniVggNet
#quantized_results
rm -rf ./build/quantized_results/
mkdir ./build/quantized_results
mkdir ./build/quantized_results/cifar10 ./build/quantized_results/fmnist
mkdir ./build/quantized_results/cifar10/LeNet ./build/quantized_results/cifar10/miniGoogleNet ./build/quantized_results/cifar10/miniResNet ./build/quantized_results/cifar10/miniVggNet
mkdir ./build/quantized_results/fmnist/LeNet ./build/quantized_results/fmnist/miniGoogleNet ./build/quantized_results/fmnist/miniResNet ./build/quantized_results/fmnist/miniVggNet
#tf_chkpts
rm -rf ./build/tf_chkpts/
mkdir ./build/tf_chkpts
mkdir ./build/tf_chkpts/cifar10 ./build/tf_chkpts/fmnist
mkdir ./build/tf_chkpts/cifar10/LeNet ./build/tf_chkpts/cifar10/miniGoogleNet ./build/tf_chkpts/cifar10/miniResNet ./build/tf_chkpts/cifar10/miniVggNet
mkdir ./build/tf_chkpts/fmnist/LeNet ./build/tf_chkpts/fmnist/miniGoogleNet ./build/tf_chkpts/fmnist/miniResNet ./build/tf_chkpts/fmnist/miniVggNet


# target boards
rm -rf target_zcu104 target_vck190 target_zcu104.tar target_vck190.tar target_zcu102.tar
rm -rf target_zcu102/cifar10 target_zcu102/fmnist target_zcu102/rpt target_zcu102/*.tar
mkdir target_zcu102/cifar10 target_zcu102/fmnist target_zcu102/rpt
mkdir target_zcu102/cifar10/LeNet target_zcu102/cifar10/miniGoogleNet target_zcu102/cifar10/miniResNet target_zcu102/cifar10/miniVggNet
mkdir target_zcu102/fmnist/LeNet target_zcu102/fmnist/miniGoogleNet target_zcu102/fmnist/miniResNet target_zcu102/fmnist/miniVggNet
'


# -------------------------------------------------------------------------------
# run CIFAR10 flow
#source ./run_cifar10.sh 2>&1 | tee cifar10_host.log


#run Fashion-MNIST flow
source ./run_fmnist.sh 2>&1 | tee fmnist_host.log



# -------------------------------------------------------------------------------
# tar files for target boards
tar -cvf target_zcu102.tar ./target_zcu102  &> /dev/null
tar -cvf target_zcu104.tar ./target_zcu104  &> /dev/null
tar -cvf target_vck190.tar ./target_vck190  &> /dev/null


# -------------------------------------------------------------------------------
# add Xilinx headers
for file in $(find . -name "*.log"); do
  echo ${file}
  cat ./header.txt ${file} > tmp.txt
  mv tmp.txt ${file}
done

# move log files
mv *.log ./build/ref_log/
