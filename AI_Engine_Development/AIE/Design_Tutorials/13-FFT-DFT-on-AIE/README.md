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

# Implementing FFT and DFT Designs on AI Engines

***Version: Vitis 2025.2***

## Abstract

Fast Fourier Transform (FFT) is essential in digital signal processing (DSP). The AI Engine array of very-long instruction word (VLIW) processors with single instruction multiple data (SIMD) vector units are highly optimized for compute-intensive DSP algorithms such as FFT and Discrete Fourier Transform (DFT). This tutorial illustrates several different techniques for mapping FFT and DFT algorithms to the AI Engine array, including the Stockham FFT used in AMD Vitis™ DSPlib, hand-coded variants implemented using the AI Engine API, and a direct form DFT using vector-matrix multiplication that can be efficient on AI Engine for small point sizes.

## Table of Contents

1. [Introduction](md/intro.md#introduction)
2. [FFT Designs On AI Engine](md/fft-aie.md#fft-aie)
3. [DFT Designs on AI Engine](md/dft-aie.md#dfe-aie)
4. [Conclusion](md/conclusion.md#conclusion)

## References

1. [The Fast Fourier Transform](https://ieeexplore.ieee.org/abstract/document/5217220)
2. [Vitis Libraries](https://docs.amd.com/r/en-US/Vitis_Libraries/dsp/index.html)
3. [UG1529: AI Engine API User Guide](https://download.amd.com/docnav/aiengine/xilinx2025_2/aiengine_api/aie_api/doc/index.html)
4. [Fast Computation of General Fourier Transforms on GPUs](https://ieeexplore.ieee.org/document/4607357)
5. [Block-by-Block Configurable Fast Fourier Transform Implementation on AI Engine (XAPP1356)](https://docs.amd.com/r/en-US/xapp1356-fft-ai-engine/Summary)

## Support

GitHub issues are used for tracking requests and bugs. For questions, go to [support.xilinx.com](https://adaptivesupport.amd.com/s/topiccatalog?language=en_US).

<p class="sphinxhide" align="center"><sub>Copyright © 2023-2026 Advanced Micro Devices, Inc.</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
