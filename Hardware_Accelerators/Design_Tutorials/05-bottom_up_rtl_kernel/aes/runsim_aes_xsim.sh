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

# encrypt/decrypt 256 words (128-bit) data, namely 4KB
export WORD_NUM=256         

# AES-256 mode KEY value. This is HEX format, and 32-bytes length
export KEY=123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0

# Random data generation
../common/plain_gen.pl $WORD_NUM ./plain.dat

# Use OpenSSL utility to generate reference encryption data for simulation
openssl aes-256-ecb \
        -e \
        -in ./plain.dat \
        -out ./cipher.dat \
        -K $KEY

# Use Vivado xsim to simulation the RTL design
# OP_MODE = 1 means encryption
xvlog -d PARA_OVERRIDE \
      -d WORD_NUM=$WORD_NUM \
      -d KEY=$KEY \
      -f ./filelist_aes.f #-d DUMP_WAVEFORM
xelab tb_aes -debug typical
xsim -t xsim.tcl work.tb_aes

