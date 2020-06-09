## file instructure

```
.
├── build.sh
├── include
│   ├── decodethread.hpp
│   ├── dputhread.hpp
│   ├── filter.hpp
│   ├── frameinfo.hpp
│   ├── guithread.hpp
│   ├── mythread.hpp
│   └── sortthread.hpp
├── README.md
├── src
│   ├── decodethread.cpp
│   ├── dputhread.cpp
│   ├── guithread.cpp
│   ├── mythread.cpp
│   └── sortthread.cpp
└── test
    └── usb_input_multi_threads_refinedet.cpp

```
## Pipeline


dataflow:

![data_flow](https://gitenterprise.xilinx.com/swm/Vitis-In-Depth-Tutorial/blob/master/Machine_Learning_Tutorial/Section_3-Basic/Module_6/app/in_depth_demo_data_flow.jpg)


- Check the device capability
- Get YUV data from device
- Decode Thread: Convert YUV data into BRG and output the frame queue.
- DPU Thread: Get the frame queue from Decode thread, For this case we will create several DPU threads.
- DPU Filter: The real task execute runner, contains pre-processing, refinedet dpu task, post processing and process result.
- Sort Thead: Get the result from the DPU Thread and sort all the result according to FrameID. output the result to GuiThread to display.
- Gui Thread: displaying the result by using DRM.




## Solution select
---

![three solutions](imgs/Solution_candidate.png)



- As the solutions picture show, We provide three APIs, the top-level API is the VITIS AI LIBRARY
The middle API is DPUTASK API and the bottom API is VITIS AI RUNTIME.  
- The green modules in the diagram indicate that they are encapsulated in the API. 

- In order to separate the resize, mean, scale, normalization operations in the pre-processing, and then use the HLS kernel to accelerate our application, this demo will use the vitis ai library API.

- If you want to implement your own post-processing of the model, you have several options.
    - The first way is that you can find the postprocessing source code and reload the postprocessing functions.
    - You can choose the DPUTASK API, but note that it encapsulates the preprocessing of the algorithm, i.e. you need to give it to DPUTASK, although you can change the values of mean and scale by adjusting the configuration parameters.
    - The most radical way to do this is to just use the VITIS AI RUNTIME API, which only contains only DpuRunner implementation. But you need to prepare your own input and output data structures, and you need to implement your own pre-processing and post-processing.
