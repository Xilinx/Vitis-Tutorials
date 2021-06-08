#!/bin/bash
#-xv

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
# date: 4 May 2021


CNN_INP_NODE="conv2d_1_input"
LENET_OUT_NODE="activation_3/Softmax"
VGG_OUT_NODE="activation_5/Softmax"
GOOGLE_OUT_NODE="activation_19/Softmax"
RESNET_OUT_NODE="activation_82/Softmax"


dos2unix_conversion() {
  #dos2unix conversion
  for file in $(find $PWD -name "*.sh"); do
      sed -i 's/\r//g' ${file}
      echo  ${file}
  done
}



2_cifar10_Keras2TF(){
    echo " "
    echo "#################################################################################################"
    echo "KERAS to TENSORFLOW GRAPH CONVERSION for miniVggNet  on CIFAR10"
    echo "#################################################################################################"
    python code/Keras2TF.py  --dataset cifar10 -n miniVggNet
    #2>&1 | tee rpt/cifar10/2_keras2TF_graph_conversion_miniVggNet.log

}


3a_cifar10_freeze(){
  echo " "
  echo "##########################################################################"
  echo "FREEZE GRAPH of miniVggNet  on CIFAR10"
  echo "##########################################################################"
  rm ./build/freeze/cifar10/miniVggNet/*
  freeze_graph --input_meta_graph=./build/tf_chkpts/cifar10/miniVggNet/float_model.ckpt.meta \
               --input_checkpoint=./build/tf_chkpts/cifar10/miniVggNet/float_model.ckpt \
               --input_binary=true \
               --output_graph=././build/freeze/cifar10/miniVggNet/frozen_graph.pb \
               --output_node_names=${VGG_OUT_NODE}
               #2>&1 | tee rpt/cifar10/3a_freeze_graph_miniVggNet.log
  echo " "
  echo "##########################################################################"
  echo "INSPECT FROZEN GRAPH of miniVggNet on CIFAR10"
  echo "##########################################################################"
  vai_q_tensorflow inspect --input_frozen_graph=./build/freeze/cifar10/miniVggNet/frozen_graph.pb

  echo " "
  echo "##########################################################################"
  echo "FREEZE GRAPH COMPLETED"
  echo "##########################################################################"
  echo " "
}

3b_cifar10_evaluate_frozen_graph() {
  echo "##########################################################################"
  echo "EVALUATE FROZEN GRAPH of miniVggNet  on CIFAR10"
  echo "##########################################################################"
  python code/cifar10_eval_graph.py --dataset cifar10 --graph ./build/freeze/cifar10/miniVggNet/frozen_graph.pb --input_node ${CNN_INP_NODE} --output_node ${VGG_OUT_NODE}  --gpu 0  #2>&1 | tee rpt/cifar10/3b_evaluate_frozen_graph_miniVggNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE FROZEN GRAPH COMPLETED  on CIFAR10"
  echo "##########################################################################"
  echo " "
}

4a_cifar10_quant() {
  cd ./code
  echo " "
  echo "##########################################################################"
  echo "QUANTIZE miniVggNet  on CIFAR10"
  echo "##########################################################################"
  vai_q_tensorflow quantize \
	--input_frozen_graph ../build/freeze/cifar10/miniVggNet/frozen_graph.pb \
	--input_nodes ${CNN_INP_NODE} \
	--input_shapes ?,32,32,3 \
	--output_nodes ${VGG_OUT_NODE} \
	--output_dir ../build/quantized_results/cifar10/miniVggNet/ \
	--method 1 \
	--input_fn cifar10_graph_input_fn.calib_input \
	--calib_iter 20 \
	--gpu 0
  #2>&1 | tee ../rpt/cifar10/4a_quant_miniVggNet.log
  echo " "
  echo "##########################################################################"
  echo "QUANTIZATION COMPLETED  on CIFAR10"
  echo "##########################################################################"
  echo " "
  cd ..
}


4b_cifar10_evaluate_quantized_graph() {
  echo "##########################################################################"
  echo "EVALUATE QUANTIZED GRAPH of miniVggNet  on CIFAR10"
  echo "##########################################################################"
  python code/cifar10_eval_graph.py --dataset cifar10 --graph ./build/quantized_results/cifar10/miniVggNet/quantize_eval_model.pb --input_node ${CNN_INP_NODE} --output_node ${VGG_OUT_NODE} --gpu 0 #2>&1 | tee rpt/cifar10/4b_evaluate_quantized_graph_miniVggNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE QUANTIZED GRAPH COMPLETED  on CIFAR10"
  echo "##########################################################################"
  echo " "
}


5_cifar10_vai_compile_zcu102() {
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU102: miniVggNet  on CIFAR10"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./build/quantized_results/cifar10/miniVggNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
       --output_dir=build/compile/cifar10/miniVggNet \
       --net_name=miniVggNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_miniVggNet.log
mv  ./build/compile/cifar10/miniVggNet/*.xmodel ./target_zcu102/cifar10/miniVggNet/
rm  ./target_zcu102/cifar10/miniVggNet/*_org.xmodel
echo " "
echo "##########################################################################"
echo "COMPILATION COMPLETED  on CIFAR10 on ZCU102"
echo "##########################################################################"
echo " "
}

5_cifar10_vai_compile_zcu104() {
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU104: miniVggNet  on CIFAR10"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./build/quantized_results/cifar10/miniVggNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
       --output_dir=build/compile/cifar10/miniVggNet \
       --net_name=miniVggNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_miniVggNet.log
mv  ./build/compile/cifar10/miniVggNet/*.xmodel ./target_zcu104/cifar10/miniVggNet/
rm  ./target_zcu104/cifar10/miniVggNet/*_org.xmodel
echo " "
echo "##########################################################################"
echo "COMPILATION COMPLETED  on CIFAR10 on ZCU102"
echo "##########################################################################"
echo " "
}

5_cifar10_vai_compile_vck190() {
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on VCK190: miniVggNet  on CIFAR10"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./build/quantized_results/cifar10/miniVggNet/quantize_eval_model.pb \
         --arch ./arch_vck190_dw.json \
       --output_dir=build/compile/cifar10/miniVggNet \
       --net_name=miniVggNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_miniVggNet.log
mv  ./build/compile/cifar10/miniVggNet/*.xmodel ./target_vck190/cifar10/miniVggNet/
rm  ./target_vck190/cifar10/miniVggNet/*_org.xmodel
echo " "
echo "##########################################################################"
echo "COMPILATION COMPLETED  on CIFAR10 on VCK190"
echo "##########################################################################"
echo " "
}


main() {



  # clean target boards
  rm -rf target_zcu104 target_vck190 target_zcu104.tar target_vck190.tar target_zcu102.tar
  rm -rf target_zcu102/cifar10 target_zcu102/fmnist target_zcu102/rpt target_zcu102/*.tar
  mkdir target_zcu102/cifar10 target_zcu102/rpt
  mkdir target_zcu102/cifar10/miniVggNet

  ##copy target_zcu102 files into the new target_zcu104 folder if you have also the ZCU104 board
  cp -r target_zcu102 target_vck190
  cp -r target_zcu102 target_zcu104

  # convert Keras model into TF inference graph
  2_cifar10_Keras2TF

  # freeze the graph to make predictions later
  3a_cifar10_freeze

  # make predictions with frozen graph
  3b_cifar10_evaluate_frozen_graph

  # quantize the CNN from 32-bit floating-point to 8-bit fixed-point
  4a_cifar10_quant

  # make predictions with quantized frozen graph
  4b_cifar10_evaluate_quantized_graph

  # compile xmodel file for ZCU102 target board
  5_cifar10_vai_compile_zcu102

  # compile xmodel file for ZCU102 target board
  5_cifar10_vai_compile_zcu102

  # compile xmodel file for VCK190 target board
  5_cifar10_vai_compile_vck190

  ## copy test images into target board
  cd build/dataset/cifar10
  cp -r ./test ./cifar10_test
  tar -cvf cifar10_test.tar ./cifar10_test &> /dev/null
  rm -rf cifar10_test
  cd ../../../
  cp ./build/dataset/cifar10/cifar10_test.tar ./target_zcu102/
  cp ./build/dataset/cifar10/cifar10_test.tar ./target_vck190/
  mv ./build/dataset/cifar10/cifar10_test.tar ./target_zcu104/

  # tar files for target boards
  tar -cvf target_zcu102.tar ./target_zcu102  &> /dev/null
  tar -cvf target_zcu104.tar ./target_zcu104  &> /dev/null
  tar -cvf target_vck190.tar ./target_vck190  &> /dev/null




}

main
