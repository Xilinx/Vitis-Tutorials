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

wget -O vitis_ai_model_ZCU104_2019.2-r1.1.0.deb https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_model_ZCU104_2019.2-r1.1.0.deb
wget -O vitis-ai-runtime-1.1.2.tar.gz https://www.xilinx.com/bin/public/openDownload?filename=vitis-ai-runtime-1.1.2.tar.gz
tar -xzvf vitis-ai-runtime-1.1.2.tar.gz
mv vitis-ai-runtime-1.1.2/unilog/aarch64/* .
mv vitis-ai-runtime-1.1.2/VART/aarch64/* .
mv vitis-ai-runtime-1.1.2/Vitis-AI-Library/aarch64/* .
mv vitis-ai-runtime-1.1.2/XIR/aarch64/* .
rm -r vitis-ai-runtime-1.1.*
wget -O vitis_ai_library_r1.1_video.tar.gz https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_library_r1.1_video.tar.gz
wget -O vitis_ai_library_r1.1_images.tar.gz https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_library_r1.1_images.tar.gz
