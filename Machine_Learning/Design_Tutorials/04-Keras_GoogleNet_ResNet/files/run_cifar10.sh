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


1_cifar10_train() {
    echo " "
    echo "##########################################################################"
    echo "TRAIN & EVAL LeNet on CIFAR10"
    echo "##########################################################################"
    python code/train_cifar10.py --network LeNet --weights keras_model/cifar10/LeNet --epochs 20 --init_lr 0.01 --batch_size 32
    #2>&1 | tee rpt/cifar10/1_train_cifar10_LeNet.log
    echo " "
    echo "##########################################################################"
    echo "TRAIN & EVAL miniVggNet  on CIFAR10"
    echo "##########################################################################"
    python code/train_cifar10.py --network miniVggNet --weights keras_model/cifar10/miniVggNet --epochs 40 --init_lr 0.01 --batch_size 64
    #2>&1 | tee rpt/cifar10/1_train_cifar10_miniVggNet.log
    echo " "
    echo "##########################################################################"
    echo "TRAIN & EVAL miniGoogleNet on CIFAR10"
    echo "##########################################################################"
    python code/train_cifar10.py   --network miniGoogleNet --weights keras_model/cifar10/miniGoogleNet --epochs 70 --init_lr 5e-3  --batch_size 128
    #2>&1 | tee rpt/cifar10/1_train_cifar10_miniGoogleNet.log

    echo " "
    echo "##########################################################################"
    echo "TRAIN & EVAL miniResNet  on CIFAR10"
    echo "##########################################################################"
    python code/train_cifar10.py  --network miniResNet --weights keras_model/cifar10/miniResNet --epochs 100 --init_lr 0.1 --batch_size 128
    #2>&1 | tee rpt/cifar10/1_train_cifar10_miniResNet.log

}


2_cifar10_Keras2TF(){
    echo " "
    echo "############################################################################################"
    echo "KERAS to TENSORFLOW GRAPH CONVERSION for LeNet on CIFAR10"
    echo "############################################################################################"
    python code/Keras2TF.py  --dataset cifar10 -n LeNet
    #2>&1 | tee rpt/cifar10/2_keras2TF_graph_conversion_LeNet.log
    echo " "
    echo "#################################################################################################"
    echo "KERAS to TENSORFLOW GRAPH CONVERSION for miniVggNet  on CIFAR10"
    echo "#################################################################################################"
    python code/Keras2TF.py  --dataset cifar10 -n miniVggNet
    #2>&1 | tee rpt/cifar10/2_keras2TF_graph_conversion_miniVggNet.log
    echo " "
    echo "#################################################################################################"
    echo "KERAS to TENSORFLOW GRAPH CONVERSION for miniGoogleNet  on CIFAR10"
    echo "#################################################################################################"
    python code/Keras2TF.py --dataset cifar10 -n miniGoogleNet
    #2>&1 | tee rpt/cifar10/2_keras2TF_graph_conversion_miniGoogleNet.log
    echo " "
    echo "#################################################################################################"
    echo "KERAS to TENSORFLOW GRAPH CONVERSION for miniResNet  on CIFAR10"
    echo "#################################################################################################"
    python code/Keras2TF.py --dataset cifar10 -n miniResNet
    #2>&1 | tee rpt/cifar10/2_keras2TF_graph_conversion_miniResNet.log
}


