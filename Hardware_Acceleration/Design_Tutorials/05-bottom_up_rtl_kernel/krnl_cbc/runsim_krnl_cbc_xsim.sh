#!/bin/bash

#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#


# encrypt/decrypt 128-bit words number per data group (packet), must be multiple times of 16
export WORD_NUM=32

# number of data groups (packet) to be processed
export GROUP_NUM=32

# AES-256 mode KEY value. This is HEX format, and 32-bytes length
export KEY=123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0
export  IV=FEDCBA9876543210FEDCBA9876543210

# # Random data generation and use OpenSSL utility to generate reference encryption data
rm -f plain.dat cipher_ecb.dat cipher_cbc.dat
for ((i = 0; i < $GROUP_NUM; i++))
do
  ../common/plain_gen.pl $WORD_NUM ./plain_temp.dat
  cat ./plain_temp.dat >> plain.dat

  openssl aes-256-ecb \
        -e \
        -in ./plain_temp.dat \
        -out ./cipher_ecb_temp.dat \
        -K $KEY \
        -nopad
  cat ./cipher_ecb_temp.dat >> cipher_ecb.dat

  openssl aes-256-cbc \
        -e \
        -in ./plain_temp.dat \
        -out ./cipher_cbc_temp.dat \
        -K $KEY \
        -iv $IV \
        -nopad
  cat ./cipher_cbc_temp.dat >> cipher_cbc.dat
done

xvlog -f ./filelist_krnl_cbc.f      \
      -L xilinx_vip                 \
      -d WORD_NUM=$WORD_NUM \
      -d GROUP_NUM=$GROUP_NUM \
      -d KEY=$KEY \
      -d IV=$IV \
      --sv # -d DUMP_WAVEFORM
      
xelab tb_krnl_cbc glbl      \
      -debug typical        \
      -L unisims_ver        \
      -L xpm                \
      -L xilinx_vip

xsim -t xsim.tcl --wdb work.tb_krnl_cbc.wdb work.tb_krnl_cbc#work.glbl

