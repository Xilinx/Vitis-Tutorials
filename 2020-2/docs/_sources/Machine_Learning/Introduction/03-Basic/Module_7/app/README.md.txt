## Migrate from OpenCV to HLS kernel
---
Use high resolution video input to demonstrate the kernel's acceleration performance. In this case, the input video is set to 2304x1296 and the bottleneck should obviously be color conversion and resizing. So before you start experimenting, please make sure that the USB camera you are using can be set to that resolution.

---
## Hardware

dpu.xclbin located at /mnt/dpu.xclbin, which contains two kernels.
Kernel: pre_processor
Kernel: dpu_xrt_top

- pre_processor
For this kernel, we need to prepare 2 buffers to read the output data. One is the full image according to the input resolution you set and the other one is resized. In this case, it's set to 480*360, which is required by the refindet model.
- dpu_xrt_top
Using the API of the vitis ai library to implement the DPU encapsulation, we only need to focus on the preparation of the input image, and the development of the upper level application.

- The directory struction is shown as below
```
.
├── build
│   ├── dpu_conf.vh                 #dpu config file
│   ├── Makefile                    #Makefile for integrated
│   ├── preprocessor_config.ini     #HLS kernel config file
│   └── prj_config_104_2dpu         #config file for integrated
├── flash_sd_card.sh                #script for generate the OS image
└── src                             #kernel source codes
    ├── pre_processor.cpp           #kernel implementation code
    └── pre_processor.h             #kernel header file

```

- Kernel function
Inspect the kernel function in [pre_processor.h](../kernel/src/pre_processor.h), the arguments description as below.


augument|description
---|---|
image_in|           The input buffer from host 
image_out|          the resize image(640*360) output buffer from device 
image_out_full|     The full image(1080p) output buffer from device
width_in|           The input image data width 3840
height_in|          The input image data height 1526
width_out|          The width of resize output image
height_out|         The height of resize output image


```c++
void pre_processor(ap_uint<AXI_WIDTH> *image_in, ap_uint<AXI_WIDTH> *image_out,
		ap_uint<AXI_WIDTH> *image_out_full, int width_in, int height_in,
		int width_out, int height_out);
```

## How to program with kernel pre-processor
For convient use the kernel, The methods of OpenCL are encapsulated in to class XilinxOcl, for the detail you can refer to [xcl2.hpp](../my_V4l2s/include/xcl2.hpp)

Take [test_hls_kernel.cpp](../my_V4l2s/test/test_hls_kernel.cpp) as an example.
- The first step is to prepare the input image data, by calling read_file_to_buf, the data buffer will be stored in input_img and the size of the input_img will be stored in yuv_size parameter. Prepare the two output buffer to receive the full size output data and resize output data from the device
```c++
    uint8_t *input_img;
    unsigned int resize_size = (OUT_RESIZE_WIDTH * OUT_RESIZE_HEIGHT * 3);
    unsigned int full_size = (OUT_WIDTH * OUT_HEIGHT * 3);
    size_t yuv_size;
    read_file_to_buf("test.yuv", &input_img, &yuv_size);
    uint8_t *out_buf_0 = (uint8_t *)malloc(resize_size);
    uint8_t *out_buf_1 = (uint8_t *)malloc(full_size);
```

- Second, create an instance of XilinxOcl initialize by the "dpu.xclbin" which is generated from IP integration. 

```c++
    xcl::XilinxOcl xocl;
    if (std::getenv("DPU_XCLBIN_PATH"))
    {
        xocl.initialize(std::getenv("DPU_XCLBIN_PATH"));
    }
    else
    {
        xocl.initialize("/mnt/dpu.xclbin");
    }
```
- Thirdly, get the CommandQueue and Kernel from the instance of XilinxOcl.

```c++
    cl::CommandQueue q = xocl.get_command_queue();
    cl::Kernel krnl = xocl.get_kernel("pre_processor");

```
- Fourth, create buffers on device side. 
```c++
// the input buffer on device side to store input yuv data
    auto imgToDevice = xocl.create_buffer(yuv_size, CL_MEM_READ_ONLY);
// the  resize buffer on device side
    auto resizeFromDevice = xocl.create_buffer(resize_size, CL_MEM_WRITE_ONLY);

// the full size buffer on device side
    auto fullFromDevice = xocl.create_buffer(full_size, CL_MEM_WRITE_ONLY);
```

