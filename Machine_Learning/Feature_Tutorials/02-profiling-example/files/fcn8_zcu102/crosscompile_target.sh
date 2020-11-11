#!/bin/bash

: '
/**

* © Copyright (C) 2016-2020 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/
'

#decompress all *.elf.gz and *.so.gz files
for file in $(find $PWD -name "*.so.gz"); do
    echo  ${file}
    gzip -v -d ${file}
done
for file in $(find $PWD -name "*.elf.gz"); do
    echo  ${file}
    gzip -v -d ${file}
done

# make softlinks
cd fcn8
cp ./model/dbg_dpu_fcn8.elf ./model/dpu_fcn8.elf
make clean all
cp ./fcn8 ./dbg_fcn8

cp ./model/nor_dpu_fcn8.elf ./model/dpu_fcn8.elf
make clean all

cd ../fcn8ups
cp ./model/dbg_dpu_fcn8ups.elf ./model/dpu_fcn8ups.elf
make clean all
cp ./fcn8ups ./dbg_fcn8ups

cp ./model/nor_dpu_fcn8ups.elf ./model/dpu_fcn8ups.elf
make clean all

cd ../..
