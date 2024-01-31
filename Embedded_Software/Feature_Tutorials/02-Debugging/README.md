# UG1515: Vitis Embedded Software Debugging Guide

***Version: Vitis 2023.2***

## Debugging Bare-Metal Applications

This section showcases the different debugging features available within the Vitis™ Embedded software development flow for bare-metal applications. Using an example application project with intentional bugs, the debug guide goes through each issue and leverages the different windows and features available in the application debug session to highlight the potential usage of debug features in a real world scenario.

## Debugging Linux Applications

The Vitis™ IDE provides a familiar environment for debugging Linux applications for a target platform. This section covers the following Linux application debug features:

- Setting breakpoints
- Stepping through program execution
- Viewing program variables, memory contents, and the stack 

## Creating a Debuggable First Stage Boot Loader

First Stage Boot Loader (FSBL) can initialize the SoC device, load the required application or data to memory, and launch applications on the target CPU core. FSBL is optimized for size because it needs to fit into OCM, which has limited space on the device. Size based optimization introduces some trouble if you wish to debug FSBL when you add customized code into FSBL. This chapter explains how you can use Vitis Unified IDE to debug FSBL step by step.

## Cross Triggering in Vitis Unified Embedded IDE

Embedded systems can be complex, that may contain multiple processors. Users may also need to debug across software and hardware domains and can use PS-PL cross triggering. However, In this simple tutorial, we will learn how we can debug over multiple processors using cross triggering.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2024 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>