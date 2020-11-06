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


# delete previous results
#rm -rf ./compile

# Compile
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI: LeNet on CIFAR10"
echo "##########################################################################"
# for Vitis AI == 1.0
#python /opt/vitis_ai/compiler/vai_c_tensorflow \

# for Vitis AI >= 1.1
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/LeNet/deploy_model.pb \
       --arch /opt/vitis_ai/compiler/arch/dpuv2/ZCU104/ZCU104.json \
       --output_dir=compile/cifar10/LeNet \
       --net_name=LeNet \
       --options    "{'mode':'normal'}" \
       2>&1 | tee rpt/cifar10/5_vai_compile_LeNet.log

mv  ./compile/cifar10/LeNet/dpu*.elf ./target_zcu104/cifar10/LeNet/model/

# Compile
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI: miniVggNet  on CIFAR10"
echo "##########################################################################"
# for Vitis AI == 1.0
#python /opt/vitis_ai/compiler/vai_c_tensorflow \

# for Vitis AI >= 1.1
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/miniVggNet/deploy_model.pb \
       --arch /opt/vitis_ai/compiler/arch/dpuv2/ZCU104/ZCU104.json \
       --output_dir=compile/cifar10/miniVggNet \
       --net_name=miniVggNet \
       --options    "{'mode':'normal'}" \
       2>&1 | tee rpt/cifar10/5_vai_compile_miniVggNet.log

mv  ./compile/cifar10/miniVggNet/dpu_miniVggNet_0.elf ./target_zcu104/cifar10/miniVggNet/model/nor_dpu_miniVggNet_0.elf

#debug mode
# for Vitis AI == 1.0
#python /opt/vitis_ai/compiler/vai_c_tensorflow \

# for Vitis AI >= 1.1
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/miniVggNet/deploy_model.pb \
       --arch /opt/vitis_ai/compiler/arch/dpuv2/ZCU104/ZCU104.json \
       --output_dir=compile/cifar10/miniVggNet \
       --net_name=miniVggNet \
       --options    "{'mode':'debug'}" \
       2>&1 | tee rpt/cifar10/5_vai_compile_miniVggNet.log

mv  ./compile/cifar10/miniVggNet/dpu_miniVggNet_0.elf ./target_zcu104/cifar10/miniVggNet/model/dbg_dpu_miniVggNet_0.elf

# Compile
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI: miniGoogleNet  on CIFAR10"
echo "##########################################################################"
# for Vitis AI == 1.0
#python /opt/vitis_ai/compiler/vai_c_tensorflow \

# for Vitis AI >= 1.1
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/miniGoogleNet/deploy_model.pb \
       --arch /opt/vitis_ai/compiler/arch/dpuv2/ZCU104/ZCU104.json \
       --output_dir=compile/cifar10/miniGoogleNet \
       --net_name=miniGoogleNet \
       --options    "{'mode':'normal'}" \
       2>&1 | tee rpt/cifar10/5_vai_compile_miniGoogleNet.log

mv  ./compile/cifar10/miniGoogleNet/dpu*.elf ./target_zcu104/cifar10/miniGoogleNet/model/

# Compile
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI: miniResNet  on CIFAR10"
echo "##########################################################################"
# for Vitis AI == 1.0
#python /opt/vitis_ai/compiler/vai_c_tensorflow \

# for Vitis AI >= 1.1
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/miniResNet/deploy_model.pb \
       --arch /opt/vitis_ai/compiler/arch/dpuv2/ZCU104/ZCU104.json \
       --output_dir=compile/cifar10/miniResNet \
       --net_name=miniResNet \
       --options    "{'mode':'normal'}" \
       2>&1 | tee rpt/cifar10/5_vai_compile_miniResNet.log

mv  ./compile/cifar10/miniResNet/dpu*.elf ./target_zcu104/cifar10/miniResNet/model/

echo " "
echo "##########################################################################"
echo "COMPILATION COMPLETED  on CIFAR10"
echo "##########################################################################"
echo " "
