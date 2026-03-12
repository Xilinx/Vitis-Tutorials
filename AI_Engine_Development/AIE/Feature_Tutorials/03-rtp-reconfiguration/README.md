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

# Runtime Parameter Reconfiguration

***Version: Vitis 2025.2***

## Introduction

This tutorial demonstrates how to use runtime parameters (RTP) during AI Engine execution to modify the parameters of AI Engine kernels. Both scalar and array parameters are supported.

>**IMPORTANT**: Before beginning the tutorial, install the AMD Vitis™ unified software platform 2025.2. This Vitis release includes all the embedded base platforms including the VCK190 base platform that this tutorial uses. Also, download the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/amd/en/downloadNav/embedded-platforms.html).

The 'common image' package contains a prebuilt Linux kernel and root file system. Use them with the AMD Versal™ board for embedded design development using Vitis tools.

Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/Common Images Dir/xilinx-versal-common-v2025.2/environment-setup-cortexa72-cortexa53-amd-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/Common Images Dir/xilinx-versal-common-v2025.2/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the `rootfs.ext4` and Image files located in the ``/Common Images Dir/xilinx-versal-common-v2025.2`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets the VCK190 production board for the 2025.2 version.

## Overview

The system infers RTP ports for each scalar or array parameter in the function argument. The following table lists the supported arguments and the corresponding RTP port.

Function Parameter | Port Class
------------ | -------------
T | Input
const T | Input
T & | Inout
const T & | Input
const T (&)[...] | Input
T(&)[...] | Inout

## Steps

The make commands to run different steps are same for all test cases in this tutorial:

To build AIE graph, run:

```bash
make aie
```

To run `aiesimulator`, run:

```bash
make aiesim
```

To run HW emulation, run:

```bash
make run_hw_emu
```

After Linux has booted, run the following commands at the Linux prompt. To exit QEMU press `Ctrl+A` and then press `x`. (this is only for HW cosim):

```bash
./host.exe a.xclbin
```

To build system design targeting HW flow, run:

```bash
make package TARGET=hw
```

To run in hardware, run the following commands at the Linux prompt after booting Linux. Login with username:`petalinux`, password:`petalinux`, then run `sudo su` with password:`petalinux` before running following commands:

  ```bash
  cd /run/media/mm*1
  ./host.exe a.xclbin
  ```

### Asynchronous Scalar RTP

For asynchronous RTP input, each iteration of the kernel tries to read the newest updated RTP value, but not synchronous to the RTP update.

The test case is [async_rtp](./async_rtp).

The kernel signature is:

```cpp
template<int32 NUM>
void vect_add(input_buffer<int32,extents<NUM>>& __restrict in,output_buffer<int32,extents<NUM>>& __restrict out,int32 value);
```

You can find the RTP specification in `aie/graph.h`:

```cpp
using namespace adf;
adf::port<adf::direction::in> value;
adf::connect<adf::parameter>(value, async(k.in[1])); 
```

Note that for RTP input, the default behavior is synchronous. So, it must specify asynchronous RTP input with keyword `async` (equivalent to `adf::async` as namespace `adf` applies).

The RTP calls for `aiesimulator` are in `aie/graph.cpp`:

```cpp
gr.run(4);
gr.update(gr.value,10);
```

From 2025.1 onward, the AI Engine compiler only supports compiling the test bench that is inside `main()` or `graph.cpp`. There is no need to compile the complete AI Engine graph if RTP values are further updated. The `--compile-testbench-only` option if used during AI Engine compilation compiles only `graph.cpp` with the updated values. For example, make changes in `graph.cpp` as

```cpp
gr.update(gr.value,20);
```

Recompile using the `compile-testbench-only` option inside the Makefile (line number 21):

```bash
v++ -c --mode aie --aie.compile-testbench-only --platform=${PLATFORM} --include="./aie" --work_dir=./Work
```

This finishes compilation quickly. From the simulation results, you can see that the RTP port values update to 20.

>**Note:** It requires at least one update of the RTP input. Otherwise, the kernel execution stalls. A way to bypass initial RTP update is to set following constraints in the graph code (`aie/graph.h`). Thus, the kernel can start execution asynchronously with the specified initial value.

```cpp
initial_value(k.in[1])=10;
```

You can find the host code for HW and HW emulation flows in `sw/host.cpp`:

```cpp
auto ghdl=xrt::graph(device,uuid,"gr");
const int ITERATION=4;
	
ghdl.run(ITERATION);
ghdl.update("gr.k.in[1]",10);
```

### Asynchronous Array RTP

An array parameter can also work similarly as scalar RTP. However, you must update the array as a whole (not partial of the array).

The test case is [async_array_rtp](./async_array_rtp). 

The kernel signature is:

```cpp
template<int32 NUM>
void vect_add(input_buffer<int32,extents<NUM>>& __restrict in,output_buffer<int32,extents<NUM>>& __restrict out,const int32 (&value)[NUM]);
```

You can find the RTP specification in `aie/graph.h`:

```cpp
using namespace adf;
adf::port<adf::direction::in> value;
adf::connect<adf::parameter>(value, async(k.in[1])); 
```

You can find the RTP calls for `aiesimulator` in `aie/graph.cpp`:

```cpp
int value[256];
gr.run(4);
gr.update(gr.value,value,256);
```

You can find the host code for HW and HW emulation flows in `sw/host.cpp`:

```cpp
int value[256];
auto ghdl=xrt::graph(device,uuid,"gr");
const int ITERATION=4;
	
ghdl.run(ITERATION);
ghdl.update("gr.k.in[1]",value);
```

### Asynchronous RTP Read

Use an asynchronous RTP port to probe AI Engine status or send minimum data without throughput requirement. 

The test case is [async_array_rtp_read](./async_array_rtp_read). 

The kernel signature is:

```cpp
template<int32 NUM>
void vect_add(input_buffer<int32,extents<NUM>>& __restrict in,output_buffer<int32,extents<NUM>>& __restrict out,const int32 (&value)[NUM], int32 &iter);
```

You can find the RTP specification in `aie/graph.h`:

```cpp
using namespace adf;
adf::port<adf::direction::in> value;
adf::port<adf::direction::inout> iter; //for RTP read
adf::connect<adf::parameter>(value, async(k.in[1])); 
adf::connect<adf::parameter>(async(k.inout[0]), iter); //for inout RTP port, default is async
```

The RTP calls for `aiesimulator` are in `aie/graph.cpp`:

```cpp
int value[256];
gr.run(TOTAL);
gr.update(gr.value,value,256);
int current_iter=-1;
gr.read(gr.iter,current_iter);
std::cout<<"Current Iteration="<<current_iter<<std::endl;
```

The host code for HW and HW emulation flows is in `sw/host.cpp`:

```cpp
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

The RTP specification is in `aie/graph.h`:

```cpp
adf::port<adf::direction::in> value;
adf::connect<adf::parameter>(value, k.in[1]); 
```

Note that for RTP input, the default behavior is synchronous. It is same as:

```cpp
adf::connect<adf::parameter>(value, sync(k.in[1]));
```

The RTP calls for `aiesimulator` are in `aie/graph.cpp`:

```cpp
gr.run(4);

//synchronous RTP value calls
gr.update(gr.value,10);
gr.update(gr.value,100);
gr.update(gr.value,1000);
gr.update(gr.value,10000);
```

The host code for HW and HW emulation flows is in `sw/host.cpp`:

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

GitHub issues are used for tracking requests and bugs. For questions go to [forums](https://adaptivesupport.amd.com/s/?language=en_US).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
