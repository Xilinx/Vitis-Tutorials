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

tar -xvf cifar10_test.tar &> /dev/null

cd ./cifar10_test

cd automobile
mv *.png ../
cd ..
rm -r automobile/

cd airplane
mv *.png ../
cd ..
rm -r airplane/

cd bird
mv *.png ../
cd ..
rm -r bird/

cd cat
mv *.png ../
cd ..
rm -r cat/

cd deer
mv *.png ../
cd ..
rm -r deer/

cd dog
mv *.png ../
cd ..
rm -r dog/

cd frog
mv *.png ../
cd ..
rm -r frog/

cd horse
mv *.png ../
cd ..
rm -r horse/

cd ship
mv *.png ../
cd ..
rm -r ship

cd truck
mv *.png ../
cd ..
rm -r truck

cd ..
