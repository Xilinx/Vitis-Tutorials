# 3.6 Usb Camera Input and Multi-Threads base on Vitis AI Library

***Note***: You must install the Vitis AI v1.1 package before you compile and run the application.
This example suite, for the Vitis AI Library and Vitis Accelerated Kernel, shows how to use the Vitis AI Library runs neural networks on DPUs and how to use the HLS kernel to speed up pre/... Postprocessing. You can refer to [pipeline instruction](app/README.md).

Some system level functions:
- working with DRM on ZynqMP
    -Using the DRM to display

- V4L2 initialization and control
    - Streaming video capture from V4L2, such as USB camera.

The directory structure and brief explanations as below:
```bash
├── app
│   ├── build.sh
│   ├── images
│   │   ├── in_depth_demo_data_flow.jpg
│   │   └── Solution_candidate.png
│   ├── include
│   │   ├── decodethread.hpp
│   │   ├── dputhread.hpp
│   │   ├── filter.hpp
│   │   ├── frameinfo.hpp
│   │   ├── guithread.hpp
│   │   ├── mythread.hpp
│   │   └── sortthread.hpp
│   ├── README.md
│   ├── src
│   │   ├── decodethread.cpp
│   │   ├── dputhread.cpp
│   │   ├── guithread.cpp
│   │   ├── mythread.cpp
│   │   └── sortthread.cpp
│   └── test
│       └── usb_input_multi_threads_refinedet.cpp 
├── myV4L2
│   ├── CMakeLists.txt
│   ├── include
│   │   ├── dpdrm.hpp
│   │   ├── V4l2Access.hpp
│   │   ├── V4l2Capture.hpp
│   │   ├── V4l2Device.hpp
│   │   ├── V4l2MmapDevice.hpp
│   │   ├── V4l2Output.hpp
│   │   └── V4l2ReadWriteDevice.hpp
│   ├── src
│   │   ├── V4l2Access.cpp
│   │   ├── V4l2Capture.cpp
│   │   ├── V4l2Device.cpp
│   │   ├── V4l2MmapDevice.cpp
│   │   ├── V4l2Output.cpp
│   │   └── V4l2ReadWriteDevice.cpp
│   └── test
│       └── test.cpp
└── README.md

```
## Program Prerequisites
This design targets the ZCU104 Vitis platform. You can refer to the [quick start](https://github.com/Xilinx/Vitis-AI/tree/master/Vitis-AI-Library#quick-start-for-edge) guide to learn how to prepare the development environment.


## Setting Up the Software Build Environment
---

- Use the following commands to setup the software environment. This installs the my_v4l2 libs and headers. Assume that you have copied Module6 into ZCU104.

```
cd ${MODULE_6}/myV4L2/
mkdir build
cd build
cmake ..
make
make install
```

- Test the libmy_v4l2s.so library to see if it works properly with the test example. When the test example works, you will notice that two JPEG files are generated in your execution directory.
```
./test
```

## Building the main application (usb_input_multi_threads_refinedet)
- Build the application

```
cd ${MODULE_6}/app/
sh build.sh

```
- Run the power patch
 There's a small bug in ZCU104, you need to run the following command to prevent board from crashing before you running the application.
```
irps5401
```

- Run the application.
 Before you run, it's needed to stop the weston service to use drm display mode for the application.
```

   /etc/init.d/weston stop;
  ./usb_input_multi_threads_refinedet_drm refinedet_pruned_0_8 0 -t 3
```
- DESCRIPTION

    - refinedet_pruned_0_8: The model used;
    - 0                   :  Camera input;
    - -t 3                :  thread number 3;

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
