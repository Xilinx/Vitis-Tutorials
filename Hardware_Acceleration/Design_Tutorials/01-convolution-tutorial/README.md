<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>


# Accelerating Video Convolution Filtering Application

***Version: Vitis 2022.2***

This tutorial introduces you to a compute-intensive application that is accelerated using the Xilinx Alveo Data Center accelerator card. It goes through the design of a specific kernel that runs on the FPGA and briefly discusses optimization of the host-side application for performance. The kernel is designed to maximize throughput, and the host application is optimized to transfer data in an effective manner that moves in-between the host and FPGA card. The host application essentially eliminates the data movement latency by overlapping data transfers for multiple kernel calls. Another essential purpose of this tutorial is to show **_how one can easily estimate the performance of hardware kernels that can be built using Vitis HLS and how accurate and close these estimates are to actual hardware performance_**

## Introduction to Acceleration

The first lab is designed to let you quickly experience the acceleration performance that can be achieved by porting the video filter to Xilinx's Alveo accelerator card. The Alveo series cards are designed for accelerating data center applications. However, this tutorial can be adapted to other accelerator cards with some simple changes.

 The steps to be carried out for this first lab include:

- Setting up the Vitis application acceleration development flow
- Running the hardware optimized accelerator and comparing its performance with a baseline of the application

This lab demonstrates the significant performance gain that can be achieved as compared to CPU performance. Whereas the next labs in this tutorial will illustrate and guide how such performance can be achieved using different optimizations and design techniques for 2D convolution kernels and the host side application.

### Cloning the GitHub Repository and Setting Up the Vitis Tool

To run this tutorial you will need to clone a git repo and also download and extract some compressed files, please follow the instruction given below:

#### Clone Git Repo

Clone the repository using the following command:

```bash
git clone https://github.com/Xilinx/Vitis-Tutorials.git
```

#### Copy and Extract Large Files

Copy and extract large files in convolution tutorial directory as follows:

```bash
cd /VITIS_TUTORIAL_REPO_PATH/Hardware_Acceleration/Design_Tutorials/01-convolution-tutorial
wget https://www.xilinx.com/bin/public/openDownload?filename=conv_tutorial_files.tar.gz -O conv_tutorial_files.tar.gz
tar -xvzf  conv_tutorial_files.tar.gz
```

**NOTE** : VITIS_TUTORIAL_REPO_PATH is the local directory path where git repo is cloned. 

#### Setting Up the Vitis Tool

Setup the application build and runtime environment using the following commands as per your local installation:

```bash
source <XILINX_VITIS_INSTALL_PATH>/settings64.sh
source <XRT_INSTALL_PATH>/setup.sh
```

### Baseline the Application Performance

The software application processes High Definition(HD) video frames/images with 1920x1080 resolution. It performs convolution on a set of images and prints the summary of performance results. It is used for measuring baseline software performance. Please the set the environment variable that points to tutorial direction relative to repo path as follow:

```bash
export CONV_TUTORIAL_DIR=/VITIS_TUTORIAL_REPO_PATH/Hardware_Acceleration/Design_Tutorials/01-convolution-tutorial
```

where **VITIS_TUTORIAL_REPO_PATH** is the local path where git repo is placed by the user after cloning.

  **NOTE**: Make sure during all of the labs in this tutorial you have set `CONV_TUTORIAL_DIR` variable appropriately 

Run the application to measure performance as follows:

```bash
cd $CONV_TUTORIAL_DIR/sw_run
./run.sh 
```

Results similar to the ones shown below will be printed. Note down the CPU throughput.

```bash
----------------------------------------------------------------------------
Number of runs    : 60
Image width       : 1920
Image height      : 1080
Filter type       : 6

Generating a random 1920x1080 input image
Running Software version on 60 images

CPU  Time         :    28.0035 s
CPU  Throughput   :    12.7112 MB/s
----------------------------------------------------------------------------
```

## Running FPGA Accelerated Application

### Launching the Host Application

Now launch the application, which uses FPGA accelerated video convolution filter. The application will be run on an actual FPGA card, also called System Run.

```bash
cd $CONV_TUTORIAL_DIR
make run
```

The result summary will be similar to the one given below:

```bash
----------------------------------------------------------------------------
Xilinx 2D Filter Example Application (Randomized Input Version)

FPGA binary       : ../xclbin/fpgabinary.hw.xclbin
Number of runs    : 60
Image width       : 1920
Image height      : 1080
Filter type       : 3
Max requests      : 12
Compare perf.     : 1

Programming FPGA device
Generating a random 1920x1080 input image
Running FPGA accelerator on 60 images
Running Software version
Comparing results

Test PASSED: Output matches reference

FPGA Time         :     0.4240 s
FPGA Throughput   :   839.4765 MB/s
CPU  Time         :    28.9083 s
CPU  Throughput   :    12.3133 MB/s
FPGA Speedup      :    68.1764 x
----------------------------------------------------------------------------
```

### Results

 From the host application console output, it is clear that the FPGA accelerated kernel can outperform CPU-only implementation by a factor of 68x. It is a large gain in terms of performance over CPU. The following labs will illustrate how this performance allows processing more than 3 HD video channels with 1080p resolution in parallel. The tutorial describes how to achieve such performance gains by building a kernel and host application written in C++.  The host application uses OpenCL APIs and Xilinx Runtime (XRT) underneath it, demonstrating how to unleash this custom-built hardware kernel's computing power effectively.

---------------------------------------

<p align="center"><b>
Next Lab Module: <a href="./lab1_app_introduction_performance_estimation.md">Video Convolution Filter : Introduction and Performance Estimation</a>
 </b></p>
 
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020–2022 Xilinx</sup><br><sup>XD018</sup></br></p></b></p>
