cmake_minimum_required(VERSION 3.0)

get_filename_component(BASENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${BASENAME})


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "-Wall")

find_package(OpenCV REQUIRED NO_CMAKE_FIND_ROOT_PATH)

if(OpenCV_FOUND)

    include_directories(${OpenCV_INCLUDE_DIRS})
    message(STATUS "    OpenCV library status:")
    message(STATUS "    version ${OpenCV_VERSION}")
    message(STATUS "    libraries: ${OpenCV_LIBS}")
    message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
endif()

find_package(Glog REQUIRED)
if(Glog_FOUND)
    include_directories(${Glog_INCLUDE_DIRS})
    message(STATUS "    Glog library status:")
    message(STATUS "    version ${Glog_VERSION}")
    message(STATUS "    libraries: ${Glog_LIBS}")
    message(STATUS "    include path: ${Glog_INCLUDE_DIRS}")    
endif()
include_directories("include")
set(MY_SRCS src/V4l2Access.cpp
            src/V4l2Capture.cpp
            src/V4l2Device.cpp
            src/V4l2MmapDevice.cpp    
            src/V4l2Output.cpp  
            src/V4l2ReadWriteDevice.cpp 


)
add_library(my_v4l2 SHARED ${MY_SRCS})
target_link_libraries(my_v4l2 ${OpenCV_LIBS} glog)
add_executable(test test/test.cpp)
target_link_libraries(test my_v4l2  ${OpenCV_LIBS} glog)
install(TARGETS my_v4l2 
        LIBRARY 
            DESTINATION /usr/lib
        )
install(FILES include/V4l2Access.hpp  include/V4l2Capture.hpp include/V4l2ReadWriteDevice.hpp include/V4l2Device.hpp include/V4l2MmapDevice.hpp include/V4l2Output.hpp 
    DESTINATION /usr/include
)
