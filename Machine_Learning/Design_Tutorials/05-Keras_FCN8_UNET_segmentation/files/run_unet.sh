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

# modified by daniele.bagni@xilinx.com
# date 4 Jan 2021

## clean up previous log files
#rm -f ./log/*.log

CNN=unet

# folders
WORK_DIR=./build
LOG_DIR=${WORK_DIR}/../log
RPT_DIR=${WORK_DIR}/../rpt
TARGET_190=${WORK_DIR}/../target_vck190
TARGET_102=${WORK_DIR}/../target_zcu102
TARGET_104=${WORK_DIR}/../target_zcu104
KERAS_MODEL_DIR=${WORK_DIR}/../keras_model
DATASET_DIR=${WORK_DIR}/dataset1

TB_LOG_DIR=${WORK_DIR}/tb_log
CHKPT_DIR=${WORK_DIR}/tf_chkpts
FREEZE_DIR=${WORK_DIR}/freeze
COMPILE_DIR=${WORK_DIR}/compile
QUANT_DIR=${WORK_DIR}/quantize_results

# checkpoints & graphs filenames
CHKPT_FILENAME=float_model.ckpt
META_GRAPH_FILENAME=${CHKPT_FILENAME}.meta
FROZEN_GRAPH_FILENAME=frozen_graph.pb
QUANTIZED_FILENAME=quantize_eval_model.pb

# logs & results files
PREPARE_DATA_LOG=${CNN}_prepare_data.log
TRAIN_LOG=${CNN}_training.log
FREEZE_LOG=${CNN}_freeze_graph.log
EVAL_FR_LOG=${CNN}_evaluate_frozen_graph.log
QUANT_LOG=${CNN}_quantize.log
EVAL_Q_LOG=${CNN}_evaluate_quantized_graph.log
COMP_LOG=${CNN}_compile.log

# CNN parameters
INPUT_NODE="input_1"
OUTPUT_NODE="conv2d_18/Relu" # output node of floating point CNN UNET v1 and v3

##################################################################################
#setup the environment and check DNNDK relese
#source ${HOME}/scripts/activate_py36_dnndk3v1.sh

##################################################################################
1_generate_images() {
    echo " "
    echo "##################################################################################"
    echo "Step1: CREATE DATA AND FOLDERS"
    echo "##################################################################################"
    echo " "
    # clean files in pre-built sub-directories
    rm -f ${DATASET_DIR}/img_*/* ${DATASET_DIR}/seg_*/*
    # unzip the original dataset
    unzip ${WORK_DIR}/../dataset1.zip -d ${WORK_DIR}  >& /dev/null
    cd code
    # put the data into proper folders
    python prepare_data.py
    cd ..
    # clean previous directories
    rm -r ${DATASET_DIR}/annotations_* ${DATASET_DIR}/images_*
}


##################################################################################
# effective training
2_unet_train() {
    cd code
    # effective training and predictions
    echo " "
    echo "##################################################################################"
    echo "Step2a: TRAINING"
    echo "##################################################################################"
    echo " "
    #python unet_training.py -m 1
    python unet_training.py -m 2
    #python unet_training.py -m 3
    echo " "

    cd ../code
    echo "##################################################################################"
    echo "Step2b: MAKING PREDICTIONS"
    echo "##################################################################################"
    echo " "
    #python unet_make_predictions.py -m 1
    python unet_make_predictions.py -m 2
    #python unet_make_predictions.py -m 3
    cd ..

}

##################################################################################
# Keras to TF chkpt files
3_unet_Keras2TF() {
    echo " "
    echo "#######################################################################################"
    echo "Step3: KERAS to TENSORFLOW GRAPH CONVERSION"
     echo "#######################################################################################"
    echo " "
    # clean TF Check Point files
    #rm ${CHKPT_DIR}/${CNN}/*
    # from Keras to TF
    cd code
    #python Keras2TF.py --model  "unet1"
    python Keras2TF.py --model  "unet2"
    #python Keras2TF.py --model  "unet3"
    cd ..
}



