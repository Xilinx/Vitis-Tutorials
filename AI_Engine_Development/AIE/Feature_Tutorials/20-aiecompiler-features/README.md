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

# AI Engine Compiler Features

***Version: Vitis 2026.1***
## Introduction

The `aiecompiler` is evolving. This tutorial shares a variety of features that are useful for AI Engine / AI Engine-ML (AIE-ML) programming to create more visible and efficient code compared to early versions of the compiler.

>**IMPORTANT**: Before beginning the tutorial, make sure that you have installed the AMD Vitis™ Unified Software Platform 2026.1.  

Data generation for this tutorial requires [Python 3](https://www.python.org/downloads/). You need the following packages:

- os
- sys
- numpy

All these designs will compile for AI Engine architecture (VCK190 production board using `ARCH=aie` in `make` command line) and the AI Engine ML architecture (VEK280 board using `ARCH=aie-ml` in `make` command line).

## Objectives

After completing this tutorial, you can:

- Work with multirate designs that allow the compiler to handle frame length mismatch between consecutive kernels in a kernel chain.
- Send output data to different other kernels for stream-based and buffer-based I/O.
- Conditionally instantiate graph objects.

This tutorial uses simple data passthrough to avoid another level of complexity.

## Tutorial Sections

| Section Link | Description |
|:---|:---|
| [Conditional Objects](ConditionalObjects) | Shows how to conditionally instantiate graph objects |
| [Multirate](Multirate) | Explains how to handle mismatching frame length between kernels |
| [Multicast](Multicast) | Provides examples on how to manipulate stream/buffer multicasting with multirate |

## Support

GitHub issues are used for tracking requests and bugs. For questions, go to [support.amd.com](https://adaptivesupport.amd.com/s/topiccatalog?language=en_US).

<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2023–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
