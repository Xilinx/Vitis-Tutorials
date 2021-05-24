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


# airplane automobile bird cat deer dog frog horse ship truck

tar -xvf fmnist_test.tar &> /dev/null

cd ./fmnist_test

cd ankleBoot
mv *.png ../
cd ..
rm -r ankleBoot/

cd bag
mv *.png ../
cd ..
rm -r bag/

cd coat
mv *.png ../
cd ..
rm -r coat/

cd dress
mv *.png ../
cd ..
rm -r dress/

cd pullover
mv *.png ../
cd ..
rm -r pullover/

cd sandal
mv *.png ../
cd ..
rm -r sandal/

cd shirt
mv *.png ../
cd ..
rm -r shirt/

cd sneaker
mv *.png ../
cd ..
rm -r sneaker/

cd top
mv *.png ../
cd ..
rm -r top

cd trouser
mv *.png ../
cd ..
rm -r trouser

cd ..