##################################################################################
# freeze the inference graph
4a_unet_freeze() {
    echo " "
    echo "##############################################################################"
    echo "Step4a: FREEZE TF GRAPHS"
    echo "##############################################################################"
    echo " "
: '
    # freeze the TF graph
    freeze_graph \
  --input_meta_graph  ${CHKPT_DIR}/${CNN}1/${META_GRAPH_FILENAME} \
	--input_checkpoint  ${CHKPT_DIR}/${CNN}1/${CHKPT_FILENAME} \
	--input_binary      true \
	--output_graph      ${FREEZE_DIR}/${CNN}1/${FROZEN_GRAPH_FILENAME} \
	--output_node_names ${OUTPUT_NODE}
'

    # freeze the TF graph
    freeze_graph \
  --input_meta_graph  ${CHKPT_DIR}/${CNN}2/${META_GRAPH_FILENAME} \
	--input_checkpoint  ${CHKPT_DIR}/${CNN}2/${CHKPT_FILENAME} \
	--input_binary      true \
	--output_graph      ${FREEZE_DIR}/${CNN}2/${FROZEN_GRAPH_FILENAME} \
	--output_node_names "conv2d_22/Relu"

: '
    # freeze the TF graph
    freeze_graph \
  --input_meta_graph  ${CHKPT_DIR}/${CNN}3/${META_GRAPH_FILENAME} \
	--input_checkpoint  ${CHKPT_DIR}/${CNN}3/${CHKPT_FILENAME} \
	--input_binary      true \
	--output_graph      ${FREEZE_DIR}/${CNN}3/${FROZEN_GRAPH_FILENAME} \
	--output_node_names ${OUTPUT_NODE}
'

    echo " "
    echo "##############################################################################"
    echo "Step4a: INSPECT FROZEN GRAPH"
    echo "##############################################################################"
    echo " "
    #vai_q_tensorflow inspect --input_frozen_graph ${FREEZE_DIR}/${CNN}1/${FROZEN_GRAPH_FILENAME}
    vai_q_tensorflow inspect --input_frozen_graph ${FREEZE_DIR}/${CNN}2/${FROZEN_GRAPH_FILENAME}
    #vai_q_tensorflow inspect --input_frozen_graph ${FREEZE_DIR}/${CNN}3/${FROZEN_GRAPH_FILENAME}
}


##################################################################################
# evaluate the original graph
4b_eval_graph() {
    echo " "
    echo "##############################################################################"
    echo "Step4b: EVALUATING THE ORIGINAL GRAPH"
    echo "##############################################################################"
    echo " "
    cd code

    : '
    python eval_graph.py \
	   --graph=../${FREEZE_DIR}/${CNN}1/${FROZEN_GRAPH_FILENAME} \
	   --input_node=${INPUT_NODE} \
	   --output_node=${OUTPUT_NODE} \
	   --gpu=0
'
    python eval_graph.py \
	   --graph=../${FREEZE_DIR}/${CNN}2/${FROZEN_GRAPH_FILENAME} \
	   --input_node=${INPUT_NODE} \
	   --output_node="conv2d_22/Relu" \
	   --gpu=0
: '
    python eval_graph.py \
	   --graph=../${FREEZE_DIR}/${CNN}3/${FROZEN_GRAPH_FILENAME} \
	   --input_node=${INPUT_NODE} \
	   --output_node=${OUTPUT_NODE} \
	   --gpu=0
'

    cd ..
}


##################################################################################
5a_unet_quantize() {
    echo " "
    echo "##########################################################################"
    echo "Step5a: QUANTIZATION"
    echo "##########################################################################"
    echo " "
    cd code

    #quantize
    : '
    vai_q_tensorflow quantize \
	 --input_frozen_graph  ../${FREEZE_DIR}/${CNN}1/${FROZEN_GRAPH_FILENAME} \
	 --input_nodes         ${INPUT_NODE} \
	 --input_shapes        ?,224,224,3 \
	 --output_nodes        ${OUTPUT_NODE} \
	 --output_dir          ../${QUANT_DIR}/${CNN}1/ \
	 --method              1 \
	 --input_fn            graph_input_fn.calib_input \
	 --calib_iter          10 \
	 --gpu 0
'

   vai_q_tensorflow quantize \
	 --input_frozen_graph  ../${FREEZE_DIR}/${CNN}2/${FROZEN_GRAPH_FILENAME} \
	 --input_nodes         ${INPUT_NODE} \
	 --input_shapes        ?,224,224,3 \
	 --output_nodes        "conv2d_22/Relu" \
	 --output_dir          ../${QUANT_DIR}/${CNN}2/ \
	 --method              1 \
	 --input_fn            graph_input_fn.calib_input \
	 --calib_iter          10 \
	 --gpu 0

: '
   vai_q_tensorflow quantize \
	 --input_frozen_graph  ../${FREEZE_DIR}/${CNN}3/${FROZEN_GRAPH_FILENAME} \
	 --input_nodes         ${INPUT_NODE} \
	 --input_shapes        ?,224,224,3 \
	 --output_nodes        ${OUTPUT_NODE} \
	 --output_dir          ../${QUANT_DIR}/${CNN}3/ \
	 --method              1 \
	 --input_fn            graph_input_fn.calib_input \
	 --calib_iter          10 \
	 --gpu 0
'

    cd ..
    ## go back to official DNNDK v3.1
    #source ${HOME}/scripts/activate_py36_dnndk3v1.sh
}

