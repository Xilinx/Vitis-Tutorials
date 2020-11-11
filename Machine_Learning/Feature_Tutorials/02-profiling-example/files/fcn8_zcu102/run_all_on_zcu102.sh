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


#unpack test images
tar -xvf ./test.tar.gz
tar -xvf ./test1.tar

# make softlinks
cd fcn8
ln -nsf ../workspace ./workspace
cd ../fcn8ups
ln -nsf ../workspace ./workspace

#run python3 script on fcn8
echo "DNNDK Python API: FCN8"
cd ../fcn8/
cd model
python3 ./run_fcn8_on_dpu.py
echo "DNNDK C++ API: FCN8 fps with multithreading"
cd ..
./fcn8 1
./fcn8 3
./fcn8 5
./fcn8 8

#run python3 script on fcn8ups
cd ../fcn8ups
cd model
echo "DNNDK Python API: FCN8UPS"
python3 ./run_fcn8ups_on_dpu.py
cd ..

./fcn8ups 1
./fcn8ups 3
./fcn8ups 5
./fcn8ups 8


# profiling
dexplorer -m profile
cd ../fcn8
ln -nsf ../workspace1 ./workspace
echo "fine-grained profiling: FCN8"
#run C++ executable with 1 thread
./dbg_fcn8 1


cd ../fcn8ups
ln -nsf ../workspace1 ./workspace
#run C++ executable with 1 thread
echo "fine-grained profiling: FCN8UPS"
./dbg_fcn8ups 1



cd ../VART
sh ./build.sh
mv VART fcn8
echo "VART: FCN8 with C++ API"
./fcn8 ../fcn8/model/nor_dpu_fcn8.elf
cd src
echo "VART: FCN8 mulithreading with Python API"
python3 ./fcn8.py 1 ../../fcn8/model/nor_dpu_fcn8.elf
python3 ./fcn8.py 2 ../../fcn8/model/nor_dpu_fcn8.elf
python3 ./fcn8.py 3 ../../fcn8/model/nor_dpu_fcn8.elf
python3 ./fcn8.py 4 ../../fcn8/model/nor_dpu_fcn8.elf
python3 ./fcn8.py 5 ../../fcn8/model/nor_dpu_fcn8.elf
python3 ./fcn8.py 6 ../../fcn8/model/nor_dpu_fcn8.elf
python3 ./fcn8.py 7 ../../fcn8/model/nor_dpu_fcn8.elf
