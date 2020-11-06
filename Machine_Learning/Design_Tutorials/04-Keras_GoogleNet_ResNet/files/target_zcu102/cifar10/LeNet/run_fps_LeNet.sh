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


CNN=LeNet

echo " "
echo "CNN 1"
./fps_${CNN} 1
echo " "
echo "CNN 2"
./fps_${CNN} 2
echo " "
echo "CNN 3"
./fps_${CNN} 3
echo " "
echo "CNN 4"
./fps_${CNN} 4
echo " "
echo "CNN 5"
./fps_${CNN} 5
echo " "
echo "CNN 6"
./fps_${CNN} 6
echo " "
echo "CNN 7"
./fps_${CNN} 7
echo " "
echo "CNN 8"
./fps_${CNN} 8
echo " "
