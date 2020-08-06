# Platform Creation Overview

## Introduction to Vitis Acceleration Platform

A platform is a starting point of your Vitis design. Vitis applications are built on top of the platforms.

Xilinx has provided base platforms for Xilinx demo boards. The platforms can be downloaded from Xilinx Download Center. Source code of base platforms can be downloaded from Xilinx github.

User can create custom embedded platforms from scratch, or modify Xilinx embedded platforms. 

There are generally four steps to create a Vitis embedded platform.

|        | Step Target                                                  | Step Details                                                 |
| ------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Step 0 | Prepare a starting point: a bootable hardware and software setting for your board. | Make sure the image can boot the board successfully and peripherals can work properly. <br />Bring up the board is not a part of Vitis embedded platform creation. It's required anyway if Vitis is not used in the development flow.<br />If it's starting from a validated board configuration, such as Xilinx demo board, this step can be omitted. |
| Step 1 | Add Hardware Interfaces for Vitis Platform                   | Add AXI interfaces for control and data access<br />Add clock and interrupt for acceleration kernels to use |
| Step 2 | Update Software Components for Vitis Platform                | Add XRT software libraries and dependencies in PetaLinux because Vitis applications use XRT to control acceleration kernels.<br />Generate PetaLinux image. |
| Step 3 | Package a Vitis Platform                                     | Use Vitis GUI or packaging command to generate platform files |

The [next module](../Module_2/README.md) provides an example to create a Vitis-AI application ready custom platform for ZCU104 board from scratch. 

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>