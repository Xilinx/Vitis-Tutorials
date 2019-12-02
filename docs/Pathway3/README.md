<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">See SDAccel™ Development Environment 2019.1 Tutorials</a>
   </td>
 </tr>
 <tr>
 <td align="center"><h1>Essential Concepts for Building and Running the Accelerated Application</h1>
 </td>
 </tr>
</table>

## Introduction

The following labs will introduce you to the essential concepts for building and running an accelerated application using the Vitis™ core development kit.

## Before You Begin

The labs in this tutorial use:

* BASH Linux shell commands.
* 2019.2 Vitis core development kit release and the *xilinx_u200_xdma_201830_2* platform. If necessary, it can be modified to use other software releases and platforms.

>**IMPORTANT:**  
>
> * Before running any of the examples, make sure you have installed the Vitis core development kit as described in [Installation](https://www.xilinx.com/html_docs/xilinx2019_2/vitis_doc/vhc1571429852245.html).
>* If you run applications on Xilinx® Alveo™ Data Center accelerator cards, ensure the card and software drivers have been correctly installed by following the instructions in the *Getting Started with Alveo Data Center Accelerator Cards Guide* ([UG1301](https://www.xilinx.com/cgi-bin/docs/bkdoc?k=accelerator-cards;v=latest;d=ug1301-getting-started-guide-alveo-accelerator-cards.pdf)).

## Accessing the Tutorial Reference Files

1. To access the reference files, type the following into a terminal: `git clone https://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to the `Pathway3` directory, and then access the `reference-files` directory.

## Next Steps

You should work through the labs in the following order.

1. [Building an Application](./BuildingAnApplication.md): Learn how to build an application's host program and the hardware kernel.
2. [Running Software and Hardware Emulation](./Emulation.md): Run hardware and software emulation on an application.
3. [Generating Profile and Trace Reports](./ProfileAndTraceReports.md): Learn how to generate profiling reports to better understand the performance of the application.
4. [Executing in Hardware](./HardwareExec.md): Finally, execute an application on the Alveo Data Center accelerator card.
</br>
<hr/>
<p align= center><b><a href="/README.md">Return to Main Page</a> — <a href="/docs/vitis-getting-started/">Return to Getting Started Pathway</a></b></p>
</br>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>