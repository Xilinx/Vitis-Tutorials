# Section_3-Module_6


***Note***: You must install the Vitis AI v1.1 package before you compile and run the application.
This example suite, targeting the Vitis AI Library and Vitis acceleration kernels, illustrates how to use Vitis AI Library to run the neural network on DPU, and how to use the HLS kernel to accelerate the app.

Some system level functions:
- working with DRM on ZynqMP
    -Using the DRM to display

- V4L2 initialization and control
    - Streaming video capture from V4L2, such as USB camera.


## Program Prerequiste
This design targets ZCU104 Vitis platform. About how to prepare the develop environmet you can refer to
[quick start](https://github.com/Xilinx/Vitis-AI/tree/master/Vitis-AI-Library#quick-start-for-edge)

## Setting up the software build environment
---

- To setup the software environment, first to install the my_V4L2 libs and headers.

```
cd ${MODULE_6}/V4L2/
mkdir build
cd build
cmake ..
make
make install
```

- Test the sample to check whether the my_v4l2s lirary can work. After the test example works correctly you will find two JPEG file generated under your execute directory.
```
./test
```

## Building the main appliction (usb_input_multi_threads_refinedet)
- Build the application

```
cd ${MODULE_6}/app/
sh build.sh

```
- Before run the application you need to run the power patch command on board.
```
irps5401
```


``` 
- DRM mode with ARM pre-processing:
   /etc/init.d/weston stop; 
  ./usb_input_multi_threads_refinedet_drm refinedet_pruned_0_8 0 -t 3
  
refinedet_pruned_0_8: The model used
0: Camera input
-t 3: thread number 3

