<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>AI Engine Runtime Parameter Reconfiguration Tutorial</h1>
   </td>
 </tr>
 <tr>
 </td>
 </tr>
</table>

## Introduction
This tutorial is designed to demonstrate how the runtime parameters (RTP) can be changed during execution to modify the behavior of AI Engine kernels. Both scalar and array parameters are supported.

__Note:__ In this tutorial, the variable "PLATFORM_REPO_PATHS" is used to find the platform installation path. Set this variable appropriately.

This tutorial targets the VCK190 ES board (see https://www.xilinx.com/products/boards-and-kits/vck190.html). This board is currently available via early access. If you have already purchased this board, download the necessary files from the lounge and ensure you have the correct licenses installed. If you do not have a board and ES license please contact your Xilinx sales contact.

## Objectives
After completing this tutorial, you will be able to:
* Specify a scalar or array parameter as part of a kernel function signature.
* Connect a parameterized kernel into a graph, exposing the parameter for runtime updates.
* Simulate a graph containing runtime parameters with AI Engine simulator (aiesimulator).
* Build an system with AI Engine kernels and PL kernels, plus PS code to control their execution.
* Use Adaptive Data Flow (ADF) API or XRT API to control graph execution and RTP operations.
* Use OpenCL API or C version XRT API or C++ version XRT API to control PL kernel execution.
* Verify the system by HW co-simulation and running in hardware.

## Steps
**Step 1**: Integrate a kernel with a scalar runtime parameter into a graph. See details in [Synchronous Update of Scalar RTP](./step1_sync_scalar.md).

**Step 2**: Mark the runtime parameter for asynchronous updates and observe the effect this has on a simulation. See details in [Asynchronous Update of Scalar RTP](./step2_async_scalar.md).

**Step 3**: Design a filter and change the array of filter coefficients at runtime, observing a change in the filter behavior. See details in [Asynchronous Update of Array RTP](./step3_async_array.md).

**Step 4**: Make a scalar input of the HLS PL kernel inside the graph as a runtime parameter. Demonstrate how to use the ADF API to control the PL kernel inside the running graph and use the XRT API to control the PL kernel outside the running graph. See details in [Asynchronous Update of Scalar RTL for PL inside a Graph, and array RTP for AI Engine Kernel](./step4_async_pl_scalar_aie_array.md).

**Step 5**: Update the AI Engine kernel with an asynchronous RTP `inout` port. Demonstrate how to use the Adaptive Data Flow (ADF) API and the XRT API to control graph execution and RTP reads. Use the C++ version of the XRT API to control the PL kernels outside the running graph. See details in [Asynchronous Array RTP Update and Read for AI Engine Kernel](./step5_async_array_update_read.md).

__Note:__ In this tutorial, a Makefile is provided. If make commands exist, you just need to run them. Detailed commands are also shown to better illustrate the concepts. You can run these commands manually.

__Hint:__ In this tutorial, the designs are self-contained in each step. You can choose to start at any step depending on your experience and requirements. Be aware that the concepts and options introduced in the previous step might not be repeated later. It is highly recommended to start from the beginning and progress to completion.


<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup><br><sup>XD001</sup></br></p>
