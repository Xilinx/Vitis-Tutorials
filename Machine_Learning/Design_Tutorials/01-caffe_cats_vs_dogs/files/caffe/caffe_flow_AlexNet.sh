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

MOD_NUM=$ALX_MOD_NUM   # model number
NUMIT=$ALX_NUMIT       # number of iterations
NET=$ALX_NET

#CAFFE_TOOLS_DIR=$CAFFE_ROOT/distribute
#working dir
WORK_DIR=$ML_DIR/caffe


# ################################################################################################################
# SCRIPTS 1 2 3 (DATABASE AND MEAN VALUES)
echo "DATABASE: training and validation in LMDB, test in JPG and MEAN values"

if [ ! -d $ML_DIR/input/lmdb/ ]; then
	# load the database from keras and write it as JPEG images
	python $WORK_DIR/code/1_write_cats-vs-dogs_images.py

	#create LMDB databases -training (50K), validation (9K), test (1K) images - and compute mean values
	python $WORK_DIR/code/2a_compute_mean.py
	python $WORK_DIR/code/2b_create_lmdb.py -i input/jpg/ -o input/lmdb

	#check goodness of LMDB databases (just for debug: you can skip it)
	python $WORK_DIR/code/3_read_lmdb.py

fi


################################################################################################################
# SCRIPT 4  (SOLVER AND TRAINING AND LEARNING CURVE)
#echo "TRAINING. Remember that: <Epoch_index = floor((iteration_index * batch_size) / (# data_samples))>"
#cd $WORK_DIR/..
python $WORK_DIR/code/4_training.py -s $WORK_DIR/models/$NET/m$MOD_NUM/solver_$MOD_NUM\_$NET.prototxt -l $WORK_DIR/models/$NET/m$MOD_NUM/logfile_$MOD_NUM\_$NET.log


# ################################################################################################################
# SCRIPT 5: plot the learning curve: method1
echo "PLOT LEARNING CURVERS (METHOD1)"
python $WORK_DIR/code/5_plot_learning_curve.py -lf $WORK_DIR/models/$NET/m$MOD_NUM/logfile_$MOD_NUM\_$NET.log -od $WORK_DIR/models/$NET/m$MOD_NUM -im plt_train_val_$MOD_NUM\_$NET.png


# ################################################################################################################
# SCRIPT 6 (PREDICTION)
echo "COMPUTE PREDICTIONS"
python $WORK_DIR/code/6_make_predictions.py -d $WORK_DIR/models/$NET/m$MOD_NUM/deploy_$MOD_NUM\_$NET.prototxt -w $WORK_DIR/models/$NET/m$MOD_NUM/snapshot_$MOD_NUM\_$NET\__iter_$NUMIT.caffemodel 2>&1 | tee $WORK_DIR/models/$NET/m$MOD_NUM/predictions_$MOD_NUM\_$NET.txt


# ################################################################################################################
# The below code is commented, as not needed to run this tutorial. But I think it can be useful for reference
# ################################################################################################################
: '
#training by direct command
$CAFFE_TOOLS_DIR/bin/caffe.bin train --solver $WORK_DIR/models/$NET/m$MOD_NUM/solver_$MOD_NUM\_$NET.prototxt 2>&1 | tee $WORK_DIR/models/$NET/m$MOD_NUM/logfile_$MOD_NUM\_$NET.log
'

: '
# example of trainining the CNN from a certain snapshot
echo "RETRAINING from previous snapshot"
$CAFFE_TOOLS_DIR/bin/caffe.bin train --solver $WORK_DIR/models/$NET/m$MOD_NUM/solver_$MOD_NUM\_$NET.prototxt --snapshot $WORK_DIR/models/$NET/m3/snapshot_3\$NET__iter_20000.solverstate 2>&1 | tee $WORK_DIR/models/$NET/m$MOD_NUM/retrain_logfile_$MOD_NUM\_$NET.log
cp -f $WORK_DIR/models/$NET/m$MOD_NUM/logfile_$MOD_NUM\_$NET.log $WORK_DIR/models/$NET/m$MOD_NUM/orig_logfile_$MOD_NUM\_$NET.log
cp -f $WORK_DIR/models/$NET/m$MOD_NUM/retrain_logfile_$MOD_NUM\_$NET.log $WORK_DIR/models/$NET/m$MOD_NUM/logfile_$MOD_NUM\_$NET.log
'
