<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Embedded Software Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a> </td>
 </tr>
</table>

# Vitis Embedded Software Debugging Guide (UG1515)

***Version: Vitis 2024.1***

## [Debugging Bare-Metal Applications](./1-debugging-bare-metal-applications/README.md)

This section showcases the different debugging features available in the AMD Vitis Embedded software development flow for bare-metal applications. Using an example application project with intentional bugs, the debug guide goes through each issue and leverages the different windows and features available in the application debug session to highlight the potential usage of debug features in a real world scenario.

## [Debugging Linux Applications](./2-debugging-linux-applications/README.md)

The Vitis IDE provides a familiar environment for debugging Linux applications for a target platform. This section covers the following Linux application debug features:

- Setting breakpoints
- Stepping through program execution
- Viewing program variables, memory contents, and the stack

## [Creating a Debuggable First Stage Boot Loader](./3-debuggable-fsbl/README.md)

First Stage Boot Loader (FSBL) can initialize the SoC device, load the required application or data to memory, and launch applications on the target CPU core. FSBL is optimized for size because it needs to fit into OCM, which has limited space on the device. Size based optimization introduces some trouble if you wish to debug FSBL when you add customized code into FSBL. This chapter explains how you can use Vitis Unified IDE to debug FSBL step by step.

## [Cross Triggering in Vitis Unified Embedded IDE](./4-cross-triggering/README.md)

Embedded systems can be complex, that may contain multiple processors. You may also need to debug across software and hardware domains and can use PS-PL cross triggering. However, in this simple tutorial, we learn how to debug over multiple processors using cross triggering.

</hr class="sphinxhide">

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2024 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
