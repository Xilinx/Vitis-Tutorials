
<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Implementing FFT and DFT Designs on AI Engines

***Version: Vitis 2024.1***

## Abstract

Fast Fourier Transform (FFT) is essential in digital signal processing (DSP). The AI Engine array of
very-long instruction word (VLIW) processors with single instruction multiple data (SIMD) vector
units are highly optimized for compute-intensive DSP algorithms such as FFT and Discrete
Fourier Transform (DFT). This tutorial illustrates several different techniques for
mapping FFT and DFT algorithms to the AI Engine array, including the Stockham FFT used in
AMD Vitis™ DSPlib, hand-coded variants implemented using the AI Engine API, and a direct form
DFT using vector-matrix multiplication that can be efficient on AI Engine for small point sizes.

## Table of Contents

1. [Introduction](md/intro.md#introduction)
2. [FFT Designs On AI Engine](md/fft-aie.md#fft-aie)
3. [DFT Designs on AI Engine](md/dft-aie.md#dfe-aie)
4. [Conclusion](md/conclusion.md#conclusion)

## References

1. [The Fast Fourier Transform](https://ieeexplore.ieee.org/abstract/document/5217220)
2. [Vitis Libraries](https://docs.xilinx.com/r/en-US/Vitis_Libraries/dsp/index.html)
3. [UG1529: AI Engine API User Guide](https://www.xilinx.com/htmldocs/xilinx2023_2/aiengine_api/aie_api/doc/index.html)
4. [Fast Computation of General Fourier Transforms on GPUs](https://ieeexplore.ieee.org/document/4607357)
5. [Block-by-Block Configurable Fast Fourier Transform Implementation on AI Engine (XAPP1356)](https://docs.xilinx.com/r/en-US/xapp1356-fft-ai-engine/Summary)

## Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](http://support.xilinx.com/).


<p class="sphinxhide" align="center"><sub>Copyright © 2023-2024 Advanced Micro Devices, Inc</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
