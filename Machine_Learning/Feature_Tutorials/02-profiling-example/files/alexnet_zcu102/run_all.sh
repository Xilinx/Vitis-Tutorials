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

############################################################################

# extract the archive
cd ~
mv alexnet_zcu102.tar ~/DNNDK/
cd ~/DNNDK
tar -xvf alexnet_zcu102.tar
cd alexnet_zcu102

############################################################################
# DNNDK APIs
############################################################################

# crosscompile the C++ applications with DNNDK APIs
bash ./crosscompile_alexnet.sh

# extract the test input images
tar -xvf test_images.tar

# run baseline CNN
cd baseline
bash ./run_all_baseline.sh
# clean everything
make clean

# run pruned CNN
cd ../pruned
bash ./run_all_pruned.sh
# clean everything
make clean


############################################################################


# remove images
rm -r test_images

# clean
make ultraclean
cd ../baseline
make ultraclean
