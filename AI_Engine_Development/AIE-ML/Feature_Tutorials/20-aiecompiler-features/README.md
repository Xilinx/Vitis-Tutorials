<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# AI Engine Compiler Features

***Version: Vitis 2024.1***


## Introduction

This tutorial targets AIE-ML device, but shares test cases and description files with [AIE 20-aiecompiler-features](../../../AIE/Feature_Tutorials/20-aiecompiler-features).

>**IMPORTANT**: Before beginning the tutorial make sure you have installed the AMD Vitis™ 2024.1 software. The Vitis release includes all the embedded base platforms including the VEK280 base platform that is used in this tutorial. 

Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets VEK280 board for 2024.1/ version.

## Objectives

After completing this tutorial, you will be able to:

- Work with multirate design that allow the compiler to handle frame length mismatch between consecutive kernels in a kernel chain.
- Send output data to different other kernels for stream-based and buffer-based I/O.
- Conditionally instantiate graph objects.

This tutorial is based on simple data passthrough to avoid another level of complexity.

## Tutorial Sections

This tutorial contains 3 different sections that are independant one from each other.

Follow instructions in [AIE Compiler Features](../../../AIE/Feature_Tutorials/20-aiecompiler-features/Readme.md). But, **run all make commands with `ARCH=aie-ml`**. 

### Conditional Objects

The following examples illustrate various ways to generate conditional objects at the graph level. All examples are available in different source directories: `src1` to `src4`. They can be compiled using: `make CASE=N clean data aie aieviz` with `N= 1,2,3,4`. Type `make help` to get the list of all available examples.

All these examples can be compiled for the AI Engine ML architecture by including `ARCH=aie-ml` in the `make` command.

#### Case 1

To get the visualization type:

```make ARCH=aie-ml CASE=1 clean data aie aieviz```

#### Case 2

To get the visualization type:

```make ARCH=aie-ml CASE=2 aie aieviz```

#### Case 3

To get the visualization type:

```make ARCH=aie-ml CASE=3 aie aieviz```

#### Case 4

To get the visualization type:

```make ARCH=aie-ml CASE=4 aie aieviz```

### Multirate

#### UpConv then DownConv (Buffer)

```BASH
make VERSION=1 ARCH=aie-ml buffer aie
```

You can run the simulation and visualize the result:

```BASH
make aiesim aieviz
```

#### DownConv then UpConv (Buffer)

In this design the first kernel in the dataflow is the down-converter 5/7 followed by the up-converter.

```BASH
make ARCH=aie-ml VERSION=2 clean buffer aie aiesim aieviz
```

#### Split and Merge (Buffer)

In this new example, two branches are fed with the same PLIO and connected on the output side to a kernel that will compute the difference between the two branch outputs.


```BASH
make ARCH=aie-ml VERSION=3 clean buffer aie
```
Let run the simulation and visualize:

```BASH
make aiesim aieviz
```

#### UpConv then DownConv (Stream)

```BASH
make ARCH=aie-ml VERSION=1 clean stream aie aiesim aieviz
```

In this configuration, the repetition rates are five (UpConv) and seven (DownConv). The simulation duration is slightly less 40 µs.

#### DownConv then UpConv (Stream)

```BASH
make ARCH=aie-ml VERSION=2 clean stream aie aiesim aieviz
```

In this configuration, the repetition rates are seven (DownConv) and five (UpConv). The simulation duration is slightly more than 40 µs.

#### Split and Merge (stream)

In this new example, two branches are fed with the same PLIO and connected on the output side to a kernel that will compute the difference between the two branch outputs.

`VERSION=3` of this design stalls almost immediately because this design needs FIFOs set up at the input and output of each branch.

`VERSION=4` sets these FIFOs and the overall simulation lasts approximately 275 µs.

### Multicast

#### Case 1: Stream and Buffer Multicasting

To explore the graph view and the array view shown by Vitis Analyzer:

```make ARCH=aie-ml CASE=1 clean data aie aieviz```

#### Case 2: Multirate Buffer Multicasting

Explore the graph view and the array view shown by the AMD Vitis™ Analyzer:

```make ARCH=aie-ml CASE=2 clean data aie aieviz```

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2024 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
