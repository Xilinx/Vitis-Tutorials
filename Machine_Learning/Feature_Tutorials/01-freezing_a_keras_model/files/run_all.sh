#!/bin/bash

# Copyright 2020 Xilinx Inc.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


# Author: Mark Harvey


# conda environment
conda activate vitis-ai-tensorflow

# remove previous results
rm -rf ./method1
rm -rf ./method2
rm -rf ./method3
rm -rf ./quant_images


echo "#####################################"
echo "TRAIN & SAVE"
echo "#####################################"
python train_save.py


echo "#####################################"
echo "CONVERT KERAS TO TF"
echo "#####################################"
# method 1
python keras_2_tf.py --keras_hdf5 ./method1/keras_chkpt.h5 \
                     --tf_ckpt=./method1/tf_chkpt.ckpt

# method 2
python keras_2_tf.py --keras_hdf5 ./method2/k_complete_model.h5 \
                     --tf_ckpt=./method2/tf_chkpt.ckpt

# method 3
python keras_2_tf.py --keras_json=./method3/k_model_architecture.json \
                     --keras_hdf5=./method3/k_model_weights.h5 \
                     --tf_ckpt=./method3/tf_chkpt.ckpt


echo "#####################################"
echo "FREEZE GRAPH"
echo "#####################################"
# method 1
freeze_graph \
    --input_meta_graph  ./method1/tf_chkpt.ckpt.meta \
    --input_checkpoint  ./method1/tf_chkpt.ckpt \
    --output_graph      ./method1/frozen_graph.pb \
    --output_node_names activation_4/Softmax \
    --input_binary      true


# method 2
freeze_graph \
    --input_meta_graph  ./method2/tf_chkpt.ckpt.meta \
    --input_checkpoint  ./method2/tf_chkpt.ckpt \
    --output_graph      ./method2/frozen_graph.pb \
    --output_node_names activation_4/Softmax \
    --input_binary      true

# method 3
freeze_graph \
    --input_meta_graph  ./method3/tf_chkpt.ckpt.meta \
    --input_checkpoint  ./method3/tf_chkpt.ckpt \
    --output_graph      ./method3/frozen_graph.pb \
    --output_node_names activation_4/Softmax \
    --input_binary      true


echo "#####################################"
echo "QUANTIZE"
echo "#####################################"


# create image files for calibration
python generate_images.py  \
    --dataset=cifar10 \
    --image_dir=quant_images \
    --image_format=jpg \
    --image_list=calib_list.txt \
    --max_images=1000


# log the quantizer version being used
vai_q_tensorflow --version


# method 1
vai_q_tensorflow quantize \
        --input_frozen_graph=./method1/frozen_graph.pb \
        --input_nodes=input_1 \
        --input_shapes=?,32,32,3 \
        --output_nodes=activation_4/Softmax  \
        --input_fn=image_input_fn.calib_input \
        --output_dir=method1 \
        --calib_iter=100

# method 2
vai_q_tensorflow quantize \
        --input_frozen_graph=./method2/frozen_graph.pb \
        --input_nodes=input_1 \
        --input_shapes=?,32,32,3 \
        --output_nodes=activation_4/Softmax  \
        --input_fn=image_input_fn.calib_input \
        --output_dir=method2 \
        --calib_iter=100

# method 3
vai_q_tensorflow quantize \
        --input_frozen_graph=./method3/frozen_graph.pb \
        --input_nodes=input_1 \
        --input_shapes=?,32,32,3 \
        --output_nodes=activation_4/Softmax  \
        --input_fn=image_input_fn.calib_input \
        --output_dir=method3 \
        --calib_iter=100



echo "#####################################"
echo "COMPILE"
echo "#####################################"


# target board
ARCH=/opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json


# method 1
vai_c_tensorflow \
       --frozen_pb=./method1/deploy_model.pb \
       --arch=${ARCH} \
       --output_dir=method1 \
       --net_name=cifar10_net

# method 2
vai_c_tensorflow \
       --frozen_pb=./method2/deploy_model.pb \
       --arch=${ARCH} \
       --output_dir=method2 \
       --net_name=cifar10_net

# method 3
vai_c_tensorflow \
       --frozen_pb=./method3/deploy_model.pb \
       --arch=${ARCH} \
       --output_dir=method3 \
       --net_name=cifar10_net

echo "#####################################"
echo "FLOW COMPLETED"
echo "#####################################"

