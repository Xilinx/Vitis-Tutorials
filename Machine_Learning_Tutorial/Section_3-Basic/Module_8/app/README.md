## Overview
---
In Module_8, we wanted to use high resolution video input to demonstrate the kernel's acceleration performance. In this case, the input video is set to 2304x1296 and the bottleneck should obviously be color conversion and resizing. So before you start experimenting, please make sure that the USB camera you are using can be set to that resolution.

---
## HLS kernel

You can find the encapsulation of the HLS kernel in xcl2.hpp, named xcl::XilinxOcl, which is used to find cl::device, create cl::kernel, context,cl::kernel, cl::Commandqueue, cl::buffer, etc.

In the structure V4l2Capture, use the methods provided by the structure XilinxOcl to do this. Use the HLS kernel in "read_images_with_kernel()" to conduct Pre-processing. Refer to the source code in V4l2Capture.cpp for details.

## Hardware
dpu.xclbin located at /mnt/dpu.xclbin, the hardware consisof two kernels.
Kernel: pre_processor
Kernel: dpu_xrt_top

- pre_processor
For this kernel, we need to prepare 2 buffers to read the output data. One is the full image according to the input resolution you set and the other one is resized. In this case, it's set to 480*360, which is required by the refindet model.
- dpu_xrt_top
Using the API of the vitis ai library to implement the DPU encapsulation, we only need to focus on the preparation of the input image, and the development of the upper level application.

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
