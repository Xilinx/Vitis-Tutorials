<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Implementing an IIR Filter on the AI Engine

***Version: Vitis 2025.2***

This is a multi-part tutorial on implementing an [infinite impulse response (IIR) filter](https://en.wikipedia.org/wiki/Infinite_impulse_response) on the AI Engine.

Part 1a describes how to parallelize the IIR filter difference equation for a single second-order section. It also describes how to implement the parallelized difference equation on the SIMD vector processor in the AI Engine.

Part 1b describes implementing a sixth-order low-pass elliptic IIR filter with three second-order sections and changing filter coefficients at runtime using runtime parameters.

Part 2a shows the process of minimizing latency and increasing throughput.

Part 2b introduces more techniques to improve throughput further.

This tutorial also provides [Julia](https://julialang.org/) scripts to calculate the IIR filter coefficients and generate test data.

Future parts are cover performance optimization.

This tutorial assumes that you are familiar with the fundamentals of AI Engine kernel development.

Use the [AI Engine, DSP IP and Tools](https://forums.xilinx.com/t5/AI-Engine-DSP-IP-and-Tools/bd-p/dspip_tools) forum for any questions regarding this tutorial.

## Support

GitHub issues are used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
