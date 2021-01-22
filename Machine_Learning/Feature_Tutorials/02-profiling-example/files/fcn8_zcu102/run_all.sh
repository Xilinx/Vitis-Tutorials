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

#########################################################################

cd ~
mv fcn8_zcu102.tar ~/DNNDK/
cd ~/DNNDK
tar -xvf fcn8_zcu102.tar
cd fcn8_zcu102

#unpack test images
tar -xvf ./test.tar
tar -xvf ./test1.tar

#########################################################################

# crosscompile the C++ applications with VART APIs
bash ./crosscompile_target.sh

#########################################################################

# make softlinks
cd fcn8
ln -nsf ../workspace ./workspace

#run python3 script on fcn8
cd model
python3 ./run_fcn8_on_dpu.py
cd ..
./fcn8 1
./fcn8 3
./fcn8 5
./fcn8 8

#########################################################################

# profiling
dexplorer -m profile

cd ../fcn8
ln -nsf ../workspace1 ./workspace
#run C++ executable with 1 thread
./dbg_fcn8 1

#########################################################################


cd ..