##################################################################################
# make predictions with quantized graph

5b_eval_quantized_graph() {
    echo " "
    echo "##############################################################################"
    echo "Step5b: EVALUATE QUANTIZED GRAPH"
    echo "##############################################################################"
    echo " "
    cd code

    : '
    python eval_quantized_graph.py \
	   --graph=../${QUANT_DIR}/${CNN}1/${QUANTIZED_FILENAME} \
	   --input_node=${INPUT_NODE} \
	   --output_node=${OUTPUT_NODE} \
	   --gpu=0
'

    python eval_quantized_graph.py \
	   --graph=../${QUANT_DIR}/${CNN}2/${QUANTIZED_FILENAME} \
	   --input_node=${INPUT_NODE} \
	   --output_node="conv2d_22/Relu" \
	   --gpu=0

: '
    python eval_quantized_graph.py \
	   --graph=../${QUANT_DIR}/${CNN}3/${QUANTIZED_FILENAME} \
	   --input_node=${INPUT_NODE} \
	   --output_node=${OUTPUT_NODE} \
	   --gpu=0
'
    cd ..
}


##################################################################################
# Compile ELF file for VCK190 with Vitis AI
6_compile_vai_vck190() {
  echo " "
  echo "##########################################################################"
  echo "COMPILE UNET XMODEL FILE WITH Vitis AI for VCK190 TARGET"
  echo "##########################################################################"
  echo " "
  # for Vitis AI == 1.0
  #python /opt/vitis_ai/compiler/vai_c_tensorflow \

: '
  vai_c_tensorflow \
      --frozen_pb ${QUANT_DIR}/${CNN}1/quantize_eval_model.pb \
      --arch /opt/vitis_ai/compiler/arch/DPUCVDX8G/VCK190/arch.json \
	 --output_dir ${COMPILE_DIR}/${CNN}1 \
	 --options    "{'mode':'normal'}" \
	 --net_name ${CNN}1
'

   # for Vitis AI >= 1.1
  vai_c_tensorflow \
	 --frozen_pb ${QUANT_DIR}/${CNN}2/quantize_eval_model.pb \
   --arch ./arch_vck190_dw.json \
	 --output_dir ${COMPILE_DIR}/${CNN}2 \
	 --options    "{'mode':'normal'}" \
	 --net_name ${CNN}2

: '
   vai_c_tensorflow \
    --frozen_pb ${QUANT_DIR}/${CNN}2/quantize_eval_model.pb \
    --arch ./arch_vck190_dw.json \
    --output_dir ${COMPILE_DIR}/${CNN}2 \
    --options    "{'mode':'debug'}" \
    --net_name dbg_${CNN}2


   # for Vitis AI >= 1.1
  vai_c_tensorflow \
	 --frozen_pb ${QUANT_DIR}/${CNN}3/quantize_eval_model.pb \
   --arch ./arch_vck190_dw.json \
	 --output_dir ${COMPILE_DIR}/${CNN}3 \
	 --options    "{'mode':'normal'}" \
	 --net_name ${CNN}3

'

 }

 6_compile_vai_zcu102() {
   echo " "
   echo "##########################################################################"
   echo "COMPILE UNET XMODEL FILE WITH Vitis AI for ZCU102 TARGET"
   echo "##########################################################################"
   echo " "

: '
   vai_c_tensorflow \
       --frozen_pb ${QUANT_DIR}/${CNN}1/quantize_eval_model.pb \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
 	 --output_dir ${COMPILE_DIR}/${CNN}1 \
 	 --options    "{'mode':'normal'}" \
 	 --net_name ${CNN}1
'

   vai_c_tensorflow \
 	 --frozen_pb ${QUANT_DIR}/${CNN}2/quantize_eval_model.pb \
   --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
 	 --output_dir ${COMPILE_DIR}/${CNN}2 \
 	 --options    "{'mode':'normal'}" \
 	 --net_name ${CNN}2

: '
    vai_c_tensorflow \
     --frozen_pb ${QUANT_DIR}/${CNN}2/quantize_eval_model.pb \
     --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
     --output_dir ${COMPILE_DIR}/${CNN}2 \
     --options    "{'mode':'debug'}" \
     --net_name dbg_${CNN}2

   vai_c_tensorflow \
 	 --frozen_pb ${QUANT_DIR}/${CNN}3/quantize_eval_model.pb \
   --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
 	 --output_dir ${COMPILE_DIR}/${CNN}3 \
 	 --options    "{'mode':'normal'}" \
 	 --net_name ${CNN}3
'
  }


  6_compile_vai_zcu104() {
    echo " "
    echo "##########################################################################"
    echo "COMPILE UNET XMODEL FILE WITH Vitis AI for ZCU104 TARGET"
    echo "##########################################################################"
    echo " "

: '
    vai_c_tensorflow \
        --frozen_pb ${QUANT_DIR}/${CNN}1/quantize_eval_model.pb \
        --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
  	 --output_dir ${COMPILE_DIR}/${CNN}1 \
  	 --options    "{'mode':'normal'}" \
  	 --net_name ${CNN}1
'

    vai_c_tensorflow \
  	 --frozen_pb ${QUANT_DIR}/${CNN}2/quantize_eval_model.pb \
    --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
  	 --output_dir ${COMPILE_DIR}/${CNN}2 \
  	 --options    "{'mode':'normal'}" \
  	 --net_name ${CNN}2

: '
     vai_c_tensorflow \
      --frozen_pb ${QUANT_DIR}/${CNN}2/quantize_eval_model.pb \
      --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
      --output_dir ${COMPILE_DIR}/${CNN}2 \
      --options    "{'mode':'debug'}" \
      --net_name dbg_${CNN}2

    vai_c_tensorflow \
  	 --frozen_pb ${QUANT_DIR}/${CNN}3/quantize_eval_model.pb \
    --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/arch.json \
  	 --output_dir ${COMPILE_DIR}/${CNN}3 \
  	 --options    "{'mode':'normal'}" \
  	 --net_name ${CNN}3
'
   }

