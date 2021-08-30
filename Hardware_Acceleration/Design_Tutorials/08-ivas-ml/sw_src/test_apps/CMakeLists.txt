cmake_minimum_required(VERSION 3.0)

# Find gstreamer required packages
find_package(PkgConfig)

set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (CMAKE_SYSROOT $ENV{SDKTARGETSYSROOT} CACHE PATH "Path to sysroot")
if (CMAKE_SYSROOT)
  set(ENV{PKG_CONFIG_SYSROOT_DIR} ${CMAKE_SYSROOT})
  set(ENV{PKG_CONFIG_PATH} ${CMAKE_SYSROOT}/usr/lib/pkgconfig)
endif()

set(GStreamer_FIND_VERSION "1.8.3")

find_package (GLIB REQUIRED)
find_package (GStreamer REQUIRED)
find_package (XRT REQUIRED)

string(REPLACE "." ";" GST_VERSION_LIST ${GSTREAMER_VERSION})
list (GET GST_VERSION_LIST 0 GST_VERSION_MAJOR)
list (GET GST_VERSION_LIST 1 GST_VERSION_MINOR)
list (GET GST_VERSION_LIST 2 GST_VERSION_MICRO)
set(GST_API_VERSION "1.0")
set(GST_SO_VERSION "0")
MATH(EXPR GST_LIB_VERSION "${GST_VERSION_MINOR}*100+${GST_VERSION_MICRO}")
# set the lib version compatible with the libtool versioning
set(GST_LIB_VERSION "${GST_SO_VERSION}.${GST_LIB_VERSION}.0")

set(GSTXCL_DEFINITIONS "-D__FORTIFY_SOURCE=2")
set(GSTXCL_LIBRARY_INCLUDEDIR "${PROJECT_SOURCE_DIR}/gst-libs")
set(GSTXCL_INSTALL_INCLUDEDIR "include/gstreamer-${GST_API_VERSION}")
set(GSTXCL_LIB_INSTALL_DIR "lib")
set(GSTXCL_PLUGIN_INSTALL_DIR "${GSTXCL_LIB_INSTALL_DIR}/gstreamer-${GST_API_VERSION}")
set(XRTUTILS_LIBRARY_INCLUDEDIR "${PROJECT_SOURCE_DIR}/ext/xrtutils")

set(GSTXCL_FLAGS "-Wall -O2 -fstack-protector-strong")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(GSTXCL_FLAGS "${GSTXCL_FLAGS} -O0 -ggdb3")
elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  set(GSTXCL_FLAGS "${GSTXCL_FLAGS} -ggdb3")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GSTXCL_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GSTXCL_FLAGS}")

# Setup common environment
include_directories(
  ${GLIB_INCLUDE_DIRS}
  ${GSTREAMER_INCLUDE_DIRS}
)

add_definitions(
  ${GSTREAMER_DEFINITIONS}
  ${GSTXCL_DEFINITIONS}
)

add_subdirectory(xilinx_ocl)
add_subdirectory(dpu_utils)
add_subdirectory(src)
