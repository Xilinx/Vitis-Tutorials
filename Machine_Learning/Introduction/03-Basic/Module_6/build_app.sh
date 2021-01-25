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
echo "[INFO] Pull the repo"
git pull
rm -rf ./build
rm -rf ./install
rm -rf ./test
echo "[INFO] Start to build"
mkdir build&&cd build
cmake ..&& make
mkdir -pv ../install
mkdir -pv ../test
cp usb_input_multi_threads_refinedet_drm ../install/
cp myV4L2/libmy_v4l2s.so ../test/
cp myV4L2/test_v4l2 ../test/
echo "[INFO] Build success"