<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

Code and files for **Module 4** (same instructions as in [Module 1](../module1_baseline) to set up Vitis, Vitis Analyzer, and Vitis HLS).

> **_In this module..._**  
>
>1. Replicate a compute loop by a programmable factor applied via a templated function.
>2. Use the Vitis HLS `dataflow` pragma.
>3. Run the full compile and program the card.

<details>
  <summary><b>Click to expand! Learn about the `DATAFLOW` pragma...</b></summary>

The `DATAFLOW` pragma enables task-level pipelining, allowing functions and loops to overlap in their operation, increasing the concurrency of the register transfer level (RTL) implementation, and increasing the overall throughput of the design.

All operations are performed sequentially in a C description. In the absence of any directives that limit resources (such as pragma HLS allocation), the Vivado High-Level Synthesis (HLS) tool seeks to minimize latency and improve concurrency. However, data dependencies can limit this. For example, functions or loops that access arrays must finish all read/write accesses to the arrays before they complete. This prevents the next function or loop that consumes the data from starting operation. The `DATAFLOW` optimization enables the operations in a function or loop to start operation before the previous function or loop completes all its operations.

When the `DATAFLOW` pragma is specified, the HLS tool analyzes the data flow between sequential functions or loops and creates channels (based on ping pong RAMs or FIFOs) that allow consumer functions or loops to start operation before the producer functions or loops have completed. This allows functions or loops to operate in parallel, which decreases latency and improves the throughput of the RTL.

If no II (number of cycles between the start of one function or loop and the next) is specified, the HLS tool attempts to minimize the initiation interval and start operation as soon as data is available.

>**TIP:** The `config_dataflow` command specifies the default memory channel and FIFO depth used in `DATAFLOW` optimization. Refer to the `config_dataflow` command in the Vivado Design Suite User Guide: High-Level Synthesis (UG902) for more information.
For the `DATAFLOW` optimization to work, the data must flow through the design from one task to the next. The following coding styles prevent the HLS tool from performing the `DATAFLOW` optimization:

+ Single-producer-consumer violations
+ Bypassing tasks
+ Feedback between tasks
+ Conditional execution of tasks
+ Loops with multiple exit conditions

>**IMPORTANT**: If any of these coding styles are present, the HLS tool issues a message and does not perform the `DATAFLOW` optimization.

You can use the `STABLE` pragma to mark variables within `DATAFLOW` regions to be stable to avoid concurrent read or write of variables.

Finally, the `DATAFLOW` optimization has no hierarchical implementation. If a sub-function or loop contains additional tasks that might benefit from the optimization, you must apply the optimization to the loop, the sub-function, or inline the sub-function.

## Syntax

Place the pragma in the C source within the boundaries of the region, function, or loop.

```cpp
#pragma HLS DATAFLOW
```

## Example

Specifies `DATAFLOW` optimization within the loop wr_loop_j.

```cpp
wr_loop_j: for (int j = 0; j < TILE_PER_ROW; ++j) {
#pragma HLS DATAFLOW
   wr_buf_loop_m: for (int m = 0; m < HEIGHT; ++m) {
      wr_buf_loop_n: for (int n = 0; n < WIDTH; ++n) {
      #pragma HLS PIPELINE
      // should burst WIDTH in WORD beat
         outFifo >> tile[m][n];
      }
   }
   wr_loop_m: for (int m = 0; m < HEIGHT; ++m) {
      wr_loop_n: for (int n = 0; n < WIDTH; ++n) {
      #pragma HLS PIPELINE
         outx[HEIGHT*TILE_PER_ROW*WIDTH*i+TILE_PER_ROW*WIDTH*m+WIDTH*j+n] = tile[m][n];
      }
   }
}
```

</details>

### Code Modifications for the Cholesky Kernel

In this module, the code for the algorithm is moved into the header file, `cholesky_kernel.hpp`.

There is now an explicit parallelization and the number of parallel compute. It is determined by `NCU`, a constant set in `cholesky_kernel.cpp` through `#define NCU 16`.

`NCU` is passed as a template parameter to the `chol_col_wrapper` function (see below). The `DATAFLOW` pragma applies to the loop that calls `chol_col` 16 times:

```cpp
template <typename T, int N, int NCU>
void chol_col_wrapper(int n, T dataA[NCU][(N + NCU - 1) / NCU][N], T dataj[NCU][N], T tmp1, int j)
{
#pragma HLS DATAFLOW

Loop_row:
    for (int num = 0; num < NCU; num++)
    {
#pragma HLS unroll factor = NCU
        chol_col<T, N, NCU>(n, dataA[num], dataj[num], tmp1, num, j);
    }
}
```

To ensure `DATAFLOW` is applied the dataA is divided into `NCU` portions.

Finally, the loop is unrolled with a factor `NCU` which implies you have `NCU` (i.e., 16) copies of `chol_col` created each working on a chunk of the data.

#### Running the Design

Same as in module 1:

+ Run the hardware emulation.
+ run Vitis Analyzer.
+ run Vitis HLS and see how it offers a dataflow viewer that can be accessed by right-clicking on the function (`chol_col_wrapper`) in which dataflow is applied from the synthesis summary report. See [**this animation**](../images/HLS_dataflow_anim.gif) to see how to access it and confirm that the replications were applied.

#### Result Summary

Actual results comparing the runs for all modules including execution on the CPU<sup>(*)</sup>:

| Module                  | CPU    | Module 1 | Module 2 | Module 3 | Module 4 (NCU=16) |
| ----------------------- | -----: | -------: | -------: | -------: | -------: |
| Execution Time (µs)     | 21,461 | 793,950  | 793,732  | 536,784  | 11,698   |
| Speed Up (CPU reference)| 1      | 0.03x    | 0.03x    | 0.04x    | 1.83x    |
| Speed Up                | N/A    | 1        | 1        | 1.48x    | 68x      |

(*): Reference CPU is the Intel® Xeon® Processor E5-2640 v3 (as available on Nimbix)

#### Conclusion

This concludes this introductory tutorial on Vitis Hardware Acceleration. [Click here](../../../../README.md) to find more Vitis design optimization tutorials.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
