#
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
#

#!/bin/bash
set -e

build_command(){
    cmake -DUSE_DRM=on -DUSE_KERNEL=on .. && make
    

}

clean_workspace(){
    if [ -d build ]; then
    rm -rf build
    rm -rf ../install
    rm -rf ../test
    fi
    mkdir -pv build
    cd build
    
}
create_dir()
{
    mkdir -pv ../install
    mkdir -pv ../test
}
copy_file()
{   
    cp usb_input_multi_threads_refinedet_hls_drm ../test
    cp HLS_*/lib* ../install
    cp HLS_*/test_hls_kernel ../test
}

main(){
    clean_workspace
    git pull
    build_command
    create_dir
    copy_file
}

main @$