3a_cifar10_freeze(){
  echo " "
  echo "##########################################################################"
  echo "FREEZE GRAPH of LeNet on CIFAR10"
  echo "##########################################################################"
  rm ./freeze/cifar10/Lenet/*
  freeze_graph --input_meta_graph=./tf_chkpts/cifar10/LeNet/float_model.ckpt.meta \
               --input_checkpoint=./tf_chkpts/cifar10/LeNet/float_model.ckpt \
               --input_binary=true \
               --output_graph=./freeze/cifar10/LeNet/frozen_graph.pb \
               --output_node_names=${LENET_OUT_NODE}
               #2>&1 | tee rpt/cifar10/3a_freeze_graph_LeNet.log
  echo " "
  echo "##########################################################################"
  echo "INSPECT FROZEN GRAPH of LeNet  on CIFAR10"
  echo "##########################################################################"
  vai_q_tensorflow inspect --input_frozen_graph=./freeze/cifar10/LeNet/frozen_graph.pb

  echo " "
  echo "##########################################################################"
  echo "FREEZE GRAPH of miniVggNet  on CIFAR10"
  echo "##########################################################################"
  rm ./freeze/cifar10/miniVggNet/*
  freeze_graph --input_meta_graph=./tf_chkpts/cifar10/miniVggNet/float_model.ckpt.meta \
               --input_checkpoint=./tf_chkpts/cifar10/miniVggNet/float_model.ckpt \
               --input_binary=true \
               --output_graph=./freeze/cifar10/miniVggNet/frozen_graph.pb \
               --output_node_names=${VGG_OUT_NODE}
               #2>&1 | tee rpt/cifar10/3a_freeze_graph_miniVggNet.log
  echo " "
  echo "##########################################################################"
  echo "INSPECT FROZEN GRAPH of miniVggNet on CIFAR10"
  echo "##########################################################################"
  vai_q_tensorflow inspect --input_frozen_graph=./freeze/cifar10/miniVggNet/frozen_graph.pb

  # freeze trained graph
  echo "##########################################################################"
  echo "FREEZE GRAPH of miniGoogleNet  on CIFAR10"
  echo "##########################################################################"
  rm ./freeze/cifar10/miniGoogleNet/*
  freeze_graph --input_meta_graph=./tf_chkpts/cifar10/miniGoogleNet/float_model.ckpt.meta \
               --input_checkpoint=./tf_chkpts/cifar10/miniGoogleNet/float_model.ckpt \
               --input_binary=true \
               --output_graph=./freeze/cifar10/miniGoogleNet/frozen_graph.pb \
               --output_node_names=${GOOGLE_OUT_NODE}
               #2>&1 | tee rpt/cifar10/3a_freeze_graph_miniGoogleNet.log
  echo " "
  echo "##########################################################################"
  echo "INSPECT FROZEN GRAPH of miniGoogleNet  on CIFAR10"
  echo "##########################################################################"
  vai_q_tensorflow inspect --input_frozen_graph=./freeze/cifar10/miniGoogleNet/frozen_graph.pb

  echo "##########################################################################"
  echo "FREEZE GRAPH of miniResNet  on CIFAR10"
  echo "##########################################################################"
  rm ./freeze/cifar10/miniResNet/*
  freeze_graph --input_meta_graph=./tf_chkpts/cifar10/miniResNet/float_model.ckpt.meta \
               --input_checkpoint=./tf_chkpts/cifar10/miniResNet/float_model.ckpt \
               --input_binary=true \
               --output_graph=./freeze/cifar10/miniResNet/frozen_graph.pb \
               --output_node_names=${RESNET_OUT_NODE}
               #2>&1 | tee rpt/cifar10/3a_freeze_graph_miniResNet.log
  echo " "
  echo "##########################################################################"
  echo "INSPECT FROZEN GRAPH of miniResNet"
  echo "##########################################################################"
  vai_q_tensorflow inspect --input_frozen_graph=./freeze/cifar10/miniResNet/frozen_graph.pb

  echo " "
  echo "##########################################################################"
  echo "FREEZE GRAPH COMPLETED"
  echo "##########################################################################"
  echo " "
}

3b_cifar10_evaluate_frozen_graph() {
  echo " "
  echo "##########################################################################"
  echo "EVALUATE FROZEN GRAPH of LeNet on CIFAR10"
  echo "##########################################################################"
  python code/cifar10_eval_graph.py --dataset cifar10 --graph ./freeze/cifar10/LeNet/frozen_graph.pb --input_node ${CNN_INP_NODE} --output_node ${LENET_OUT_NODE} --gpu 0  #2>&1 | tee rpt/cifar10/3b_evaluate_frozen_graph_LeNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE FROZEN GRAPH of miniVggNet  on CIFAR10"
  echo "##########################################################################"
  python code/cifar10_eval_graph.py --dataset cifar10 --graph ./freeze/cifar10/miniVggNet/frozen_graph.pb --input_node ${CNN_INP_NODE} --output_node ${VGG_OUT_NODE}  --gpu 0  #2>&1 | tee rpt/cifar10/3b_evaluate_frozen_graph_miniVggNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE FROZEN GRAPH of GoogleNet  on CIFAR10"
  echo "##########################################################################"
  python code/cifar10_eval_graph.py --dataset cifar10 --graph ./freeze/cifar10/miniGoogleNet/frozen_graph.pb --input_node ${CNN_INP_NODE} --output_node ${GOOGLE_OUT_NODE}  --gpu 0  #2>&1 | tee rpt/cifar10/3b_evaluate_frozen_graph_miniGoogleNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE FROZEN GRAPH of ResNet  on CIFAR10"
  echo "##########################################################################"
  python code/cifar10_eval_graph.py --dataset cifar10 --graph ./freeze/cifar10/miniResNet/frozen_graph.pb --input_node ${CNN_INP_NODE} --output_node ${RESNET_OUT_NODE}  --gpu 0  #2>&1 | tee rpt/cifar10/3b_evaluate_frozen_graph_miniResNet.log
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
  echo "QUANTIZE LeNet on CIFAR10"
  echo "##########################################################################"
  vai_q_tensorflow  quantize \
	--input_frozen_graph ../freeze/cifar10/LeNet/frozen_graph.pb \
	--input_nodes ${CNN_INP_NODE} \
	--input_shapes ?,32,32,3 \
	--output_nodes ${LENET_OUT_NODE}\
	--output_dir ../quantized_results/cifar10/LeNet/ \
	--method 1 \
	--input_fn cifar10_graph_input_fn.calib_input \
	--calib_iter 20 \
	--gpu 0
  #2>&1 | tee ../rpt/cifar10/4a_quant_LeNet.log
  echo " "
  echo "##########################################################################"
  echo "QUANTIZE miniVggNet  on CIFAR10"
  echo "##########################################################################"
  vai_q_tensorflow quantize \
	--input_frozen_graph ../freeze/cifar10/miniVggNet/frozen_graph.pb \
	--input_nodes ${CNN_INP_NODE} \
	--input_shapes ?,32,32,3 \
	--output_nodes ${VGG_OUT_NODE} \
	--output_dir ../quantized_results/cifar10/miniVggNet/ \
	--method 1 \
	--input_fn cifar10_graph_input_fn.calib_input \
	--calib_iter 20 \
	--gpu 0
  #2>&1 | tee ../rpt/cifar10/4a_quant_miniVggNet.log
  echo " "
  echo "##########################################################################"
  echo "QUANTIZE miniGoogleNet  on CIFAR10"
  echo "##########################################################################"
  vai_q_tensorflow quantize \
	--input_frozen_graph ../freeze/cifar10/miniGoogleNet/frozen_graph.pb \
	--input_nodes ${CNN_INP_NODE} \
	--input_shapes ?,32,32,3 \
	--output_nodes ${GOOGLE_OUT_NODE} \
	--output_dir ../quantized_results/cifar10/miniGoogleNet/ \
	--method 1 \
	--input_fn cifar10_graph_input_fn.calib_input \
	--calib_iter 20 \
	--gpu 0
  #2>&1 | tee ../rpt/cifar10/4a_quant_miniGoogleNet.log
  echo " "
  echo "##########################################################################"
  echo "QUANTIZE miniResNet  on CIFAR10"
  echo "##########################################################################"
  vai_q_tensorflow quantize \
	--input_frozen_graph ../freeze/cifar10/miniResNet/frozen_graph.pb \
	--input_nodes ${CNN_INP_NODE} \
	--input_shapes ?,32,32,3 \
	--output_nodes ${RESNET_OUT_NODE} \
	--output_dir ../quantized_results/cifar10/miniResNet/ \
	--method 1 \
	--input_fn cifar10_graph_input_fn.calib_input \
	--calib_iter 20 \
	--gpu 0
  #2>&1 | tee ../rpt/cifar10/4a_quant_miniResNet.log
  echo " "
  echo "##########################################################################"
  echo "QUANTIZATION COMPLETED  on CIFAR10"
  echo "##########################################################################"
  echo " "
  cd ..
}


4b_cifar10_evaluate_quantized_graph() {
  echo " "
  echo "##########################################################################"
  echo "EVALUATE QUANTIZED GRAPH of LeNet on CIFAR10"
  echo "##########################################################################"
  python code/cifar10_eval_graph.py --dataset cifar10 --graph ./quantized_results/cifar10/LeNet/quantize_eval_model.pb --input_node ${CNN_INP_NODE} --output_node ${LENET_OUT_NODE} --gpu 0  #2>&1 | tee rpt/cifar10/4b_evaluate_quantized_graph_LeNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE QUANTIZED GRAPH of miniVggNet  on CIFAR10"
  echo "##########################################################################"
  python code/cifar10_eval_graph.py --dataset cifar10 --graph ./quantized_results/cifar10/miniVggNet/quantize_eval_model.pb --input_node ${CNN_INP_NODE} --output_node ${VGG_OUT_NODE} --gpu 0 #2>&1 | tee rpt/cifar10/4b_evaluate_quantized_graph_miniVggNet.log
  echo " "
  echo "##############################################################################"
  echo "EVALUATE QUANTIZED GRAPH of miniGoogleNet  on CIFAR10"
  echo "##############################################################################"
  python code/cifar10_eval_graph.py --dataset cifar10 --graph ./quantized_results/cifar10/miniGoogleNet/quantize_eval_model.pb --input_node ${CNN_INP_NODE} --output_node ${GOOGLE_OUT_NODE} --gpu 0 #2>&1 | tee rpt/cifar10/4b_evaluate_quantized_graph_miniGoogleNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE QUANTIZED GRAPH of miniResNet  on CIFAR10"
  echo "##########################################################################"
  python code/cifar10_eval_graph.py --dataset cifar10 --graph ./quantized_results/cifar10/miniResNet/quantize_eval_model.pb --input_node ${CNN_INP_NODE} --output_node ${RESNET_OUT_NODE} --gpu 0 #2>&1 | tee rpt/cifar10/4b_evaluate_quantized_graph_miniResNet.log
  echo " "
  echo "##########################################################################"
  echo "EVALUATE QUANTIZED GRAPH COMPLETED  on CIFAR10"
  echo "##########################################################################"
  echo " "
}

5_cifar10_vai_compile_vck190() {
    echo " "
    echo "##########################################################################"
    echo "COMPILE WITH Vitis AI on VCK190: LeNet on CIFAR10"
    echo "##########################################################################"
    vai_c_tensorflow \
        --frozen_pb=./quantized_results/cifar10/LeNet/quantize_eval_model.pb \
        --arch /opt/vitis_ai/compiler/arch/DPUCVDX8G/VCK190/arch.json \
        --output_dir=compile/cifar10/LeNet \
        --net_name=LeNet \
        --options    "{'mode':'normal'}" \
        #2>&1 | tee rpt/cifar10/5_vai_compile_LeNet.log
    mv  ./compile/cifar10/LeNet/*.xmodel ./target_vck190/cifar10/LeNet/model/
    rm  ./target_vck190/cifar10/LeNet/model/*_org.xmodel

    echo " "
    echo "##########################################################################"
    echo "COMPILE WITH Vitis AI on VCK190: miniVggNet  on CIFAR10"
    echo "##########################################################################"
    vai_c_tensorflow \
        --frozen_pb=./quantized_results/cifar10/miniVggNet/quantize_eval_model.pb \
        --arch /opt/vitis_ai/compiler/arch/DPUCVDX8G/VCK190/arch.json \
        --output_dir=compile/cifar10/miniVggNet \
        --net_name=miniVggNet \
        --options    "{'mode':'normal'}" \
        #2>&1 | tee rpt/cifar10/5_vai_compile_miniVggNet.log
    mv  ./compile/cifar10/miniVggNet/*.xmodel ./target_vck190/cifar10/miniVggNet/model/
    rm  ./target_vck190/cifar10/miniVggNet/model/*_org.xmodel
    : '
    vai_c_tensorflow \
        --frozen_pb=./quantized_results/cifar10/miniVggNet/quantize_eval_model.pb \
        --arch /opt/vitis_ai/compiler/arch/DPUCVDX8G/VCK190/arch.json \
        --output_dir=compile/cifar10/miniVggNet \
        --net_name=miniVggNet \
        --options    "{'mode':'debug'}" \
        #2>&1 | tee rpt/cifar10/5_vai_compile_miniVggNet.log
    mv  ./compile/cifar10/miniVggNet/*.xmodel ./target_vck190/cifar10/miniVggNet/model/dbg_dpu_miniVggNet_0.xmodel
    rm  ./target_vck190/cifar10/miniVggNet/model/*_org.xmodel
    '
    echo " "
    echo "##########################################################################"
    echo "COMPILE WITH Vitis AI on VCK190: miniGoogleNet  on CIFAR10"
    echo "##########################################################################"
    vai_c_tensorflow \
        --frozen_pb=./quantized_results/cifar10/miniGoogleNet/quantize_eval_model.pb \
        --arch /opt/vitis_ai/compiler/arch/DPUCVDX8G/VCK190/arch.json \
        --output_dir=compile/cifar10/miniGoogleNet \
        --net_name=miniGoogleNet \
        --options    "{'mode':'normal'}" \
        #2>&1 | tee rpt/cifar10/5_vai_compile_miniGoogleNet.log
    mv  ./compile/cifar10/miniGoogleNet/*.xmodel ./target_vck190/cifar10/miniGoogleNet/model/
    rm  ./target_vck190/cifar10/miniGoogleNet/model/*_org.xmodel

    : '
    #This CNN is not yet supprted on XVDPU because of a missing layer
    echo " "
    echo "##########################################################################"
    echo "COMPILE WITH Vitis AI on VCK190: miniResNet  on CIFAR10"
    echo "##########################################################################"
    vai_c_tensorflow \
        --frozen_pb=./quantized_results/cifar10/miniResNet/quantize_eval_model.pb \
        --arch /opt/vitis_ai/compiler/arch/DPUCVDX8G/VCK190/arch.json \
        --output_dir=compile/cifar10/miniResNet \
        --net_name=miniResNet \
        --options    "{'mode':'normal'}" \
        #2>&1 | tee rpt/cifar10/5_vai_compile_miniResNet.log
    mv  ./compile/cifar10/miniResNet/*.xmodel ./target_vck190/cifar10/miniResNet/model/
    rm  ./target_vck190/cifar10/miniResNet/model/*_org.xmodel
    '

    echo " "
    echo "##########################################################################"
    echo "COMPILATION COMPLETED  on CIFAR10 on VCK190"
    echo "##########################################################################"
    echo " "
}

5_cifar10_vai_compile_zcu102() {
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU102: LeNet on CIFAR10"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/LeNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
       --output_dir=compile/cifar10/LeNet \
       --net_name=LeNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_LeNet.log

mv  ./compile/cifar10/LeNet/*.xmodel ./target_zcu102/cifar10/LeNet/model/
rm  ./target_zcu102/cifar10/LeNet/model/*_org.xmodel

echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU102: miniVggNet  on CIFAR10"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/miniVggNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
       --output_dir=compile/cifar10/miniVggNet \
       --net_name=miniVggNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_miniVggNet.log

mv  ./compile/cifar10/miniVggNet/*.xmodel ./target_zcu102/cifar10/miniVggNet/model/
rm  ./target_zcu102/cifar10/miniVggNet/model/*_org.xmodel

: '
#debug mode
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/miniVggNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
       --output_dir=compile/cifar10/miniVggNet \
       --net_name=miniVggNet \
       --options    "{'mode':'debug'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_miniVggNet.log

mv  ./compile/cifar10/miniVggNet/*.xmodel ./target_zcu102/cifar10/miniVggNet/model/dbg_dpu_miniVggNet_0.elf
rm  ./target_zcu102/cifar10/miniVggNet/model/*_org.xmodel
'

echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU102: miniGoogleNet  on CIFAR10"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/miniGoogleNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
       --output_dir=compile/cifar10/miniGoogleNet \
       --net_name=miniGoogleNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_miniGoogleNet.log

mv  ./compile/cifar10/miniGoogleNet/*.xmodel ./target_zcu102/cifar10/miniGoogleNet/model/
rm  ./target_zcu102/cifar10/miniGoogleNet/model/*_org.xmodel

echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU102: miniResNet  on CIFAR10"
echo "##########################################################################"
# for Vitis AI >= 1.1
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/miniResNet/quantize_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
       --output_dir=compile/cifar10/miniResNet \
       --net_name=miniResNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_miniResNet.log

mv  ./compile/cifar10/miniResNet/*.xmodel ./target_zcu102/cifar10/miniResNet/model/
rm  ./target_zcu102/cifar10/miniResNet/model/*_org.xmodel

echo " "
echo "##########################################################################"
echo "COMPILATION COMPLETED  on CIFAR10 on ZCU102"
echo "##########################################################################"
echo " "
}

5_cifar10_vai_compile_zcu104() {
echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU104: LeNet on CIFAR10"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/LeNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
       --output_dir=compile/cifar10/LeNet \
       --net_name=LeNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_LeNet.log

mv  ./compile/cifar10/LeNet/*.xmodel ./target_zcu104/cifar10/LeNet/model/
rm  ./target_zcu104/cifar10/LeNet/model/*_org.xmodel

echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU104: miniVggNet  on CIFAR10"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/miniVggNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
       --output_dir=compile/cifar10/miniVggNet \
       --net_name=miniVggNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_miniVggNet.log

mv  ./compile/cifar10/miniVggNet/*.xmodel ./target_zcu104/cifar10/miniVggNet/model/
rm  ./compile/cifar10/miniVggNet/*_org.xmodel

: '
#debug mode
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/miniVggNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
       --output_dir=compile/cifar10/miniVggNet \
       --net_name=miniVggNet \
       --options    "{'mode':'debug'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_miniVggNet.log

mv  ./compile/cifar10/miniVggNet/*.xmodel ./target_zcu104/cifar10/miniVggNet/model/dbg_dpu_miniVggNet_0.elf
rm  ./target_zcu104/cifar10/miniVggNet/model/*_org.xmodel
'

echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU104: miniGoogleNet  on CIFAR10"
echo "##########################################################################"
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/miniGoogleNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
       --output_dir=compile/cifar10/miniGoogleNet \
       --net_name=miniGoogleNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_miniGoogleNet.log

mv  ./compile/cifar10/miniGoogleNet/*.xmodel ./target_zcu104/cifar10/miniGoogleNet/model/
rm  ./target_zcu104/cifar10/miniGoogleNet/model/*_org.xmodel

echo " "
echo "##########################################################################"
echo "COMPILE WITH Vitis AI on ZCU104: miniResNet  on CIFAR10"
echo "##########################################################################"
# for Vitis AI >= 1.1
vai_c_tensorflow \
       --frozen_pb=./quantized_results/cifar10/miniResNet/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
       --output_dir=compile/cifar10/miniResNet \
       --net_name=miniResNet \
       --options    "{'mode':'normal'}"
       #2>&1 | tee rpt/cifar10/5_vai_compile_miniResNet.log

mv  ./compile/cifar10/miniResNet/*.xmodel ./target_zcu104/cifar10/miniResNet/model/
rm  ./target_zcu104/cifar10/miniResNet/model/*_org.xmodel

echo " "
echo "##########################################################################"
echo "COMPILATION COMPLETED  on CIFAR10 on ZCU104"
echo "##########################################################################"
echo " "
}


main() {

  ##copy target_zcu102 files into the new target_zcu104 folder if you have also the ZCU104 board
  cp -r target_zcu102 target_zcu104
  ##copy target_zcu102 files into the new target_vck190 folder if you have also the VCK190 board
  cp -r target_zcu102 target_vck190

  # training from scratch with CIFAR10
  1_cifar10_train

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

  # compile ELF file for ZCU102 target board
  5_cifar10_vai_compile_zcu102

  # compile ELF file for ZCU104 target board
  5_cifar10_vai_compile_zcu104

  # compile ELF file for VCK190 target board
  5_cifar10_vai_compile_vck190

  ## copy test images into target board
  tar -cvf cifar10_test.tar ./dataset/cifar10/test
  cp ./cifar10_test.tar ./target_zcu102/
  cp ./cifar10_test.tar ./target_vck190/
  mv ./cifar10_test.tar ./target_zcu104/


}

main
