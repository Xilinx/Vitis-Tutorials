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
# date: 26 Nov 2020


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


1_fmnist_train() {
    echo " "
    echo "##########################################################################"
    echo "TRAIN & EVAL LeNet on FMNIST"
    echo "##########################################################################"
    python code/train_fmnist.py --network LeNet --weights keras_model/fmnist/LeNet --epochs 20 --init_lr 0.01 --batch_size 32
    #2>&1 | tee rpt/fmnist/1_train_fmnist_LeNet.log

    echo " "
    echo "##########################################################################"
    echo "TRAIN & EVAL miniVggNet  on FMNIST"
    echo "##########################################################################"
    python code/train_fmnist.py --network miniVggNet --weights keras_model/fmnist/miniVggNet --epochs 40 --init_lr 0.01 --batch_size 64
    #2>&1 | tee rpt/fmnist/1_train_fmnist_miniVggNet.log
    echo " "
    echo "##########################################################################"
    echo "TRAIN & EVAL miniGoogleNet on FMNIST"
    echo "##########################################################################"
    python code/train_fmnist.py   --network miniGoogleNet --weights keras_model/fmnist/miniGoogleNet --epochs 70 --init_lr 5e-3  --batch_size 128
    #2>&1 | tee rpt/fmnist/1_train_fmnist_miniGoogleNet.log
    echo " "
    echo "##########################################################################"
    echo "TRAIN & EVAL miniResNet  on FMNIST"
    echo "##########################################################################"
    python code/train_fmnist.py  --network miniResNet --weights keras_model/fmnist/miniResNet --epochs 100 --init_lr 0.1 --batch_size 128
    #2>&1 | tee rpt/fmnist/1_train_fmnist_miniResNet.log


}


2_fmnist_Keras2TF(){
    echo " "
    echo "############################################################################################"
    echo "KERAS to TENSORFLOW GRAPH CONVERSION for LeNet on FMNIST"
    echo "############################################################################################"
    python code/Keras2TF.py  --dataset fmnist -n LeNet
    #2>&1 | tee rpt/fmnist/2_keras2TF_graph_conversion_LeNet.log
    echo " "
    echo "#################################################################################################"
    echo "KERAS to TENSORFLOW GRAPH CONVERSION for miniVggNet  on FMNIST"
    echo "#################################################################################################"
    python code/Keras2TF.py  --dataset fmnist -n miniVggNet
    #2>&1 | tee rpt/fmnist/2_keras2TF_graph_conversion_miniVggNet.log
    echo " "
    echo "#################################################################################################"
    echo "KERAS to TENSORFLOW GRAPH CONVERSION for miniGoogleNet  on FMNIST"
    echo "#################################################################################################"
    python code/Keras2TF.py --dataset fmnist -n miniGoogleNet
    #2>&1 | tee rpt/fmnist/2_keras2TF_graph_conversion_miniGoogleNet.log
    echo " "
    echo "#################################################################################################"
    echo "KERAS to TENSORFLOW GRAPH CONVERSION for miniResNet  on FMNIST"
    echo "#################################################################################################"
    python code/Keras2TF.py --dataset fmnist -n miniResNet
    #2>&1 | tee rpt/fmnist/2_keras2TF_graph_conversion_miniResNet.log
}


