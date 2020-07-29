# Acceleration with HLS kernel


***Note***: You need to use the new image you generated in Section_3-Module_7. After the OS boot succesfully, you can refer to [quick_start](https://github.com/Xilinx/Vitis-AI/tree/master/Vitis-AI-Library#quick-start-for-edge) guide to learn how to prepare the development evironment.
This example suite, for the Vitis AI Library and Vitis Accelerated Kernel, shows how to use the Vitis AI Library runs neural networks on DPUs and how to use the HLS kernel to speed up pre/... Postprocessing. You can find a description of the pipeline design of the program in ${app}/README.md

Some system level functions:
- working with DRM on ZynqMP
    - Using the DRM to display

- V4L2 initialization and control
    - Streaming video capture from V4L2, such as USB camera.

The directory structure and brief explanations as below:
```bash
.
??? app
?   ??? build.sh              #compile script
?   ??? include               #headers directory of work pipeline
?   ?   ??? decodethread.hpp  #decode thread 
?   ?   ??? dputhread.hpp     #dpu encapsulation thread
?   ?   ??? filter.hpp        #specific model in filter
?   ?   ??? frameinfo.hpp     #important data structure in pipeline
?   ?   ??? guithread.hpp     #display thread using drm
?   ?   ??? mythread.hpp      #thread pool 
?   ?   ??? sortthread.hpp    #sorting frame thread
?   ??? README.md
?   ??? src
?   ?   ??? decodethread.cpp
?   ?   ??? dputhread.cpp
?   ?   ??? guithread.cpp
?   ?   ??? mythread.cpp
?   ?   ??? sortthread.cpp
?   ??? test    
?       ??? usb_input_multi_threads_xcl_refinedet.cpp #main application
??? kernel
?   ??? build
?   ?   ??? dpu_conf.vh
?   ?   ??? Makefile
?   ?   ??? preprocessor_config.ini
?   ?   ??? prj_config_104_2dpu
?   ??? flash_sd_card.sh
?   ??? src
?       ??? pre_processor.cpp
?       ??? pre_processor.h
??? my_V4l2s
?   ??? CMakeLists.txt
?   ??? include
?   ?   ??? dpdrm.hpp
?   ?   ??? line_exception.hpp
?   ?   ??? V4l2Access.hpp
?   ?   ??? V4l2Capture.hpp
?   ?   ??? V4l2Device.hpp
?   ?   ??? V4l2MmapDevice.hpp
?   ?   ??? V4l2Output.hpp
?   ?   ??? V4l2ReadWriteDevice.hpp
?   ?   ??? xcl2.hpp
?   ??? src
?   ?   ??? V4l2Access.cpp
?   ?   ??? V4l2Capture.cpp
?   ?   ??? V4l2Device.cpp
?   ?   ??? V4l2MmapDevice.cpp
?   ?   ??? V4l2Output.cpp
?   ?   ??? V4l2ReadWriteDevice.cpp
?   ?   ??? xcl2.cpp
?   ??? test
?       ??? build.sh
?       ??? test_hls_kernel.cpp
?       ??? test.yuv
??? README.md

```

## Platform integrated

- Hardware required:
  - ZCU104 evaluation board
  - Micro-USB cable, connect to lattop for the terminal emulator.
  - SD card to burn the OS image.
- Software required:
- Vitis 2019.2 [Vitis Core Development Kit](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis/2019-2.html) install in server.
- [Silicon Labs quad CP210x USB-to-UART bridge driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) install in laptop.
- Serial terminal emulator e.g. teraterm, Mobaxterm install in laptop
- [XRT 2019.2](https://github.com/Xilinx/XRT/tree/2019.2) install in laptop
- [zcu104 dpu platform](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/2019.2/Xilinx_Official_Platforms/zcu104_dpu) install in server
- [Vitis AI runtime package](https://xilinx-ax-dl.entitlenow.com/dl/ul/2020/04/27/R210316037/vitis-ai-runtime-1.1.2.tar.gz/73b825a7fd3fb54fa9693eeb634ac491/5F0EBBB3?akdm=0&filename=vitis-ai-runtime-1.1.2.tar.gz)
- [Vitis AI model packages for ZCU104](https://xilinx-ax-dl.entitlenow.com/dl/ul/2020/03/19/R210290235/vitis_ai_model_ZCU104_2019.2-r1.1.0.deb/960da8e4ed428df080602e9ba62e09f3/5F0EBB60?akdm=0&filename=vitis_ai_model_ZCU104_2019.2-r1.1.0.deb)


---
## [Building the Vitis Platform](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/2019.2/Xilinx_Official_Platforms/zcu104_dpu)

Last Tested Vivado Release: 2019.2
The platform build process is entirely scripted. Note that as this platform build process involves cross-compiling Linux, build of the platform is supported on Linux environments only (although it is possible to build inside a VM or Docker container).

Also note that the default PetaLinux configuration uses local scratchpad areas. This will not work if you are building on a networked file system; Yocto will error out. Update PetaLinux to change the build area to a locally-mounted hard drive (most Xilinx internal network servers have a /scratch or /tmp area for this purpose).

After cloning the platform source, and with both Vivado and PetaLinux set up, run make from the top-level platform directory.

Note that by default this Makefile will install the platform to "platform_repo/zcu104_dpu/export/zcu104_dpu/"

### Installing the Yocto SDK
A bundled Yocto SDK "sysroot" is not available with this package by default. To build non-trivial Linux software for this platform sysroot need to be built and installed. This can be done with command "make peta_sysroot" It is installed to "platform_repo/sysroot" once the build completes.

To cross-compile against this platform from the command line, source the environment-setup-aarch64-xilinx-linux script to set up your environment (cross compiler, build tools, libraries, etc).

### Build instructions
This packages comes with sources to generate hardware specification file (xsa) from Vivado, petalinux sources to generate the image.ub and platform sources to generate the Vitis platform.

Build platform from scratch: make all

Build a platform without modifying hardware: make petalinux_proj XSA_DIR= make pfm XSA_DIR=
```
example:
	make petalinux_proj XSA_DIR=/home/user/zcu104_dpu/vivado
	make pfm /home/user/zcu104_dpu/vivado
```
---
### IP Integrated
- Step1: Use below commands to clone the Vitis-AI from github project.
```
git clone -b v1.1 git@github.com:Xilinx/Vitis-AI.git
```
- Step2: copy the files below to under the directory "${Vitis-AI/DPU-TRD}DPU-TRD/prj/Vitis"
```
cp ${Module_7}/kernel/build/* ${Vitis-AI/DPU-TRD}DPU-TRD/prj/Vitis
cp ${Module_7}/kernel/src/* ${Vitis-AI/DPU-TRD}DPU-TRD/prj/Vitis
```
- Step3: Run the below commands to start IP integration and wait for it to complete.
```
cd ${Module_7}/kernel/src/* ${Vitis-AI/DPU-TRD}DPU-TRD/prj/Vitis
make -j
```
- Step4: Generate the system image
```
cp ${Module_7}/kernel/flash_sd_card.sh  ${Vitis-AI}/DPU-TRD/prj/Vitis/binary_container_1/sd_card
cd ${Module_7}/kernel/flash_sd_card.sh ${Vitis-AI}/DPU-TRD/prj/Vitis/binary_container_1/sd_card
sudo ./flash_sd_card.sh
```
- Step5: Use etcher or other tools to burn the image to 16GB sd card.
- Step6: Set the Mode to SD card

    ```
    Rev 1.0:  SW6[4:1] - off, off, off, on
    Rev D2:   SW6[4:1] - on, off, on, off
    ```
### Board development setting.

- Copy the model file, glog and runtime package to ZCU104 Board.
```
scp glog-0.4.0-Linux.tar.gz root@IP_OF_BOARD:~/
tar -xzvf glog-0.4.0-Linux.tar.gz --strip-components=1 -C /usr

tar -xvzf vitis-ai-runtime-1.1.2.tar.gz
dpkg â€“i --force-all libunilog-1.1.0-Linux-build<xx>.deb
dpkg â€“i libxir-1.1.0-Linux-build<xx>.deb
dpkg â€“i libvart-1.1.0-Linux-build<xx>.deb


dpkg -i vitis_ai_model_ZCU104_2019.2-r1.1.0.deb

```
---

## Setting up the software build environment


- To set up the software environment, first install the lib and header files associated with my_V4L2. Assume that you have copied Module8 into ZCU104.

```
cd ${Module_7}/myV4L2s/
mkdir build
cd build
cmake ..
make
make install
```

- Test the HLS kernel to see if it works properly with the test example. When the test example works, you will notice that two PNG files are generated in your execution directory.
```
cd ${Module_8/my_V4l2s/test}
sh build.sh
./test_hls_kernel
```

## Build the Applictions
- Use the following command to build the application. This "build.sh " script generates two executables, one using ARM for color conversion and resize, and the other using the HLS kernel to speed up color conversion and image resizing.

```
cd ${Module_8}/app/
sh build.sh

```
- You need to run the power patch command on the board before you can run the application, otherwise the application may cause the board to crash.
```
irps5401
```

-Run this two applications
  - Use the below command to close the weston service:
```
   /etc/init.d/weston stop
```
 - Use the following command to run this application. Please note that this application will use ARM for color conversion and image resizing.


```
  ./usb_input_multi_threads_arm_refinedet_drm refinedet_pruned_0_8 0 -t 3

```
- Use the following command to run this application. Please note that this application will use "preprocess" HLS kernel for color conversion and image resizing.

```
 ./usb_input_multi_threads_kernel_refinedet_drm refinedet_pruned_0_8 0 -t 3
```

- DESCRIPTION

    - refinedet_pruned_0_8: The model used;
    - 0                   :  Camera input;
    - -t 3                :  thread number 3;

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
