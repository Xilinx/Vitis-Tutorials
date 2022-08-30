cmake_minimum_required(VERSION 3.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/src/xilinx_ocl/cmake/Modules")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/src/xilinx_edge_utils/cmake/Modules")

set(CMAKE_CXX_STANDARD 11)

# Turn of optimization for debug builds
if (CMAKE_COMPILER_IS_GNUCC)
    MESSAGE(STATUS "Disabling GCC optimization for debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_C_FLGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

    if (ENABLE_PROFILING)
        # Optional profiling flags
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    endif()
endif(CMAKE_COMPILER_IS_GNUCC)

find_package(OpenCL REQUIRED)
find_package(XRT REQUIRED)
find_package(OpenCV REQUIRED)
find_package(DNNDK REQUIRED)

if (NOT XRT_FOUND)
    MESSAGE(FATAL_ERROR "XRT not found, please source XRT setup script before building")
endif()

if (NOT DNNDK_FOUND)
    MESSAGE(FATAL_ERROR "DNNDK not found!")
endif()

# DPU test (DenseBox example from DNNDK)
add_executable(sort_reid_test
    sort_reid_test.cpp
    event_timer.cpp
)

target_include_directories(sort_reid_test PRIVATE
    ${DNNDK_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/../../dpu_utils
)

target_link_libraries(sort_reid_test PRIVATE
    dputils
    ${DNNDK_LIBS}
    xilinx_ocl
    ${OpenCV_LIBRARIES}
    pthread
)

install (TARGETS sort_reid_test DESTINATION bin)
