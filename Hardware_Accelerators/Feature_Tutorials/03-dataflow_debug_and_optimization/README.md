<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2021.1 Vitis™ Application Acceleration Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">See 2021.1 Vitis Application Acceleration Development Flow Tutorials</a>
  </td>
 </tr>
 <tr>
 <td align="center"><h1>Vitis HLS Analysis and Optimization</h1>
 </td>
 </tr>
</table>

# Introduction

Vitis High-Level Synthesis (HLS) is a key part of the Vitis application acceleration development flow. The tool is responsible for compiling C/C++ and OpenCL code into a kernel for acceleration in the programmable logic (PL) region of Xilinx devices. Thus, it is the tool that compiles the hardware kernels for the Vitis tools by performing high-level synthesis.

The aim of this tutorial is to give the user an overview of the techniques available in the Vitis HLS GUI with respect to understanding and debugging the dataflow optimization. You will be working through the Vivado kernel flow in the Vitis tool. For more information, refer to [Enabling the Vivado Kernel Flow](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitis_hls_process.html#uiy1584905571731) in the Vitis HLS Flow of the Vitis Unified Software Platform Documentation (UG1416).

## Before You Begin

The labs in this tutorial use:

* BASH Linux shell commands.
* 2020.2 Vitis core development kit release and the *xilinx_u200_gen3x16_xdma_1_202110_1* platform. If necessary, it can be easily ported to other versions and platforms.

>**IMPORTANT:**  
>
> * Before running any of the examples, make sure you have installed the Vitis core development kit as described in [Installation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitis_embedded_installation.html#tlp1602134446371) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).
>* If you run applications on the Xilinx® Alveo™ Data Center accelerator cards, ensure the card and software drivers have been correctly installed by following the instructions To complete installation, follow the instructions on the [Alveo Product Documentation tab](https://www.xilinx.com/products/boards-and-kits/alveo.html).

## Accessing the Tutorial Reference Files

1. To access the reference files, type the following into a terminal: `git clone https://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to the `Hardware_accelerators/Feature_Tutorials/03-dataflow_debug_and_optimization` directory, and then access the `reference-files` directory.

# Next Steps

Complete the labs in the following order:

1. [Dataflow Viewer and the basics](./dataflow_viewer.md): Review how to launch the dataflow viewer and go over the various features.
2. [FIFO Sizing and Deadlocks](./fifo_sizing_and_deadlocks.md): Review how to use the deadlock detection feature in the GUI and how to resize FIFOs to avoid deadlocks and achieve better performance.


</br>
<hr/>
<p align= center><b><a href="/README.md">Return to Main Page</a> — <a href="/docs/vitis-getting-started/">Return to Getting Started Pathway</a></b></p>
</br>
<p align="center"><sup>Copyright&copy; 2020-2021</sup></p>
