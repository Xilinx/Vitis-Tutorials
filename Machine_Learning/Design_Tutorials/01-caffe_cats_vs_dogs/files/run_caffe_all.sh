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

CNN=alexnetBNnoLRN

source caffe/set_prj_env_variables.sh

#python set_the_CATSvsDOGS_prj.py -i $ML_DIR

source caffe/caffe_flow_AlexNet.sh             2>&1 | tee logfile_caffe_AlexNet.txt

source deploy/${CNN}/quantiz/vaiq_${CNN}.sh    2>&1 | tee logfile_vaiq_${CNN}.txt
source deploy/${CNN}/quantiz/vaic_${CNN}.sh    2>&1 | tee logfile_vaic_${CNN}.txt


cd input/jpg/
tar -cvf test_images.tar ./test
mv test_images.tar ../../deploy/${CNN}/zcu102/
cd ../../
