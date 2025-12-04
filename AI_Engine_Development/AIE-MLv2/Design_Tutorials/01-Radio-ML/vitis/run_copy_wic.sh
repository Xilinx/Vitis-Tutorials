#!/bin/bash
#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani
unset LD_LIBRARY_PATH
source ${YOCTO_ARTIFACTS}/amd-cortexa78-mali-common_meta-edf-app-sdk/sdk/environment-setup-cortexa72-cortexa53-amd-linux
echo "sourced"
wic cp --sector-size=4096 ${EMBD_EXC}                                 ./${QEMU_COMBINED}/${WIC_PARTITION}
echo "copied embedded_exec.sh"                                 
wic cp --sector-size=4096 ${APP_EXE}                                  ./${QEMU_COMBINED}/${WIC_PARTITION}
echo "copied host.exe"                                                
wic cp --sector-size=4096 ${BUILD_DIR}/${XCLBIN}                      ./${QEMU_COMBINED}/${WIC_PARTITION}
echo "copied xclbin"                                                  
wic cp --sector-size=4096 ${SD_CARD}/${DUT}.dtbo                      ./${QEMU_COMBINED}/${WIC_PARTITION}
echo "copied dtbo"
wic cp --sector-size=4096 ${SD_CARD}/${DUT}.pdi                       ./${QEMU_COMBINED}/${WIC_PARTITION}
echo "copied pdi"
wic cp --sector-size=4096 ${STIMULUS_DIR}/data_iq.txt                 ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/data_o.txt                  ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w1_weights-rtp.txt          ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w1_biases-rtp.txt           ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w3_weights-rtp.txt          ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w3_biases-rtp.txt           ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w5_weights-rtp.txt          ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w5_biases-rtp.txt           ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w7_weights-rtp.txt          ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w7_biases-rtp.txt           ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w9_weights-rtp.txt          ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w9_biases-rtp.txt           ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w11_weights-rtp.txt         ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w11_biases-rtp.txt          ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w13_weights-rtp.txt         ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w13_biases-rtp.txt          ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w16_0_weights-rtp.txt       ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w16_1_weights-rtp.txt       ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w16_biases-rtp.txt          ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w17_weights-rtp.txt         ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w17_biases-rtp.txt          ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w18_weights-rtp.txt         ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/w18_biases-rtp.txt          ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${STIMULUS_DIR}/modulation_classes.txt      ./${QEMU_COMBINED}/${WIC_PARTITION}
echo "copied Stimulus IO files"

