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


# Author: Mark Harvey, Xilinx Inc


echo "-----------------------------------------"
echo "MAKE TARGET ZCU102 STARTED.."
echo "-----------------------------------------"

# remove previous results
rm -rf ${TARGET_ZCU102}
mkdir -p ${TARGET_ZCU102}
mkdir -p ${TARGET_ZCU102}/model_dir

# copy application to TARGET_ZCU102 folder
cp -ar ${APP}/* ${TARGET_ZCU102}
echo "  Copied application to target folder"


# copy xmodel to TARGET_ZCU102 folder
cp ${COMPILE_ZCU102}/${NET_NAME}.xmodel ${TARGET_ZCU102}/model_dir/.
echo "  Copied ${COMPILE_ZCU102}/${NET_NAME}.xmodel file(s) to target folder"


mkdir -p ${TARGET_ZCU102}/images
cp ${DSET_ROOT}/test_images/* ${TARGET_ZCU102}/images/.
echo "  Copied images to target folder"

echo "-----------------------------------------"
echo "MAKE TARGET ZCU102 COMPLETED"
echo "-----------------------------------------"




