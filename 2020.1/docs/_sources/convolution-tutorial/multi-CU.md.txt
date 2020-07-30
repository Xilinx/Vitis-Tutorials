<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">See 2019.2 Vitis Application Acceleration Development Flow Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 6. Using Out-of-Order Queues and Multiple Compute Units

In the previous labs, you concentrated on extracting parallelism within a kernel using techniques such as pipelining and dataflow. One of the very powerful features of FPGAs is that you can create multiple compute units (CUs), which are identical copies of your kernel, allowing more processing to happen in parallel. These CUs can be used to process multiple images at the same time, or divide one image into smaller regions, so that you can process each frame faster. In this tutorial, you are going to take the latter approach to speed up computation of each individual frame.

To take advantage of acceleration potential offered by the multiple CUs, the host application needs to be able to issue and manage multiple concurrent requests to the CUs. For maximum performance, it is important to ensure that the application keeps all the CUs busy. Any delay in transferring data or starting a CU will reduce the overall performance.

In this lab, you will first implement changes in the host code to handle multiple CUs, then make updates to the kernel to handle subregions of a frame.

## Executing Queued Operations Out-of-Order

The host application uses OpenCL™ APIs to communicate with kernels on an FPGA. Those commands are executed through a command queue object. By default, the command queue is handled in order; however, you can change this behavior to execute your operations in any order by passing a special flag to the command queue. This type of queue will execute whatever operation is ready to execute as soon as the resources are available.

Out-of-order queues allow you to launch multiple operations at the same time, including memory transfers and kernel calls. You can add dependencies on tasks using OpenCL API events and wait lists. Events are objects that are associated with a particular task. It is usually passed into a call as the last argument. If an operation depends on another task, you can pass the event into a wait list. The operation will need to wait for all events in the wait list to finish before executing.

>**TIP:** The completed host code source file is provided under the `reference-files/multicu` folder. You can use it as a reference if needed.

To take advantage of the out-of-order queues and events, modify the host program.

1. Open the `convolve.cpp` file from `src/multicu` and modify the following line.

        cl::CommandQueue q(context, device, cl::QueueProperties::Profiling);

    to:

        cl::CommandQueue q(context, device, cl::QueueProperties::Profiling | cl::QueueProperties::OutOfOrder);

    Passing `cl::QueueProperties::OutOfOrder` enum to the `CommandQueue` constructor tells the runtime that the operations on this queue can be executed out-of-order.

2. With an out-of-order queue, you must now enforce ordering between the read, enqueueTask, and write calls to make sure that you do not read the buffer before the copy operation has completed. You will create a `cl::Event` object and pass it as the last argument of the `enqueueWriteBuffer` function. Change line 95 from:

        q.enqueueWriteBuffer(buffer_input, CL_FALSE, 0, frame_bytes, inFrame.data());

    to:

        cl::Event write_event;
        q.enqueueWriteBuffer(buffer_input, CL_FALSE, 0, frame_bytes, inFrame.data(), nullptr, &write_event);

    The `write_event` object will be used to enforce this operation's dependency on the next task.

3. You need to pass the `write_event` to the `enqueueTask` call. You must also create an event object for the task to pass to the read operation. The `write_event` object from the previous call must be passed into this call through a pointer to a vector. Modify the `enqueueTask` call in line 96 as follows.

        vector<cl::Event> iteration_events{write_event};
        cl::Event task_event;
        q.enqueueTask(convolve_kernel, &iteration_events, &task_event);

4. The read call needs to be executed after the `convolve_kernel` has finished executing. Just like in the previous operations, you can also send the event as the last argument of this function. Modify the `enqueueReadBuffer` call in line 97 as follows.

        iteration_events.push_back(task_event);
        cl::Event read_event;
        q.enqueueReadBuffer(buffer_output, CL_FALSE, 0, frame_bytes, outFrame.data(), &iteration_events, &read_event);
        iteration_events.push_back(read_event);

    Here you added the `task_event` object to the end of the `iteration_events` vector. Then, you pass `iteration_events` in as the second to the last argument to the `enqueueReadBuffer` call. You could also have created a new vector because the `enqueueTask` call depends on the previous call.

5. You need to make sure that `ffmpeg` does not write to the output stream before you transfer the data back to the host. You can block the thread from continuing by calling the wait call on the `read_event` object. Add this line after the `push_back` function call on the `iteration_events` object.

        read_event.wait();

## Using Multiple Compute Units

In previous labs, only one CU is used for the kernel. In this section, you will modify the design to use multiple CUs, and each CU will process a smaller region of the image. To achieve that, you are going to make further modifications based on the output from the previous step.

