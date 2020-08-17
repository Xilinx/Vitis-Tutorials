
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

# 4. Optimizing Using Fixed Point Data Types

In the last lab, you optimized the memory accesses between the kernel and the global memory. In this lab, you will focus on improving the efficiency of the kernel by converting from floating point to fixed point data types.  

The original code uses floating point for coefficients and accumulators. Here you will use the ap_fixed<16,9> type, representing a 9-bit signed integer with seven decimal bits. This type was chosen because it improves performance and resource utilization, while maintaining the necessary precision for the application.

Look at the following inner loop of your convolution kernel.

    for(int pixel = 0; pixel < img_width; ++pixel)
    {
        float sum_r = 0, sum_g=0, sum_b=0;
        for(int m = 0; m < coefficient_size; ++m)
        {
            for(int n = 0; n < coefficient_size; ++n)
            {
                int jj = pixel + n - center;
                if(jj >= 0 && jj < img_width)
                {
                    sum_r += window_mem[window_line_idx][jj].r * coef[m * coefficient_size + n];
                    sum_g += window_mem[window_line_idx][jj].g * coef[m * coefficient_size + n];
                    sum_b += window_mem[window_line_idx][jj].b * coef[m * coefficient_size + n];
                }
            }
            window_line_idx=(window_line_idx + 1) == MAX_FILTER ? 0 : window_line_idx + 1;
        }
        window_line_idx = top_idx;
        out_line[pixel].r =  fabsf(sum_r);
        out_line[pixel].g =  fabsf(sum_g);
        out_line[pixel].b =  fabsf(sum_b);
    }

The inner loop is multiplying individual members of an `RGBPixel` object which are unsigned char with the floating `coef` array. The operation result is stored back into the floating point variables `sum_r`, `sum_g`, `sum_b`, and finally to a `RGBPixel`. Based on these calculations, you can assume that the largest number that can be represented by the sum argument would be 256 because that is the maximum value of an unsigned char. Based on this, you can use a fixed point data type that is 16-bits wide and 8-bits dedicated to the integer side.

## Kernel Code Modifications

>**TIP:** The modified kernel source file is provided in the `reference-files/fixedpoint` folder. You can use it as a reference if needed.

Open the `convolve_fpga.cpp` file from `src/fixedpoint` and make the following modifications.

1. Include the `ap_fixed.h` header at the top of the file.

        #include "ap_fixed.h"

2. Create a typedef for a fixed point type that maps to `ap_fixed<16,9>`.

        typedef ap_fixed<16,9> fixed;

3. Replace the following line (line 39).

        float coef[MAX_FILTER * MAX_FILTER];

    with:

        fixed coef[MAX_FILTER * MAX_FILTER];

    This modifies the type of the `coef` array to a fixed array.

4. Because the type of `coef` is different from `coefficient`, the `memcpy` command is not supported by the Vivado® High-Level Synthesis (HLS) tool. Instead, convert it to a `for` loop implementation. Replace (line 40).

        memcpy(coef, coefficient, coefficient_size * sizeof(float));

   with:

        int num_coefficients = coefficient_size * coefficient_size;
        for(int i = 0; i < num_coefficients; i++) {
            coef[i] = coefficient[i];
        }

    This performs the same operation, but also converts the floating point elements in the `coefficient` array to fixed point elements in the `coef` array.

5. Next, replace the types of the `sum_r`, `sum_g`, and `sum_b` variables to the fixed type. Replace (line 70):

        float sum_r = 0, sum_g=0, sum_b=0;

    with:

        fixed sum_r = 0, sum_g=0, sum_b=0;

## Run Hardware Emulation

1. Go to the `design/makefile` directory.
2. Use the following command to run hardware emulation.

   ```
   make run TARGET=hw_emu STEP=fixedpoint SOLUTION=1 NUM_FRAMES=1
   ```

   You should see the following results.

   ```
   Processed 0.02 MB in 108.788s (0.00 MBps)

   INFO: [Vitis-EM 22] [Wall clock time: 21:17, Emulation time: 0.510047 ms] Data transfer between kernel(s) and global memory(s)
   convolve_fpga_1:m_axi_gmem1-DDR[0]          RD = 20.000 KB              WR = 20.000 KB
   convolve_fpga_1:m_axi_gmem2-DDR[0]          RD = 0.035 KB               WR = 0.000 KB  
   ```

## View the Profile Summary Report for Hardware Emulation

 Use the following command to view the Profile Summary report.

```
make view_run_summary TARGET=hw_emu STEP=fixedpoint
```

The kernel execution time is now reduced to 0.46 ms. The reason for this significant speedup is that the computation for-loop is pipelined when using fixed point operations. Therefore, the total latency is improved significantly.

Here is the updated table. There is a 3.4x boost on kernel execution time perspective.

| Step              | Image Size | Time (HW-EM)(ms) | Reads (KB)      | Writes (KB) | Avg. Read (KB) | Avg. Write (KB) | BW (MBps)  |
| :---------------- | :--------- | ---------------: | --------------: | ----------: | -------------: | --------------: | ---------: |
| baseline          |     512x10 | 3.903            | 344             |        20.0 |          0.004 |           0.004 |    5.2     |
| localbuf          |     512x10 | 1.574 (2.48x)    | 21  (0.12x)     |        20.0 |          0.064 |           0.064 |    13      |
| fixedpoint data   |     512x10 | 0.46 (3.4x)      | 21              |        20.0 |          0.064 |           0.064 |    44      |


[fixedtype_hwemu_profilesummary]: ./images/191_fixedtype_hwemu_pfsummary_new_2.jpg "Fixed-type data version hardware emulation profile summary"

## Next Step

In the next section, you examine how breaking a single function into sub-functions lets you achieve task-level parallelism between the different functions. In this case, you will be [optimizing with dataflow](./dataflow.md).
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="../vitis-getting-started/README.md">Return to Getting Started Pathway</a> — <a href="../convolution-tutorial/README.md">Return to Start of Tutorial</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
