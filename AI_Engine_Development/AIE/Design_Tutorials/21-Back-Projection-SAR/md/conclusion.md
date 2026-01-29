<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">Refer to the Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">Refer to the Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Back-Projection for Synthetic Aperture Radar on AI Engines

## Conclusion

This tutorial presented a detailed design of a Back-Projection engine for Synthetic Aperture Radar on AI Engines. Using the GOTCHA data set with 586 radar pulses, the design achieves ~2.5 frames per second for a $512\times 512$ target image with fewer than 32 AI Engine tiles. Eight instances of the engine achieves almost an 8-fold increase in throughput. 

The tutorial has illustrated some important concepts and new features in the Versal AI Engine tool flows and methodology:

* This design makes heavy use of the new `func_approx()` IP available in Vitis DSP Libraries to approximate `sqrt()`, `sin()`, and `cos()` with piecewise linear approximations based on LUTs. 

* The design illustrates how to use PL URAM storage coupled in a high performance feedback loop with the AI Engine array. HLS coding techniques illustrate how to handle AXI-S stream back-pressure to isolate the read and write portions of the design.  

* Vitis Functional Simulation methodology evaluated system level performance of the SAR algorithm on the GOTCHA data set using a SSIM image quality metric. This methodology supports system level validation of the AI Engine algorithms with design models integrated directly into the MATLAB-based system modeling framework.

The tutorial provides full device level designs for both the single engine and 8-engine versions of the design. The tutorial explores opportunities for further design optimization, including migrating the PL URAM buffers to DDR and to use a GMIO-based data flow. This topic is currently under study.



<p class="sphinxhide" align="center"><sub>Copyright © 2025 Advanced Micro Devices, Inc</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
