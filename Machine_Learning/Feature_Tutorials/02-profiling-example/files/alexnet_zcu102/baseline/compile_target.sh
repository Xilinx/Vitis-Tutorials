#!/bin/bash

##
##* © Copyright (C) 2016-2020 Xilinx, Inc
##*
##* Licensed under the Apache License, Version 2.0 (the "License"). You may
##* not use this file except in compliance with the License. A copy of the
##* License is located at
##*
##*     http://www.apache.org/licenses/LICENSE-2.0
##*
##* Unless required by applicable law or agreed to in writing, software
##* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
##* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
##* License for the specific language governing permissions and limitations
##* under the License.
##*/

# *******************************************************************************

cp ./src/fps_main_method1.cc ./main.cc
cp ./model/arm64_4096/dbg_dpu_alexnetBNnoLRN_0.elf ./model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
make clean
make all
cp alexnetBNnoLRN dbg_fps_alexnetBNnoLRN_method1

cp ./model/arm64_4096/nor_dpu_alexnetBNnoLRN_0.elf ./model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
make clean
make all
cp alexnetBNnoLRN fps_alexnetBNnoLRN_method1

cp ./src/fps_main_method2.cc ./main.cc
cp ./model/arm64_4096/dbg_dpu_alexnetBNnoLRN_0.elf ./model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
make clean
make all
cp alexnetBNnoLRN dbg_fps_alexnetBNnoLRN_method2

cp ./model/arm64_4096/nor_dpu_alexnetBNnoLRN_0.elf ./model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
make clean
make all
cp alexnetBNnoLRN fps_alexnetBNnoLRN_method2

cp ./src/fps_main_method3.cc ./main.cc
cp ./model/arm64_4096/dbg_dpu_alexnetBNnoLRN_0.elf ./model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
make clean
make all
cp alexnetBNnoLRN dbg_fps_alexnetBNnoLRN_method3

cp ./model/arm64_4096/nor_dpu_alexnetBNnoLRN_0.elf ./model/arm64_4096/dpu_alexnetBNnoLRN_0.elf
make clean
make all
cp alexnetBNnoLRN fps_alexnetBNnoLRN_method3

cp ./src/top2_main.cc ./main.cc
make clean
make all
cp alexnetBNnoLRN top2_alexnetBNnoLRN

cp ./src/fps_main.cc ./main.cc
make clean
make all
cp alexnetBNnoLRN fps_alexnetBNnoLRN
