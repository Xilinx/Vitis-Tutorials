<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Runtime Parameter Reconfiguration

***Version: Vitis 2023.2***

## Introduction

This tutorial is designed to demonstrate how the runtime parameters (RTP) can be used during AIE execution to modify the parameters of AI Engine kernels. Both scalar and array parameters are supported.

>**IMPORTANT**: Before beginning the tutorial make sure you have installed the AMD Vitis™ 2023.2 software. The Vitis release includes all the embedded base platforms including the VCK190 base platform that is used in this tutorial. In addition, ensure that you have downloaded the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

The ‘common image’ package contains a prebuilt Linux kernel and root file system that can be used with the AMD Versal™ board for embedded design development using Vitis tools.

Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/Common Images Dir/xilinx-versal-common-v2023.2/environment-setup-cortexa72-cortexa53-xilinx-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/Common Images Dir/xilinx-versal-common-v2023.2/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the `rootfs.ext4` and Image files located in the ``/Common Images Dir/xilinx-versal-common-v2023.2`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets the VCK190 production board for the 2023.2 version.

## Overview

RTP ports are inferred for each scalar or array parameter in the function argument. The following table lists the supported arguments and the corresponding RTP port.

Function Parameter | Port Class
------------ | -------------
T | Input
const T | Input
T & | Inout
const T & | Input
const T (&)[ …] | Input
T(&)[…] | Inout

## Steps

The make commands to run different steps are same for all test cases in this tutorial:

To build AIE graph, run:

```
make aie
```

To run `aiesimulator`, run:
	
```
make aiesim
```

To run HW emulation, run:

```
make run_hw_emu
```

After Linux has booted, run the following commands at the Linux prompt. To exit QEMU press `Ctrl+A` and then press `x`. (this is only for HW cosim):

```bash
./host.exe a.xclbin
```

To build system design targeting HW flow, run:

```
make package TARGET=hw
```

To run in hardware, after booting Linux, run the following commands at the Linux prompt. (login with user name:`petalinux`, password:`petalinux`. run `sudo su` with password:`petalinux` before running following commands):

```bash
cd /run/media/mm*1
./host.exe a.xclbin
```

### Asynchronous Scalar RTP

For asynchronous RTP input, each iteration of the kernel tries to read the newest updated RTP value, but not synchronous to the RTP update.   

The test case is [async_rtp](./async_rtp). 

The kernel signature is:

```
template<int32 NUM>
void vect_add(input_buffer<int32,extents<NUM>>& __restrict in,output_buffer<int32,extents<NUM>>& __restrict out,int32 value);
```

The RTP specification can be found in `aie/graph.h`:

```
using namespace adf;
adf::port<adf::direction::in> value;
adf::connect<adf::parameter>(value, async(k.in[1])); 
```

Note that for RTP input, the default behavior is synchronous. So, it has to specify asynchronous RTP input with keyword `async` (equivalent to `adf::async` as namespace `adf` is used).

The RTP calls for `aiesimulator` can be found in `aie/graph.cpp`:

```
gr.run(4);
gr.update(gr.value,10);
```

**Note:** It requires at least one update of the RTP input. Otherwise, the kernel execution will be stalled. A way to bypass initial RTP update is to set following constraints in the graph code (`aie/graph.h`). Thus, kernel can start execution asynchronously with the specified initial value.

```
initial_value(k.in[1])=10;
```

The host code for HW and HW emulation flows can be found in `sw/host.cpp`:

```
auto ghdl=xrt::graph(device,uuid,"gr");
const int ITERATION=4;
	
