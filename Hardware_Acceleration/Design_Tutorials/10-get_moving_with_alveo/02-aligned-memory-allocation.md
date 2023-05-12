<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™Hardware Acceleration Introduction Tutorial</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

## Overview

In our last example, we allocated memory simply, but as we saw the direct memory access (DMA) engine requires that our buffers be aligned to 4 KiB pages boundaries. If the buffers are not so aligned, which they likely will not be if they do not explicitly ask for it, then the runtime will copy the buffers so that their contents are aligned.

That is an expensive operation, but can we quantify how expensive? And how can we allocate aligned memory?

## Key Code

This is a relatively short example in that we are only changing four lines vs. Example 1, our  buffer allocation.  There are various ways to allocate aligned memory but in this case, we will make use of a POSIX
function, `posix_memalign()`. This change replaces our previous allocation with the code shown below. We also need to include an additional header not shown in the listing, `memory`.

```cpp
uint32_t*a,*b,*c,*d = NULL;
posix_memalign((void**)&a, 4096, BUFSIZE*sizeof(uint32_t));
posix_memalign((void**)&b, 4096, BUFSIZE*sizeof(uint32_t));
posix_memalign((void**)&c, 4096, BUFSIZE*sizeof(uint32_t));
posix_memalign((void**)&d, 4096, BUFSIZE*sizeof(uint32_t));
```

For our calls to `posix_memalign()`, we are passing in our requested alignment, which in this case is 4 KiB as we discussed previously.

Otherwise, this is the only change to the code versus the first example. Note that we have changed the allocation for _all_ of the buffers, including buffer `d` which is only used by the CPU baseline VADD function. We will see if this has any impact on the runtime performance for both the accelerator and the CPU.

## Running the Application

With the Xilinx Runtime (XRT) initialized, run the application by running the following command from the build directory:

`./02_aligned_malloc alveo_examples`

The program will output a message similar to this:

```
-- Example 2: Vector Add with Aligned Allocation --

Loading XCLBin to program the Alveo board:

Found Platform
Platform Name: Xilinx
XCLBIN File Name: alveo_examples
INFO: Importing ./alveo_examples.xclbin
Loading: './alveo_examples.xclbin'
Running kernel test with aligned virtual buffers

Simple malloc vadd example complete!

--------------- Key execution times ---------------
OpenCL™™™ Initialization:                 256.254 ms
Allocating memory buffer:              0.055 ms
Populating buffer inputs:              47.884 ms
Software VADD run:                     35.808 ms
Map host buffers to OpenCL™™ buffers :   9.103 ms
Memory object migration enqueue:       6.615 ms
Set kernel arguments:                  0.014 ms
OCL Enqueue task:                      0.116 ms
Wait for kernel to complete:           92.110 ms
Read back computation results:         2.479 ms
```

This seems at first glance to be much better! Compare these results to our results from Example 1 to see how things have changed. Refer to the following table for details, noting that we will exclude minor run-to-run variation from the comparison to help keep things clean.

| Operation                              |  Example 1  |  Example 2  | &Delta;1&rarr;2 |
| -------------------------------------- | :---------: | :---------: | :-------------: |
| OCL Initialization                     | 247.371 ms  | 256.254 ms  |        -        |
| Buffer Allocation                      | 30 &micro;s | 55 &micro;s |   25 &micro;s   |
| Buffer Population                      |  47.955 ms  |  47.884 ms  |        -        |
| Software VADD                          |  35.706 ms  |  35.808 ms  |        -        |
| Buffer Mapping                         |  64.656 ms  |  9.103 ms   |   -55.553 ms    |
| Write Buffers Out                      |  24.829 ms  |  6.615 ms   |   -18.214 ms    |
| Set Kernel Args                        | 9 &micro;s  | 14 &micro;s |        -        |
| Kernel Runtime                         |  92.118 ms  |  92.110 ms  |        -        |
| Read Buffer In                         |  24.887 ms  |  2.479 ms   |   -22.408 ms    |
| &Delta;Alveo&rarr;CPU                  | -418.228 ms | -330.889 ms |    87.339 ms    |
| &Delta;Alveo&rarr;CPU (algorithm only) | -170.857 ms | -74.269 ms  |    96.588 ms    |

Nice! By only changing four lines of code, we have managed to shave nearly 100 ms off of our execution time. The CPU is still faster, but just by changing one minor thing about how we are allocating memory we saw huge improvement. That is really down to the memory copy that is needed for alignment; if we take a few extra microseconds to ensure the buffers are aligned when we allocate them, we can save orders of magnitude more time later when those buffers are consumed.

As expected in this use case, the software runtime is the same. We are changing the alignment of the allocated memory, but otherwise it is normal userspace memory allocation.

## Extra Exercises

Some things to try to build on this experiment:

- Once again vary the size of the buffers allocated. Do the relationships that you derived in the previous example still hold true?
- Experiment with other methods of allocating aligned memory (not the OCL API). Do you see differences between the approaches, beyond minor run-to-run fluctuations?

## Key Takeaways

- Unaligned memory will kill your performance. Always ensure buffers you want to share with the AMD Alveo™ card are aligned.

Now we are getting somewhere! Try using the OpenCL™ API to allocate memory and see what happens.

Read [**Example 3:** Memory Allocation with XRT](./03-xrt-memory-allocation.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
