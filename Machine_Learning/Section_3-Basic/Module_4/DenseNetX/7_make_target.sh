#!/bin/bash

# Copyright 2020 Xilinx Inc.
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


echo "-----------------------------------------"
echo "MAKE TARGET STARTED.."
echo "-----------------------------------------"


# remove previous results
rm -rf ${TARGET}

# copy target template to build folder
cp -ar ${TARGET_TEMPLATE} ${BUILD}
mv ${BUILD}/target_template ${TARGET}
echo "  Copied target template to target folder"


# copy elf to target folder
cp ${COMPILE}/*.elf ${TARGET}/model_dir/.
echo "  Copied elf file(s) to target folder"


mkdir -p ${TARGET}/images


python tf_gen_images.py  \
    --dataset=test \
    --image_dir=${TARGET}/images \
    --max_images=10000
echo "  Copied images to target folder"

echo "-----------------------------------------"
echo "MAKE TARGET COMPLETED"
echo "-----------------------------------------"

