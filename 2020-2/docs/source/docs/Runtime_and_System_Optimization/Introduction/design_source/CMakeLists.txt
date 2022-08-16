cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCL REQUIRED)
find_package(XRT REQUIRED)
find_package(OpenMP)
find_package(OpenCV)

if(NOT XRT_FOUND)
  MESSAGE(FATAL_ERROR "XRT not found, please source XRT setup script before building")
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/hw_src/alveo_examples.xclbin")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/hw_src/alveo_examples.xclbin ${CMAKE_CURRENT_BINARY_DIR}/alveo_examples.xclbin COPYONLY)
else()
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/alveo_examples_u200.xclbin ${CMAKE_CURRENT_BINARY_DIR}/alveo_examples.xclbin COPYONLY)
endif()

# Library of utility functions common to all applications
add_library(example_utils STATIC
  sw_src/event_timer.cpp
  sw_src/xilinx_ocl_helper.cpp
)

target_include_directories(example_utils PUBLIC
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
)

# Basic kernel load/unload example
add_executable(00_load_kernels
  sw_src/00_load_kernels.cpp)

target_include_directories(00_load_kernels PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/sw_src
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
  )

target_link_libraries(00_load_kernels PRIVATE
  ${XRT_LIBS}
  ${OpenCL_LIBRARIES}
  pthread
  uuid
  ${CMAKE_DL_LIBS}
  example_utils
  )

# Simple malloc example
add_executable(01_simple_malloc
  sw_src/01_simple_malloc.cpp)

target_include_directories(01_simple_malloc PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/sw_src
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
  )

target_link_libraries(01_simple_malloc PRIVATE
  ${XRT_LIBS}
  ${OpenCL_LIBRARIES}
  pthread
  uuid
  ${CMAKE_DL_LIBS}
  example_utils
  )

# Aligned malloc example
add_executable(02_aligned_malloc
  sw_src/02_aligned_malloc.cpp)

target_include_directories(02_aligned_malloc PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/sw_src
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
  )

target_link_libraries(02_aligned_malloc PRIVATE
  ${XRT_LIBS}
  ${OpenCL_LIBRARIES}
  pthread
  uuid
  ${CMAKE_DL_LIBS}
  example_utils
  )

# OpenCL buffer map example
add_executable(03_buffer_map
  sw_src/03_buffer_map.cpp)

target_include_directories(03_buffer_map PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/sw_sr
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
  )

target_link_libraries(03_buffer_map PRIVATE
  ${XRT_LIBS}
  ${OpenCL_LIBRARIES}
  pthread
  uuid
  ${CMAKE_DL_LIBS}
  example_utils
  )

  # Wide VADD example
add_executable(04_wide_vadd
sw_src/04_wide_vadd.cpp)

target_include_directories(04_wide_vadd PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/sw_src
${XRT_INCLUDE_DIRS}
${OpenCL_INCLUDE_DIRS}
)

target_link_libraries(04_wide_vadd PRIVATE
${XRT_LIBS}
${OpenCL_LIBRARIES}
pthread
uuid
${CMAKE_DL_LIBS}
example_utils
)

  # Wide VADD example (Pipelined)
  add_executable(05_pipelined_vadd
  sw_src/05_pipelined_vadd.cpp)

  target_include_directories(05_pipelined_vadd PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/sw_src
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
  )

  target_link_libraries(05_pipelined_vadd PRIVATE
  ${XRT_LIBS}
  ${OpenCL_LIBRARIES}
  pthread
  uuid
  ${CMAKE_DL_LIBS}
  example_utils
  )

# Wide VADD example (Pipelined)
if (OpenMP_FOUND)
  add_executable(06_wide_processor
  sw_src/06_wide_processor.cpp)

  target_include_directories(06_wide_processor PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/sw_src
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
  )

  target_link_libraries(06_wide_processor PRIVATE
  ${XRT_LIBS}
  ${OpenCL_LIBRARIES}
  pthread
  uuid
  ${CMAKE_DL_LIBS}
  example_utils
  ${OpenMP_LIBRARIES}
  ${OpenMP_CXX_FLAGS}
  )

  target_compile_options(06_wide_processor PRIVATE
  ${OpenMP_CXX_FLAGS}
  )
else()
  MESSAGE(WARNING "Will not build example 6, OpenMP not found")
endif()

# OpenCV small pipeline example
if (OpenCV_FOUND)
  add_executable(07_opencv_resize
  sw_src/07_opencv_resize.cpp)

  target_include_directories(07_opencv_resize PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/sw_src
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  )

  target_link_libraries(07_opencv_resize PRIVATE
  ${XRT_LIBS}
  ${OpenCL_LIBRARIES}
  pthread
  uuid
  ${CMAKE_DL_LIBS}
  example_utils
  xml2
  ${OpenCV_LIBRARIES}
  )
else()
  MESSAGE(STATUS "Will not build example 7, OpenCV not found")
endif()

# OpenCV small pipeline example
if (OpenCV_FOUND)
  add_executable(08_opencv_resize_blur
  sw_src/08_opencv_resize_blur.cpp)

  target_include_directories(08_opencv_resize_blur PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/sw_src
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  )

  target_link_libraries(08_opencv_resize_blur PRIVATE
  ${XRT_LIBS}
  ${OpenCL_LIBRARIES}
  pthread
  uuid
  ${CMAKE_DL_LIBS}
  example_utils
  xml2
  ${OpenCV_LIBRARIES}
  )
else()
  MESSAGE(STATUS "Will not build example 8, OpenCV not found")
endif()



