#!/bin/bash

##
##* © Copyright (C) 2016-2020 Xilinx, Inc
##*
##* Licensed under the Apache License, Version 2.0 (the "License"). You may
##* not use this file except in compliance with the License. A copy of the
##* License is located at
##*
##*     http://www.apache.org/licenses/LICENSE-2.0
##*
##* Unless required by applicable law or agreed to in writing, software
##* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
##* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
##* License for the specific language governing permissions and limitations
##* under the License.
##*/

#working directory
CNN=alexnetBNnoLRN
work_dir=${ML_DIR}/deploy/${CNN}/pruned

model_dir=${work_dir}/vaiq_output
output_dir=${work_dir}/vaic_output

echo "Compiling network: ${CNN}"

vai_c_caffe  --prototxt=${model_dir}/deploy.prototxt     \
     --caffemodel=${model_dir}/deploy.caffemodel \
     --output_dir=${output_dir}                  \
     --net_name=${CNN} \
     --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
     --options    "{'mode':'normal', 'save_kernel':''}"


echo " copying xmodel file into /../zcu102/baseline/model/arm64_4096 "
cp ${output_dir}/${CNN}.xmodel  ${output_dir}/../../zcu102/pruned/model/arm64_4096/

echo " copying the test images to be used by the ZCU102"
cp -r $ML_DIR/input/jpg/test ${output_dir}/../../zcu102/test_images
