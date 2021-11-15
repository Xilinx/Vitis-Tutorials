<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis-AI™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Implementing an IIR Filter on the AI Engine

This is a multi-part tutorial on how to implement an [infinite impulse response (IIR) filter](https://en.wikipedia.org/wiki/Infinite_impulse_response) on the AI engine. 

Part 1a shows how to parallelize the IIR filter difference equation for a *single* second order section and implement it on the SIMD AI engine vector processor.

Part 1b shows to implement a 6<sup>th</sup> order low pass elliptic IIR filter with three second order sections and change the filter coefficients dynamically using run-time parameters.

This tutorial also provides [Julia](https://julialang.org/) scripts to calculate the IIR filter coefficients and generate test data.

Future parts will cover performance optimization.

This tutorial assumes that the reader is familiar with the fundamentals of AI engine kernel development.

Please use the [AI Engine, DSP IP and Tools](https://forums.xilinx.com/t5/AI-Engine-DSP-IP-and-Tools/bd-p/dspip_tools) forum for any questions regarding this tutorial.

This tutorial was developed and tested using Vitis 2021.1.

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

### License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )


Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD108 | &copy; Copyright 2021 Xilinx, Inc.</sup></p>