<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Implementing an IIR Filter on the AI Engine
***Version: Vitis 2022.2***

This is a multi-part tutorial on how to implement an [infinite impulse response (IIR) filter](https://en.wikipedia.org/wiki/Infinite_impulse_response) on the AI engine. 

Part 1a shows how to parallelize the IIR filter difference equation for a *single* second order section and implement it on the SIMD AI engine vector processor.

Part 1b shows to implement a 6<sup>th</sup> order low pass elliptic IIR filter with three second order sections and change the filter coefficients dynamically using run-time parameters.

Part 2a illustrates the process of minimizing latency and increasing throughput.

Part 2b introduces more techniques to further improve throughput.

This tutorial also provides [Julia](https://julialang.org/) scripts to calculate the IIR filter coefficients and generate test data.

Future parts will cover performance optimization.

This tutorial assumes that the reader is familiar with the fundamentals of AI engine kernel development.

Please use the [AI Engine, DSP IP and Tools](https://forums.xilinx.com/t5/AI-Engine-DSP-IP-and-Tools/bd-p/dspip_tools) forum for any questions regarding this tutorial.

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
