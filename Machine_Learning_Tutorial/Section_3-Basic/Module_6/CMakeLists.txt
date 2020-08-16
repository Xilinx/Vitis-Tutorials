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
cmake_minimum_required(VERSION 3.5)
project(In_Depth_Tutorial VERSION 1.2.0 LANGUAGES C CXX)
find_package(OpenCV COMPONENTS opencv_core opencv_video opencv_videoio opencv_imgproc opencv_imgcodecs opencv_highgui)
if(NOT OpenCV_FOUND)
  find_package(OpenCV_LIBS opencv_core opencv_video opencv_videoio opencv_imgproc opencv_imgcodecs opencv_highgui)
endif(NOT OpenCV_FOUND)
add_subdirectory(myV4L2)
add_definitions(-DUSE_DRM)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/app/include")
include_directories("{CMAKE_SYSROOT}/usr/include/drm")
add_executable(usb_input_multi_threads_refinedet_drm ${CMAKE_CURRENT_SOURCE_DIR}/app/test/usb_input_multi_threads_refinedet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/decodethread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dputhread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guithread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sortthread.cpp


)
target_link_libraries(usb_input_multi_threads_refinedet_drm OpenCV my_v4l2s
vitis_ai_library-refinedet
vitis_ai_library-model_config
json-c
glog
pthread
drm
vart-runner
 )