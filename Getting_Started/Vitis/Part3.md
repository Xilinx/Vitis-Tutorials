# Vitis Flow 101 – Part 3 : Meet the Vector-Add Example

 The example used in this tutorial is a trivial vector-add application. The simplicity of this example allows focusing on the key concepts of FPGA acceleration without being distracted by complicated algorithmic consideration.

 

## The Source Code for the Vector-Add Kernel 

In this tutorial the hardware accelerator (also referred to as kernel) is modeled in C++. The Vitis flow also supports kernels coded in Verilog or VHDL. A example using an Verilog RTL version of the vector-add kernel can be found [here](https://github.com/Xilinx/Vitis_Accel_Examples/tree/master/rtl_kernels/rtl_vadd).

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

The pragmas are used to map function parameters to distinct kernel ports. By mapping the two inputs parameters to different input ports, the kernel will be able to read both inputs in parallel.

The Vitis online documentation provides comprehensive information on [C++ kernel coding considerations](https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/devckernels.html#rjk1519742919747) as well as a complete [pragma reference guide](https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/tfo1593136615570.html).




## The Source Code for the Host Program 

The source code for the host program is written in C/C++ and uses standard OpenCL APIs to interact with the hardware-accelerated vector-add kernel.

* Open the [`host.cpp`](./example/src/host.cpp) file located in the `src` directory of this tutorial

There are 5 main sections in the source code for this simple example.

* Section 1: local variables are declared and initialized. Of note, the input and output vectors are aligned on a 4k page boundary during allocation. On data-center platforms, this results in more for efficient management of buffers.

* Section 2: the OpenCL environment is initialized. In this section, the host detects the attached Xilinx device, loads the FPGA binary (.xclbin file) from file and programs it into the first Xilinx device it found. Then a command queue and the kernel object are created. All Vitis applications will have code very similar to the one in this section.

* Section 3: the application creates the three buffers needed to share data with the kernel: one for each input vector and one for the output. In this example, the buffers are declared with the CL_MEM_USE_HOST_PTR flag. With this flag, the buffer maintains a reference to the specific host object and uses this memory instead of allocating its own. This is more memory efficient and on data-center platforms, it eliminates a memory copy operation when host data is transferred to the device. 

```cpp
  OCL_CHECK(err, cl::Buffer buffer_in1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes, in1.data(), &err));
  OCL_CHECK(err, cl::Buffer buffer_in2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes, in2.data(), &err));
  OCL_CHECK(err, cl::Buffer buffer_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY,vector_size_bytes, out.data(), &err));
```

* Section 4: after all the declaration and initialization code, this is where the action is. The host program sets the arguments of the kernel, then schedules three operations: the transfers of the two input vectors to the device, the execution of the kernel, and lastly the transfer of the results back to the host. These operations are enqueue in the command queue previously declared. It is important to keep in my mind that these three function calls are non-blocking. The commands are put in the queue and the Xilinx Runtime library (XRT) is responsible for submitting them to the device. Because the queue used in this example is an ordered queue, these commands are guaranteed to execute in the specified order. The call to `q.finish()` is necessary to wait until all enqueued commands run to completion. 

```cpp
  OCL_CHECK(err, err = krnl_vector_add.setArg(0, buffer_in1));
  OCL_CHECK(err, err = krnl_vector_add.setArg(1, buffer_in2));
  OCL_CHECK(err, err = krnl_vector_add.setArg(2, buffer_out));
  OCL_CHECK(err, err = krnl_vector_add.setArg(3, size));

  OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_in1, buffer_in2},0/* 0 means from host*/));  
  OCL_CHECK(err, err = q.enqueueTask(krnl_vector_add));
  OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_out},CL_MIGRATE_MEM_OBJECT_HOST));
```

* Section 5: after the call to `q.finish()` returns, the results of hardware kernel are checked against the golden results and the program finishes.



This example shows the simplest way of using OpenCL APIs to interact with the hardware accelerator. As always, additional information can be found in the [Vitis documentation](https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/devhostapp.html#vpy1519742402284). 



## Next Step

Now that you understand the source code for both kernel and the host program, [**Part 4**](./Part4.md) will explain how to compile and run this example.

 

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
