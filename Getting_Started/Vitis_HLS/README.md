<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ Application Acceleration Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">See 2019.2 Vitis Application Acceleration Development Flow Tutorials</a>
  </td>
 </tr>
 <tr>
 <td align="center"><h1></h1>
 </td>
 </tr>
</table>

# Vitis HLS Analysis and Optimization

## Introduction

Vitis High-Level Synthesis (HLS) is a key part of the Vitis application acceleration development flow. The tool is responsible for compiling C/C++ and OpenCL code into a kernel for acceleration in the programmable logic (PL) region of Xilinx devices. Thus, it is the tool that compiles the hardware kernels for the Vitis tools by performing high-level synthesis.

In this tutorial, you will work through the Vitis HLS tool GUI to build, analyze, and optimize a hardware kernel. You are working through the Vitis kernel flow in the Vitis tool. For more information, refer to [Enabling the Vitis Kernel Flow](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=creatingnewvitishlsproject.html;a=uiy1584905571731) in the Vitis HLS Flow of the Vitis Unified Software Platform Documentation (UG1416).

### Before You Begin

The labs in this tutorial use:

* BASH Linux shell commands.
* 2020.1 Vitis core development kit release and the *xilinx_u200_xdma_201830_2* platform. If necessary, it can be easily ported to other versions and platforms.

>**IMPORTANT:**  
>
> * Before running any of the examples, make sure you have installed the Vitis core development kit as described in [Installation](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vhc1571429852245.html) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).
>* If you run applications on the Xilinx® Alveo™ Data Center accelerator cards, ensure the card and software drivers have been correctly installed by following the instructions To complete installation, follow the instructions on the [Alveo Product Documentation tab](https://www.xilinx.com/products/boards-and-kits/alveo.html).

### Accessing the Tutorial Reference Files

1. To access the reference files, type the following into a terminal: `git clone https://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to the `vitis_hls_analysis` directory, and then access the `reference-files` directory.

## Next Steps

Complete the labs in the following order:
 
* [Creating a Vitis HLS Project](./new_project.md)
* [Running High-Level Synthesis and Analyzing Results](./synth_and_analysis.md)
* [Using Optimization Techniques](./optimization_techniques.md)
* [Reviewing the DATAFLOW Optimization](./dataflow_design.md)
* [Using the HLS Kernel in the Vitis Tool](./using_the_kernel.md)

<!--

1. [Creating the Vitis HLS Project](./new_project.md) - Create the project to specify the source code and testbench.

2. [Running High-Level Synthesis and Analyzing Results](./synth_and_analysis.md) - Simulate and synthesize the design, and analyze the results.

3. [Using Optimization Techniques](./optimization_techniques.md) - Try different optimization techniques to achieve the initiation interval (II)=1.

4. [Reviewing the Dataflow Optimization](./dataflow_design.md) - Add the Dataflow optimization to achieve even better results.

5. [Using the HLS Kernel in the Vitis Tool](./using_the_kernel.md) - Use the Vitis integrated design environment (IDE) to create a new application project with the host and kernel code.

-->

</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="./README.md">Return to Main Page</a> — <a href="/docs/vitis-getting-started/README.md">Return to Getting Started Pathway</a></b></p>
</br>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
