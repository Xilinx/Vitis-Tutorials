<table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Application Acceleration Development Flow Tutorials</h1>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Vitis Getting Started Tutorial

## Part 3: Review the Kernel Code and Host Application

 The example used in this tutorial is a trivial vector-add application. The simplicity of this example allows focusing on the key concepts of FPGA acceleration without being distracted by complicated algorithmic consideration.

### The Source Code for the Vector-Add Kernel

In this tutorial, the hardware accelerator (also referred to as kernel) is modeled in C++. The Vitis flow also supports kernels coded in Verilog or VHDL. An example using a Verilog RTL version of the vector-add kernel can be found [here](https://github.com/Xilinx/Vitis_Accel_Examples/tree/master/rtl_kernels/rtl_vadd).

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

Other than this, the functionality of the vector-add kernel is very easily recognizable. The vadd function reads in two inputs vectors (`in1` and `in2`) and adds them into the `out` vector using a simple for loop. The `size` parameter indicates the number of elements in the input and output vector.

The pragmas are used to map function parameters to distinct kernel ports. By mapping the two inputs parameters to different input ports, the kernel will be able to read both inputs in parallel. As a general rule, and without going into further details in this introductory tutorial, it is important to think about interface requirements of hardware accelerators and they will have a determining impact on maximum achievable performance.

The Vitis online documentation provides comprehensive information on [C/C++ Kernels](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Developing-PL-Kernels-using-C) as well as a complete [HLS Pragmas](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/HLS-Pragmas) reference guide.

### The Source Code for the Host Program

The source code for the host program is written in C/C++ and uses the native XRT APIs to interact with the hardware-accelerated vector-add kernel.

* Open the [`host.cpp`](./example/src/host.cpp) file located in the `src` directory of this tutorial

There are four main steps in the source code for this simple example.

* **Step 1:** The runtime environment is initialized. In this section, the host detects the attached Xilinx device, loads the FPGA binary (.xclbin file) from file and programs it into the first AMD device it found. Then, the kernel object is created. All Vitis applications will have code very similar to the one in this section.

* **Step 2:** The application creates the three buffers needed to share data with the kernel: one for each input and one for the output. On data-center platforms.

```cpp
    std::cout << "Allocate Buffer in Global Memory\n";
    auto boIn1 = xrt::bo(device, vector_size_bytes, krnl.group_id(0)); //Match kernel arguments to RTL kernel
    auto boIn2 = xrt::bo(device, vector_size_bytes, krnl.group_id(1));
    auto boOut = xrt::bo(device, vector_size_bytes, krnl.group_id(2));

    // Map the contents of the buffer object into host memory
    auto bo0_map = boIn1.map<int*>();
    auto bo1_map = boIn2.map<int*>();
    auto bo2_map = boOut.map<int*>();
    std::fill(bo0_map, bo0_map + DATA_SIZE, 0);
    std::fill(bo1_map, bo1_map + DATA_SIZE, 0);
    std::fill(bo2_map, bo2_map + DATA_SIZE, 0);
```

>**NOTE:** A common alternative is for the application to explicitly allocate host memory, and reuse the corresponding pointers when creating the buffers. The approach used in this example was chosen because it is the most portable and efficient across both data center and embedded platforms.

* **Step 3:** The host program sets the arguments of the kernel, then schedules three operations: the transfers of the two input vectors to device memory, the execution of the kernel, and lastly, the transfer of the results back to host memory. These operations are enqueued in the command queue declared in Step 1. It is important to keep in mind that these three function calls are non-blocking. The commands are put in the queue and the XRT is responsible for submitting them to the device. Because the queue used in the host code in this example is an ordered queue, these commands are guaranteed to execute in the specified order. However, the queue could also be an out-of-order queue in which the non-blocking calls would be executed when ready, rather than in order. The call to `q.finish()` is necessary to wait until all enqueued commands run to completion.

  ```cpp
      // Synchronize buffer content with device side
      std::cout << "synchronize input buffer data to device global memory\n";
      boIn1.sync(XCL_BO_SYNC_BO_TO_DEVICE);
      boIn2.sync(XCL_BO_SYNC_BO_TO_DEVICE);

      std::cout << "Execution of the kernel\n";
      auto run = krnl(boIn1, boIn2, boOut, DATA_SIZE); //DATA_SIZE=size
      run.wait();

      // Get the output;
      std::cout << "Get the output data from the device" << std::endl;
      boOut.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  ```

* **Step 4:** The `run.wait()` returns when the kernel has completed. At that time, the output buffer containing the results of the kernel are migrated back to host memory and can safely be used by the software application. Here the results are simply checked against expected values before the program finishes.

This example shows the simplest way of using XRT API to interact with the hardware accelerator.

## Next Step

  **Click here to [Build and Run the Data Center Application](./Part4-data_center.md)**

  **Click here to [Build and Run the Embedded Processor Application](./Part4-embedded_platform.md)**

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