##################################################################################
##################################################################################

main() {

    conda activate vitis-ai-tensorflow

    # assuming you have run first the run_fcn8.sh script, you do not need to clean up anything

    # clean up previous results
    #rm -rf ${WORK_DIR}; mkdir ${WORK_DIR}
    #rm -rf ${LOG_DIR}; mkdir ${LOG_DIR}
    #rm -rf ${RPT_DIR}; mkdir ${RPT_DIR}
    #rm -rf ${CHKPT_DIR}; mkdir ${CHKPT_DIR}
    #rm -rf ${DATASET_DIR}; mkdir ${DATASET_DIR}
    #mkdir ${DATASET_DIR}/img_calib ${DATASET_DIR}/img_test ${DATASET_DIR}/img_train ${DATASET_DIR}/img_valid
    #mkdir ${DATASET_DIR}/seg_calib ${DATASET_DIR}/seg_test ${DATASET_DIR}/seg_train ${DATASET_DIR}/seg_valid
    #rm -rf ${FREEZE_DIR}; mkdir ${FREEZE_DIR}
    #rm -rf ${QUANT_DIR}; mkdir ${QUANT_DIR}
    #rm -rf ${COMPILE_DIR}; mkdir ${COMPILE_DIR}

    mkdir ${LOG_DIR}/${CNN}

    rm -r ${CHKPT_DIR}/${CNN}1   ${CHKPT_DIR}/${CNN}2   ${CHKPT_DIR}/${CNN}3
    rm -r ${FREEZE_DIR}/${CNN}1  ${FREEZE_DIR}/${CNN}2  ${FREEZE_DIR}/${CNN}3
    rm -r ${QUANT_DIR}/${CNN}1   ${QUANT_DIR}/${CNN}2   ${QUANT_DIR}/${CNN}3
    rm -r ${COMPILE_DIR}/${CNN}1 ${COMPILE_DIR}/${CNN}2 ${COMPILE_DIR}/${CNN}3
    mkdir ${CHKPT_DIR}/${CNN}1   ${CHKPT_DIR}/${CNN}2   ${CHKPT_DIR}/${CNN}3
    mkdir ${FREEZE_DIR}/${CNN}1  ${FREEZE_DIR}/${CNN}2  ${FREEZE_DIR}/${CNN}3
    mkdir ${QUANT_DIR}/${CNN}1   ${QUANT_DIR}/${CNN}2   ${QUANT_DIR}/${CNN}3
    mkdir ${COMPILE_DIR}/${CNN}1 ${COMPILE_DIR}/${CNN}2 ${COMPILE_DIR}/${CNN}3

    ## create the proper folders and images from the original dataset
    #1_generate_images #2>&1 | tee ${LOG_DIR}/${CNN}/${PREPARE_DATA_LOG}

    # do the training and make predictions
    2_unet_train     #2>&1 | tee ${LOG_DIR}/${CNN}/${TRAIN_LOG}

    # from Keras to TF
    3_unet_Keras2TF  #2>&1 | tee ${LOG_DIR}/${CNN}/unet_keras2tf.log

    # freeze the graph and inspect it
    4a_unet_freeze   #2>&1 | tee ${LOG_DIR}/${CNN}/${FREEZE_LOG}

    # evaluate the frozen graph performance
    4b_eval_graph #2>&1 | tee ${LOG_DIR}/${CNN}/${EVAL_FR_LOG}

    # quantize
    5a_unet_quantize #2>&1 | tee ${LOG_DIR}/${CNN}/${QUANT_LOG}

    # evaluate post-quantization model
    5b_eval_quantized_graph #2>&1 | tee ${LOG_DIR}/${CNN}/${EVAL_Q_LOG}

    # compile with Vitis AI to generate elf file for ZCU102
    6_compile_vai_vck190 #2>&1 | tee ${LOG_DIR}/${CNN}/${COMP_LOG}
    # move xmodel to  target board directory
    mv  ${COMPILE_DIR}/${CNN}2/*.xmodel  ${TARGET_190}/${CNN}/v2/model/
    cp ${COMPILE_DIR}/${CNN}2/*.json     ${TARGET_190}/${CNN}/v2/model/
    rm ${TARGET_190}/${CNN}/v2/model/*_org.xmodel

    # compile with Vitis AI to generate elf file for ZCU102
    6_compile_vai_zcu102 #2>&1 | tee ${LOG_DIR}/${CNN}/${COMP_LOG}
    # move xmodel to  target board directory
    mv  ${COMPILE_DIR}/${CNN}2/*.xmodel  ${TARGET_102}/${CNN}/v2/model/
    cp ${COMPILE_DIR}/${CNN}2/*.json     ${TARGET_102}/${CNN}v2/model/
    rm ${TARGET_102}/${CNN}/v2/model/*_org.xmodel

    # compile with Vitis AI to generate elf file for ZCU104
    6_compile_vai_zcu104 #2>&1 | tee ${LOG_DIR}/${CNN}/${COMP_LOG}
    # move xmodel to  target board directory
    mv  ${COMPILE_DIR}/${CNN}2/*.xmodel  ${TARGET_104}/${CNN}/v2/model/
    cp ${COMPILE_DIR}/${CNN}2/*.json     ${TARGET_104}/${CNN}/v2/model/
    rm ${TARGET_104}/${CNN}/v2/model/*_org.xmodel

    # copy test images into target board
    #tar -cvf "test.tar" ${DATASET_DIR}/img_test ${DATASET_DIR}/seg_test
    #gzip test.tar
    #cp test.tar.gz ${TARGET_190}/ ${TARGET_102}/ ${TARGET_104}/
    tar -cvf target_vck190.tar ${TARGET_190}/  >& /dev/null
    tar -cvf target_zcu102.tar ${TARGET_102}/  >& /dev/null
    tar -cvf target_zcu104.tar ${TARGET_104}/  >& /dev/null


    echo "#####################################"
    echo "MAIN UNET FLOW COMPLETED"
    echo "#####################################"


}

main
