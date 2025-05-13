
<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Back-Projection for Synthetic Aperture Radar on AI Engines
## Introduction

### Goals
This tutorial implements the Back-Projection (BP) algorithm for Synthetic Aperture Radar (SAR) on AI Engines. The goal is to illustrate a larger system design example with several different computational workloads. BP makes a good example since it contains a half-dozen workloads with different characteristics that must be stitched together into an efficient data flow between DDR and the AI Engine over the NoC using GMIO. The tutorial illustrates the following AI Engine design concepts:

* Using system partitioning methodology to plan and de-risk a larger system design example.
* Using multi-rate AI Engine scheduling to coordinate processing of different workloads at the system level.
* Implementing vectorized function approximation kernels for `cos()`, `sin()` and `sqrt()` using new Vitis DSP Library IP's.
* Managing dynamic range across several AI Engine kernels & compute workloads.
* Optimizing the throughput performance of a single AI Engine compute engine.
* Illustrating how to parallelize the system level application across multiple compute engines to increase throughput.

### GOTCHA Volumetric SAR Data Set

This tutorial uses the "GOTCHA Volumetric SAR Data Set" [[1]] to develop, validate, test & characterize the performance of the BP algorithm on AI Engines. This data set was captured by the U.S. Air Force Sensor Data Management System and is made available for public download using the link below. The repository consists of SAR phase history data collected at X-band with a 640 MHz bandwidth with full azimuth coverate at 8 different elevation angles with full polarization. The target scene consists of many civilian vehicles and calibration targets. 

In order to run the design examples provided in this tutorial, please download the GOTCHA data set to the folder `<path-to-tutorial>/GOTCHA-CP` such that each of its 8 different elevation angle folders appear as `<path-to-tutorial>/GOTCHA-CP/DATA/pass1` through `<path-to-tutorial>/GOTCHA-CP/DATA/pass8`, respectively.


### References

[1]: <https://www.sdms.afrl.af.mil/index.php?collection=gotcha> "GOTCHA Volumetric SAR Data Set"
[[1]]: U.S. Air Force, "GOTCHA Volumetric SAR Data Set", U.S. Air Force Sensor Data Management System.

<p class="sphinxhide" align="center"><sub>Copyright © 2025 Advanced Micro Devices, Inc</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
