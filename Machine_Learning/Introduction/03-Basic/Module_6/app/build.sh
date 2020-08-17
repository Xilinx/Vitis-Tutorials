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

#!/bin/bash
g++ -std=c++14 -O2 -o usb_input_multi_threads_refinedet_drm test/usb_input_multi_threads_refinedet.cpp -Iinclude src/*.cpp -lglog -lmy_v4l2s -lopencv_core -lopencv_video -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -lvitis_ai_library-refinedet -lvitis_ai_library-model_config -ljson-c -lglog -lpthread -ldrm -lvart-runner -I/usr/include/drm -DUSE_DRM=1
