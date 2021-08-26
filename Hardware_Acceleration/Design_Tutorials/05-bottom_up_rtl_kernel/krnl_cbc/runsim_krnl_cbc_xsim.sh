#!/bin/bash

#
# Copyright 2021 Xilinx, Inc.
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

