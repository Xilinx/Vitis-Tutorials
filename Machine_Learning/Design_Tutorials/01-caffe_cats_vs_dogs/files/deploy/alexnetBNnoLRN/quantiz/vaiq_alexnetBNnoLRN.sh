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
work_dir=${ML_DIR}/deploy/${CNN}/quantiz
#path of float model
model_dir=${work_dir}
#output directory
output_dir=${work_dir}/vaiq_output

#force a soft link to the calibration data
ln -nsf $ML_DIR/input/jpg/calib  $ML_DIR/deploy/${CNN}/quantiz/data/calib

# copy input files from miniVggNet Caffe project via soft links force (nf)
ln -nsf $ML_DIR/caffe/models/${CNN}/m${ALX_MOD_NUM}/q_train_val_${ALX_MOD_NUM}_${CNN}.prototxt $ML_DIR/deploy/${CNN}/quantiz/float.prototxt

ln -nsf $ML_DIR/caffe/models/${CNN}/m${ALX_MOD_NUM}/snapshot_${ALX_MOD_NUM}_${CNN}__iter_${ALX_NUMIT}.caffemodel $ML_DIR/deploy/${CNN}/quantiz/float.caffemodel


# run DECENT
vai_q_caffe   quantize                                    \
           -model ${model_dir}/float.prototxt     \
           -weights ${model_dir}/float.caffemodel \
           -output_dir ${output_dir} \
           -keep_fixed_neuron \
	   -method 1 \
	   -auto_test -test_iter 50
