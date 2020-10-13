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

cmake_minimum_required(VERSION 3.3)

get_filename_component(COMPONENT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${COMPONENT_NAME} VERSION 1.0.0 LANGUAGES C CXX)


find_package(OpenCV COMPONENTS opencv_core opencv_video opencv_videoio opencv_imgproc opencv_imgcodecs opencv_highgui)
if(NOT OpenCV_FOUND)
  find_package(OpenCV_LIBS opencv_core opencv_video opencv_videoio opencv_imgproc opencv_imgcodecs opencv_highgui)
endif(NOT OpenCV_FOUND)


find_package(Glog REQUIRED)
if(Glog_FOUND)
    include_directories(${Glog_INCLUDE_DIRS})
    message(STATUS "    Glog library status:")
    message(STATUS "    version ${Glog_VERSION}")
    message(STATUS "    libraries: ${Glog_LIBS}")
    message(STATUS "    include path: ${Glog_INCLUDE_DIRS}")    
endif()
include_directories("include" 
"${CMAKE_SYSROOT}/usr/include/xrt"
)
set(MY_SRCS src/hlsV4l2Access.cpp
            src/hlsV4l2Capture.cpp
            src/hlsV4l2Device.cpp
            src/hlsV4l2MmapDevice.cpp    
            src/hlsV4l2Output.cpp  
            src/hlsV4l2ReadWriteDevice.cpp
            src/xcl2.cpp 
)

add_library(hls_v4l2s SHARED ${MY_SRCS})

target_link_libraries(hls_v4l2s ${OpenCV_LIBS} glog)
add_executable(test_hls_kernel test/test_hls_kernel.cpp)
target_link_libraries(test_hls_kernel
      hls_v4l2s 
      ${OpenCV_LIBS} 
      glog xrt_core
      xrt_coreutil
      xilinxopencl
      )
install(TARGETS hls_v4l2s
        LIBRARY 
        DESTINATION /usr/lib
        )
install(FILES include/hlsV4l2Access.hpp  include/line_exception.hpp include/xcl2.hpp include/hlsV4l2Capture.hpp include/hlsV4l2ReadWriteDevice.hpp include/hlsV4l2Device.hpp include/hlsV4l2MmapDevice.hpp include/hlsV4l2Output.hpp 
    DESTINATION /usr/include
)
install(FILES include/dpdrm.hpp DESTINATION /usr/include/vitis/ai)
