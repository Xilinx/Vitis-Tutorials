<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™Hardware Acceleration Introduction Tutorial</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

## Overview

The FPGA image that we have loaded contains a very simple vector addition core. It takes two buffers of arbitrary length as inputs and produces a buffer of equal length as an output. As the name implies, during the process, it adds them together.

Our code has not really been optimized to run well in an FPGA. It is mostly equivalent to directly putting the algorithm in the folowing listing directly into the FPGA fabric. This is not particularly efficient. The implementation tools will do some optimization for us, but we can make it better. With this code, we can process one addition operation on each tick of the clock, but we are still only processing one 32-bit output at
a time.

```cpp
void vadd_sw(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t size)
{
    for (int i = 0; i < size; i++) {
       c[i] = a[i] + b[i];
    }
}
```

It is very important to note that at this point there is no way this code will beat the processor. The clock in the FPGA fabric is significantly slower than the CPU clock. This is expected, though — thinking back to our earlier example, we are only loading a single passenger into each car on the train.  We also have overhead to pass the data over PCIe®, set up DMA, etc. For the next few examples, we will look at how to efficiently manage the buffers for our inputs and outputs to this function. Only after that will we start to take advantage of the acceleration we can get from the AMD Alveo™ Data Center accelerator card.

## Key Code

This example is the first time we are going to actually run something on the FPGA, modest though it might be. In order to run something on the card, there are four things that we must do:

1. Allocate and populate the buffers we will use to send and receive data from the card.
2. Transfer those buffers between the host memory space and the Alveo global memory.
3. Run the kernel to act on those buffers.
4. Transfer the results of the kernel operation back to the host memory space so that they can be accessed via the processor.

As you can see, only one of those things actually takes place on the card. Memory management will make or break your application's performance, so start to take a look at that.

If you have not done acceleration work before, you might be tempted to jump in and just use normal calls to `malloc()` or `new` to allocate your memory. In this example, we will do just that, allocating a series of buffers to transfer between the host and the Alveo card. We will allocate four buffers: two input buffers to add together, one output buffer for the Alveo to use, and an extra buffer for a software implementation of our `vadd` function. This allows us to see something interesting: how we allocate memory for Alveo also impacts how efficiently the processor will run.

Buffers are allocated simply, as in the following listing. In our case, `BUFSIZE` is 24 MiB, or 6 × 1024 × 1024 values of type `uint32_t`. Any code not mentioned here is either identical or functionally equivalent to the previous examples.

```cpp
uint32_t* a =new uint32_t[BUFSIZE];
uint32_t* b =new uint32_t[BUFSIZE];
uint32_t* c =new uint32_t[BUFSIZE];
uint32_t* d =new uint32_t[BUFSIZE];
```

This will allocate memory that is **virtual**, **paged**, and most importantly, **non-aligned**. In particular, it is this last one that is going to cause some problems, as we will soon see.

When we allocate the buffers and populate them with initial test vectors, the next acceleration step is to send them down to the Alveo global memory.  We do that by creating OpenCL™ buffer objects using the flag `CL_MEM_USE_HOST_PTR`. This tells the API that rather than allocating its own buffer, we are providing our own pointers. This is not necessarily bad, but because we have not taken care allocating our pointers, it is going to hurt our performance.

The following listing contains the code mapping our allocated buffers to OpenCL buffer objects.

```cpp
std::vector<cl::Memory> inBufVec, outBufVec;
cl::Buffer a_to_device(context,
                       static_cast<cl_mem_flags>(CL_MEM_READ_ONLY |
                                                 CL_MEM_USE_HOST_PTR),
                       BUFSIZE * sizeof (uint32_t),
                       a,
                       NULL);
cl::Buffer b_to_device (context,
                       static_cast<cl_mem_flags(CL_MEM_READ_ONLY |
                                                CL_MEM_USE_HOST_PTR),
                       BUFSIZE * sizeof(uint32_t),
                       b,
                       NULL);
cl::Buffer c_from_device(context,
                        static_cast<cl_mem_flags>(CL_MEM_WRITE_ONLY |
                                                  CL_MEM_USE_HOST_PTR),
                        BUFSIZE * sizeof(uint32_t),
                        c,
                        NULL);
inBufVec.push_back(a_to_device);
inBufVec.push_back(b_to_device);
outBufVec.push_back(c_from_device);
```

What we are doing here is allocating `cl::Buffer` objects, which are recognized by the API, and passing in pointers `a`, `b`, and `c` from our previously-allocated buffers. The additional flags `CL_MEM_READ_ONLY` and `CL_MEM_WRITE_ONLY` specify to the runtime the visibility of these buffers from the perspective of the kernel. In other words, `a` and `b` are written to the card by the host - to the kernel they are **read only**. Then, `c` is read back from the card to the host. To the kernel, it is **write only**. We additionally add these buffer objects to vectors so that we can transfer multiple buffers at once (note that
we are essentially adding pointers to the vectors, not the data buffers themselves).

Next, we can transfer the input buffers down to the Alveo card:

```cpp
cl::Event event_sp;
q.enqueueMigrateMemObjects(inBufVec, 0, NULL, &event_sp);
clWaitForEvents(1, (const cl_event *)&event_sp);
```

In this code snippet, the "main event" is the call to `enqueueMigrateMemObjects()` on line 108. We pass in our vector of buffers; the 0 indicates that this is a transfer from host to device, and we also pass in a `cl::Event` object.

This is a good time to segue briefly into synchronization. When we enqueue the transfer, we are adding it to the runtime's "to-do list", if you will, but not actually waiting for it to complete.  By registering a `cl::Event` object, we can then decide to wait on that event at any point in the future.  In general this is not a point where you would necessarily want to wait, but we have done this at various points throughout the code to more easily instrument it to display the time taken for various operations.  This adds a small amount of overhead to the application, but again, this is a learning exercise and not an example of optimizing for maximum performance.

