#!/bin/bash
g++ -std=c++11 -o test test.cpp -I../include -I/usr/include/xrt ../src/xcl2.cpp -lxrt_core -lxrt_coreutil -lxilinxopencl