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


CNN=miniVggNet

## compile the executable for target board
## top5
cp ./src/top5_tf_main.cc ./tf_main.cc
make clean
make
mv ./${CNN} ./top5_${CNN}
## fps
cp ./src/fps_tf_main.cc ./tf_main.cc
make clean
make
mv ./${CNN} ./fps_${CNN}

