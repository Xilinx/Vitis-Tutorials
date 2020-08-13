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

tar -xzvf glog-0.4.0-Linux.tar.gz --strip-components=1 -C /usr
irps5401
dpkg -i --force-all libunilog-1.1.0-Linux-build46.deb
dpkg -i libxir-1.1.0-Linux-build46.deb
dpkg -i libvart-1.1.0-Linux-build48.deb
dpkg -i libvitis_ai_library-1.1.0-Linux-build46.deb
dpkg -i vitis_ai_model_ZCU104_2019.2-r1.1.0.deb
tar -xzvf vitis_ai_library_r1.1_images.tar.gz -C ~/overview
tar -xzvf vitis_ai_library_r1.1_video.tar.gz -C ~/overview
