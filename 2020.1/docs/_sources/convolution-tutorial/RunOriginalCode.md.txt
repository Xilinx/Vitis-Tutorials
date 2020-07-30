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

# 1. Evaluating the Original Application

The C application used in this tutorial performs a 2D convolution of a given set of filter coefficients and an RGBA video.
The application uses FFmpeg to create input and output video streams that are used as a pipe for reading each frame of the input video and writing out the corresponding processed frame.

![](images/ffmpeg_usage.png)

In this step, you build and run this application to create baseline performance data for the original, non-accelerated application. You also save the output of the original application as golden data to compare with and verify the output of the hardware accelerated application.

## Build the C Application

1. Navigate to the `cpu_src` directory for the original application code.
2. Execute the following `make` command.

   ```
   cd convolution-tutorial/design/cpu_src
   make
   ```

The command compiles the C source code and builds the `convolve` executable. The executable requires the path to a video file; in this case, one is in the project root directory.

>**TIP:** The Makefile used in this lab is detailed and contains many steps and variables. For a discussion of the structure and contents of the Makefile, refer to [Understanding the Makefile](./HowToRunTutorial.md).

## Run the C Application and Generate the Golden Result

In this step, run the original C application with a specified input video file in different formats, and generate the corresponding golden output files for comparison purposes using the following commands.

```
cd convolution-tutorial/design/cpu_src
make golden
```

The first output file generated, `golden_out_full.mp4` is a full movie with 132 frames. Each frame is 1980x1080 pixels. This golden output file is used for all hardware runs as applications can run very quickly on hardware. However, for emulation, you use smaller files with only one frame for a quicker turnaround.  

Here is the summary of the generated golden output files.

- `golden_out_full.mp4`: Used for testing the accelerated application when running on hardware.
- `golden_out_small.mp4`: Used for testing software and hardware emulation runs, except in Step 5: Using Out-ofOrder Queues and Multiple Compute Units.
- `golden_out_small_40.mp4`: Used for testing in Step 5: Using Out-of-Order Queues and Multiple Compute Units.

## Profile the Application and Establish Performance Goals

As stated in the [Methodology for Accelerating Applications with the Vitis Unified Software Platform](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=methodologyacceleratingapplications.html) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416), you can use the `gprof` tool to profile the application and identify potential functions for acceleration.

1. Add the `-pg` option in the gcc command line. 

   This is already done in the `Makefile`.
2. Change directory into the `cpu_src` folder and run `make` to generate the executable file.

   ```
   cd cpu_src
   ```

3. Run the executable file.

   ```
   ./convolve --gray true ../video.mp4
   ```

4. Extract the profile result.

   ```
   gprof convolve gmon.out> gprofresult.txt
   ```

5. To view the Profile Summary report, open the `gprofresult.txt` file in a text editor. You should see results similar to the following table.

   Each sample counts as 0.01 seconds.

   | % Time | Cumulative Seconds | Self Seconds | Total Calls  | ms/Call  | ms/Call  | Name                         |  
   |--------:|-----------:|----------:|----------:|----------:|----------:|:------------------------------|  
   | 95.29  |     7.28  |   7.28   |    132   |  55.15   |  55.15   | convolve_cpu                 |
   |  4.85  |     7.65  |   0.37   |    132   |   2.81   |   2.81   | grayscale_cpu                |
   |  0.00  |     7.65  |   0.00   |    132   |   0.00   |   0.00   | print_progress(int, int)     |
   |  0.00  |     7.65  |   0.00   |      1   |   0.00   |   0.00   | GLOBAL__sub_I_default_output |

   The report indicates that the convolve_cpu uses 95% of the execution time. Accelerating that function will significantly improve the total performance.

### Determine the Maximum Achievable Throughput

In most FPGA-accelerated systems, the maximum achievable throughput is limited by the PCIe® bus. The PCIe performance is influenced by many different aspects, such as motherboard, drivers, targeted shell, and transfer sizes. The Vitis core development kit provides a utility, `xbutil`, and you can run the `xbutil dmatest` command to measure the maximum PCIe bandwidth it can achieve. The throughput on your design target cannot exceed this upper limit.

### Establish Overall Acceleration Goals

On the CPU, it takes 19.25 seconds to process 132 1920x1080 frames. This means you are achieving a performance of 132/19.25 = <7 frames per second (fps). You want your application to achieve a minimum real-time performance of 30 fps, so you need to accelerate it by a factor of ~5x compared to the CPU.

Given that the size of a frame is 1920 x 1080 x 4bytes = 8.29 MB, this means that, in absolute terms, your accelerated application must deliver a minimum throughput of 8.29 MB x 30 fps = 249 MB/s.

This throughput goal is well within the bounds of maximum achievable throughput of an Alveo Data Center accelerator card. This tutorial will walk you through a predictable process for achieving that goal.

## Next Step

You have identified the functions from the original application that are targets for acceleration and established the performance goals. In the following labs, you will create a baseline of the original `convolve` function running in hardware and perform a series of host and kernel code optimizations to meet your performance goals. You begin by [creating a Vitis core development kit application](./baseline.md) from the original application.

You will use Hardware Emulation runs for measuring performance in each step. As part of the final step, you can run all these steps in hardware to demonstrate how the performance was improved at each step.
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">Return to Getting Started Pathway</a> — <a href="/docs/convolution-tutorial/README.md">Return to Start of Tutorial</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
