# 3.7 Acceleration with HLS kernel
---

***Version: Vitis 2020.2+Vitis AI 1.3***

**_Note_**: You need to use the new image you generated in Section_3-Module_7. After the OS boot succesfully, you can refer to [quick_start](https://github.com/Xilinx/Vitis-AI/tree/master/demo/Vitis-AI-Library#quick-start-for-edge) guide to learn how to prepare the development evironment.
This example suite, for the Vitis AI Library and Vitis Accelerated Kernel, shows how to use the Vitis AI Library runs neural networks on DPUs and how to use the HLS kernel to speed up pre/... Postprocessing. About how to immigrate from OpenCV to HLS, please refer to [app/README.md](app/README.md)

Some system level functions:

- working with DRM on ZynqMP

  - Using the DRM to display

- V4L2 initialization and control
  - Streaming video capture from V4L2, such as USB camera.

The directory structure and brief explanations as below:

```bash
├── app
│   ├── include                    #headers directory of work pipeline
│   │   ├── decodethread.hpp       #header file of decode thread
│   │   ├── dputhread.hpp          #header file of dpu threadd
│   │   ├── filter.hpp             #dpu filer header 
│   │   ├── frameinfo.hpp          #important data structure 
│   │   ├── guithread.hpp          #display thread headerfile
│   │   ├── mythread.hpp           #thread pool 
│   │   └── sortthread.hpp         #sort thread header
│   ├── README.md                  #instruction of how to migrate from OpenCV to HLS
│   ├── src                        #implementation of the work threads
│   │   ├── decodethread.cpp
│   │   ├── dputhread.cpp
│   │   ├── guithread.cpp
│   │   ├── mythread.cpp
│   │   └── sortthread.cpp
│   └── test
│       └── usb_input_multi_threads_xcl_refinedet.cpp #main application
├── kernel                            #HLS kernel work directory
│   ├── build                         #directory of config file
│   │   ├── dpu_conf.vh               #dpu IP config file
│   │   ├── Makefile                  #The Makefile used to replace the file in ${DPU_TRD}
│   │   ├── preprocessor_config.ini   #config file of pre_processor
│   │   └── prj_config_104_2dpu       #Config file required be Makefile for IP integration
│   ├── flash_sd_card.sh              #Script to generate the OS image
│   └── src                           #HLS kernel source codes directory
│       ├── pre_processor.cpp   
│       └── pre_processor.h
├── my_V4l2s                          # V4l2 libraries work directory to get image data from USB camera
│   ├── CMakeLists.txt                # cmake file to build libs
│   ├── include                       
│   │   ├── dpdrm.hpp                 #Encapsulate drm library
│   │   ├── line_exception.hpp        #Encapsulate throw error
│   │   ├── V4l2Access.hpp            #Interface to access the camera device
│   │   ├── V4l2Capture.hpp           #Methods to get the image
│   │   ├── V4l2Device.hpp            #Device capablity 
│   │   ├── V4l2MmapDevice.hpp        #Device type
│   │   ├── V4l2Output.hpp            #asistant class
│   │   ├── V4l2ReadWriteDevice.hpp   #Device type
│   │   └── xcl2.hpp                  #Encapsulate the kernel usage
│   ├── src
│   │   ├── V4l2Access.cpp
│   │   ├── V4l2Capture.cpp
│   │   ├── V4l2Device.cpp
│   │   ├── V4l2MmapDevice.cpp
│   │   ├── V4l2Output.cpp
│   │   ├── V4l2ReadWriteDevice.cpp
│   │   └── xcl2.cpp
│   └── test
│       ├── build.sh                #compile script
│       ├── test_hls_kernel.cpp     #test case for hls kernel
│       └── test.yuv                #test data
└── README.md
└── CMakeLists.txt
└── build_app_arm.sh                # Build application with hls kernel
└── build_app_hls.sh                # Build application with arm preprocessing

```



## Pre-Requirement
- Hardware required:
  - ZCU104 evaluation board
  - Micro-USB cable, connect to lattop for the terminal emulator.
  - SD card to burn the OS image.
- Software required:
- Vitis 2020.2 [Vitis Core Development Kit](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis/2020-2.html) install in server.
- [Silicon Labs quad CP210x USB-to-UART bridge driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) install in laptop.
- Serial terminal emulator e.g. teraterm, Mobaxterm install in laptop
- [XRT 2020.2](https://github.com/Xilinx/XRT/tree/2020.2) install in laptop
- [zcu104 base platform](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/master/Xilinx_Official_Platforms/zcu104_base) install in server
- [Vitis AI runtime package](https://www.xilinx.com/bin/public/openDownload?filename=vitis-ai-runtime-1.3.tar.gz) base on VAI1.3
- [Vitis AI model packages ](https://github.com/Xilinx/Vitis-AI/tree/master/Vitis-AI-Library#quick-start-for-edge)for ZCU104
- [dpu_sw_optimize.tar.gz](https://github.com/Xilinx/Vitis-AI/blob/master/DPU-TRD/app/dpu_sw_optimize.tar.gz) Running zynqmp_dpu_optimize.sh to optimize the board setting
---

## Workflow Overview
- Step 1: Create Base Platform
- Step 2: DPU and HLS kernel integration
- Step 3: Cross Compiler Environtment Set up and Build application
- Step 4: Board Development Setting
- Step 5: Run the Application.

## Step 1:Create Base Platform 
In this step you can choose build a platform form scratch, the detail tutorial you can refer to [Vitis_Platform_Creation](../../../../Vitis_Platform_Creation/README.md).

The other way is to use the Official Platofrm code of [Xilinx_Official_Platform](https://github.com/Xilinx/Vitis_Embedded_Platform_Source), in which the platform build process is entirely scripted. Note that as this platform build process involves cross-compiling Linux, build of the platform is supported on Linux environments only (although it is possible to build inside a VM or Docker container).

Also note that the default PetaLinux configuration uses local scratchpad areas. This will not work if you are building on a networked file system; Yocto will error out. Update PetaLinux to change the build area to a locally-mounted hard drive (most Xilinx internal network servers have a /scratch or /tmp area for this purpose).

After cloning the platform source, and with both Vivado and PetaLinux set up, run ***make*** from the top-level platform directory. In this case we choose ZCU104_Base platform.



### Step 2:  DPU and HLS kernel integration

-  Clone the repository of Vitis-AI and vitis_library, the preprocessor kernel implements base on vitis_library, Befor you build the haredware design, you need to clone the vitis_library under the ***${DPU-TRD}/prj/Vitis*** directory.
```
$ git clone https://github.com/Xilinx/Vitis-AI.git
$ cd Vitis-AI/dsa/DPU-TRD/prj/Vitis/
$ git clone https://github.com/Xilinx/Vitis_Libraries.git


```
- download the [mpsoc common system](https://www.xilinx.com/member/forms/download/xef.html?filename=xilinx-zynqmp-common-v2020.1.tar.gz), and unzip the files.

```
$ tar -xvzf  xilinx-zynqmp-common-v2020.1.tar.gz
$ cd xilinx-zynqmp-common-v2020.1
$ gzip -d rootfs.ext4.gz
```

- copy the kernel source code and config files below to DPU-TRD work directory

```
$ cp ${Module_7}/kernel/build/Makefile vitis-ai-staging/dsa/DPU-TRD/prj/Vitis
$ cp ${Module_7}/kernel/build/preprocessor_config.ini vitis-ai-staging/dsa/DPU-TRD/prj/Vitis
$ cp ${Module_7}/kernel/build/dpu_conf.vh vitis-ai-staging/dsa/DPU-TRD/prj/Vitis
$ cp ${Module_7}/kernel/src/pre_processor.cpp vitis-ai-staging/dsa/DPU-TRD/prj/Vitis
$ cp ${Module_7}/kernel/src/pre_processor.h vitis-ai-staging/dsa/DPU-TRD/prj/Vitis
$ cp ${Module_7}/kernel/config_file/prj_config_104_2dpu vitis-ai-staging/dsa/DPU-TRD/prj/Vitis/config_file
```

- Step4: Run the below commands to start IP integration and wait for it to complete.

```
export SDX_PLATFORM=<path-to-platform>
export EDGE_COMMON_SW=<path-to-edge_common_sw>
make KERNEL=DPU DEVICE=ZCU104
```
There are two methods to set up the target environment
1. Replace the BOOT.BIN and the dpu.xclbin on target.
```
$ scp vitis-ai-staging/dsa/DPU-TRD/prj/Vitis/binary_container_1/BOOT.BIN root@<zcu104 board ip>:/mnt/sd-mmcblk0p1/
$ scp vitis-ai-staging/dsa/DPU-TRD/prj/Vitis/binary_container_1/dpu.xclbin root@<zcu104 board ip>:/mnt/sd-mmcblk0p1/


```
2. Burn the SD card with the image generated in ***${Vitis-AI}/dsa/DPU-TRD/prj/Vitis/binary_container_1/sd_card.img***

## Step 3: Cross Compiler Environtment Set up and Build application
---
- Install the SDK package and the Vitis AI cross compile package

```
$sh sdk-2020.2.0.0.sh
$tar -xvzf vitis_ai_2020.2-r1.3.0.tar.gz -C <path-to-sysroot>/
```
- After installing the sdk package, we use the script to set up the cross-compile environemnt by using the following commands.
```
$ source <full_path_of_sdk>/environment-setup-aarch64-xilinx-linux
$ cd ${Section_3-Basic}/Module_7/
$ sh build_app_hls.sh
```
- Copy the test yuv file to target.

```
$ scp test.yuv root@<zcu104 ip>:/home/root
```
- Copy the library and the applications to target board
```
$ scp ${Module_7}/install/libhls_v4l2s.so root@Board_IP:/home/root
$ scp ${Module_7}/test/test_hls_kernel  root@Board_IP:/home/root
$ scp ${Module_7}/test/usb_input_multi_threads_refinedet_hls_drm  root@Board_IP:/home/root


```

### Step 4: Run the Application on Target

- Test the HLS kernel to see if it works properly with the test example. When the test example works, you will notice that two PNG files are generated in your execution directory. 
```
# chmod +x test_hls_kernel
# ./test_hls_kernel
```
- In this case we use refinedet model to show the body detecition.
```
  ./usb_input_multi_threads_refinedet_hls_drm refinedet_pruned_0_8 0 -t 3
```
- DESCRIPTION
```
    - refinedet_pruned_0_8: The model used;
    - 0                   :  Camera input;
    - -t 3                :  thread number 3;

```
- Option
  If you are interested in the gap between the processing performance of hls kenel and the processing performance of ARM. You can also try the same application by preprocessing on the arm side. 
  - Build the application
```
$ build_app_arm.sh

```

- Copy the library and the applications to target board
```
$ scp ${Module_7}/install/libhls_v4l2s.so root@Board_IP:/home/root
$ scp ${Module_7}/test/test_hls_kernel  root@Board_IP:/home/root
$ scp ${Module_7}/test/usb_input_multi_threads_refinedet_hls_drm  root@Board_IP:/home/root


```

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
