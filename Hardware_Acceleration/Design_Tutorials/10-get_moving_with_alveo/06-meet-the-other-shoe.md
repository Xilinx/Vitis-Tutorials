<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™Hardware Acceleration Introduction Tutorial</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

## Overview

You did not think you were getting out of here quite so fast, did you?  As I said at the beginning: `_vadd` will never beat the processor. It is too simple; if you do not have to transfer data and you can burn through local cache, the CPU will always win in the end.

The results from the previous session look good — on paper.

## Key Code

For simple algorithms, an accelerator just will not win. Use OpenMP® to parallelize the processor loop. We include the header `omp.h`, and then apply an OpenMP pragma to the CPU code as in listing 3.19.

```cpp
void vadd_sw(uint32_t* a, uint32_t *b, uint32_t* c, uint32_t size)
{
#pragma omp parallel for
      for (inti = 0; i < size; i++) {
          c[i] = a[i] + b[i];
      }
}
```

And that is it. There are some command line flags to pass to GCC, but CMake will take care of those (assumingyou have OpenMP installed), so we can directly build and run. The code for this example is otherwise identical to the code from Example 5.

## Running the Application

With the Xilinx Runtime (XRT) initialized, run the application by running the following command from the build directory:

`./06_wide_processor alveo_examples`

The program will output a message similar to this:

```
-- Example 6: VADD with OpenMP --

Loading XCLBin to program the Alveo board:

Found Platform
Platform Name: Xilinx
XCLBIN File Name: alveo_examples
INFO: Importing ./alveo_examples.xclbin
Loading: ’./alveo_examples.xclbin’

-- Running kernel test with XRT-allocated contiguous buffers and wide VADD (16 values/clock), with software OpenMP

OCL-mapped contiguous buffer example complete!

--------------- Key execution times ---------------
OpenCL™ Initialization:              253.898 ms
Allocate contiguous OpenCL buffers: 907.183 ms
Map buffers to userspace pointers:  0.307 ms
Populating buffer inputs:           1188.315 ms
Software VADD run :                 157.226 ms
Memory object migration enqueue :   1.429 ms
Wait for kernel to complete :       618.231 ms
```

| Operation             |  Example 5  | Example 6  | &Delta;5&rarr;6 |
| --------------------- | :---------: | :--------: | :-------------: |
| Software VADD         | 1166.471 ms | 157.226 ms |  −1009.245 ms   |
| Hardware VADD (Total) | 692.172 ms  | 618.231 ms |    −73.94 ms    |
| &Delta;Alveo&rarr;CPU | −503.402 ms | 461.005 ms |   964.407 ms    |

The accelerator runtime fluctuation is primarily a result of running these tests in a virtualized cloud environment, but that is not the point of the exercise.

## Extra Exercises

Some things to try to build on this experiment:

- Try to beat the processor at vector addition!
- Play with the OpenMP pragmas; how many CPU cores are needed to beat a hardware accelerator?

## Key Takeaways

- I have said it before and I will say it again: simple O(N) will never win.

But despair not!  Now it is time to look at something real.

Read [**Example 7:** Image Resizing with Vitis Vision](./07-image-resizing-with-vitis-vision.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
