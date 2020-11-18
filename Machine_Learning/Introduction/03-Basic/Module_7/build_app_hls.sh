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
BOARD_IP=10.176.18.178

build_command(){
    cmake -DUSE_DRM=on -DUSE_KERNEL=on .. && make
    cp *drm ../

}

clean_workspace(){
    if [ -d build ]; then
    rm -rf build
    fi
    mkdir -pv build
    cd build
    
}
copy_file()
{
    scp usb_input* root@$BOARD_IP:/home/root/
    scp HLS_*/lib* root@$BOARD_IP:/usr/lib
    scp HLS_*/test_hls_kernel root@$BOARD_IP:/home/root/
}

main(){
    clean_workspace
    git pull
    build_command
    copy_file
}

main @$