- The next step is to set args to kernel functions. Please note there are two arguments, the full size output image width and height, have been set the initial values with 1920 and 1080. 
```c++
krnl.setArg(0,  imgToDevice);
krnl.setArg(1,  resizeFromDevice);
krnl.setArg(2,  fullFromDevice);
krnl.setArg(3,  IN_WIDTH);
krnl.setArg(4,  IN_HEIGHT);
krnl.setArg(5,  OUT_RESIZE_WIDTH);
krnl.setArg(6,  OUT_RESIZE_HEIGHT);

```
- At last, enqueue the task and run it, wait for the task to complete, and the results will be stored in out_buf_0 and out_buf_1.
```c++

printf("Enqueue Stage\n");
q.enqueueWriteBuffer(imgToDevice, CL_TRUE, 0, yuv_size, input_img);

printf("Task Stage\n");
q.enqueueTask(krnl, NULL, &event_sp);
printf("Wait Stage\n");
clWaitForEvents(1, (const cl_event *)&event_sp);


q.enqueueReadBuffer(resizeFromDevice, CL_TRUE, 0, resize_size, out_buf_0);
q.enqueueReadBuffer(fullFromDevice, CL_TRUE, 0, full_size, out_buf_1);

q.finish();
```
---
## How to migrate from OpenCV to HLS kernel.
The V4l2Capture class contains the method read_images_with_kernel() for color conversion and image resizing, which is implemented in another method read_images() using OpenCV.

```c++
        /**
         * @brief Using HLS kernel to decode the yuv data and do resize
         * 
         * @param readImage output parameters
         * @return int 
         */
        int read_images_with_kernel(std::vector<cv::Mat> &readImage);

                /**
         * @brief Read images from the device
         * 
         * @param readImage and output parameters to store the image data.
         * @return int 
         */
       int read_images(std::vector<cv::Mat>& readImage);

```
The constructor of class V4l2Capture will using the xclbin to initialized the xocl, the instance of class xcl::XilinxOcl. The detail definition of XilinxOcl you can refer to [xcl2.hpp](../my_V4l2s/include/xcl2.hpp)

```C++
V4l2Capture::V4l2Capture(V4l2Device *device) : V4l2Access(device)
{
    if (std::getenv("DPU_XCLBIN_PATH"))
    {
        xocl.initialize(std::getenv("DPU_XCLBIN_PATH"));
    }
    else
    {
        xocl.initialize("/mnt/dpu.xclbin");
    }
}
Get the command queue and create the buffers 
```c++
                q = xocl.get_command_queue();
                imgToDevice = xocl.create_buffer(rsize, CL_MEM_READ_ONLY);
                resizeFromDevice = xocl.create_buffer(resize_size, CL_MEM_WRITE_ONLY);
                fullFromDevice = xocl.create_buffer(full_size, CL_MEM_WRITE_ONLY);
```
Set the arguments to kernel, imgToDevice is the buffer to store the input image data get from the USB camera. resizeFromDevice is the buffer to store the resize image data on device side. The fullFromDevice is the buffer to store the full image(1080P) data on device side.

```c++

                krnl = xocl.get_kernel("pre_processor");
                krnl.setArg(0, imgToDevice);
                krnl.setArg(1, resizeFromDevice);
                krnl.setArg(2, fullFromDevice);
                krnl.setArg(3, IN_WIDTH);
                krnl.setArg(4, IN_HEIGHT);
                krnl.setArg(5, OUT_RESIZE_WIDTH);
                krnl.setArg(6, OUT_RESIZE_HEIGHT);
                xocl_initialized=true;
             }
```
write the image data which store in (void*) buffer to imgToDevice, then enqueueTask to run the kernel function.
```c++
            q.enqueueWriteBuffer(imgToDevice, CL_TRUE, 0, rsize, (void *)buffer);

            q.enqueueTask(krnl, NULL, &event_sp);

            clWaitForEvents(1, (const cl_event *)&event_sp);
```
Read the full resize image buffer to out_buf_0 and the full size image buffer to out_buf_1.
```c++
            q.enqueueReadBuffer(resizeFromDevice, CL_TRUE, 0, resize_size, out_buf_0);
            q.enqueueReadBuffer(fullFromDevice, CL_TRUE, 0, full_size, out_buf_1);
```
Convert the data into cv::Mat format, and push to the vector container of readImage.

```c++
            cv::Mat roi_mat0(OUT_RESIZE_HEIGHT, OUT_RESIZE_WIDTH, CV_8UC3, out_buf_0);
            cv::Mat roi_mat1(OUT_HEIGHT, OUT_WIDTH, CV_8UC3, out_buf_1);
            readImage.emplace_back(roi_mat1);
            readImage.emplace_back(roi_mat0);
            printf("DONE\n");

```
<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
