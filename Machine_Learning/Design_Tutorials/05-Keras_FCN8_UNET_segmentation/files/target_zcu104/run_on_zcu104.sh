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
#unpack test images
tar -xvf ./test.tar.gz

# make softlinks
cd fcn8
ln -nsf ../workspace ./workspace
cd ../fcn8ups
ln -nsf ../workspace ./workspace
cd ../unet/v2
ln -nsf ../../workspace ./workspace

#run python3 script on fcn8
cd ../../fcn8/
cd model
python3 ./run_fcn8_on_dpu.py 2>&1 | tee dpu_logfile_fcn8_py.txt
cd ..
#run C++ executable with 1 thread
./fcn8 1 2>&1 | tee dpu_logfile_fcn8_th1.txt

#run python3 script on fcn8ups
cd ../fcn8ups
cd model
python3 ./run_fcn8ups_on_dpu.py 2>&1 | tee dpu_logfile_fcn8ups_py.txt
cd ..
#run C++ executable with 1 thread
./fcn8ups 1 2>&1 | tee dpu_logfile_fcn8ups_th1.txt


#run python3 script on unet
cd ../unet/v2
cd model
python3 ./run_unet2_on_dpu.py 2>&1 | tee dpu_logfile_unet2_py.txt
cd ..
#run C++ executable with 1 thread
./unet2 1 2>&1 | tee dpu_logfile_unet2_th1.txt



cd ../..