>**TIP:** The completed kernel source file is provided under the `reference-files/multicu` folder. You can use it as a reference if needed.

Here you are going to modify the kernel code. Open the `convolve_fpga.cpp` file from `src/multicu`, and make following modifications:

1. Modify the signature of the `convolve_fpga` kernel to accept the offset and number of lines each kernel will process (line 106).

   ```
           void convolve_fpga(const RGBPixel* inFrame, RGBPixel* outFrame, const float* coefficient, int coefficient_size, int img_width, int img_height, int line_offset, int num_lines) {
               ...
   ```

   Depending on the image size and the number of CUs, you will divide the work evenly, and the offset will be used to determine the starting location of the kernel. The `line_offset` parameter is the first line that the CU will process. The `num_lines` argument will hold the number of lines processed by each CU.

   >**TIP**: Ensure the declaration of the `convolve_fpga` function in `kernels.h` matches with the `convolve_fpga.cpp` file.

2. Modify the main kernel, so that you can calculate the padding and offsets for each of the CUs to process (line 123).

   ```
        int half = COEFFICIENT_SIZE / 2;

        hls::stream<RGBPixel> read_stream("read");
        hls::stream<RGBPixel> write_stream("write");

        int elements = img_width * num_lines;
        int offset = std::max(0, line_offset - half) * img_width;
        int top_padding = 0;
        int bottom_padding = 0;
        int padding = 0;
        if(line_offset == 0) {
            top_padding = half * img_width;
        } else {
            padding = img_width *  half;
        }
        if(line_offset + num_lines < img_height) {
            padding += img_width * half + COEFFICIENT_SIZE;
        }else {
            bottom_padding = img_width * (half) + COEFFICIENT_SIZE;
        }

        #pragma HLS dataflow
        read_dataflow(read_stream, inFrame + offset, img_width, elements + padding, half, top_padding, bottom_padding);
        compute_dataflow(write_stream, read_stream, coefficient, img_width, elements, half);
        write_dataflow(outFrame + line_offset * img_width, write_stream, elements);
   ```

   * The `offset` variable is used to calculate the offsets from the beginning of the image to the first pixel that the CU will read.
   * The `top_padding` and `bottom_padding` variables will determine the padding of zeros to add to the top and the bottom of the image.
   * The `padding` variable, on the other hand, is the number of pixels to read including the regions around the convolution window.

3. Modify the read_dataflow kernel to send zeros for the padding areas for the top and the bottom of the image (line 20).

   ```
        void read_dataflow(hls::stream<RGBPixel>& read_stream, const RGBPixel * in, int img_width, int elements, int half, int top_padding, int bottom_padding) {
            while(top_padding--) {
                read_stream << zero;
            }
            int pixel = 0;
            while(elements--) {
                read_stream << in[pixel++];
            }
            while(bottom_padding--) {
                read_stream << zero;
            }
        }
   ```

4. Because you are handling the padding logic in the read_dataflow module, you can remove the initialization logic for zeroing out the padded area. Remove the following lines from compute_dataflow (line 45).

   ```
        while(line_idx < center) {
            for(int i = 0; i < img_width; i++) {
                window_mem[line_idx][i] = zero;
            }
                line_idx++;
        }
   ```

You still need to modify a few things on the host code side to launch multiple CUs in parallel.

## Host Code Updates to Support Multiple Compute Units

The following steps need to be performed for supporting CUs.

1. Open `convolve.cpp` and add the following lines before the `frame_count` `for` loop.

   ```
        int compute_units = 4;
        int lines_per_compute_unit = height / compute_units;
   ```

   These variables define the number of CUs you will have in your binary. You then divide the lines of the image evenly between the CUs. This code assumes that you can evenly divide the image among the CUs.

2. Instead of launching one task, launch a task on each of the CUs you created. Modify the following code from:

   ```
        cl::Event task_event;
        q.enqueueTask(convolve_kernel, &iteration_events, &task_event);
   ```

    to:

   ```
        vector<cl::Event> task_events;
        for(int cu = 0; cu < compute_units; cu++) {
            cl::Event task_event;
            convolve_kernel.setArg(6, cu * lines_per_compute_unit);
            convolve_kernel.setArg(7, lines_per_compute_unit);
            q.enqueueTask(convolve_kernel, &iteration_events, &task_event);
            task_events.push_back(task_event);
        }
        copy(begin(task_events), end(task_events), std::back_inserter(iteration_events));
   ```

   This `for` loop will launch one task per CU. You will pass an event object to each of the tasks, and then add it to the `task_events` vector. Notice that you are not adding it to the `iteration_events` until after the end of the loop. This is because you only want the tasks to depend on the `enqueueWriteBuffer` call and not each other.

