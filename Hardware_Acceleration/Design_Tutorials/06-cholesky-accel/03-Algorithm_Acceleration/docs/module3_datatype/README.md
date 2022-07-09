<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

Code, files and instructions for **module 3** (same flow as in [module1](../module1_baseline) to setup the GUI or run make)

In this module both the kernel and host code are modified to use 32-bit floating point data types (float) instead of the 64-bit floating point (double) to show the performance and Xilinx utilization beneficial impact of downsizing data types.

The Xilinx device fitted on the U50 card is an UltraScale+ device that can implement floating point using hardware resources (DSP blocks and logic). Floating point operators tend to use a good amount of hardware. Here in this module, we can measure the savings when scaling down to 32-bit floating point types.

+ Run Vitis and hardware emulation only (as described in module 1).  No need to run software emulation or actual hardware compilation.
+ Run Vitis Analyzer to measure both the impact on the device resources and performance improvement.
+ Compare to utilization seen in previous module.

Did it shorten kernel execution time?

+ Run Vitis HLS to measure detailed parameters including the II (remember, the II is the initiation interval, the ability of the generated hardware to consume data every <II> clock cycle).

Are resources used less than for modules 1 and 2?
Is the II any different than before?

#### Kernel Resources Used (regular floating point versus double)

| Name                 | LUT           | LUTAsMem   | REG       | BRAM     |  DSP  |
|----------------------|--------------:|-----------:|----------:|---------:|------:|
| kernel with `double` |  10,190       |    799     |   10,191  |  514     |  18   |
| kernel with `float`  |   5,071       |    746     |    5,124  |  258     |  12   |

As expected, the resource utilization comes down all across for both logic and storage
 - About half for storage as we are storing 32-bit words versus 64-bit for the double data types
 - The operators also consume less logic hence less DSP blocks

***
#### Takeaway for this module...
Users can often get better performance by scaling down the precision of their data.  Xilinx devices can produce dramatically better results once the data types are simplified.
***

#### Next

Proceed to [**module 4**](../module4_dataflow/README.md).

<p align="center"><sup>Copyright&copy; 2020-2022 Xilinx</sup></p>
