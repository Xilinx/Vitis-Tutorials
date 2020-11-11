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
echo "COMPILE WITH Vitis AI: LeNet on FMNIST"
echo "##########################################################################"
# for Vitis AI == 1.0
#python /opt/vitis_ai/compiler/vai_c_tensorflow \

# for Vitis AI >= 1.1
vai_c_tensorflow \
       --frozen_pb=./quantized_results/fmnist/LeNet/deploy_model.pb \
       --arch /opt/vitis_ai/compiler/arch/dpuv2/ZCU102/ZCU102.json \
       --output_dir=compile/fmnist/LeNet \
       --net_name=LeNet \
       --options    "{'mode':'normal'}" \
       2>&1 | tee rpt/fmnist/5_vai_compile_LeNet.log

mv  ./compile/fmnist/LeNet/dpu*.elf ./target_zcu102/fmnist/LeNet/model/

# Compile
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI: miniVggNet  on FMNIST"
echo "##########################################################################"
# for Vitis AI == 1.0
#python /opt/vitis_ai/compiler/vai_c_tensorflow \

# for Vitis AI >= 1.1
vai_c_tensorflow \
       --frozen_pb=./quantized_results/fmnist/miniVggNet/deploy_model.pb \
       --arch /opt/vitis_ai/compiler/arch/dpuv2/ZCU102/ZCU102.json \
       --output_dir=compile/fmnist/miniVggNet \
       --net_name=miniVggNet \
       --options    "{'mode':'normal'}" \
       2>&1 | tee rpt/fmnist/5_vai_compile_miniVggNet.log

mv  ./compile/fmnist/miniVggNet/dpu*.elf ./target_zcu102/fmnist/miniVggNet/model/

# Compile
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI: miniGoogleNet  on FMNIST"
echo "##########################################################################"
# for Vitis AI == 1.0
#python /opt/vitis_ai/compiler/vai_c_tensorflow \

# for Vitis AI >= 1.1
vai_c_tensorflow \
       --frozen_pb=./quantized_results/fmnist/miniGoogleNet/deploy_model.pb \
       --arch /opt/vitis_ai/compiler/arch/dpuv2/ZCU102/ZCU102.json \
       --output_dir=compile/fmnist/miniGoogleNet \
       --net_name=miniGoogleNet \
       --options    "{'mode':'normal'}" \
       2>&1 | tee rpt/fmnist/5_vai_compile_miniGoogleNet.log

mv  ./compile/fmnist/miniGoogleNet/dpu*.elf ./target_zcu102/fmnist/miniGoogleNet/model/

# Compile
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI: miniResNet  on FMNIST"
echo "##########################################################################"
# for Vitis AI == 1.0
#python /opt/vitis_ai/compiler/vai_c_tensorflow \

# for Vitis AI >= 1.1
vai_c_tensorflow \
       --frozen_pb=./quantized_results/fmnist/miniResNet/deploy_model.pb \
       --arch /opt/vitis_ai/compiler/arch/dpuv2/ZCU102/ZCU102.json \
       --output_dir=compile/fmnist/miniResNet \
       --net_name=miniResNet \
       --options    "{'mode':'normal'}" \
       2>&1 | tee rpt/fmnist/5_vai_compile_miniResNet.log

mv  ./compile/fmnist/miniResNet/dpu*.elf ./target_zcu102/fmnist/miniResNet/model/

echo " "
echo "##########################################################################"
echo "COMPILATION COMPLETED  on FMNIST"
echo "##########################################################################"
echo " "
