<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# A to Z AI Engine Bare-metal Flow for VEK385

***Version: Vivado / Vitis 2026.1***

***Supported Boards: VEK385 Rev. A, VEK385 Rev.B***

This tutorial introduces a complete end-to-end flow for a bare-metal host application using AI Engines and PL kernels. The tutorial is based on the 2026.1 Vivado and Vitis unified software platform.

In this tutorial, you will learn how to create an HW extensible platform for the VEK385 in Vivado using a Configurable Example Design. You will then use this platform in the Vitis Unified IDE to create a simple AIE-MLv2 application that will be connected to the HW extensible platform using HLS kernels.
You will then run this design on the VEK385 board by booting from JTAG using the Vitis Unified IDE.

>Note: The AI Engine application shown in this tutorial is targeting DSP design. For ML application, please contact your FAE for availability for the Vitis AI tools.

The flow is broken into four different steps shown in the following diagram.

![missing image](images/diagram_22.1.png)

[Start with Custom Platform Creation](./01-custom_base_platform_creation.md)


<p class="sphinxhide" align="center"><sub>Copyright © 2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
