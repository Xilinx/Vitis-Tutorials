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

# fine tuning
finetune_pruned_model() {
  python train_ft.py \
    --input_ckpt      ${PRUNE_DIR}/${PRUNED_CKPT} \
    --epochs          ${FT_EPOCHS} \
    --batchsize       ${BATCHSIZE} \
    --init_lr         ${INIT_LR} \
    --output_ckpt     ${FT_DIR}/${FT_CKPT} \
    --tboard_logs     ${BUILD}/prune_tb \
    --gpu             ${CUDA_VISIBLE_DEVICES}
}


# evaluate pruned/fine-tuned checkpoint
eval_checkpoint() {
  python eval_ckpt.py \
       --dataset_dir  ${DSET_ROOT} \
       --input_ckpt   "$1" \
       --batchsize    ${BATCHSIZE} \
       --gpu          ${CUDA_VISIBLE_DEVICES}
}



echo "-----------------------------------------"
echo "PRUNING STARTED"
echo "-----------------------------------------"

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


# pruning loop - 8 iterations of 10% reduction
for i in {1..8}
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
   finetune_pruned_model 2>&1 | tee ${LOG}/${FT_LOG}_$i

   # evaluate fine-tuned checkpoint
#   echo " ** Evaluating fine-tuned checkpoint.."
#   eval_checkpoint ${FT_DIR}/${FT_CKPT} 2>&1 | tee ${LOG}/${EVAL_FT_LOG}_$i

done


echo "-----------------------------------------"
echo "PRUNING FINISHED"
echo "-----------------------------------------"

