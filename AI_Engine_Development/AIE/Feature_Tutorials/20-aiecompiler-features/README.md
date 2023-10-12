</table>
<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# AI Engine Compiler Features

***Version: Vitis 2023.2***

## Introduction

## Introduction

The `aiecompiler` is evolving and therefore it may be difficult for you to keep up with new features. This tutorial shares a variety of features that are useful for AI Engine / AI Engine-ML (AIE-ML) programming to create more visible and efficient code compared to early versions of the compiler.

>**IMPORTANT**: Before beginning the tutorial, make sure that you have installed the AMD Vitis™ Unified Software Platform 2023.2.  

Data generation for this tutorial requires [Python 3](https://www.python.org/downloads/). The following packages are required:

- os
- sys
- numpy

All these designs will compile for AI Engine architecture (VCK190 production board using `ARCH=1` in `make` command line) and the AI Engine ML architecture (VEK280 es1 board using `ARCH=2` in `make` command line)

## Objectives

After completing this tutorial, you will be able to:

- Work with multirate design that allow the compiler to handle frame length mismatch between consecutive kernels in a kernel chain.
- Send output data to different other kernels for stream-based and buffer-based I/O.
- Conditionally instantiate graph objects.

This tutorial is based on simple data passthrough to avoid another level of complexity.


## Tutorial Sections


| Section Link | Description |
|:---|:---|
| [Conditional Objects](ConditionalObjects) | Shows how to conditionally instantiate graph objects |
| [Multirate](Multirate) | Explains how to handle mismatching frame length in between kernels |
| [Multicast](Multicast) | Provides examples on how to manipulate stream/buffer multicasting with multirate |

## Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](https://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2023 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
