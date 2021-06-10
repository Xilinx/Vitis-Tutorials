#!/bin/bash

# Copyright 2021 Xilinx Inc.
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

# Author: Daniele Bagni, Xilinx Inc

echo " "
echo "-----------------------------------------"
echo "PREPARE TARGET FOLDER."
echo "-----------------------------------------"

#prepare test images
python -c "from common import *; generate_images('./build/target/test_images')" 2>&1 | tee ./build/log/generate_test_images.log

#copy xmodel
cp -r ./application ./build/target
cp ./build/compiled_model/CNN_int_*.xmodel ./build/target/application

# launch functional debug
python -c "import common; dbg_model=common.RUN_CNN_DEBUG('./build/target/application/test')" 2>&1 | tee ./build/log/functional_debug_on_host.log
mv *.bin.npy ./build/target/application/

# make a tar file
cd ./build
tar -cvf target.tar ./target &> /dev/null
cd ..
