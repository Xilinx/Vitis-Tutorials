cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

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

if (NOT XRT_FOUND)
    MESSAGE(FATAL_ERROR "XRT not found, please source XRT setup script before building")
endif()

add_executable(resize_kernel_test_fileio
    resize_test_fileio.cpp
    event_timer.cpp
)

target_include_directories(resize_kernel_test_fileio PRIVATE
    ${XRT_INCLUDE_DIRS}
    ${OpenCL_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(resize_kernel_test_fileio PRIVATE
    xilinx_ocl
    ${XRT_LIBS}
    ${OpenCL_LIBRARIES}
    pthread
    uuid
    ${CMAKE_DL_LIBS}
    xml2
    ${OpenCV_LIBRARIES}
)

install (TARGETS resize_kernel_test_fileio DESTINATION bin)
