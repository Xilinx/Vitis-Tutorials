<!--
Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
Author: Mark Rollins
-->
﻿<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Implementing FFT and DFT Designs on AI Engines

***Version: Vitis 2023.2***

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

## License

Components: xilinx-images

images in the documentation

Components: xilinx-files

The MIT License (MIT)

Copyright (c) 2024 Advanced Micro Devices, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

<p class="sphinxhide" align="center">  &copy; Copyright 2024 Advanced Micro Devices, Inc.</p>
<p class="sphinxhide" align="center">  &copy; Copyright 2021 Xilinx Inc.</p>

