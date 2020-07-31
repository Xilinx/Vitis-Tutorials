#!/bin/sh

TARGET=ZCU104
NET_NAME=resnet50
DEPLOY_MODEL_PATH=vai_q_output
TF_NETWORK_PATH=./tf_resnetv1_50_imagenet_224_224_6.97G

ARCH=./arch.json

vai_c_tensorflow --frozen_pb ${TF_NETWORK_PATH}/quantized/deploy_model.pb \
                 --arch ${ARCH} \
		 --output_dir ${TF_NETWORK_PATH}/vai_c_output_${TARGET}/ \
		 --net_name ${NET_NAME} \
		 --options "{'save_kernel':''}"
