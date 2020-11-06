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



# activate DECENT_Q Python3.6 virtual environment

cd ./code


# run quantization
echo " "
echo "##########################################################################"
echo "QUANTIZE LeNet on CIFAR10"
echo "##########################################################################"
# LeNet
vai_q_tensorflow  quantize \
	 --input_frozen_graph ../freeze/cifar10/LeNet/frozen_graph.pb \
	 --input_nodes conv2d_1_input \
	 --input_shapes ?,32,32,3 \
	 --output_nodes activation_4/Softmax \
	 --output_dir ../quantized_results/cifar10/LeNet/ \
	 --method 1 \
	 --input_fn cifar10_graph_input_fn.calib_input \
	 --calib_iter 20 \
	 --gpu 0  2>&1 | tee ../rpt/cifar10/4a_quant_LeNet.log

echo " "
echo "##########################################################################"
echo "QUANTIZE miniVggNet  on CIFAR10"
echo "##########################################################################"
#miniVggNet
vai_q_tensorflow quantize \
	 --input_frozen_graph ../freeze/cifar10/miniVggNet/frozen_graph.pb \
	 --input_nodes conv2d_1_input \
	 --input_shapes ?,32,32,3 \
	 --output_nodes activation_6/Softmax \
	 --output_dir ../quantized_results/cifar10/miniVggNet/ \
	 --method 1 \
	 --input_fn cifar10_graph_input_fn.calib_input \
	 --calib_iter 20 \
	 --gpu 0  2>&1 | tee ../rpt/cifar10/4a_quant_miniVggNet.log

echo " "
echo "##########################################################################"
echo "QUANTIZE miniGoogleNet  on CIFAR10"
echo "##########################################################################"
#miniGoogleNet
vai_q_tensorflow quantize \
	 --input_frozen_graph ../freeze/cifar10/miniGoogleNet/frozen_graph.pb \
	 --input_nodes conv2d_1_input \
	 --input_shapes ?,32,32,3 \
	 --output_nodes activation_20/Softmax \
	 --output_dir ../quantized_results/cifar10/miniGoogleNet/ \
	 --method 1 \
	 --input_fn cifar10_graph_input_fn.calib_input \
	 --calib_iter 20 \
	 --gpu 0  2>&1 | tee ../rpt/cifar10/4a_quant_miniGoogleNet.log

echo " "
echo "##########################################################################"
echo "QUANTIZE miniResNet  on CIFAR10"
echo "##########################################################################"
#miniResNet
vai_q_tensorflow quantize \
	 --input_frozen_graph ../freeze/cifar10/miniResNet/frozen_graph.pb \
	 --input_nodes conv2d_1_input \
	 --input_shapes ?,32,32,3 \
	 --output_nodes activation_83/Softmax \
	 --output_dir ../quantized_results/cifar10/miniResNet/ \
	 --method 1 \
	 --input_fn cifar10_graph_input_fn.calib_input \
	 --calib_iter 20 \
	 --gpu 0  2>&1 | tee ../rpt/cifar10/4a_quant_miniResNet.log

echo " "
echo "##########################################################################"
echo "QUANTIZATION COMPLETED  on CIFAR10"
echo "##########################################################################"
echo " "

cd ..
