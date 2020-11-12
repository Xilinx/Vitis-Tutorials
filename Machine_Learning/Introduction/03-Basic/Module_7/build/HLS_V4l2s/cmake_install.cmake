# Install script for directory: /scratch/dachang/Vitis-In-Depth-Tutorial/Machine_Learning/Introduction/03-Basic/Module_7/HLS_V4l2s

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/lib/libhls_v4l2s.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/lib/libhls_v4l2s.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/lib/libhls_v4l2s.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/lib/libhls_v4l2s.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/lib" TYPE SHARED_LIBRARY FILES "/scratch/dachang/Vitis-In-Depth-Tutorial/Machine_Learning/Introduction/03-Basic/Module_7/build/HLS_V4l2s/libhls_v4l2s.so")
  if(EXISTS "$ENV{DESTDIR}/usr/lib/libhls_v4l2s.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/lib/libhls_v4l2s.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/scratch/dachang/vai1.3/petalinux/sysroots/x86_64-petalinux-linux/usr/bin/aarch64-xilinx-linux/aarch64-xilinx-linux-strip" "$ENV{DESTDIR}/usr/lib/libhls_v4l2s.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/include/hlsV4l2Access.hpp;/usr/include/line_exception.hpp;/usr/include/xcl2.hpp;/usr/include/hlsV4l2Capture.hpp;/usr/include/hlsV4l2ReadWriteDevice.hpp;/usr/include/hlsV4l2Device.hpp;/usr/include/hlsV4l2MmapDevice.hpp;/usr/include/hlsV4l2Output.hpp")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/include" TYPE FILE FILES
    "/scratch/dachang/Vitis-In-Depth-Tutorial/Machine_Learning/Introduction/03-Basic/Module_7/HLS_V4l2s/include/hlsV4l2Access.hpp"
    "/scratch/dachang/Vitis-In-Depth-Tutorial/Machine_Learning/Introduction/03-Basic/Module_7/HLS_V4l2s/include/line_exception.hpp"
    "/scratch/dachang/Vitis-In-Depth-Tutorial/Machine_Learning/Introduction/03-Basic/Module_7/HLS_V4l2s/include/xcl2.hpp"
    "/scratch/dachang/Vitis-In-Depth-Tutorial/Machine_Learning/Introduction/03-Basic/Module_7/HLS_V4l2s/include/hlsV4l2Capture.hpp"
    "/scratch/dachang/Vitis-In-Depth-Tutorial/Machine_Learning/Introduction/03-Basic/Module_7/HLS_V4l2s/include/hlsV4l2ReadWriteDevice.hpp"
    "/scratch/dachang/Vitis-In-Depth-Tutorial/Machine_Learning/Introduction/03-Basic/Module_7/HLS_V4l2s/include/hlsV4l2Device.hpp"
    "/scratch/dachang/Vitis-In-Depth-Tutorial/Machine_Learning/Introduction/03-Basic/Module_7/HLS_V4l2s/include/hlsV4l2MmapDevice.hpp"
    "/scratch/dachang/Vitis-In-Depth-Tutorial/Machine_Learning/Introduction/03-Basic/Module_7/HLS_V4l2s/include/hlsV4l2Output.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/include/vitis/ai/dpdrm.hpp")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/include/vitis/ai" TYPE FILE FILES "/scratch/dachang/Vitis-In-Depth-Tutorial/Machine_Learning/Introduction/03-Basic/Module_7/HLS_V4l2s/include/dpdrm.hpp")
endif()