3a_fmnist_freeze(){
  echo " "
  echo "##########################################################################"
  echo "FREEZE GRAPH of LeNet on FMNIST"
  echo "##########################################################################"
  rm ./freeze/fmnist/Lenet/*
  freeze_graph --input_meta_graph=./tf_chkpts/fmnist/LeNet/float_model.ckpt.meta \
               --input_checkpoint=./tf_chkpts/fmnist/LeNet/float_model.ckpt \
               --input_binary=true \
               --output_graph=./freeze/fmnist/LeNet/frozen_graph.pb \
               --output_node_names=${LENET_OUT_NODE}
               #2>&1 | tee rpt/fmnist/3a_freeze_graph_LeNet.log
  echo " "
  echo "##########################################################################"
  echo "INSPECT FROZEN GRAPH of LeNet  on FMNIST"
  echo "##########################################################################"
  vai_q_tensorflow inspect --input_frozen_graph=./freeze/fmnist/LeNet/frozen_graph.pb

  echo " "
  echo "##########################################################################"
  echo "FREEZE GRAPH of miniVggNet  on FMNIST"
  echo "##########################################################################"
  rm ./freeze/fmnist/miniVggNet/*
  freeze_graph --input_meta_graph=./tf_chkpts/fmnist/miniVggNet/float_model.ckpt.meta \
               --input_checkpoint=./tf_chkpts/fmnist/miniVggNet/float_model.ckpt \
               --input_binary=true \
               --output_graph=./freeze/fmnist/miniVggNet/frozen_graph.pb \
               --output_node_names=${VGG_OUT_NODE}
               #2>&1 | tee rpt/fmnist/3a_freeze_graph_miniVggNet.log
  echo " "
  echo "##########################################################################"
  echo "INSPECT FROZEN GRAPH of miniVggNet on FMNIST"
  echo "##########################################################################"
  vai_q_tensorflow inspect --input_frozen_graph=./freeze/fmnist/miniVggNet/frozen_graph.pb

  # freeze trained graph
  echo "##########################################################################"
  echo "FREEZE GRAPH of miniGoogleNet  on FMNIST"
  echo "##########################################################################"
  rm ./freeze/fmnist/miniGoogleNet/*
  freeze_graph --input_meta_graph=./tf_chkpts/fmnist/miniGoogleNet/float_model.ckpt.meta \
               --input_checkpoint=./tf_chkpts/fmnist/miniGoogleNet/float_model.ckpt \
               --input_binary=true \
               --output_graph=./freeze/fmnist/miniGoogleNet/frozen_graph.pb \
               --output_node_names=${GOOGLE_OUT_NODE}
               #2>&1 | tee rpt/fmnist/3a_freeze_graph_miniGoogleNet.log
  echo " "
  echo "##########################################################################"
  echo "INSPECT FROZEN GRAPH of miniGoogleNet  on FMNIST"
  echo "##########################################################################"
  vai_q_tensorflow inspect --input_frozen_graph=./freeze/fmnist/miniGoogleNet/frozen_graph.pb

  echo "##########################################################################"
  echo "FREEZE GRAPH of miniResNet  on FMNIST"
  echo "##########################################################################"
  rm ./freeze/fmnist/miniResNet/*
  freeze_graph --input_meta_graph=./tf_chkpts/fmnist/miniResNet/float_model.ckpt.meta \
               --input_checkpoint=./tf_chkpts/fmnist/miniResNet/float_model.ckpt \
               --input_binary=true \
               --output_graph=./freeze/fmnist/miniResNet/frozen_graph.pb \
               --output_node_names=${RESNET_OUT_NODE}
               #2>&1 | tee rpt/fmnist/3a_freeze_graph_miniResNet.log
  echo " "
  echo "##########################################################################"
  echo "INSPECT FROZEN GRAPH of miniResNet"
  echo "##########################################################################"
  vai_q_tensorflow inspect --input_frozen_graph=./freeze/fmnist/miniResNet/frozen_graph.pb

  echo " "
  echo "##########################################################################"
  echo "FREEZE GRAPH COMPLETED"
  echo "##########################################################################"
  echo " "
}

3b_fmnist_evaluate_frozen_graph() {
  echo " "
  echo "##########################################################################"
  echo "EVALUATE FROZEN GRAPH of LeNet on FMNIST"
  echo "##########################################################################"
  python code/fmnist_eval_graph.py --dataset fmnist --graph ./freeze/fmnist/LeNet/frozen_graph.pb --input_node ${CNN_INP_NODE} --output_node ${LENET_OUT_NODE} --gpu 0  #2>&1 | tee rpt/fmnist/3b_evaluate_frozen_graph_LeNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE FROZEN GRAPH of miniVggNet  on FMNIST"
  echo "##########################################################################"
  python code/fmnist_eval_graph.py --dataset fmnist --graph ./freeze/fmnist/miniVggNet/frozen_graph.pb --input_node ${CNN_INP_NODE} --output_node ${VGG_OUT_NODE}  --gpu 0  #2>&1 | tee rpt/fmnist/3b_evaluate_frozen_graph_miniVggNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE FROZEN GRAPH of GoogleNet  on FMNIST"
  echo "##########################################################################"
  python code/fmnist_eval_graph.py --dataset fmnist --graph ./freeze/fmnist/miniGoogleNet/frozen_graph.pb --input_node ${CNN_INP_NODE} --output_node ${GOOGLE_OUT_NODE}  --gpu 0  #2>&1 | tee rpt/fmnist/3b_evaluate_frozen_graph_miniGoogleNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE FROZEN GRAPH of ResNet  on FMNIST"
  echo "##########################################################################"
  python code/fmnist_eval_graph.py --dataset fmnist --graph ./freeze/fmnist/miniResNet/frozen_graph.pb --input_node ${CNN_INP_NODE} --output_node ${RESNET_OUT_NODE}  --gpu 0  #2>&1 | tee rpt/fmnist/3b_evaluate_frozen_graph_miniResNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE FROZEN GRAPH COMPLETED  on FMNIST"
  echo "##########################################################################"
  echo " "
}

4a_fmnist_quant() {
  cd ./code
  echo " "
  echo "##########################################################################"
  echo "QUANTIZE LeNet on FMNIST"
  echo "##########################################################################"
  vai_q_tensorflow  quantize \
	--input_frozen_graph ../freeze/fmnist/LeNet/frozen_graph.pb \
	--input_nodes ${CNN_INP_NODE} \
	--input_shapes ?,32,32,3 \
	--output_nodes ${LENET_OUT_NODE}\
	--output_dir ../quantized_results/fmnist/LeNet/ \
	--method 1 \
	--input_fn fmnist_graph_input_fn.calib_input \
	--calib_iter 20 \
	--gpu 0
  #2>&1 | tee ../rpt/fmnist/4a_quant_LeNet.log
  echo " "
  echo "##########################################################################"
  echo "QUANTIZE miniVggNet  on FMNIST"
  echo "##########################################################################"
  vai_q_tensorflow quantize \
	--input_frozen_graph ../freeze/fmnist/miniVggNet/frozen_graph.pb \
	--input_nodes ${CNN_INP_NODE} \
	--input_shapes ?,32,32,3 \
	--output_nodes ${VGG_OUT_NODE} \
	--output_dir ../quantized_results/fmnist/miniVggNet/ \
	--method 1 \
	--input_fn fmnist_graph_input_fn.calib_input \
	--calib_iter 20 \
	--gpu 0
  #2>&1 | tee ../rpt/fmnist/4a_quant_miniVggNet.log
  echo " "
  echo "##########################################################################"
  echo "QUANTIZE miniGoogleNet  on FMNIST"
  echo "##########################################################################"
  vai_q_tensorflow quantize \
	--input_frozen_graph ../freeze/fmnist/miniGoogleNet/frozen_graph.pb \
	--input_nodes ${CNN_INP_NODE} \
	--input_shapes ?,32,32,3 \
	--output_nodes ${GOOGLE_OUT_NODE} \
	--output_dir ../quantized_results/fmnist/miniGoogleNet/ \
	--method 1 \
	--input_fn fmnist_graph_input_fn.calib_input \
	--calib_iter 20 \
	--gpu 0
  #2>&1 | tee ../rpt/fmnist/4a_quant_miniGoogleNet.log
  echo " "
  echo "##########################################################################"
  echo "QUANTIZE miniResNet  on FMNIST"
  echo "##########################################################################"
  vai_q_tensorflow quantize \
	--input_frozen_graph ../freeze/fmnist/miniResNet/frozen_graph.pb \
	--input_nodes ${CNN_INP_NODE} \
	--input_shapes ?,32,32,3 \
	--output_nodes ${RESNET_OUT_NODE} \
	--output_dir ../quantized_results/fmnist/miniResNet/ \
	--method 1 \
	--input_fn fmnist_graph_input_fn.calib_input \
	--calib_iter 20 \
	--gpu 0
  #2>&1 | tee ../rpt/fmnist/4a_quant_miniResNet.log
  echo " "
  echo "##########################################################################"
  echo "QUANTIZATION COMPLETED  on FMNIST"
  echo "##########################################################################"
  echo " "
  cd ..
}


4b_fmnist_evaluate_quantized_graph() {
  echo " "
  echo "##########################################################################"
  echo "EVALUATE QUANTIZED GRAPH of LeNet on FMNIST"
  echo "##########################################################################"
  python code/fmnist_eval_graph.py --dataset fmnist --graph ./quantized_results/fmnist/LeNet/quantize_eval_model.pb --input_node ${CNN_INP_NODE} --output_node ${LENET_OUT_NODE} --gpu 0  #2>&1 | tee rpt/fmnist/4b_evaluate_quantized_graph_LeNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE QUANTIZED GRAPH of miniVggNet  on FMNIST"
  echo "##########################################################################"
  python code/fmnist_eval_graph.py --dataset fmnist --graph ./quantized_results/fmnist/miniVggNet/quantize_eval_model.pb --input_node ${CNN_INP_NODE} --output_node ${VGG_OUT_NODE} --gpu 0 #2>&1 | tee rpt/fmnist/4b_evaluate_quantized_graph_miniVggNet.log
  echo " "
  echo "##############################################################################"
  echo "EVALUATE QUANTIZED GRAPH of miniGoogleNet  on FMNIST"
  echo "##############################################################################"
  python code/fmnist_eval_graph.py --dataset fmnist --graph ./quantized_results/fmnist/miniGoogleNet/quantize_eval_model.pb --input_node ${CNN_INP_NODE} --output_node ${GOOGLE_OUT_NODE} --gpu 0 #2>&1 | tee rpt/fmnist/4b_evaluate_quantized_graph_miniGoogleNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE QUANTIZED GRAPH of miniResNet  on FMNIST"
  echo "##########################################################################"
  python code/fmnist_eval_graph.py --dataset fmnist --graph ./quantized_results/fmnist/miniResNet/quantize_eval_model.pb --input_node ${CNN_INP_NODE} --output_node ${RESNET_OUT_NODE} --gpu 0 #2>&1 | tee rpt/fmnist/4b_evaluate_quantized_graph_miniResNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE QUANTIZED GRAPH COMPLETED  on FMNIST"
  echo "##########################################################################"
  echo " "
}

5_fmnist_vai_compile_vck190() {
    echo " "
    echo "##########################################################################"
    echo "COMPILE WITH Vitis  on VCK190: LeNet on FMNIST"
    echo "##########################################################################"
    vai_c_tensorflow \
        --frozen_pb=./quantized_results/fmnist/LeNet/quantize_eval_model.pb \
        --arch /opt/vitis_ai/compiler/arch/DPUCVDX8G/VCK190/arch.json \
        --output_dir=compile/fmnist/LeNet \
        --net_name=LeNet \
        --options    "{'mode':'normal'}"
        #2>&1 | tee rpt/fmnist/5_vai_compile_LeNet.log
    mv  ./compile/fmnist/LeNet/*.xmodel ./target_vck190/fmnist/LeNet/model/
    rm ./target_vck190/fmnist/LeNet/model/*_org.xmodel
    echo " "
    echo "##########################################################################"
    echo "COMPILE WITH Vitis AI on VCK190: miniVggNet  on FMNIST"
    echo "##########################################################################"
    vai_c_tensorflow \
        --frozen_pb=./quantized_results/fmnist/miniVggNet/quantize_eval_model.pb \
        --arch /opt/vitis_ai/compiler/arch/DPUCVDX8G/VCK190/arch.json \
        --output_dir=compile/fmnist/miniVggNet \
        --net_name=miniVggNet \
        --options    "{'mode':'normal'}"
        #2>&1 | tee rpt/fmnist/5_vai_compile_miniVggNet.log
    mv  ./compile/fmnist/miniVggNet/*.xmodel ./target_vck190/fmnist/miniVggNet/model/
    rm ./target_vck190/fmnist/miniVggNet/model/*_org.xmodel

    echo " "
    echo "##########################################################################"
    echo "COMPILE WITH Vitis AI on VCK190: miniGoogleNet  on FMNIST"
    echo "##########################################################################"
    vai_c_tensorflow \
        --frozen_pb=./quantized_results/fmnist/miniGoogleNet/quantize_eval_model.pb \
        --arch /opt/vitis_ai/compiler/arch/DPUCVDX8G/VCK190/arch.json \
        --output_dir=compile/fmnist/miniGoogleNet \
        --net_name=miniGoogleNet \
        --options    "{'mode':'normal'}"
        #2>&1 | tee rpt/fmnist/5_vai_compile_miniGoogleNet.log
    mv  ./compile/fmnist/miniGoogleNet/*.xmodel ./target_vck190/fmnist/miniGoogleNet/model/
    rm ./target_vck190/fmnist/miniGoogleNet/model/*_org.xmodel

    : '
    # this CNN is not yet supported by XVDPU due to a missing layer
    echo " "
    echo "##########################################################################"
    echo "COMPILE WITH Vitis AI on VCK190: miniResNet  on FMNIST"
    echo "##########################################################################"
    vai_c_tensorflow \
        --frozen_pb=./quantized_results/fmnist/miniResNet/quantize_eval_model.pb \
        --arch /opt/vitis_ai/compiler/arch/DPUCVDX8G/VCK190/arch.json \
        --output_dir=compile/fmnist/miniResNet \
        --net_name=miniResNet \
        --options    "{'mode':'normal'}"
        #2>&1 | tee rpt/fmnist/5_vai_compile_miniResNet.log
    mv  ./compile/fmnist/miniResNet/*.xmodel ./target_vck190/fmnist/miniResNet/model/
    rm ./target_vck190/fmnist/miniResNet/model/*_org.xmodel
    '

    echo " "
    echo "##########################################################################"
    echo "COMPILATION COMPLETED  on FMNIST on VCK190"
    echo "##########################################################################"
    echo " "
}


5_fmnist_vai_compile_zcu104() {
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU104: LeNet on FMNIST"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/fmnist/LeNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
       --output_dir=compile/fmnist/LeNet \
       --net_name=LeNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/fmnist/5_vai_compile_LeNet.log

mv  ./compile/fmnist/LeNet/*.xmodel ./target_zcu104/fmnist/LeNet/model/
rm ./target_zcu104/fmnist/LeNet/model/*_org.xmodel

# Compile
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU104: miniVggNet  on FMNIST"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/fmnist/miniVggNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
       --output_dir=compile/fmnist/miniVggNet \
       --net_name=miniVggNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/fmnist/5_vai_compile_miniVggNet.log

mv  ./compile/fmnist/miniVggNet/*.xmodel ./target_zcu104/fmnist/miniVggNet/model/
rm ./target_zcu104/fmnist/miniVggNet/model/*_org.xmodel

echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU104: miniGoogleNet  on FMNIST"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/fmnist/miniGoogleNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
       --output_dir=compile/fmnist/miniGoogleNet \
       --net_name=miniGoogleNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/fmnist/5_vai_compile_miniGoogleNet.log

mv  ./compile/fmnist/miniGoogleNet/*.xmodel ./target_zcu104/fmnist/miniGoogleNet/model/
rm  ./target_zcu104/fmnist/miniGoogleNet/model/*_org.xmodel

echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU104: miniResNet  on FMNIST"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/fmnist/miniResNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
       --output_dir=compile/fmnist/miniResNet \
       --net_name=miniResNet \
       --options    "{'mode':'normal'}"
      # 2>&1 | tee rpt/fmnist/5_vai_compile_miniResNet.log

mv  ./compile/fmnist/miniResNet/*.xmodel ./target_zcu104/fmnist/miniResNet/model/
rm ./target_zcu104/fmnist/miniResNet/model/*_org.xmodel

echo " "
echo "##########################################################################"
echo "COMPILATION COMPLETED  on FMNIST on ZCU104"
echo "##########################################################################"
echo " "
}


5_fmnist_vai_compile_zcu102() {
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU102: LeNet on FMNIST"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/fmnist/LeNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
       --output_dir=compile/fmnist/LeNet \
       --net_name=LeNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/fmnist/5_vai_compile_LeNet.log

mv ./compile/fmnist/LeNet/*.xmodel ./target_zcu102/fmnist/LeNet/model/
rm ./target_zcu102/fmnist/LeNet/model/*_org.xmodel

# Compile
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU102: miniVggNet  on FMNIST"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/fmnist/miniVggNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
       --output_dir=compile/fmnist/miniVggNet \
       --net_name=miniVggNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/fmnist/5_vai_compile_miniVggNet.log

mv  ./compile/fmnist/miniVggNet/*.xmodel ./target_zcu102/fmnist/miniVggNet/model/
rm ./target_zcu102/fmnist/miniVggNet/model/*_org.xmodel

echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU102: miniGoogleNet  on FMNIST"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/fmnist/miniGoogleNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
       --output_dir=compile/fmnist/miniGoogleNet \
       --net_name=miniGoogleNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/fmnist/5_vai_compile_miniGoogleNet.log

mv  ./compile/fmnist/miniGoogleNet/*.xmodel ./target_zcu102/fmnist/miniGoogleNet/model/
rm  ./target_zcu102/fmnist/miniGoogleNet/model/*_org.xmodel

echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU102: miniResNet  on FMNIST"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/fmnist/miniResNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
       --output_dir=compile/fmnist/miniResNet \
       --net_name=miniResNet \
       --options    "{'mode':'normal'}"
      # 2>&1 | tee rpt/fmnist/5_vai_compile_miniResNet.log

mv  ./compile/fmnist/miniResNet/*.xmodel ./target_zcu102/fmnist/miniResNet/model/
rm ./target_zcu102/fmnist/miniResNet/model/*_org.xmodel

echo " "
echo "##########################################################################"
echo "COMPILATION COMPLETED on ZCU102 on FMNIST"
echo "##########################################################################"
echo " "
}

main() {

  ##copy target_zcu102 files into the new target_zcu104 folder if you have also the ZCU104 board
  cp -r target_zcu102 target_zcu104
  ##copy target_zcu102 files into the new target_vck190 folder if you have also the VCK190 board
  cp -r target_zcu102 target_vck190

  # training from scratch with FMNIST
  1_fmnist_train

  # convert Keras model into TF inference graph
  2_fmnist_Keras2TF

  # freeze the graph to make predictions later
  3a_fmnist_freeze

  # make predictions with frozen graph
  3b_fmnist_evaluate_frozen_graph

  # quantize the CNN from 32-bit floating-point to 8-bit fixed-point
  4a_fmnist_quant

  # make predictions with quantized frozen graph
  4b_fmnist_evaluate_quantized_graph

  ## compile ELF file for VCK190 target board
  5_fmnist_vai_compile_vck190

  # compile ELF file for ZCU102 target board
  5_fmnist_vai_compile_zcu102

  # compile ELF file for ZCU104 target board
  5_fmnist_vai_compile_zcu104

  # compile ELF file for VCK190 target board
  5_cifar10_vai_compile_vck190

  ## copy test images into target board
  tar -cvf fmnist_test.tar ./dataset/fmnist/test
  cp ./fmnist_test.tar ./target_zcu102/
  cp ./fmnist_test.tar ./target_vck190/
  mv ./fmnist_test.tar ./target_zcu104/


}

main
