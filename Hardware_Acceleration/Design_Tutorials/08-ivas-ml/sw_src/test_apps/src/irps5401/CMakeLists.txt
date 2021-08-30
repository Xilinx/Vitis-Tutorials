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

add_executable(irps5401
    irps5401.c
)

install (TARGETS irps5401 DESTINATION bin)
