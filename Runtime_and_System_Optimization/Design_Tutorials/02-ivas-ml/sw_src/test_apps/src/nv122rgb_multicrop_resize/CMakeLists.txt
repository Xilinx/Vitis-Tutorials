project(nv122rgb_multicrop_resize)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

#MESSAGE(${CMAKE_CURRENT_SOURCE_DIR})
#
#set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
#
#set (CMAKE_SYSROOT $ENV{SDKTARGETSYSROOT} CACHE PATH "Path to sysroot")
#if (CMAKE_SYSROOT)
#    set(ENV{PKG_CONFIG_SYSROOT_DIR} ${CMAKE_SYSROOT})
#    set(ENV{PKG_CONFIG_PATH} ${CMAKE_SYSROOT}/usr/lib/pkgconfig)
#endif()

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

add_executable(nv12_multicrop_resize_test
        host.cpp
        xcl2.cpp
        xcl2.hpp)

include_directories(nv12_multicrop_resize_test
        ${OpenCV_INCLUDE_DIRS}
        ${XRT_INCLUDE_DIRS})

target_link_libraries(nv12_multicrop_resize_test
        ${OpenCV_LIBRARIES}
        ${XRT_LIBS}
)