ghdl.run(ITERATION);
ghdl.update("gr.k.in[1]",10);
```

### Asynchronous Array RTP

An array parameter can also work similarly as scalar RTP. Only that the array should be updated as a whole (not partial of the array).

The test case is [async_array_rtp](./async_array_rtp). 

The kernel signature is:

```
template<int32 NUM>
void vect_add(input_buffer<int32,extents<NUM>>& __restrict in,output_buffer<int32,extents<NUM>>& __restrict out,const int32 (&value)[NUM]);
```

The RTP specification can be found in `aie/graph.h`:

```
using namespace adf;
adf::port<adf::direction::in> value;
adf::connect<adf::parameter>(value, async(k.in[1])); 
```

The RTP calls for `aiesimulator` can be found in `aie/graph.cpp`:

```
int value[256];
gr.run(4);
gr.update(gr.value,value,256);
```

The host code for HW and HW emulation flows can be found in `sw/host.cpp`:

```
int value[256];
auto ghdl=xrt::graph(device,uuid,"gr");
const int ITERATION=4;
	
ghdl.run(ITERATION);
ghdl.update("gr.k.in[1]",value);
```

### Asynchronous RTP Read

An asynchronous RTP port can be used to probe AIE status or send minimum data without throughput requirement. 

The test case is [async_array_rtp_read](./async_array_rtp_read). 

The kernel signature is:

```
template<int32 NUM>
void vect_add(input_buffer<int32,extents<NUM>>& __restrict in,output_buffer<int32,extents<NUM>>& __restrict out,const int32 (&value)[NUM], int32 &iter);
```

The RTP specification can be found in `aie/graph.h`:

```
using namespace adf;
adf::port<adf::direction::in> value;
adf::port<adf::direction::inout> iter; //for RTP read
adf::connect<adf::parameter>(value, async(k.in[1])); 
adf::connect<adf::parameter>(async(k.inout[0]), iter); //for inout RTP port, default is async
```

The RTP calls for `aiesimulator` can be found in `aie/graph.cpp`:

```
int value[256];
gr.run(TOTAL);
gr.update(gr.value,value,256);
int current_iter=-1;
gr.read(gr.iter,current_iter);
std::cout<<"Current Iteration="<<current_iter<<std::endl;
```

The host code for HW and HW emulation flows can be found in `sw/host.cpp`:

```
int value[256];
auto ghdl=xrt::graph(device,uuid,"gr");
	
ghdl.run(ITERATION);
ghdl.update("gr.k.in[1]",value);
int current_iter=-1;
ghdl.read("gr.k.inout[0]",current_iter);
std::cout<<"Current Iteration="<<current_iter<<std::endl;
```

### Synchronous RTP

Kernel execution depends on the availability of the buffer of data on their inputs. For synchronous RTP input, each iteration of the kernel requires one RTP update on the RTP input. 

The test case is [sync_rtp](./sync_rtp). 

The kernel signature is:

```
template<int32 NUM>
void vect_add(input_buffer<int32,extents<NUM>>& __restrict in,output_buffer<int32,extents<NUM>>& __restrict out,int32 value);
```

The RTP specification can be found in `aie/graph.h`:

```
adf::port<adf::direction::in> value;
adf::connect<adf::parameter>(value, k.in[1]); 
```

Note that for RTP input, the default behavior is synchronous. It's same as:

```
adf::connect<adf::parameter>(value, sync(k.in[1]));
```

The RTP calls for `aiesimulator` can be found in `aie/graph.cpp`:

```
gr.run(4);

//synchronous RTP value calls
gr.update(gr.value,10);
gr.update(gr.value,100);
gr.update(gr.value,1000);
gr.update(gr.value,10000);
```

The host code for HW and HW emulation flows can be found in `sw/host.cpp`:

```
auto ghdl=xrt::graph(device,uuid,"gr");
const int ITERATION=4;
	
ghdl.run(ITERATION);
ghdl.update("gr.k.in[1]",10);
ghdl.update("gr.k.in[1]",100);
ghdl.update("gr.k.in[1]",1000);
ghdl.update("gr.k.in[1]",10000);
```

## Summary

After completing this tutorial, you learned:

* Specify a scalar or array parameter as part of a kernel function signature.
* Connect a parameterized kernel into a graph, exposing the parameter for runtime updates.
* Simulate a graph containing runtime parameters with AI Engine simulator (aiesimulator).
* Build a system with AI Engine kernels and PL kernels, plus PS code to control their execution.
* Use XRT API to control graph execution and RTP operations.
* Verify the system by HW co-simulation and running in hardware.

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
