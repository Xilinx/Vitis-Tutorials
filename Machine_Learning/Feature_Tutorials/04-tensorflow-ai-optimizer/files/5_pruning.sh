#!/bin/bash
set -e

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


# Author: Mark Harvey, Xilinx Inc


# pruning
# --workspace folder must be same as pruning analysis
# as pruning uses the .ana file created by pruning analysis
prune() {
  sparsity=$1
  vai_p_tensorflow \
    --action                prune \
    --input_graph           ${TRAIN_DIR}/${BASE_GRAPH} \
    --input_ckpt            ${FT_DIR}/${FT_CKPT} \
    --output_graph          ${PRUNE_DIR}/${PRUNED_GRAPH} \
    --output_ckpt           ${PRUNE_DIR}/${PRUNED_CKPT} \
    --input_nodes           ${INPUT_NODES} \
    --input_node_shapes     ${INPUT_SHAPE} \
    --workspace             ${PRUNE_ANA} \
    --output_nodes          ${OUTPUT_NODES} \
    --sparsity              $sparsity \
    --gpu                   ${CUDA_VISIBLE_DEVICES}
}

# fine tuning is just training starting from a trained model
# the output of the pruning command is checkpoint which requires to
# be fine-tuned (i.e. retrained)
finetune_pruned_model() {
  target_acc=$1
  python -u train_ft.py \
    --target_acc      $target_acc \
    --input_ckpt      ${PRUNE_DIR}/${PRUNED_CKPT} \
    --epochs          ${FT_EPOCHS} \
    --batchsize       ${BATCHSIZE} \
    --init_lr         ${INIT_LR} \
    --output_ckpt     ${FT_DIR}/${FT_CKPT} \
    --tboard_logs     ${BUILD}/prune_tb \
    --input_height    ${INPUT_HEIGHT} \
    --input_width     ${INPUT_WIDTH} \
    --input_chan      ${INPUT_CHAN} \
    --gpu             ${CUDA_VISIBLE_DEVICES}
}


echo "-----------------------------------------"
echo "PRUNING STARTED"
echo "-----------------------------------------"

TF_CPP_MIN_LOG_LEVEL=0

# activate the TF pruning conda environment
conda activate vitis-ai-optimizer_tensorflow

# clear any previous results
rm -rf    ${PRUNE_DIR}
rm -rf    ${FT_DIR}
mkdir -p  ${PRUNE_DIR}
mkdir -p  ${FT_DIR}

# copy trained checkpoint to fine-tuned checkpoint
cp -f ${TRAIN_DIR}/${BASE_CKPT}.data-00000-of-00001 ${FT_DIR}/${FT_CKPT}.data-00000-of-00001
cp -f ${TRAIN_DIR}/${BASE_CKPT}.index ${FT_DIR}/${FT_CKPT}.index
cp -f ${TRAIN_DIR}/${BASE_CKPT}.meta ${FT_DIR}/${FT_CKPT}.meta 


# pruning loop - 8 iterations of 10% reduction each
# first 7 iterations in this loop are done with a lower target_acc
for i in {1..7}
do
   echo "----------------------------------"
   echo " PRUNING STEP" $i
   echo "----------------------------------"
   # sparsity value, increments by 0.1 at each step
   SPARSITY=$(printf %.1f "$(($i))"e-1)
   echo " ** Sparsity value:" $SPARSITY

   # prune
   prune ${SPARSITY} 2>&1 | tee ${LOG}/${PRUNE_LOG}_$i
   
   # fine-tuning
   echo " ** Running fine-tune.."
   rm ${FT_DIR}/*
   finetune_pruned_model 0.90 2>&1 | tee ${LOG}/${FT_LOG}_$i

done

# last iteration with final target_acc
echo "----------------------------------"
echo " PRUNING STEP 8"
echo "----------------------------------"
# sparsity value, increments by 0.1 at each step
SPARSITY=0.8
echo " ** Sparsity value:" $SPARSITY

# prune
prune ${SPARSITY} 2>&1 | tee ${LOG}/${PRUNE_LOG}_8

# fine-tuning
echo " ** Running fine-tune.."
rm ${FT_DIR}/*
finetune_pruned_model 0.92 2>&1 | tee ${LOG}/${FT_LOG}_8


echo "-----------------------------------------"
echo "PRUNING FINISHED"
echo "-----------------------------------------"

