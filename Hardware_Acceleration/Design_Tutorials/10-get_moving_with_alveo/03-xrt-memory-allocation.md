<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™Hardware Acceleration Introduction Tutorial</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

## Overview

Ensuring that our allocated memory is aligned to page boundaries gave us a significant improvement over our initial configuration. There is another workflow we can use with OpenCL™, though, which is to have OpenCL and Xilinx Runtime (XRT) allocate the buffers and then map them to userspace pointers for use by the application. Experiment with that and see the effect it has on our timing.

## Key Code

Conceptually, this is a small change, but unlike Example 2, this example is a bit more involved in terms of the required code changes. This is mostly because instead of using standard userspace memory allocation, we are going to ask the OpenCL runtime to allocate buffers for us. When we have the buffers, we then need to map them into userspace so that we can access the data they contain.

For our allocation, we change from the previous example's code to the following:

```cpp
std::vector<cl::Memory> inBufVec, outBufVec;
cl::Buffer a_buf(context,
                 static_cast<cl_mem_flags>(CL_MEM_READ_ONLY |
                                           CL_MEM_ALLOC_HOST_PTR),
                 BUFSIZE*sizeof(uint32_t),
                 NULL,
                 NULL);
cl::Buffer b_buf(context,
                 static_cast<cl_mem_flags>(CL_MEM_READ_ONLY |
                                           CL_MEM_ALLOC_HOST_PTR),
                 BUFSIZE*sizeof(uint32_t),
                 NULL,
                 NULL);
cl::Buffer c_buf(context,
                 static_cast<cl_mem_flags>(CL_MEM_WRITE_ONLY |
                                           CL_MEM_ALLOC_HOST_PTR),
                 BUFSIZE*sizeof(uint32_t),
                 NULL,
                 NULL);
cl::Buffer d_buf(context,
                 static_cast<cl_mem_flags>(CL_MEM_READ_WRITE |
                                           CL_MEM_ALLOC_HOST_PTR),
                 BUFSIZE*sizeof(uint32_t),
                 NULL,
                 NULL);
```

In this case, we are allocating our OpenCL buffer objects significantly earlier in the program, and we also do not have userspace pointers yet. We can still, though, pass these buffer objects to enqueue `MigrateMemObjects()` and other OpenCL functions. The backing storage is allocated at this point, we just do not have a userspace pointer to it.

The call to the `cl::Buffer` constructor looks very similar to what we had before.  In fact, only two things have changed: we pass in the flag `CL_MEM_ALLOC_HOST_PTR` instead of `CL_MEM_USE_HOST_PTR` to tell the runtime that we want to allocate a buffer instead of using an existing buffer. We also no longer need to pass in a pointer to the user buffer (since we are allocating a new one), so we pass `NULL` instead.

We then need to map our OpenCL buffers to the userspace pointers to `a`, `b`, and `d` that we will use immediately in software. There is no need to map a pointer to `c` at this time, we can do that later when we need to read from that buffer after kernel execution. We do this as follows:

```cpp
uint32_t*a = (uint32_t*)q.enqueueMapBuffer(a_buf,
                                           CL_TRUE,
                                           CL_MAP_WRITE,
                                           0,
                                           BUFSIZE*sizeof(uint32_t));

uint32_t*b = (uint32_t*)q.enqueueMapBuffer(b_buf,
                                           CL_TRUE,
                                           CL_MAP_WRITE,
                                           0,
                                           BUFSIZE*sizeof(uint32_t));
uint32_t*d = (uint32_t*)q.enqueueMapBuffer(d_buf,
                                           CL_TRUE,
                                           CL_MAP_WRITE | CL_MAP_READ,
                                           0,
                                           BUFSIZE*sizeof(uint32_t));
```

Once we perform the mapping, we can use the userspace pointers as normal to access the buffer contents. One thing to note, though, is that the OpenCL runtime does do reference counting of the opened buffers, so we need a corresponding call to `enqueueUnmapMemObject()` for each buffer that we map.

The execution flow through the kernel is the same, but we see something new when the time comes to migrate the input buffer back into the device. Rather than manually enqueueing a migration, we can instead just map the buffer. The OpenCL runtime will recognize that the buffer contents are currently resident in the AMD Alveo™ Data Center accelerator card global memory and will take care of migrating the buffer back to the host for us. This is a coding style choice you must make, but fundamentally the code below is sufficient to migrate `c` back to the host memory.

```cpp
uint32_t*c = (uint32_t*)q.enqueueMapBuffer(c_buf,
                                           CL_TRUE,
                                           CL_MAP_READ,
                                           0,
                                           BUFSIZE * sizeof(uint32_t));
```

