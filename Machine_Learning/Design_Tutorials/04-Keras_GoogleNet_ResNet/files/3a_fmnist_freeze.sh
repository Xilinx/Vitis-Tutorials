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



# freeze trained graph
echo " "
echo "##########################################################################"
echo "FREEZE GRAPH of LeNet on Fashion MNIST"
echo "##########################################################################"
rm ./freeze/fmnist/Lenet/* # remove previous results
freeze_graph --input_graph=./tf_chkpts/fmnist/LeNet/infer_graph.pb \
             --input_checkpoint=./tf_chkpts/fmnist/LeNet/float_model.ckpt \
             --input_binary=true \
             --output_graph=./freeze/fmnist/LeNet/frozen_graph.pb \
             --output_node_names=activation_4/Softmax \
             2>&1 | tee rpt/fmnist/3a_freeze_graph_LeNet.log

# check possible input/output node names
echo " "
echo "##########################################################################"
echo "INSPECT FROZEN GRAPH of LeNet  on Fashion MNIST"
echo "##########################################################################"
vai_q_tensorflow inspect --input_frozen_graph=./freeze/fmnist/LeNet/frozen_graph.pb



# freeze trained graph
echo " "
echo "##########################################################################"
echo "FREEZE GRAPH of miniVggNet  on Fashion MNIST"
echo "##########################################################################"
rm ./freeze/fmnist/miniVggNet/* #remove previous results
freeze_graph --input_graph=./tf_chkpts/fmnist/miniVggNet/infer_graph.pb \
             --input_checkpoint=./tf_chkpts/fmnist/miniVggNet/float_model.ckpt \
             --input_binary=true \
             --output_graph=./freeze/fmnist/miniVggNet/frozen_graph.pb \
             --output_node_names=activation_6/Softmax \
             2>&1 | tee rpt/fmnist/3a_freeze_graph_miniVggNet.log

# check possible input/output node names
echo " "
echo "##########################################################################"
echo "INSPECT FROZEN GRAPH of miniVggNet  on Fashion MNIST"
echo "##########################################################################"
vai_q_tensorflow inspect --input_frozen_graph=./freeze/fmnist/miniVggNet/frozen_graph.pb


# freeze trained graph
echo " "
echo "##########################################################################"
echo "FREEZE GRAPH of miniGoogleNet  on Fashion MNIST"
echo "##########################################################################"
rm ./freeze/fmnist/miniGoogleNet/* #remove previous results
freeze_graph --input_graph=./tf_chkpts/fmnist/miniGoogleNet/infer_graph.pb \
             --input_checkpoint=./tf_chkpts/fmnist/miniGoogleNet/float_model.ckpt \
             --input_binary=true \
             --output_graph=./freeze/fmnist/miniGoogleNet/frozen_graph.pb \
             --output_node_names=activation_20/Softmax \
             2>&1 | tee rpt/fmnist/3a_freeze_graph_miniGoogleNet.log

# check possible input/output node names
echo " "
echo "##########################################################################"
echo "INSPECT FROZEN GRAPH of miniGoogleNet  on Fashion MNIST"
echo "##########################################################################"
vai_q_tensorflow inspect --input_frozen_graph=./freeze/fmnist/miniGoogleNet/frozen_graph.pb


# freeze trained graph
echo " "
echo "##########################################################################"
echo "FREEZE GRAPH of miniResNet  on Fashion MNIST"
echo "##########################################################################"
rm ./freeze/fmnist/miniResNet/* #remove previous results
freeze_graph --input_graph=./tf_chkpts/fmnist/miniResNet/infer_graph.pb \
             --input_checkpoint=./tf_chkpts/fmnist/miniResNet/float_model.ckpt \
             --input_binary=true \
             --output_graph=./freeze/fmnist/miniResNet/frozen_graph.pb \
             --output_node_names=activation_83/Softmax \
             2>&1 | tee rpt/fmnist/3a_freeze_graph_miniResNet.log

# check possible input/output node names
echo " "
echo "##########################################################################"
echo "INSPECT FROZEN GRAPH of miniResNet  on Fashion MNIST"
echo "##########################################################################"
vai_q_tensorflow inspect --input_frozen_graph=./freeze/fmnist/miniResNet/frozen_graph.pb

echo " "
echo "##########################################################################"
echo "FREEZE GRAPH COMPLETED"
echo "##########################################################################"
echo " "
