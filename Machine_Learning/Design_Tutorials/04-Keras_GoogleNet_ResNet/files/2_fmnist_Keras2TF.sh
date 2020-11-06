#!/bin/bash

## © Copyright (C) 2016-2020 Xilinx, Inc
##
## Licensed under the Apache License, Version 2.0 (the "License"). You may
## not use this file except in compliance with the License. A copy of the
## License is located at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
## WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
## License for the specific language governing permissions and limitations
## under the License.




echo " "
echo "############################################################################################"
echo "KERAS to TENSORFLOW GRAPH CONVERSION for LeNet on FMNIST"
echo "############################################################################################"
# convert Keras model into TF inference graph
python code/Keras2TF.py -n LeNet 2>&1 | tee rpt/fmnist/2_keras2TF_graph_conversion_LeNet.log


echo " "
echo "#################################################################################################"
echo "KERAS to TENSORFLOW GRAPH CONVERSION for miniVggNet  on FMNIST"
echo "#################################################################################################"
python code/Keras2TF.py -n miniVggNet 2>&1 | tee rpt/fmnist/2_keras2TF_graph_conversion_miniVggNet.log


echo " "
echo "#################################################################################################"
echo "KERAS to TENSORFLOW GRAPH CONVERSION for miniGoogleNet  on FMNIST"
echo "#################################################################################################"
python code/Keras2TF.py -n miniGoogleNet 2>&1 | tee rpt/fmnist/2_keras2TF_graph_conversion_miniGoogleNet.log

echo " "
echo "#################################################################################################"
echo "KERAS to TENSORFLOW GRAPH CONVERSION for miniResNet  on FMNIST"
echo "#################################################################################################"
python code/Keras2TF.py -n miniResNet 2>&1 | tee rpt/fmnist/2_keras2TF_graph_conversion_miniResNet.log
