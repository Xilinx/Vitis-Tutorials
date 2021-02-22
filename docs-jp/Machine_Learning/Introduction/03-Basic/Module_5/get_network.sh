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


#NETWORK_NAME=cf_refinedet_coco_480_360_0.96_1.1.zip
#NETWORK_NAME=cf_refinedet_coco_480_360_0.96_1.2.zip
NETWORK_NAME_PRUNED_5G=cf_refinedet_coco_360_480_0.96_5.08G_1.3.zip
NETWORK_NAME_BASELINE=cf_refinedet_coco_360_480_123G_1.3.zip


wget -O $NETWORK_NAME_PRUNED_5G  https://www.xilinx.com/bin/public/openDownload?filename=$NETWORK_NAME_PRUNED_5G
wget -O $NETWORK_NAME_BASELINE  https://www.xilinx.com/bin/public/openDownload?filename=$NETWORK_NAME_BASELINE

unzip $NETWORK_NAME_PRUNED_5G
unzip $NETWORK_NAME_BASELINE

rm $NETWORK_NAME_PRUNED_5G
rm $NETWORK_NAME_BASELINE

