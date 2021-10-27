<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2021.1 Vitis™ Getting Started Tutorial</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.2">See 2020.2 Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Vitis Flow 101 – Part 3 : Meet the Vector-Add Example

 The example used in this tutorial is a trivial vector-add application. The simplicity of this example allows focusing on the key concepts of FPGA acceleration without being distracted by complicated algorithmic consideration.

 

## The Source Code for the Vector-Add Kernel 

In this tutorial the hardware accelerator (also referred to as kernel) is modeled in C++. The Vitis flow also supports kernels coded in Verilog or VHDL. An example using a Verilog RTL version of the vector-add kernel can be found [here](https://github.com/Xilinx/Vitis_Accel_Examples/tree/master/rtl_kernels/rtl_vadd).

Using C++, the description of the hardware accelerator fits in less than 20 lines of code and can be easily and efficiently implemented in FPGA using the Vitis compiler.

```cpp
extern "C" {
  void vadd(
    const unsigned int *in1, // Read-Only Vector 1
    const unsigned int *in2, // Read-Only Vector 2
    unsigned int *out,       // Output Result
    int size                 // Size in integer
  )
  {

#pragma HLS INTERFACE m_axi port=in1 bundle=aximm1
#pragma HLS INTERFACE m_axi port=in2 bundle=aximm2
#pragma HLS INTERFACE m_axi port=out bundle=aximm1

    for(int i = 0; i < size; ++i)
    {
      out[i] = in1[i] + in2[i];
    }
  }
}
```


This simple example highlights two important aspects of C++ kernels:
1. Vitis requires C++ kernels to be declared as `extern “C”` to avoid name mangling issues
2. The results of the Vitis compilation process are controlled by the usage of pragmas in the source code. 

Other than this, the functionality of the vector-add kernel is very easily recognizable. The vadd function reads in two inputs vectors (in1 and in2) and adds them into the out vector using a simple for loop. The ‘size’ parameter indicates the number of elements in the input and output vector. 

The pragmas are used to map function parameters to distinct kernel ports. By mapping the two inputs parameters to different input ports, the kernel will be able to read both inputs in parallel. As a general rule, and without going into further details in this introductory tutorial, it is important to think about interface requirements of hardware accelerators and they will have a determining impact on maximum achievable performance.

The Vitis online documentation provides comprehensive information on [C++ kernel coding considerations](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/devckernels.html#rjk1519742919747) as well as a complete [pragma reference guide](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/hlspragmas.html#tfo1593136615570).




## The Source Code for the Host Program 

The source code for the host program is written in C/C++ and uses standard OpenCL APIs to interact with the hardware-accelerated vector-add kernel.

* Open the [`host.cpp`](./example/src/host.cpp) file located in the `src` directory of this tutorial

There are 4 main steps in the source code for this simple example.

* **Step 1:** The OpenCL environment is initialized. In this section, the host detects the attached Xilinx device, loads the FPGA binary (.xclbin file) from file and programs it into the first Xilinx device it found. Then a command queue and the kernel object are created. All Vitis applications will have code very similar to the one in this section.

* **Step 2:** The application creates the three buffers needed to share data with the kernel: one for each input and one for the output. On data-center platforms, it is more efficient to allocate memory aligned on 4k page boundaries. On embedded platforms, it is more efficient to perform contiguous memory allocation. A simple way of achieving either of these is to let the Xilinx Runtime allocate host memory when creating the buffers. This is done by using the cl::Buffer constructor to create the buffers and then mapping the allocated memory to user-space pointers.

```cpp
    // Create the buffers and allocate memory   
    cl::Buffer in1_buf(context, CL_MEM_READ_ONLY,  sizeof(int) * DATA_SIZE, NULL, &err);

    // Map host-side buffer memory to user-space pointers
    int *in1 = (int *)q.enqueueMapBuffer(in1_buf, CL_TRUE, CL_MAP_WRITE, 0, sizeof(int) * DATA_SIZE);
```

*NOTE: A common alternative is for the application to explicitly allocate host memory and reuse the corresponding pointers when creating the buffers. The approach used in this example was chosen because it is the most portable and efficient across both data-center and embedded platforms.*


* **Step 3:** The host program sets the arguments of the kernel, then schedules three operations: the transfers of the two input vectors to device memory, the execution of the kernel, and lastly the transfer of the results back to host memory. These operations are enqueued in the command queue declared in Step 1. It is important to keep in mind that these three function calls are non-blocking. The commands are put in the queue and the Xilinx Runtime is responsible for submitting them to the device. Because the queue used in the host code in this example is an ordered queue, these commands are guaranteed to execute in the specified order. However, the queue could also be an out-of-order queue in which the non-blocking calls would be executed when ready, rather than in order. The call to `q.finish()` is necessary to wait until all enqueued commands run to completion. 

```cpp
    // Set kernel arguments
    krnl_vector_add.setArg(0, in1_buf);
    krnl_vector_add.setArg(1, in2_buf);
    krnl_vector_add.setArg(2, out_buf);
    krnl_vector_add.setArg(3, DATA_SIZE);

    // Schedule transfer of inputs to device memory, execution of kernel, and transfer of outputs back to host memory
    q.enqueueMigrateMemObjects({in1_buf, in2_buf}, 0 /* 0 means from host*/); 
    q.enqueueTask(krnl_vector_add);
    q.enqueueMigrateMemObjects({out_buf}, CL_MIGRATE_MEM_OBJECT_HOST);

    // Wait for all scheduled operations to finish
    q.finish();
```

* **Step 4:** The call to `q.finish()` returns when all previously enqueued operations have completed. In this case, it implies that the output buffer containing the results of the kernel have been migrated back to host memory and can safely be used by the software application. Here the results are simply checked against expected values before the program finishes.

This example shows the simplest way of using OpenCL APIs to interact with the hardware accelerator. As always, additional information can be found in the [Vitis documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/devhostapp.html#vpy1519742402284). 



## Next Step

Now that you understand the source code for both kernel and the host program, [**Part 4**](./Part4.md) will explain how to compile and run this example.

 

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
