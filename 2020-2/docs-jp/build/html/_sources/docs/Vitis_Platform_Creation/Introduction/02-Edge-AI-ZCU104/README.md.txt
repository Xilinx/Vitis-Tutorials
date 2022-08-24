<!--
# Copyright 2020 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
-->

# Vitis Custom Embedded Platform Creation Example on ZCU104

***Version: Vitis 2020.2 + Vitis AI 1.3***

In this module, we will create a custom Vitis embedded platform for ZCU104. It will be capable to run ***Vitis acceleration applications*** including ***Vitis-AI applications***. Of course, general ***embedded software application*** can also run on this platform.

Since ZCU104 is a validated Xilinx Evaluation Board and Vivado includes its processing system presets, we will skip step 0 mentioned in previous overview module and start to build the full hardware and generate XSA.

To prepare the software components, we will import XSA into PetaLinux project and do some configurations. After building software and hardware components, we'll package the platform. At last, we'll run several test applications including Vitis-AI application on this platform to test this customized platform.



## Custom Platform Planning

The platform we create should be able to run these applications finally

- Vitis acceleration examples like Vector Addition
- Vitis-AI applications

Vadd application requires 1x clock, 1x interrupt, 1x M_AXI for kernel control and 1x S_AXI for memory access.

Vitis-AI application would integrate DPU as RTL kernel. It would require two clocks: clk and clk2x. We can give 200MHz and 400MHz clocks for easy timing closure. One interrupt is needed as well. DPU is a memory hungry IP so the platform needs to provide multiple AXI HP interfaces.

On the software side, the platform needs to provide the XRT, ZOCL packages. Host application can use XRT OpenCL API to control Kernel. Vitis-AI software framework can control DPU with XRT. ZOCL is the kernel module that talks to acceleration kernels. It needs a device tree node so it will be added. Other Vitis-AI dependencies will also be added. Common image provided by Xilinx can accomplish all these features. Besides these common features, we wish to add GCC compilers to do application native compilation, add mesa-mega driver for Vitis-AI demo applications and replace the dropbear with openssh so that the network speed can be faster.



## Step-by-Step Tutorial

We'll introduce the platform creation steps in the following pages. Each page describes one major step in the platform creation process.

- [Step 1: Create the Vivado Hardware Design and Generate XSA](./step1.md)
- [Step 2: Create the Software Components with PetaLinux](./step2.md)
- [Step 3: Create the Vitis Platform](./step3.md)
- [Step 4: Test the Platform](./step4.md)

***Let's start from [step 1: Vivado Design](./step1.md).***



## References

- [UG1393: Vitis Acceleration Flow User Guide](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/index.html)
  - [Platform Creation General Rules](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/vcm1596051749044.html)
  - [Setting up the Vitis environment](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/settingupvitisenvironment.html)
  - [Installing Xilinx Runtime](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/pjr1542153622642.html)
- Platform Examples
  - [zcu102](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/2020.2/Xilinx_Official_Platforms/zcu102_base) and [zcu104](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/2020.2/Xilinx_Official_Platforms/zcu104_base) base platform source code in [Vitis Embedded Platform Source Github Repository](https://github.com/Xilinx/Vitis_Embedded_Platform_Source)
- [Xilinx Run Time (XRT)](https://xilinx.github.io/XRT/master/html/index.html)
- [Vitis-AI Github Repository](https://github.com/Xilinx/Vitis-AI)

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>

