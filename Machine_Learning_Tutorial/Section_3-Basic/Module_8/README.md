# Section_3-Module_8


***Note***: You need to use the new image you generated in Section_4-Module_7. After the OS boot succesfully, you can refer to [quick_start](https://github.com/Xilinx/Vitis-AI/tree/master/Vitis-AI-Library#quick-start-for-edge) guide to learn how to prepare the development evironment.
This example suite, for the Vitis AI Library and Vitis Accelerated Kernel, shows how to use the Vitis AI Library runs neural networks on DPUs and how to use the HLS kernel to speed up pre/... Postprocessing. You can find a description of the pipeline design of the program in ${app}/README.md

Some system level functions:
- working with DRM on ZynqMP
    -Using the DRM to display

- V4L2 initialization and control
    - Streaming video capture from V4L2, such as USB camera. 


## Program Prerequisties
This design targets the ZCU104 Vitis platform. You can refer to [quick start](https://github.com/Xilinx/Vitis-AI/tree/master/Vitis-AI-Library#quick-start-for-edge) guid to prepare the software envirnment.

## Setting up the software build environment
---

- To set up the software environment, first install the lib and header files associated with my_V4L2. Assume that you have copied Module8 into ZCU104.

```
cd ${Module_8}/myV4L2s/
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
