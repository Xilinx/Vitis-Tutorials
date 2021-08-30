cmake_minimum_required(VERSION 3.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

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

# Add board-specific settings
if (BOARD MATCHES "Ultra96")
    message(STATUS "Setting board definition to Ultra96")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOARD_ULTRA96")
else()
    message(STATUS "Using standard board definition")
endif()

find_package(OpenCL REQUIRED)
find_package(XRT REQUIRED)

add_library(xilinx_ocl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/xilinx_ocl.cpp
)

target_include_directories(xilinx_ocl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${XRT_INCLUDE_DIRS}
    ${OpenCL_INCLUDE_DIRS}
)

target_link_libraries(xilinx_ocl PRIVATE
    ${XRT_LIBS}
    ${OpenCL_LIBRARIES}
    xml2
)
