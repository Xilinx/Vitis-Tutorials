#!/bin/sh
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
# *******************************************************************************

#conda activate vitis-ai-optimizer_caffe


NET=alexnetBNnoLRN

#ML_DIR=${HOME}/ML/VAI/Vitis-AI/tutorials/VAI-Caffe-ML-CATSvsDOGS/files
WORKSPACE=/workspace
#WORKSPACE=${HOME}/vai1v2_bash/vitis-ai-staging-master

ML_DIR=${WORKSPACE}/tutorials/VAI-Caffe-ML-CATSvsDOGS/files

PRUNE_ROOT=${ML_DIR}/pruning
WORK_DIR=${ML_DIR}/pruning/alexnetBNnoLRN

export XILINXD_LICENSE_FILE=$PRUNE_ROOT/pruning_Xilinx.lic

export VAI_P_CAFFE=vai_p_caffe
#export VAI_P_CAFFE=$PRUNE_ROOT/vai_p_caffe



#take the caffemodel with a soft link to save HD space
ln -nsf ${ML_DIR}/caffe/models/alexnetBNnoLRN/m2/snapshot_2_alexnetBNnoLRN__iter_20000.caffemodel  ${WORK_DIR}/float.caffemodel

##replace the number of iterations
#for file in $(find $PWD -name "config*.prototxt"); do
#    sed -i 's/12000/6000/g' ${file}
#    echo  ${file}
#done


conda activate vitis-ai-optimizer_caffe

# analysis: you do it only once
$VAI_P_CAFFE ana -config ${WORK_DIR}/config0.prototxt      2>&1 | tee ${WORK_DIR}/rpt/logfile_ana_alexnetBNnoLRN.txt

# compression: zero run
$VAI_P_CAFFE prune -config ${WORK_DIR}/config0.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_compress0_alexnetBNnoLRN.txt
# fine-tuning: zero run
$VAI_P_CAFFE finetune -config ${WORK_DIR}/config0.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_finetune0_alexnetBNnoLRN.txt

# compression: first run
$VAI_P_CAFFE prune -config ${WORK_DIR}/config1.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_compress1_alexnetBNnoLRN.txt
# fine-tuning: first run
$VAI_P_CAFFE finetune -config ${WORK_DIR}/config1.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_finetune1_alexnetBNnoLRN.txt

# compression: second run
$VAI_P_CAFFE prune -config ${WORK_DIR}/config2.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_compress2_alexnetBNnoLRN.txt
## fine-tuning: second run
$VAI_P_CAFFE finetune -config ${WORK_DIR}/config2.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_finetune2_alexnetBNnoLRN.txt

## compression: third run
$VAI_P_CAFFE prune -config ${WORK_DIR}/config3.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_compress3_alexnetBNnoLRN.txt
## fine-tuning: third run
$VAI_P_CAFFE finetune -config ${WORK_DIR}/config3.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_finetune3_alexnetBNnoLRN.txt

## compression: fourth run
$VAI_P_CAFFE prune -config ${WORK_DIR}/config4.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_compress4_alexnetBNnoLRN.txt
## fine-tuning: fourth run
$VAI_P_CAFFE finetune -config ${WORK_DIR}/config4.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_finetune4_alexnetBNnoLRN.txt

## compression: fift run
$VAI_P_CAFFE prune -config ${WORK_DIR}/config5.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_compress5_alexnetBNnoLRN.txt
## fine-tuning: fift run
$VAI_P_CAFFE finetune -config ${WORK_DIR}/config5.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_finetune5_alexnetBNnoLRN.txt

## compression: 6-th run
$VAI_P_CAFFE prune -config ${WORK_DIR}/config6.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_compress6_alexnetBNnoLRN.txt
## fine-tuning: 6-th run
$VAI_P_CAFFE finetune -config ${WORK_DIR}/config6.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_finetune6_alexnetBNnoLRN.txt

## compression: 7-th run
$VAI_P_CAFFE prune -config ${WORK_DIR}/config7.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_compress7_alexnetBNnoLRN.txt
## fine-tuning: 7-th run
$VAI_P_CAFFE finetune -config ${WORK_DIR}/config7.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_finetune7_alexnetBNnoLRN.txt


## last step: get the final output model
## note that it does not work if you used the "final.prototxt" as wrongly described by transform help
$VAI_P_CAFFE transform -model ${WORK_DIR}/train_val.prototxt -weights ${WORK_DIR}/regular_rate_0.7/sparse.caffemodel 2>&1 | tee ${WORK_DIR}/rpt/logfile_transform_alexnetBNnoLRN.txt

# get flops and the number of parameters of the original not pruned  model
$VAI_P_CAFFE stat -model ${WORK_DIR}/train_val.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_stat_alexnetBNnoLRN_original.txt

# get flops and the number of parameters of the newly pruned  model
$VAI_P_CAFFE stat -model ${WORK_DIR}/regular_rate_0.7/final.prototxt 2>&1 | tee ${WORK_DIR}/rpt/logfile_stat_alexnetBNnoLRN_pruned.txt

for file in $(find $ML_DIR -name transformed.caffemodel); do
    mv ${file} ${WORK_DIR}
done
cp ${WORK_DIR}/regular_rate_0.7/final.prototxt ${WORK_DIR}/q_final.prototxt

conda deactivate
