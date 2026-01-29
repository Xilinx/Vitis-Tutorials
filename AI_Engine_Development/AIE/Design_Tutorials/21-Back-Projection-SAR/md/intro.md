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

## Introduction

### Goals
This tutorial implements the Back-Projection (BP) algorithm for Synthetic Aperture Radar (SAR) on AI Engines. The goal is to show a larger system design example with several different computational workloads. BP makes a good example because it contains a half-dozen workloads with different characteristics that must be stitched together into an efficient data flow between DDR and the AI Engine over the NoC using GMIO. The tutorial shows the following AI Engine design concepts:

* Using system partitioning methodology to plan and de-risk a larger system design example.
* Using multi-rate AI Engine scheduling to coordinate processing of different workloads at the system level.
* Implementing vectorized function approximation kernels for `cos()`, `sin()` and `sqrt()` using new Vitis DSP Library IPs.
* Managing dynamic range across several AI Engine kernels and compute workloads.
* Optimizing the throughput performance of a single AI Engine compute engine.
* Illustrating how to parallelize the system level application across multiple compute engines to increase throughput.

### GOTCHA Volumetric SAR Data Set

This tutorial uses the "GOTCHA Volumetric SAR Data Set" [[1]] to develop, validate, test, and characterize the performance of the BP algorithm on AI Engines. This data set is from the U.S. Air Force Sensor Data Management System and is available for public download from the following link. The repository consists of SAR phase history data. The data was collected at X-band with a 640 MHz bandwidth, and full azimuth coverage at eight elevation angles with full polarization. The target scene consists of many civilian vehicles and calibration targets. 

To run the design examples provided in this tutorial, download the GOTCHA data set to `<path-to-tutorial>/GOTCHA-CP` such that each of its eight different elevation angle folders appear as `<path-to-tutorial>/GOTCHA-CP/DATA/pass1` through `<path-to-tutorial>/GOTCHA-CP/DATA/pass8`, respectively.


### References

[1]: <https://www.sdms.afrl.af.mil/index.php?collection=gotcha> "GOTCHA Volumetric SAR Data Set"
[[1]]: U.S. Air Force, "GOTCHA Volumetric SAR Data Set," U.S. Air Force Sensor Data Management System.

<p class="sphinxhide" align="center"><sub>Copyright © 2025 Advanced Micro Devices, Inc</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
