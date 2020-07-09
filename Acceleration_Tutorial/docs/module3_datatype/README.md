Code, files and instructions for **module 4** (same flow as in [module1](../module1_baseline) to setup the GUI or run make)

In this module both the kernel and host code are modified to use 32-bit floating point data types (float) instead of the 64-bit floating point (double).

The Xilinx device fitted on the U50 card is an UltraScale + device that does not offer dedicated floating point hardware (as in Versal). Therefore floating point operators tend to use a good amount of hardware. Here we can measure the savings when scaling back to 32-bit types.

Run Vitis and harware emulation (as described in module 1).
Run Vitis Analyser to measure impact on resources used and performance improvement.

Did it shorten kernel execution time?

Run Vitis HLS to measure detailed parameters including the II.

Is the II any different than before?

Please proceed to [**module 5**](../module5_dataflow).