Now you can compile and run the design, and you should see results similar to the following section.

## Run Hardware Emulation for Multiple Compute Units

1. Before running emulation, you need to set the CU number to 4. To do that, open the `design.cfg` and modify the `nk` option as follows.

   ```
   nk=convolve_fpga:4
   ```

   The `nk` option is used to specify the number of kernel instances, or CUs, generated during the linking step of the build process. For this lab, set it to 4.

2. Go to the `makefile` directory.
3. Use the following command to run hardware emulation.

   ```
   make run TARGET=hw_emu STEP=multicu SOLUTION=1 NUM_FRAMES=1
   ```

    The following code shows the results of this kernel running on four CUs.

    ```
    Processed 0.08 MB in 42.810s (0.00 MBps)

    INFO: [Vitis-EM 22] [Wall clock time: 01:34, Emulation time: 0.102462 ms] Data transfer between kernel(s) and global memory(s)
    convolve_fpga_1:m_axi_gmem1-DDR[0]          RD = 24.012 KB              WR = 0.000 KB
    convolve_fpga_1:m_axi_gmem2-DDR[0]          RD = 0.000 KB               WR = 20.000 KB
    convolve_fpga_1:m_axi_gmem3-DDR[0]          RD = 0.035 KB               WR = 0.000 KB
    convolve_fpga_2:m_axi_gmem1-DDR[0]          RD = 22.012 KB              WR = 0.000 KB
    convolve_fpga_2:m_axi_gmem2-DDR[0]          RD = 0.000 KB               WR = 20.000 KB
    convolve_fpga_2:m_axi_gmem3-DDR[0]          RD = 0.035 KB               WR = 0.000 KB
    convolve_fpga_3:m_axi_gmem1-DDR[0]          RD = 24.012 KB              WR = 0.000 KB
    convolve_fpga_3:m_axi_gmem2-DDR[0]          RD = 0.000 KB               WR = 20.000 KB
    convolve_fpga_3:m_axi_gmem3-DDR[0]          RD = 0.035 KB               WR = 0.000 KB
    convolve_fpga_4:m_axi_gmem1-DDR[0]          RD = 22.000 KB              WR = 0.000 KB
    convolve_fpga_4:m_axi_gmem2-DDR[0]          RD = 0.000 KB               WR = 20.000 KB
    convolve_fpga_4:m_axi_gmem3-DDR[0]          RD = 0.035 KB               WR = 0.000 KB
    ```

    You can now perform four times more work in about the same amount of time. You transfer more data from global memory because each CU needs to read the surrounding padding lines.

## View Profile Summary Report for Hardware Emulation

Use the following command to view the Profile Summary report.

```
make view_run_summary TARGET=hw_emu STEP=multicu
```

The kernel execution time for four CUs is around 0.065 ms each.

Here is the updated table.

| Step                    | Image Size | Time (HW-EM)(ms) | Reads (KB)      | Writes (KB)    | Avg. Read (KB) | Avg. Write (KB) | BW (MBps)  |
| :-------------------    | :--------- | ---------------: | --------------: | -------------: | -------------: | --------------: | ---------: |
| baseline                |     512x10 | 3.903            | 344             |        20.0    |          0.004 |           0.004 |    5.2     |
| localbuf                |     512x10 | 1.574 (2.48x)    | 21 (0.12x)      |        20.0    |          0.064 |           0.064 |    13      |
| fixed-type data         |     512x10 | 0.46 (3.4x)      | 21              |        20.0    |          0.064 |           0.064 |    44      |
| dataflow                |     512x10 | 0.059 (7.8x)     | 21              |        20.0    |          0.064 |           0.064 |    347     |
| multi-CU                |     512x40*| 0.06 (0.98x)     | 92 (4.3x)       |       80.0 (4x)|          0.064 |           0.064 |    1365*   |

>**NOTE:**
>
>* The multi-CU version processed four times the amount of data compared to previous versions. Even if the CU execution time does not change for each CU, the four parallel CUs increase the system performance by almost four times.
>* This is calculated by 4x data/time. Here the data transfer time is not accounted for, and you assume that the four CUs are executing in parallel. This is not as accurate as the hardware run, but you will use it as a reference for optimizations effectiveness.

## Next Steps

In this step, you performed host code optimizations by using out-of-order command queue and executing multiple CUs. In the next step, you will be [Using QDMA Streaming with Multiple Compute Units](./qdma.md).

[hostopt_hwemu_profilesummary]: ./images/191_hostopt_hwemu_pfsummary_40_2.jpg "Host code optimization version hardware emulation profile summary"
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">Return to Getting Started Pathway</a> — <a href="/docs/convolution-tutorial/README.md">Return to Start of Tutorial</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
