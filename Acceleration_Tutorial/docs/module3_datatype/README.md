Code, files and instructions for **module 3** (same flow as in [module1](../module1_baseline) to setup the GUI or run make)

In this module both the kernel and host code are modified to use 32-bit floating point data types (float) instead of the 64-bit floating point (double) to show the performance and Xilinx utilization beneficial impact of downsizing data types.

The Xilinx device fitted on the U50 card is an UltraScale + device that can implement floating point using hardware resources (DSP blocks and logic). Floating point operators tend to use a good amount of hardware. Here in this module, we can measure the savings when scaling down to 32-bit floting point types.

+ Run Vitis and harware emulation (as described in module 1).
+ Run Vitis Analyser to measure impact on resources used and performance improvement.
+ Compare to utilization seen in previous module.

Did it shorten kernel execution time?

+ Run Vitis HLS to measure detailed parameters including the II (remember, the II is the initiation interval, the ability of the generated hardware to consume data every <II> clock cycle).

Are resources used less than for modules 1 and 2?
Is the II any different than before?
***
Please proceed to [**module 4**](../module4_dataflow).

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
