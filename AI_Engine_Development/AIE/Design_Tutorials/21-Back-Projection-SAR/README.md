
<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Back-Projection for Synthetic Aperture Radar on AI Engines

***Version: Vitis 2025.1***

## Abstract

Synthetic Aperture Radar (SAR) [[1]] creates a 2D or 3D image of stationary objects or landscapes from a moving platform such as an airplane or spacecraft. The distance travelled over the target by the platform creates a large synthetic antenna aperture that mimics a much larger antenna array yielding superior image resolution. Signal processing combines coherently many radar pulses collected from multiple platform positions above the target. Many algorithms exist to perform SAR. The Back-Projection (BP) algorithm for SAR is one of the easiest to understand and has $O(N^3)$ operations for an $N\times N$ image. While this computational cost is high, BP lends itself naturally to parallel processing [[2]] and finds use in practical systems. This tutorial builds a reference design for BP-based SAR on AI Engines. Using the GOTCHA data set [[3]] with 586 radar pulses, the SAR engine achieves ~2.5 frames per second for a $512\times 512$ image with fewer than 32 tiles. With eight instances of the engine, the design achieves close to 19 frames per second using around 224 tiles. 



## Table of Contents

1. [Introduction](md/intro.md#introduction)
2. [System Model](md/system-model.md#system-model)
3. [System Partitioning](md/system-partitioning.md#system-partitioning)
4. [BackProject Engine](md/backproject-engine.md#backproject-engine)
5. [Multiple Engines](md/multiple-engines.md#multiple-engines)
6. [Design Builds](md/design-builds.md#design-builds) 
7. [Conclusion](md/conclusion.md#conclusion)
8. [References](#references)
9. [Support](#support)
10. [License](#license)

## References

[1]:<https://www.wiley.com/en-us/Synthetic+Aperture+Radar+Signal+Processing+with+MATLAB+Algorithms-p-9780471297062> "Synthetic Aperture Radar Signal Processing with MATLAB Algorithms"
[[1]]: M. Soumekh, "Synthetic Aperture Radar Signal Processing with MATLAB Algorithms", Wiley, April 1999.

[2]:<https://www.spiedigitallibrary.org/conference-proceedings-of-spie/7699/1/SAR-image-formation-toolbox-for-MATLAB/10.1117/12.855375.short> "SAR image formation toolbox for MATLAB"
[[2]]: L.A. Gorham & L.J. Moore, "SAR Image Formation Toolbox for MATLAB", SPIE Defense, Security, and Sensing, Orlando, FL, 2010.

[3]: <https://www.sdms.afrl.af.mil/index.php?collection=gotcha> "GOTCHA Volumetric SAR Data Set"
[[3]]: U.S. Air Force, "GOTCHA Volumetric SAR Data Set", U.S. Air Force Sensor Data Management System.

## Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](http://support.xilinx.com/).

## License

Components: xilinx-images

images in the documentation

Components: xilinx-files

The MIT License (MIT)

Copyright (c) 2025 Advanced Micro Devices, Inc.

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


<p class="sphinxhide" align="center"><sub>Copyright © 2025 Advanced Micro Devices, Inc</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
