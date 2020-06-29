Code and files for **module 5** (same flow as in [module1](../module1_baseline) to setup the GUI or run make)

<details>
  <summary><b>Click to expand! Learn about the <code>dataflow</code> pragma...</b></summary>
  
  he DATAFLOW pragma enables task-level pipelining, allowing functions and loops to overlap in their operation, increasing the concurrency of the register transfer level (RTL) implementation, and increasing the overall throughput of the design.

All operations are performed sequentially in a C description. In the absence of any directives that limit resources (such as pragma HLS allocation), the Vivado High-Level Synthesis (HLS) tool seeks to minimize latency and improve concurrency. However, data dependencies can limit this. For example, functions or loops that access arrays must finish all read/write accesses to the arrays before they complete. This prevents the next function or loop that consumes the data from starting operation. The DATAFLOW optimization enables the operations in a function or loop to start operation before the previous function or loop completes all its operations. 

When the DATAFLOW pragma is specified, the HLS tool analyzes the dataflow between sequential functions or loops and creates channels (based on ping pong RAMs or FIFOs) that allow consumer functions or loops to start operation before the producer functions or loops have completed. This allows functions or loops to operate in parallel, which decreases latency and improves the throughput of the RTL.

If no initiation interval (number of cycles between the start of one function or loop and the next) is specified, the HLS tool attempts to minimize the initiation interval and start operation as soon as data is available.

TIP: The config_dataflow command specifies the default memory channel and FIFO depth used in dataflow optimization. Refer to the config_dataflow command in the Vivado Design Suite User Guide: High-Level Synthesis (UG902) for more information.
For the DATAFLOW optimization to work, the data must flow through the design from one task to the next. The following coding styles prevent the HLS tool from performing the DATAFLOW optimization:

   + Single-producer-consumer violations
   + Bypassing tasks
   + Feedback between tasks
   + Conditional execution of tasks
   + Loops with multiple exit conditions

**IMPORTANT**: If any of these coding styles are present, the HLS tool issues a message and does not perform DATAFLOW optimization.

You can use the STABLE pragma to mark variables within DATAFLOW regions to be stable to avoid concurrent read or write of variables.

Finally, the DATAFLOW optimization has no hierarchical implementation. If a sub-function or loop contains additional tasks that might benefit from the optimization, you must apply the optimization to the loop, the sub-function, or inline the sub-function.

**Syntax**

Place the pragma in the C source within the boundaries of the region, function, or loop.

```cpp
#pragma HLS dataflow
```

**Example**

Specifies DATAFLOW optimization within the loop wr_loop_j.

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
