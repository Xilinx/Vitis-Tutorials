# Vitis AI Custom Platform Creation

In this module, we will create a custom platform to run Vitis-AI applications for ZCU104. Since ZCU104 is a validated Xilinx demo board and Vivado includes its processing system presets, we will skip step 0 mentioned in previous overview module and start to build the full hardware and generate XSA. To prepare the software components, we will import XSA into PetaLinux project and do some configurations. After building software and hardware components, we'll package the platform. At last, we'll run several test applications including Vitis-AI application on this platform to test this customized platform.

## Table of Contents

- [Vitis AI Custom Platform Creation](#vitis-ai-custom-platform-creation)
  - [Table of Contents](#table-of-contents)
  - [Custom Platform Planning](#custom-platform-planning)
  - [Step 1: Create the Vivado Hardware Design and Generate XSA](./step1.md#step-1-create-the-vivado-hardware-design-and-generate-xsa)
    - [Create Base Vivado Project from Preset](#create-base-vivado-project-from-preset)
    - [Customize System Design for Clock and Reset](#customize-system-design-for-clock-and-reset)
    - [Add Interrupt Support](#add-interrupt-support)
    - [Configuring Platform Interface Properties](#configuring-platform-interface-properties)
    - [Export Hardware XSA](#export-hardware-xsa)
  - [Step 2: Create the Software Components with PetaLinux](#step-2-create-the-software-components-with-petalinux)
    - [PetaLinux Project Settings](#petalinux-project-settings)
    - [Customize Root File System, Kernel, Device Tree and U-boot](#customize-root-file-system-kernel-device-tree-and-u-boot)
    - [Build Image and Prepare for Platform Packaging](#build-image-and-prepare-for-platform-packaging)
  - [Step 3: Create the Vitis Platform](#step-3-create-the-vitis-platform)
  - [Step 4: Test the Platform](#step-4-test-the-platform)
    - [Test 1: Read Platform Info](#test-1-read-platform-info)
    - [Test 2: Run Vector Addition Application](#test-2-run-vector-addition-application)
      - [Creating Vector Addition Application](#creating-vector-addition-application)
      - [Running Vector Addition Application](#running-vector-addition-application)
    - [Test 3: Run a DNNDK Demo](#test-3-run-a-dnndk-demo)
      - [Prepare for the DPU Kernel](#prepare-for-the-dpu-kernel)
      - [Create and Build a Vitis Application](#create-and-build-a-vitis-application)
      - [Prepare the Network Deployment File](#prepare-the-network-deployment-file)
      - [Run Application on Board](#run-application-on-board)
  - [Reference](#reference)
    - [More Information about Install and Set Vitis and XRT Environment](#more-information-about-install-and-set-vitis-and-xrt-environment)



## Custom Platform Planning

For the hardware part of Vitis AI design, DPU is integrated as RTL kernel. It's connected to the interfaces of platform and controlled by software running on ARM processor. DPU requires two clocks: clk and clk2x. We'll give 200MHz and 400MHz clocks for easy timing closure. DPU is a memory hungry IP so the platform needs to provide multiple AXI HP interfaces.

On the software side, the platform needs to provide the XRT, ZOCL packages for Vitis-AI to control DPU. ZOCL is the kernel module that talks to acceleration kernels. It needs a device tree node so it will be added. Other Vitis-AI dependencies will also be added. The above are standard Vitis platform software settings. Common image provided by Xilinx can accomplish all these features. Besides these common features, we wish to add GCC compilers to do application native compilation, add mesa-megadriver for Vitis-AI demo applications and replace the dropbear with openssh so that the network speed can be faster.

## Reference

- https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/index.html
- https://github.com/Xilinx/Vitis-AI
- https://github.com/Xilinx/Vitis_Embedded_Platform_Source
- https://github.com/Xilinx/Vitis-AI-Tutorials/tree/Vitis-AI-Custom-Platform
- https://github.com/Xilinx/Edge-AI-Platform-Tutorials/tree/3.1/docs/DPU-Integration

***Note: If you would like to try with one click creating VAI platform flow it is recommended to try with the official base platform source code for*** [zcu102_dpu](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/master/Xilinx_Official_Platforms/zcu102_base) ***and*** [zcu104_dpu](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/master/Xilinx_Official_Platforms/zcu104_base)***.***

### More Information about Install and Set Vitis and XRT Environment

- [Setting up the Vitis environment](https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/settingupvitisenvironment.html)
- [Installing Xilinx Runtime](https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/pjr1542153622642.html)

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
