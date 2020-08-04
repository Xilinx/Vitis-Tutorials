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

# Getting Started with the Alveo Portfolio

## Introduction

This module directs you to the various resources available to become familiar with the Xilinx® Alveo™ Data Center accelerator cards and perform an installation.

For details on installing part of the Alveo portfolio on your server, see http://www.xilinx.com/alveo, which provides links to all the resources necessary to become familiar with the Alveo portfolio along with installation instructions on both RedHat/CentOS and Ubuntu operating systems.

With the Alveo portfolio installed on a server, you can deploy applications to be run on the host server and accelerator card. However, to develop applications, you will need to install the Vitis core development kit. The Vitis core development includes an optimized compiler for host applications, cross-compiler for the hardware kernel, a robust debugging environment, and profilers to identify performance bottlenecks and optimize the application. The Vitis core development kit does not need to be installed on the same server as the Alveo product. It can be installed on a system without any cards installed.

Alternatively, you can also develop and deploy applications on cloud servers such as [AWS](https://aws.amazon.com/ec2/instance-types/f1/) or [Nimbix](https://www.nimbix.net/alveo/). Both come with everything you need to develop, simulate, debug, and compile your hardware acceleration code.

## Installation

Before installing the Alveo portfolio, watch the [Quick Take Video](https://www.xilinx.com/cgi-bin/docs/ndoc?t=video;d=fpga/getting-started-with-alveo-u200-u250.html) for an overview of the hardware and driver software installation.

To complete installation, follow the instructions on https://www.xilinx.com/products/boards-and-kits/alveo.html. It provides the latest hardware and driver software installation procedures and allows you to deploy hardware accelerated applications using the Alveo portfolio. It also includes:

* Minimum system requirements
* List of validated servers
* Overview of the Alveo portfolio
* Card bring-up and validation steps

For developing application in-house, install the Vitis core development kit, by following the detailed installation procedures found in [Installation](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vhc1571429852245.html) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).

## Next Steps

Understand the [Essential Concepts for Building and Running the Accelerated Application](../Pathway3/README.md) using the Vitis core development kit, including building a software application, building a hardware platform, performing emulation, viewing reports, and executing in hardware.
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="./README.md">Return to Main Page</a> — <a href="/docs/vitis-getting-started/README.md">Return to Getting Started Pathway</a></b></p>
</br>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
