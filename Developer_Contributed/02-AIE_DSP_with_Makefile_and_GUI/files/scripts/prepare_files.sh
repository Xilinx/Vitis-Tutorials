#!/bin/bash

# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

cd $WRK_DIR #your working directory
cd $TUTORIAL/files
rm -rf thin-plat

# copy original Thin Platform repo and rename it
cp -r ../../01-Versal_Custom_Thin_Platform_Extensible_System .
mv ./01-Versal_Custom_Thin_Platform_Extensible_Systems ./thin-plat
cd thin-plat

# clean everything just to be shure
make clean

# remove folders that are not needed
rm -rf ip/vadd* ip/counter ip/subtractor
rm -rf ps_apps/vadd*
rm -rf documentation
# rename folders
mv ps_apps/aie_dly_test ps_apps/aie_test
mv ip/mm2s_vadd_s ip/mm2s_aie
mv ip/s2mm_vadd_s ip/s2mm_aie

cd ..
# remove files that are not needed
rm ./thin-plat/README*              ./thin-plat/*.json
rm ./thin-plat/ip/aie/data/*        ./thin-plat/ip/aie/src/*
rm ./thin-plat/ip/aie/src/graphs/*  ./thin-plat/ip/aie/src/kernels/*
rm ./thin-plat/ip/mm2s_aie/src/*    ./thin-plat/ip/s2mm_aie/src/*
rm ./thin-plat/ps_apps/aie_test/src/*
rm ./thin-plat/vitis/src/system.cfg

# copy the new files
cp ./make-flow/src/thin-plat/Makefile     ./thin-plat/
cp ./make-flow/src/ip/aie/data/*          ./thin-plat/ip/aie/data/
cp ./make-flow/src/ip/aie/Makefile        ./thin-plat/ip/aie/
cp ./make-flow/src/ip/aie/src/*           ./thin-plat/ip/aie/src/
cp ./make-flow/src/ip/aie/src/graphs/*    ./thin-plat/ip/aie/src/graphs/
cp ./make-flow/src/ip/mm2s_aie/Makefile   ./thin-plat/ip/mm2s_aie/
cp ./make-flow/src/ip/mm2s_aie/src/*      ./thin-plat/ip/mm2s_aie/src/
cp ./make-flow/src/ip/s2mm_aie/Makefile   ./thin-plat/ip/s2mm_aie/
cp ./make-flow/src/ip/s2mm_aie/src/*      ./thin-plat/ip/s2mm_aie/src/
cp ./make-flow/src/ps_apps/aie_test/Make* ./thin-plat/ps_apps/aie_test/
cp ./make-flow/src/ps_apps/aie_test/src/* ./thin-plat/ps_apps/aie_test/src/
cp ./make-flow/src/vitis/src/system.cfg   ./thin-plat/vitis/src/system.cfg
