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


<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Platform Creation Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

# Versal Custom Platform Creation Tutorial

***Version: 2021.2***

In this module, we will create a custom platform to run acceleration applications for VCK190 evaluation board. 

In a general Vitis acceleration platform design, we start from building a base bootable design, then add platform required components and properties to it. We need to validate each step before going to the next step to prevent error prograting. This workflow is described in the following diagram.

![Vitis Platform Creation Steps Diagram](images/platform_creation_steps.svg)

To enable platform creators build platforms quickly, Vivado provides a Versal extensible platform example design. It adds presets for CIPS, NOC, DDR controllers and platform properties. This would make the platform creation process efficient in step 0 and step 1. Optionally, if user would like to add more components to the platform hardware design, they can be added after the example design has been created.

To prepare the software components, we will import XSA into PetaLinux project and do some additional configurations. After building software and hardware components, we'll package the platform. 

In each step, we'll test the generated files to make sure they work as expected. A frequent test methodology can help to narrow down the root causes if any error occurs. At last, we'll run several test applications on this platform to test this customized platform.

Please navigate through these steps with the following table of contents links.

- [Step 0: Create a Base Bootable Design](./step0.md)
- [Step 1: Hardware Settings for Vitis Platform](./step1.md)
- [Step 2: Create the Software Components with PetaLinux](./step2.md)
- [Step 3: Create the Vitis Platform](./step3.md)
- [Step 4: Test the Platform](./step4.md)
- [Frequently Asked Questions](faq.md)



## Requirements

This tutorial design supports VCK190 production board and VCK190 ES1 board.

- Vitis 2021.2 supports VCK190 production board. No additional licenses and settings are needed to use this board.
- Vitis 2021.2 requires additional Versal ES device installation, additional EA license setup to use VCK190 ES1 board.

This design requires a Linux host machine with Internet access. The Linux OS needs to support the Vitis software and PetaLinux. 

## Reference

- [UG1393: Vitis Acceleration Flow User Guide](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/index.html)
   - [Platform Creation General Rules](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/create_embedded_platforms.html)
- [Vitis Embedded Platform Source Github Repository](https://github.com/Xilinx/Vitis_Embedded_Platform_Source)


## Changelog

### 2021.2
- In Step 1, the Vivado project exports XSA for hardware and hardware emulation seperatedly. When creating the platform in [step3](./step3.md), it's recommended to provide seperate XSA for hardware and hardware emulation.

### 2021.1
- Step 0: Vivado example design has more customizable options for clocks and interrupts.
- Step 1: Removed tcl commands for platform setup because the Platform Setup Wizard can do the job well.
- Step 2: In PetaLinux root file system configuration, only **xrt** package is needed. **packagegroup-petalinux-xrt** is removed by PetaLinux.
- Step 2: PetaLinux can create ZOCL node device tree. So manual update device tree instructions are removed.
- Step 2: PetaLinux 2021.1 requires GCC > 6.0. Workaround is to enable "Enable buildtools extended" in Yocto settings.
- Step 3: Vitis can generate BIF. Manual steps of adding custom BIF are removed.


<p align="center"><sup>Copyright&copy; 2021 Xilinx</sup></p>