We now need to tell the runtime what to pass to our kernel, and we do that in the next listing.  Recall that our argument list looked like this:

`(uint32_t*a, uint32_t*b, uint32_t*c, uint32_t size)`

In our case, `a` is argument 0, `b` is argument 1, and so on.

```cpp
krnl.setArg(0, a_to_device);
krnl.setArg(1, b_to_device);
krnl.setArg(2, c_from_device);
krnl.setArg(3, BUFSIZE);
```

Next, we add the kernel itself to the command queue so that it will begin executing.  Generally speaking, you would enqueue the transfers and the kernel such that they would execute back-to-back rather than synchronizing in between. The line of code that adds the execution of the kernel to the command queue is:

```cpp
q.enqueueTask(krnl, NULL, &event_sp);
```

If you do not want to wait at this point, you can again pass in `NULL` instead of a `cl::Event` object.

And, finally, when the kernel completes, we want to transfer the memory back to the host so that we can access the new values from the CPU. This is done as follows:

```cpp
q.enqueueMigrateMemObjects(outBufVec, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &event_sp);
clWaitForEvents(1, (const cl_event *)&event_sp);
```

In this instance, we do want to wait for synchronization. This is important; recall that when we call these enqueue functions, we are placing entries onto the command queue in a **non-blocking** manner. If we then attempt to access the buffer immediately after enqueuing the transfer, it have finished reading back in.

Excluding the FPGA configuration from example 0, the new additions in order to run the kernel are:

1. Allocate buffers in the normal way.  We will soon see that there are better ways of doing this, but this is the way many people experimenting with acceleration might do it their first time.
2. Map the allocated buffers to cl::Buffer objects.
3. Enqueue the migration of the input buffers (**a** and **b**) to Alveo device global memory.
4. Set the kernel arguments, both buffers and scalar values.
5. Run the kernel.
6. Read the results of the kernel back into CPU host memory, synchronizing on the completion of the read.

Only one synchronization is needed were this a real application. As previously mentioned, we are using several to better report on the timing of various operations in the workflow.

## Running the Application

With the Xilinx Runtime (XRT) initialized, run the application by running the following command from the build directory.

`./01_simple_malloc alveo_examples`

The program will output a message similar to this:

```
-- Example 1: Vector Add with Malloc() --

Loading XCLBin to program the Alveo board:

Found Platform
Platform Name: Xilinx
XCLBINFile Name: alveo_examples
INFO: Importing ./alveo_examples.xclbin
Loading: ./alveo_examples.xclbin
Running kernel test with malloc()ed buffers
WARNING: unaligned host pointer 0x154f7909e010 detected, this leads to extra memcpy
WARNING: unaligned host pointer 0x154f7789d010 detected, this leads to extra memcpy
WARNING: unaligned host pointer 0x154f7609c010 detected, this leads to extra memcpy

Simple malloc vadd example complete!

--------------- Key execution times ---------------
OpenCL Initialization:              247.371 ms
Allocating memory buffer:           0.030 ms
Populating buffer inputs:           47.955 ms
Software VADD run:                  35.706 ms
Map host buffers to OpenCL buffers: 64.656 ms
Memory object migration enqueue:    24.829 ms
Set kernel arguments:               0.009 ms
OCL Enqueue task:                   0.064 ms
Wait for kernel to complete:        92.118 ms
Read back computation results:      24.887 ms
```

Note that we have some warnings about unaligned host pointers. Because we did not take care with our allocation, none of our buffers that we are transferring to or from the Alveo card are aligned to the 4 KiB boundaries needed by the Alveo DMA engine. Because of this, we need to copy the buffer contents so they are aligned before transfer, and that operation is quite expensive.

From this point on in our examples, keep a close eye on these numbers. While there will be some variability on the latency run-to-run, generally speaking we are looking for deltas in each particular area.
For now, establish a baseline:

| Operation                              |  Example 1  |
| -------------------------------------- | :---------: |
| OCL Initialization                     | 247.371 ms  |
| Buffer Allocation                      | 30 &micro;s |
| Buffer Population                      |  47.955 ms  |
| Software VADD                          |  35.706 ms  |
| Buffer Mapping                         |  64.656 ms  |
| Write Buffers Out                      |  24.829 ms  |
| Set Kernel Args                        | 9 &micro;s  |
| Kernel Runtime                         |  92.118 ms  |
| Read Buffer In                         |  24.887 ms  |
| &Delta;Alveo&rarr;CPU                  | -418.228 ms |
| &Delta;Alveo&rarr;CPU (algorithm only) | -170.857 ms |

## Extra Exercises

Some things to try to build on this experiment:

- Vary the size of the buffers allocated. Can you derive an approximate relationship between buffer size and
  the timing for individual operations? Do they all scale at the same rate?
- If you remove synchronization between each step, what is the quantitative effect on the runtime?
- What happens if you remove the synchronization after the final buffer copy from Alveo back to the host?

## Key Takeaways

- Once again, we have to pay our FPGA configuration "tax". We will need to save at least 250 ms over the CPU
 to make up for it.  Note that our trivial example will never be at the CPU if we are just looking at processing a single buffer!
- Simply-allocated memory is not a good candidate for passing to accelerators, as we will incur a memory copy to compensate.  We will  investigate the impact this has in subsequent examples.
- OpenCL works on command queues. It is up to the developer how and when to synchronize, but care must be taken when reading buffers back in from the Alveo global memory to ensure synchronization before the CPU accesses the data in the buffer.

Read [**Example 2:** Aligned Memory Allocation](./02-aligned-memory-allocation.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
