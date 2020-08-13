<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">See 2019.2 Vitis Application Acceleration Development Flow Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 2. Coding My First Host Program

After coding the kernel, you will now learn about the host application. The host application is written in either C or C++ using OpenCL APIs calls to interact with the FPGA accelerators.

An example host code is provided in the `./reference-files/src` folder, which shows both the C++ version and the C version. In this tutorial, you will only be looking at the C++ version.

- The C++ version of the host code can be found in the `host.cpp` file.
- The C version of the host code can be found in the `host.c` file.

In general, you can divide the structure of the host application into three sections:

1. Setting up the hardware.
2. Executing the kernels.
3. Releasing the hardware resources after the kernel returns.

As you go through this tutorial, you will be looking at each step individually.

## Setting up the Hardware

The application must start by setting up and initializing the FPGA. This typically involves the following steps:

- Retrieving the list of available Xilinx platforms.
- Retrieving the list of devices supported by each Xilinx platform.
- Creating a context.
- Creating a program object from the pre-compiled FPGA binary (`xclbin`).
- Creating a kernel object.

As you work through this section, refer to step 1 in the `host.cpp` file.

>**TIP:** This lab references the C++ code, but C code is also provided in the reference files. For more information on specific OpenCL API calls listed here, refer to the [OpenCL Reference Pages](https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/).

1. The application should start by identifying the platforms composed of Xilinx FPGA devices. To identify the presence of Xilinx platforms, you should use the `cl::Platform::get` OpenCL API. This call returns the available platforms in the system.

    ```Cpp
    cl::Platform::get(&platforms)
    ```

    After retrieving the available platform, the host will verify the platform for a particular vendor. Because each platform contains a vendors' installation, a system can have a mix of platforms. The `cl::platform.getInfo` API call will return the specific information about the available OpenCL platform. In this host code, you will retrieve the platform vendor information to verify it with the user input, `XILINX`.

    ```Cpp
     platform.getInfo<CL_PLATFORM_NAME>(&err)
    ```

    Now the host needs to select a specific device from the respective platform. This is done using the `cl::platform::getDevices` API.

    ```Cpp
    platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices)
    ````

   >**NOTE:** In the current C++ code, the above operations are combined and kept inside a user-defined `get_devices("Xilinx")` function defined in the `host.hpp` file.
   >
   >```Cpp
   >get_devices("Xilinx")
   >```

2. After you select the platform and the device, you need to create a context. The context is used by the runtime to manage objects, such as command-queues and kernel objects. The context is created using the `cl::Context` OpenCL API.

   ```Cpp
   cl::Context context(device, NULL, NULL, NULL, &err))
   ```

3. After creating a context, you create a command-queue. The application will place commands in this queue for actions like transferring data, executing kernels and synchronizing. These commands are then scheduled on the devices within the context. The command queue is created using the `cl::CommandQueue` OpenCL API.

   ```Cpp
   cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err)
   ```

4. Next, you need to create a program object. The program object is created from the pre-compiled FPGA binary file (`xclbin`). It contains the collection of user-defined kernel functions and is programed onto the FPGA.

   >**TIP:** The `xclbin` is the compiled kernel binary created, as explained in [Building an Application](../Pathway3/BuildingAnApplication.md).

   First, the application needs to read the contents of `xclbin` file. In this tutorial, you will use a user-defined function, `read_binary_file`, to achieve this. This function returns a pointer to the contents of `xclbin` file.

   ```Cpp
   fileBuf = read_binary_file(binaryFile, fileBufSize)
   ```

   Then, create a `cl::Program::Binaries` object to store the contents of the `xclbin` binary file.

   ```Cpp
   cl::Program::Binaries bins{{fileBuf, fileBufSize}}
   ```

   Lastly, create the program object and initialize it with the contents of `xclbin` binary stored in the bins variable. To do this, use the `cl::Program program` API.

    ```Cpp
    cl::Program program(context, devices, bins, NULL, &err)
    ```

    This step programs the FPGA with the binary loaded in the bins variable. If successful, this function will return `CL_SUCCESS`; make sure to check the return code.

5. Next, you must create kernel objects. Kernel objects are handles which the software application will use to pass arguments to the actual hardware kernels and execute them. Kernel objects are created using the `cl::Kernel` API.

      ```Cpp
      cl::Kernel krnl_vector_add(program,"vadd", &err)
      ```

   > **NOTE**: The mentioned operations are common to most applications and can be reused.

## Executing the Kernel

Now that you set up the hardware, the host application is ready to issue commands to the device and interact with the kernel. These commands include:

- Buffer transfer to/from the FPGA
- Setting up the kernel arguments
- Kernel execution on the FPGA
- Event Synchronization

As you work through this section, refer to step 2 in the `host.cpp` file.

1. First, you must create buffers in the global memory. Buffers are used to transfer data back and forth between the host and the device. Kernels will read, process, and write back data in these buffers. Buffer objects are created using the `cl::Buffer` API.

      ```Cpp
      cl::Buffer buffer_in1 (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes, source_in1.data(), &err)
      ```

      You will create the following buffers:

     - `buffer_in1`: Stores source_in1
     - `buffer_in2`: Stores source_in2
     - `buffer_output`: Stores results (source_hw_results)

2. Before executing the kernel, you need to set each of its arguments. Kernel arguments are either scalar values or buffer objects. Kernel arguments are set using the `cl::Kernel::setArg` API.

      ```Cpp
      krnl_vector_add.setArg(0, buffer_in1)
      ```

    This will pass to the kernel the pointers where the input data is located, where the output should be stored, and what is the size of each buffer. The following arguments are set:

    - `in1` (input): Input Vector1
    - `in2` (input): Input Vector2
    - `out` (output): Output Vector
    - `size` (input): Size of Vector in Integer

3. Next, request the transfer of input data from the host memory to the device memory (global memory) using the `cl::CommandQueue::enqueueMigrateMemObjects` API.

      ```Cpp
      q.enqueueMigrateMemObjects({buffer_in1, buffer_in2},0/* 0 means from host*/)
      ```

4. Now, request the execution of the kernel using the `cl::CommandQueue::enqueueTask` API.

    ```Cpp
    q.enqueueTask(krnl_vector_add)
    ```

5. Then, request the transfer of the output results from the device global memory to the host memory. To do this, use the `cl::CommandQueue::enqueueMigrateMemoryObjects` API.

    ```Cpp
    q.enqueueMigrateMemObjects({buffer_output},CL_MIGRATE_MEM_OBJECT_HOST)
    ```

6. Finally, wait for all the requests placed in the command queue to complete.

    ```Cpp
    q.finish();
    ```

    It is important to understand that an "enqueue" API call does not actually execute the specified command; it only requests its execution. When an "enqueue" function returns, it does not mean that that the command has actually been executed. It is up to the runtime to schedule the execution of the command. Therefore, the application must use synchronization methods to know when commands have completed.

## Releasing the FPGA After the Application Returns

The last step in building the host application is releasing the objects. As you work through this section, refer to step 3 in the `host.cpp` file. The C++ wrapper releases the objects automatically once the object passes out of scope.

## Next Step

The next step in this tutorial is to [compile, link, and run the application and the kernel](./building_application.md).
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">Return to Getting Started Pathway</a> — <a href="./README.md">Return to Start of Tutorial</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
