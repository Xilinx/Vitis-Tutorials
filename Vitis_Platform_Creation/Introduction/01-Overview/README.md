<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Platform Creation Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

# Platform Creation Overview

## Introduction to Vitis Acceleration Platform

A platform is a starting point of your Vitis design. Vitis applications are built on top of the platforms. Acceleration applications need acceleration platforms. They can be embedded platforms or DC platforms. Software development applications can be built on software development platforms or embedded acceleration platforms. This document focuses on embedded acceleration platform creation.

Xilinx has provided base platforms for Xilinx Evaluation Boards. The platforms can be downloaded from [Xilinx Download Center](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html). Source code of base platforms can be downloaded from Xilinx [github](https://github.com/Xilinx/Vitis_Embedded_Platform_Source).

User can create custom embedded acceleration platforms by building from scratch, or modifing Xilinx embedded platforms. This document focuses on building from scratch workflow.

There are generally three steps to create a Vitis embedded platform: Setup hardware interface, setup software components and package the platform. The whole flow would require some preparation and final testing.

![missing image](./images/steps_overview.png)

Here's a brief description of what are covered in each step.

|       | Step Target                                                                        | Step Details                                                                                                                                                                                                                                                                                                                                                   |
| ----- | ---------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Step0 | Prepare a starting point: a bootable hardware and software setting for your board. | Make sure the image can boot the board successfully and peripherals can work properly. <br />Bring up the board is not a part of Vitis embedded platform creation. It's required anyway if Vitis is not used in the development flow.<br />If it's starting from a validated board configuration, such as a Xilinx Evaluation Board, this step can be omitted. |
| Step1 | Add Hardware Interfaces for Vitis Platform                                         | Add AXI interfaces for control and data access<br />Add clock and interrupt for acceleration kernels to use                                                                                                                                                                                                                                                    |
| Step2 | Update Software Components for Vitis Platform                                      | Add XRT software libraries and dependencies in PetaLinux because Vitis applications use XRT to control acceleration kernels.<br />Generate PetaLinux image.                                                                                                                                                                                                    |
| Step3 | Package a Vitis Platform                                                           | Use Vitis GUI or XSCT packaging command to generate platform files                                                                                                                                                                                                                                                                                             |
| Step4 | Platform Verification                                                              | Test the platform with various test cases to make sure it works as expected.                                                                                                                                                                                                                                                                                   |

Detailed requirements of these steps are described in [UG1393](https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/rjs1596051748503.html).

The tutorials in this document will divide the chapters to follow these steps.

The [next module](../02-Edge-AI-ZCU104/README.md) provides an example to create a custom platform for ZCU104 board from scratch.

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
