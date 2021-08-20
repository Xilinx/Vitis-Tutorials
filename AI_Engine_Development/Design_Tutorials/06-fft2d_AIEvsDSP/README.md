<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2021.1 Versal™ 2D-FFT Implementation Using Vitis Acceleration Library</h1>
   </td>
 </tr>
</table>

# Table of Contents
[Introduction](#introduction)

[Before You Begin](#Before-you-Begin)

[Design Implementations](#Design-Implementations)

[Important Observations between AIE and DSP & PL Implementations](#important-observations-between-aie-and-dsp--pl-implementations)

[References](#references)

# Introduction
The Xilinx® Versal ACAP is a fully software-programmable, heterogeneous compute platform that combines the Scalar Engines, (Processor Subsystem (PS) that includes the Arm® processors), Adaptable Engines, (Programmable Logic (PL) and memory) and the Intelligent Engines, including both AI and DSP Engines.

This tutorial is one of several to perform two implementations of a system-level design using AI Engine in one and the DSP Engine is another. In each implementation, the tutorial takes users through hardware emulation and hardware flow in the context of a complete Versal ACAP system design.\
A `Makefile` is provided for each implementation which can be used to create the design for various `Point Sizes`(32x64, 64x128, 128x256, 256x512 and 1024x2048), different number of `fft_2d kernel/graph instances`(1, 5 and 10) and different `targets`(hw_emu and hw).\
The design documentation demonstrates hardware and software design details including methodology for each implementation, elaborating on the functional partitioning. Additionally, compilation, execution, measurement steps and observations are given.

<details>
  <summary>Objectives</summary> 
	
## Objectives
After completing the tutorial, you should be able to:
* Develop system level design (2D-FFT in this case) by identifying the algorithm and deploying the same algorithm on AI Engines or PL and DSP Engines. 
* Build a complete system design by going through the various steps in the Vitis™ unified software platform flow, including creating the AI Engine Adaptive Data Flow API (ADF) graph, compiling the A72 host application and compiling PL kernels, using the Vitis compiler (V++) to link the AI Engine and HLS kernels with the platform, and packaging the design. You will also be able to run the design through the hardware emulation and hardware flow in a mixed System C/RTL cycle-accurate/QEMU-based simulator
* Develop a consistent harness to have the data-mover kernels maintain similar interface with AI Engine/DSP kernels (with AXI4-stream)
* Develop an understanding of graph control APIs for the AI Engine implementation
* HLS APIs for controlling HLS/PL kernels
* Understand the methodological differences in the design done using AI Engines and the same done using PL and DSP Engines.
* Develop an understanding of metrics including utilization, performance/throughput and power across various number of instances of the FFT arrays of different dimensions

</details>

<details>
  <summary>Design Overview</summary> 
 
## Design Overview
This design aims to implement a 2D-FFT algorithm performed on, for example a 1024x2048 Matrix using 1024 and 2048 point 1D-FFT kernels. The pseudo-code to implement the algorithm is shown below
```
x = rand(a,b);         % Create matrix of dimensions m x n

x_2fft = fft2(x);      % Reference generation, 2 dimensional FFT of matrix 'x'

FFT2D, using 1D – FFT.
x_row = fft(x);        % Row wise 1D-FFT
x_row = x_row.’;       % Transpose
x_col = fft(x_row);    % Col wise 1D-FFT
x_col = x_col.’;       % Transpose

x_2fft - x_col         % Calculate the error difference
```

In the above algorithm, in calculating each 1D-FFT, one dimension is kept constant while the other is computed. The transpose function is applied after each 1D-FFT compute. The transpose function moves the entry along each element of the dimension to the corresponding element of the other dimension. A golden data set is generated as reference and the error difference is calculated.

A similar algorithm is deployed in the two implementations using either AI Engines or PL and DSP Engines. The design will compile through v++, and create a Petalinux-based Platform via script as well as generate the PDI and host application.\
But instead of the transpose part, a PL-based data-generator and checker(referred to as data-mover) is used to give an impulse(value=1) input to the 'Row wise' 1D-FFT check it's output against the expected fft output for the same(first row containing all 1s and remaining 0s) and then the transposed pattern of that, generated within the PL, is streamed as input to 'Col wise' 1D-FFT and it's output checked against the expected output(all 1s). And the data-mover kernel returns the total error count in both the stages to the host application, which is used to declare a Pass or Fail of the Testcase.

To enable methodology comparison of the AI Engine and DSP Engine based implementations, the design build process can be modified to analyze how each implementation scales with different number of instances(x1, x5 and x10) of various matrix array dimensions(RowsxCols = 32x64, 64x128, 128x256, 256x512 and 1024x2048). A similar set of harnesses is developed and maintained between the two implementations to generate and check input/output vectors via the PL-based data-mover kernels and, to move data to and from AI and DSP Engine kernels. In both cases, XRT running on A-72 controls data flow in compute and data-mover kernels via, graph control APIs which control the AI Engine kernels and HLS APIs, which control the HLS/PL kernels.

</details>

<details>
  <summary>Directory Structure</summary> 
	
## Directory Structure
```
fft2d_AIEvsDSP
|__AIE......................contains AI Engine implementation
|    |Makefile....................with recipes for each step of the design compilation
|    |images......................contains images used for AIE Design documentation
|    |description.json............required for XOAH
|    |build.......................created and contains subfolders from design build
|    |design......................contains source and include files
|    |       |aie_src....................contains all the aie source files
|    |       |pl_src.....................contains all the data-mover source files
|    |       |host_app_src...............contains host application source files
|    |       |system_configs.............contains all system configurations files
|    |       |profiling_configs..........contains xrt.ini file
|    |       |hw_emu_files...............contains hw_emu launch script
|__DSP......................contains DSP implementation
|    |Makefile....................with recipes for each step of the design compilation
|    |images......................contains images used for DSP Design documentation
|    |description.json............required for XOAH
|    |build.......................created and contains subfolders from design build
|    |design......................contains source and include files
|    |       |pl_src.....................contains all the fft_2d and data-mover source files
|    |       |host_app_src...............contains host application source files
|    |       |system_configs.............contains all system configurations files
|    |       |profiling_configs..........contains xrt.ini file
|    |       |directives.................contains directives for various vitis compilation stages like hls.pre_tcl etc.
|    |       |hw_emu_files...............contains hw_emu launch script
```
</details>

# Before You Begin

<details>
	
<summary>Documentation: Explore AI Engine Architecture</summary> 

## *Documentation*: Explore AI Engine Architecture

* [AI Engine Development Design Process](https://www.xilinx.com/support/documentation-navigation/design-process/ai-engine-development.html)

* [AM009 AI Engine Architecture Manual](https://www.xilinx.com/support/documentation/architecture-manuals/am009-versal-ai-engine.pdf)

* [Versal ACAP AI Engines for Dummies](https://forums.xilinx.com/t5/Design-and-Debug-Techniques-Blog/Versal-ACAP-AI-Engines-for-Dummies/ba-p/1132493)

</details>

<details>
<summary>Tools: Installing the Tools</summary> 
	
## *Tools*: Installing the Tools

Tools Documentation: 

* [AI Engine Tools lounge](https://www.xilinx.com/member/versal_ai_tools_ea.html)

* [AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2021.1/vitis_doc/yii1603912637443.html)

To build and run the 2D-FFT tutorial (AI Engine and DSP implementations), you will need the following tools downloaded/installed:

* Install the [Vitis Software Platform 2021.1](https://www.xilinx.com/html_docs/xilinx2021.1/vitis_doc/acceleration_installation.html#dhg1543555360045__ae364401) 

* Obtain licenses for AI Engine tools

* Follow the instructions in [Installing Xilinx Runtime and Platforms](https://www.xilinx.com/html_docs/xilinx2021.1/vitis_doc/acceleration_installation.html#dhg1543555360045__ae364401) (XRT)

* Download and setup the [VCK190 Vitis Platform for 2021.1](https://www.xilinx.com/member/vck190_headstart.html#docs)

</details>

<details>
<summary>Platform</summary> 

## Platform
Before beginning the tutorial make sure you have read and followed the [Vitis Software Platform Release Notes (v2021.1)](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/acceleration_release_notes.html) for setting up software and installing the VCK190 base platform.

This tutorial targets the VCK190 Production Board (see https://www.xilinx.com/products/boards-and-kits/vck190.html). If you have already purchased this board, download the necessary files from the lounge and ensure you have the correct licenses installed. If you do not have a board and the required license please contact your Xilinx sales contact.

</details>

<details>
<summary>Setting Up the Environment</summary>
 
## Setting up the Environment
When the elements of the Vitis software platform are installed, update the shell environment script. Set the environment variables to your system specific paths.

To Setup XRT(if not done already):
* source \<XRT-Location\>/setup.sh

In the design directory of each implementation, edit `env_setup.sh` script with your file paths, then source the environment script: 
```bash
source env_setup.sh
``` 

The script sets up the environment variables and sources scripts mentioned below:
1. `PLATFORM_REPO_PATHS` environment variable based upon where you downloaded the platform.
2. `XILINX_TOOLS_LOCATION`, path to the xilinx tools, used to source settings64.sh script.
3. `XLNX_VERSAL`, path to xilinx-versal-common-v2021.1 directory, used in the step below.
4. Platform by running the `xilinx-versal-common-v2021.1/environment-setup-cortexa72-cortexa53-xilinx-linux` script as provided in the platform download. This script sets up the `SDKTARGETSYSROOT` and `CXX` variables. If the script is not present, you **must** run the `xilinx-versal-common-v2021.1/sdk.sh`.
5. `DSPLIB_ROOT`, path to the downloaded Vitis DSP Libraries. Needed only for the AI Engine Implementation.
6. In the script user can optionally setup `XRT_ROOT` environment variable, pointing to XRT - RPMs, which can be packaged in the v++ packaging step. If not setup then it will automatically be excluded while packaging.
7. The script also sets up `PLATFORM` variable pointing to the required `.xpfm` file of the target platform set by variable `tgt_plat`.

</details>

<details>
<summary>Validation: Confirming Tool Installation</summary> 
	
## Validation: Confirming Tool Installation
```bash
which vitis
which aiecompiler
```

Confirm you have the VCK190 Base Platform. 
```bash
platforminfo --list | grep -m 1 -A 9 vck190
```
Output of the above command should be as follows:
```bash
 "baseName": "xilinx_vck190_base_202110_1",
            "version": "1.0",
            "type": "sdsoc",
            "dataCenter": "false",
            "embedded": "true",
            "externalHost": "false",
            "serverManaged": "false",
            "platformState": "pre_synth",
            "usesPR": "false",
```

</details>

# Design Implementations
The Makefile and source files for the AI Engine and DSP implementations are in the respective directories "AIE" and "DSP". For the documentation of the flow to build the design and details of the hardware and software design, click on each of the links below:

[AI Engine design implementation](AIE)

[DSP design implementation](DSP)

# Important Observations between AIE and DSP & PL Implementations

Below is a comparison between 1024 x 2048 point - 10 instance FFT-2D design implemented using AI and DSP Engines respectively. The picture below shows the throughput, resource utilization, power comsumption and Performance in Throughput/Watt.

| Design Target | Aggregate Throughput<br/>(in MSPS) | Average Latency (in us) | AIE Vector Cores | AIE Vector Load | Active Mem Banks /<br/> Mem R/W Rate | Active AIE Tiles | FF (Regs) /<br/> CLB LUTs | BRAMs | DSPs | Dynamic Power<br/>(in mW) | Performance per Watt<br/>(in MSPS/Watt) |
|:-------------:|:----------------------------------:|:-----------------------:|:----------------:|:---------------:|:------------------------------------:|:----------------:|:-------------------------:|:-----:|:----:|:-------------------------:|:---------------------------------------:|
| AIE           | 5835.315                           | 1770.840                | 20               | 76%             | 396 /<br/>20%                        | 56               | 11720 /<br/> 4154         | 0     | 0    | 5081                      | 1148.46                                 |
| DSP           | 4995.440                           | 2107.723                | NA               | NA              | NA                                   | NA               | 108471 /<br/> 70475       | 335   | 180  | 7875                      | 634.34                                  |

From the table you can see some clear indications of where the AI Engines in Versal can offer improvements:
* Almost 20% improvement on the aggregate throughput
* Reduced latency by ~20%
* Performance increase of ~2x to 1148 MSPS/Watt.
* Moving to AI Engine implementation, reduces the PL and DSP resources considerably. 180 DSPs, ~108k FFs, ~70K Luts and 335 BRAMs are reduced to just 56 AI Engines, 11k FFs and 4k Luts.

It is important to understand that those 56 AI Engines are not all required for the 2D-FFT compute, 20 AI Engines/Vector Cores are required for Computation and 36 AI Engines are required for memory to store the FFT twiddle factors and also enable connectivity around the array. The average load on these additional 36 AI Engine tiles is at only 25%. This means that if your application needs it, these AI Engines can be either: 
* Shared with other functions to run sequentially or 
* Use user constraints to better Map & Route this function to a reduced numbers of AI Engine tiles. [Details on AIE Mapper/Router.](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/maproutemethod.html#mcq1622542393364)

Additionally, further increasing the number of instances in the AIE design is easier compared to the DSP design, which will run into timing-closure issues especially for higher FFT point size designs.

**Measurment:**
1. Resources: AIE Design - Resource utilization is measured using XPE and Vivado(Report Utilization under implementation for FFs and CLB LUTs). DSP Design - Resource utilization is measured using Vivado.
2. Power: AIE Power consumption is measured using XPE and DSP Power Consumption is measured using Vivado(Report Power under implementation).
3. Throughput: Is measured via viewing runtime-profiling generated trace texts, in vitis_analyzer.

For Detailed Instructions on taking measurements of the parameters refer to the individual implementation section.

# References

### [AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/yii1603912637443.html)
Contains sections on how to develop AI Engine graphs, how to use the Ai Engine compiler, and AI Engine simulation, and performance analysis.

### [Vitis DSP Libraries](https://github.com/Xilinx/Vitis_Libraries/tree/master/dsp)

* [Vitis DSP Libraries Comprehensive Documentation](https://xilinx.github.io/Vitis_Libraries/dsp/2021.1/) 

### [Xilinx Runtime (XRT) Architecture](https://xilinx.github.io/XRT/master/html/index.html)
Below are links to the XRT information used by this tutorial: 

* [XRT Documentation](https://xilinx.github.io/XRT/master/html/index.html): Explains general XRT API calls used in the PS Host Application. 

* [XRT Github Repo](https://github.com/Xilinx/XRT): Contains the XRT source code. 

* [XRT AIE API](https://github.com/Xilinx/XRT/blob/master/src/runtime_src/core/include/experimental/xrt_aie.h): Documents the AI Engine XRT API calls

* [XRT Release Notes](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2021_1/ug1451-xrt-release-notes-pu1.pdf)

### [Vitis Unified Software Development Platform 2021.1 Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/index.html)
Below are links to Vitis related information referenced in this tutorial:

* [Vitis Application Acceleration Development Flow Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/kme1569523964461.html)

* [Vitis Application Acceleration Development Flow Tutorials](https://github.com/Xilinx/Vitis-Tutorials)

* [Vitis HLS](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/irn1582730075765.html)
