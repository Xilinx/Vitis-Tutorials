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

![data_flow](imgs/in_depth_demo_data_flow.jpg)


- Check the device capability
- Get YUV data from device
- Decode Thread: Convert YUV data into BRG and output the frame queue.
- DPU Thread: Get the frame queue from Decode thread, For this case we will create several DPU threads.
- DPU Filter: The real task execute runner, contains pre-processing, refinedet dpu task, post processing and process result.
- Sort Thead: Get the result from the DPU Thread and sort all the result according to FrameID. output the result to GuiThread to display.
- Gui Thread: displaying the result by using DRM.