Finally, as we mentioned earlier, you need to unmap the memory objects, so that they can be destroyed cleanly by the runtime. We do this at the end of the program instead of using `free()` on the buffers as before. This must be done before the command queue is finished.

```cpp
q.enqueueUnmapMemObject(a_buf, a);
q.enqueueUnmapMemObject(b_buf, b);
q.enqueueUnmapMemObject(c_buf, c);
q.enqueueUnmapMemObject(d_buf, d);
q.finish();
```

To summarize the key workflow for this use model, we need to:

1. **Allocate** our buffers using the CL_MEM_ALLOC_HOST_PTR flag.
2. **Map** our input buffers to userspace pointers to populate them.
3. **Run** the kernel as usual.
4. **Map** the output buffer(s) to migrate them back to host memory.
5. **Unmap** all of our buffers once we're done using them so they can be destroyed properly.

## Running the Application

With the XRT initialized, run the application by running the following command from the build directory:

`./03_buffer_map alveo_examples`

The program will output a message similar to the following:

```
-- Example 3: Allocate and Map Contiguous Buffers --

Loading XCLBin to program the Alveo board:

Found Platform
Platform Name: Xilinx
XCLBIN File Name: alveo_examples
INFO: Importing ./alveo_examples.xclbin
Loading: './alveo_examples.xclbin'
Running kernel test with XRT-allocated contiguous buffers

OCL-mapped contiguous buffer example complete!

--------------- Key execution times ---------------
OpenCL Initialization:              247.460 ms
Allocate contiguous OpenCL buffers: 30.365 ms
Map buffers to userspace pointers:  0.222 ms
Populating buffer inputs:           22.527 ms
Software VADD run:                 24.852 ms
Memory object migration enqueue:   6.739 ms
Set kernel arguments:               0.014 ms
OCL Enqueue task:                   0.102 ms
Wait for kernel to complete:       92.068 ms
Read back computation results:     2.243 ms
```

| Operation                             |  Example 2  |  Example 3   | &Delta;2&rarr;3 |
| ------------------------------------- | :---------: | :----------: | :-------------: |
| OCL Initialization                    | 256.254 ms  |  247.460 ms  |        -        |
| Buffer Allocation                     | 55 &micro;s |  30.365 ms   |    30.310 ms    |
| Buffer Population                     |  47.884 ms  |  22.527 ms   |   -25.357 ms    |
| Software VADD                         |  35.808 ms  |  24.852 ms   |   -10.956 ms    |
| Buffer Mapping                        |  9.103 ms   | 222 &micro;s |    -8.881 ms    |
| Write Buffers Out                     |  6.615 ms   |   6.739 ms   |        -        |
| Set Kernel Args                       | 14 &micro;s | 14 &micro;s  |        -        |
| Kernel Runtime                        |  92.110 ms  |  92.068 ms   |        -        |
| Read Buffer In                        |  2.479 ms   |   2.243 ms   |        -        |
| &Delta;Alveo&rarr;CPU                 | -330.889 ms | -323.996 ms  |    -6.893 ms    |
| &Delta;FPGA&rarr;CPU (algorithm only) | -74.269 ms  |  -76.536 ms  |        -        |

You might have expected a speedup here, but we see that rather than speeding up any particular operation;
instead, we have shifted the latencies in the system around. Effectively we have paid our taxes from a different bank account, but at the end of the day, we cannot escape them. On embedded systems with a unified memory map for the processor and the kernels, we would see significant differences here, but on server-class CPUs, we do not.

One thing to think about is that although pre-allocating the buffers in this way took longer, you do not generally want to allocate buffers in your application's critical path. By using this mechanism, the runtime use of your buffers is much faster.

You might even wonder why it is faster for the CPU to access this memory. While we have not discussed it to this point, allocating memory via this API pins the virtual addresses to physical memory. This makes it more efficient for both the CPU and the DMA to access it. As with all things in engineering, though, this comes at a price — allocation time is higher and you run the risk of fragmenting the available memory if you allocate many small buffers.

In general, buffers should be allocated outside the critical path of your application, and this method shifts the burden away from your high-performance sections if used correctly.

## Extra Exercises

Some things to try to build on this experiment:

- Once again, vary the size of the buffers allocated. Do the relationships that you derived in the previous
  example still hold true?
- Experiment with other sequences for allocating memory and enqueuing transfers.
- What happens if you modify the input buffer and run the kernel a second time?

## Key Takeaways

- Using the OpenCL and XRT APIs can lead to localized performance boosts, although fundamentally there is no escaping your taxes.
- Our long pole is becoming our kernel runtime, though, and we can easily speed that up. Take a look at that in our next few examples.

Read [**Example 4:** Parallelizing the Data Path](./04-parallelizing-the-data-path.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
