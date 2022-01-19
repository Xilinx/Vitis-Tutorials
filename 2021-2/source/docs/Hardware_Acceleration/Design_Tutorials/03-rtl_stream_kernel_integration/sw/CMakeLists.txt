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

project(rtl_stream_ref)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(XRT_INCLUDE_DIRS $ENV{XILINX_XRT}/include)
set(XRT_LIBS xilinxopencl xrt_core)
set(XRT_LIBS_EMU xilinxopencl xrt_hwemu)

find_package(OpenCV REQUIRED)

include_directories(
${XRT_INCLUDE_DIRS}
${OpenCV_INCLUDE_DIRS}
)

link_directories(
$ENV{XILINX_XRT}/lib
)

add_executable(rtc_gen_test 
src/rtc_gen_test.cpp
src/xcl2/xcl2.cpp
)

target_link_libraries(rtc_gen_test
${XRT_LIBS}
${OpenCV_LIBRARIES}
)

add_executable(rtc_alpha_tb 
src/rtc_alpha_tb.cpp
src/xcl2/xcl2.cpp
)

target_link_libraries(rtc_alpha_tb
${XRT_LIBS}
${OpenCV_LIBRARIES}
)

add_executable(rtc_gen_test_emu 
src/rtc_gen_test.cpp
src/xcl2/xcl2.cpp
)

target_link_libraries(rtc_gen_test_emu
${XRT_LIBS_EMU}
${OpenCV_LIBRARIES}
)

add_executable(rtc_alpha_tb_emu 
src/rtc_alpha_tb.cpp
src/xcl2/xcl2.cpp
)

target_link_libraries(rtc_alpha_tb_emu
${XRT_LIBS_EMU}
${OpenCV_LIBRARIES}
)

EXEC_PROGRAM(rm ARGS "-rf ./rtc_alpha_hw.xclbin")
EXEC_PROGRAM(rm ARGS "-rf ./rtc_gen_test_hw.xclbin")
EXEC_PROGRAM(rm ARGS "-rf ./rtc_alpha_hw_emu.xclbin")
EXEC_PROGRAM(rm ARGS "-rf ./rtc_gen_test_hw_emu.xclbin")

EXEC_PROGRAM(ln ARGS "-s ../../hw/rtc_alpha_hw.xclbin")
EXEC_PROGRAM(ln ARGS "-s ../../hw/rtc_gen_test_hw.xclbin")
EXEC_PROGRAM(ln ARGS "-s ../../hw/rtc_alpha_hw_emu.xclbin")
EXEC_PROGRAM(ln ARGS "-s ../../hw/rtc_gen_test_hw_emu.xclbin")
