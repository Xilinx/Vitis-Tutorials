<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Platform Creation Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

# Vitis Custom Embedded Platform Creation Example on ZCU104

***Version: Vitis 2023.2 and Vivado 2023.2***


In this module, you will create a custom Vitis embedded platform for ZCU104 capable of running ***Vitis acceleration applications***. Additionally, this platform is versatile enough to support general **embedded software applications**. The diagram below illustrates the structure of this example system.

![missing image](./images/structure.svg)

Since ZCU104 is a validated AMD Evaluation Board and the AMD Vivado™ Design Suite includes its processing system presets, you can skip validating the board and start to build the full hardware and generate XSA.

To prepare the software components, follow these steps:

1. Use the common image and create the DTB file according to the XSA file exported from step1.
2. Package the platform.
3. Run several test applications including Vitis AI applications on this customized platform to test it.

## Custom Platform Planning

The platform should be able to run the following applications:

- Vitis acceleration examples like Vector Addition
- Vitis AI applications

Vadd application requires 1x clock, 1x interrupt, 1x M_AXI for kernel control, and 1x S_AXI for memory access.

On the software side, the platform should provide XRT and ZOCL packages. The host application can use the XRT OpenCL API to control the kernel. ZOCL is the kernel module that communicates with acceleration kernels. It needs a device tree node, so it should be added. The common image provided by AMD is loaded with these features. Besides these common features, you can also add GCC compilers for application native compilation. Additionally, you can replace the dropbear with openssh so that the network speed can be faster.

## Step-by-Step Tutorial

The following sections will introduce the platform creation steps. Each section describes one major step in the platform creation process.

- [Step 1: Create the Hardware Platform](./step1.md)</br>
            Use Vivado tools to generate an XSA file which contains the hardware information for the platform
- [Step 2: Create the Vitis Software Platform](./step2.md)</br>
            Prepare software components and create the Vitis platform
- [Step 3: Create Vitis Application](./step3.md)</br>
            Create a simple application to validate the Vitis platform

- [Iteration Guidelines](./Iteration_guideline.md)</br>
            Iteration steps and guidelines
            
***Start from [step 1: Create the Hardware Platform](./step1.md).***

## Change Log
### 2023.2
- Vitis Unified IDE becomes the default IDE.
### 2022.2

- `createdts` add user's device tree file support

### 2022.1

- From this version, the pre-built Linux common image is used instead of building Linux components with PetaLinux because the pre-built common image provides the most common features for Vitis acceleration and it can help expedite the platform creation process. If you wish to leverage the PetaLinux tool to customize your system, refer to the [PetaLinux Customization Tutorial](../../Feature_Tutorials/02_petalinux_customization/README.md).
- Simplify the device tree generation flow by using the newly introduced `createdts` XSCT command.

### 2021.2

- The Vivado project exports the XSA for hardware and hardware emulation separately. When creating the platform in [step3](./step3.md), it is recommended that you provide separate XSA for hardware and hardware emulation.

## References

- [UG1393: Vitis Acceleration Flow User Guide](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration)
  - [Platform Creation General Rules](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Creating-Embedded-Platforms-in-Vitis)
  - [Setting up the Vitis environment](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Setting-Up-the-Environment-to-Run-the-Vitis-Software-Platform)
- Platform Examples
  - [zcu102](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/2023.2/Xilinx_Official_Platforms/xilinx_zcu102_base) and [zcu104](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/2023.2/Xilinx_Official_Platforms/xilinx_zcu104_base) base platform source code in [Vitis Embedded Platform Source Github Repository](https://github.com/Xilinx/Vitis_Embedded_Platform_Source)
- [Vitis AI Github Repository](https://github.com/Xilinx/Vitis-AI)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
