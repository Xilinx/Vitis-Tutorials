#!/bin/bash


: '
/**

* © Copyright (C) 2016-2020 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/
'

MODEL=$1 #./build/freeze/fcn8/frozen_graph.pb

PYSCRIPT=/workspace/tools/Vitis-AI-Quantizer/vai_q_tensorflow1.x/tensorflow/python/tools/import_pb_to_tensorboard.py

python $PYSCRIPT --model_dir=$MODEL --log_dir=./tb_log/

tensorboard --logdir=./tb_log/
#TensorBoard 1.12.2 at http://Prec5820Tow:6006 (Press CTRL+C to quit)
