# 3.6 Usb Camera Input and Multi-Threads base on Vitis AI Library

***Note***:The programs in Module_6 are compiled in cross compile environment. Assumed that you have set up the cross compile environment, for the detail you could refer to [Module3](../Module_3/README.md).

Some system level functions:
- working with DRM on ZynqMP
    -Using the DRM to display

- V4L2 initialization and control
    - Streaming video capture from V4L2, such as USB camera.

The directory structure and brief explanations as below:
```bash
├── app
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
└── CMakeLists.txt

```
## Program Prerequisites
This design targets the ZCU104 Vitis platform. You can refer to the [quick start](https://github.com/Xilinx/Vitis-AI/tree/master/demo/Vitis-AI-Library#quick-start-for-edge) guide to learn how to prepare the development environment.


## Setting Up the cross-compile environment and Build app.
---

- After cloning the project, use the following commands to compile the applications. Regard the sdk path to be ${SDK_PATH}.


```
source ${SDK_PATH}/environment-setup-aarch64-xilinx-linux
cd ${Section_3-Basic}/Module_6/
sh build_app.sh
```
Copy the executable file to board.

```
scp ${Section_3-Basic}/Module_6/test/usb_input_multi_threads_refinedet_drm root@$[IP_OF_BOARD]:/home/root
scp ${Section_3-Basic}/Module_6/install/libmy_v4l2s.so root@$[IP_OF_BOARD]:/usr/lib
```
Running zynqmp_dpu_optimize.sh on board to optimize the board setting.
```
#cd ~/dpu_sw_optimize/zynqmp/
#./zynqmp_dpu_optimize.sh
```
The script runs automatically after the board boots up with the official image
- Test the libmy_v4l2s.so library to see if it works properly with the test example. When the test example works, you will notice that two JPEG files are generated in your execution directory.
```
./test_v4l2
```


- Run the application.
 Before you run, it's needed to stop the weston service to use drm display mode for the application.
```
  ./usb_input_multi_threads_refinedet_drm refinedet_pruned_0_8 0 -t 3
```
- DESCRIPTION
```
    - refinedet_pruned_0_8: The model used;
    - 0                   :  Camera input;
    - -t 3                :  thread number 3;

```


Learn More about the work pipeline:[work_pipeline](app/README.md)
<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
