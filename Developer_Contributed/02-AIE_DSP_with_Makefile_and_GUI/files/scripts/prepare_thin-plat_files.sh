#!/bin/bash

# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# remove previous folder without deleting it
mv ${WRK_DIR}/${TUTORIAL}/files/thin-plat ${WRK_DIR}/${TUTORIAL}/files/tmp
# copy original Thin Platform repo and rename it
cp -r ${WRK_DIR}/01-Versal_Custom_Thin_Platform_Extensible_System ${WRK_DIR}/${TUTORIAL}/files/
mv ${WRK_DIR}/${TUTORIAL}/files/01-Versal_Custom_Thin_Platform_Extensible_System ${WRK_DIR}/${TUTORIAL}/files/thin-plat
# clean just to be shure
make clean -C ${WRK_DIR}/${TUTORIAL}/files/thin-plat

# remove folders that are not needed
cd ${WRK_DIR}/${TUTORIAL}/files/thin-plat
rm -rf vitis/ip/vadd* vitis/ip/counter vitis/ip/subtractor
rm -rf ps_apps/linux/vadd*
rm -rf documentation
# rename folders
mv ps_apps/linux/aie_dly_test ps_apps/linux/aie_test
mv vitis/ip/mm2s_vadd_s vitis/ip/mm2s_aie
mv vitis/ip/s2mm_vadd_s vitis/ip/s2mm_aie

# remove files that are not needed
cd ${WRK_DIR}/${TUTORIAL}/files/
rm -f  ./thin-plat/README*
rm -f  ./thin-plat/*.json
rm -f  ./thin-plat/vitis/ip/aie/data/*
rm -f  ./thin-plat/vitis/ip/aie/src/*
rm -rf ./thin-plat/vitis/ip/aie/vitis*wrapper*
rm -rf ./thin-plat/vitis/ip/aie/vck190*wrapper*
rm -rf ./thin-plat/vitis/ip/aie/src/graphs/*
rm -rf ./thin-plat/vitis/ip/aie/src/kernels/*
rm -f  ./thin-plat/vitis/ip/mm2s_aie/src/*
rm -f  ./thin-plat/vitis/ip/s2mm_aie/src/*
rm -f  ./thin-plat/ps_apps/linux/aie_test/src/*
rm -f  ./thin-plat/vitis/src/system.cfg

# copy the source code files
cp ./src/ip/aie/data/*          ./thin-plat/vitis/ip/aie/data/
cp ./src/ip/aie/src/*           ./thin-plat/vitis/ip/aie/src/
cp ./src/ip/aie/src/graphs/*    ./thin-plat/vitis/ip/aie/src/graphs/
cp ./src/ip/mm2s_aie/src/*      ./thin-plat/vitis/ip/mm2s_aie/src/
cp ./src/ip/s2mm_aie/src/*      ./thin-plat/vitis/ip/s2mm_aie/src/
cp ./src/ps_apps/aie_test/src/* ./thin-plat/ps_apps/linux/aie_test/src/
cp ./src/vitis/src/system.cfg   ./thin-plat/vitis/src/system.cfg

# copy the modified makefiles
cp -rf ./src/thin-plat/*  ./thin-plat/
