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
#path of float model
model_dir=${work_dir}
#output directory
output_dir=${work_dir}/vaiq_output

#force a soft link to the calibration data
ln -nsf $ML_DIR/input/jpg/calib  ${work_dir}/data/calib

python3 ${work_dir}/calibr.py -f ${work_dir}/../../../pruning/${CNN}/regular_rate_0.7/final.prototxt -i ${ML_DIR}/caffe/models/alexnetBNnoLRN/m2/header_calibr.prototxt -o ${model_dir}/q_final.prototxt


# copy input files from CNN Caffe project via soft links force (nf)
ln -nsf ${model_dir}/q_final.prototxt ${model_dir}/float.prototxt
ln -nsf ${work_dir}/../../../pruning/${CNN}/transformed.caffemodel ${model_dir}/float.caffemodel


# run DECENT
vai_q_caffe   quantize                                    \
           -model ${model_dir}/float.prototxt     \
           -weights ${model_dir}/float.caffemodel \
           -output_dir ${output_dir} \
           -keep_fixed_neuron \
	   -method 1 \
	   -auto_test -test_iter 50
