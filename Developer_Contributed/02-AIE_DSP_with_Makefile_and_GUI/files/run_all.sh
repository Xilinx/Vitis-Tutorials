#!/bin/bash

# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# DO NOT CHANGE THE ORDER OF THE FOLLOWING COMMANDS!

# global variable

# where you are
#echo " "
#echo "STARTING DIR IS: " $PWD
#echo " "
arg=$PWD

# https://stackoverflow.com/questions/25416991/bash-script-to-check-file-name-begins-with-expected-string
#grab last 5  characters of the variable with
last5=${arg:${#arg}-5}
echo $last5
if [[ "${last5}" == "files" ]];
then

   echo "you started from the right folder" $arg

   # clean everything
   cd make-flow
   make clean
   cd ..
    # clean file text formats
   source ./scripts/dos2unix_all.sh

   # set the environment ## XILINX-AMD GUUP SERVER
   source ./scripts/vck190_launch_ts.sh

   ##############################################################################
   # prepare folders for the GUI-based flow
   rm -rf gui-flow   # remove previous content
   mkdir ./gui-flow  # make a new dir
   mkdir ./gui-flow/fir-dsplib_prj  # make another subdir

   ##############################################################################
   # design with Makefile flow
   # build everything
   cd make-flow
   make all
   # return to <*/files> folder
   cd ..

   ##############################################################################
   # design with the THIN PLATFORM

   #prepare the files for makefile flow
   source ./scripts/prepare_files.sh

   # build all by using prebuilt petalinux and xpfm
   cd thin-plat
   make all TARGET=hw
   # return to <*/files> folder
   cd ..


else
    echo ${arg}
    echo "IS A WRONG DIRECTORY: <*/files> must be the last folder from where to start!"
    echo "EXITING"
fi
