# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11

project(rtl_stream_ref)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(XRT_INCLUDE_DIRS $ENV{XILINX_XRT}/include)
set(XRT_LIBS xrt_coreutil)

find_package(OpenCV REQUIRED)

include_directories(
${XRT_INCLUDE_DIRS}
)

link_directories(
$ENV{XILINX_XRT}/lib
)

add_executable(rtc_gen_test 
src/rtc_gen_test.cpp
)

target_link_libraries(rtc_gen_test
${XRT_LIBS}
${OpenCV_LIBRARIES}
)

add_executable(rtc_alpha_tb 
src/rtc_alpha_tb.cpp
)

target_link_libraries(rtc_alpha_tb
${XRT_LIBS}
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
