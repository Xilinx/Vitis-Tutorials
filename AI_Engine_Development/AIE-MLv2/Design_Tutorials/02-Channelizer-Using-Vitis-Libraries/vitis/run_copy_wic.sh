#!/bin/bash
#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
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
wic cp --sector-size=4096 ${DATA_DIR}/sig_i.txt                       ./${QEMU_COMBINED}/${WIC_PARTITION}
wic cp --sector-size=4096 ${DATA_DIR}/sig_o.txt                       ./${QEMU_COMBINED}/${WIC_PARTITION}
echo "copied Stimulus IO files"

