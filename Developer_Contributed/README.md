<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1> Vitis™ - Developer Contributed Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Pull Request (PR) Guidelines

> Contributions to Vitis-Tutorials are welcome!

## Send your PRs to the branch that matches the version of the tool you created and verified with the tutorial

1. Provide a clear answer to "What does this PR do?".
2. Read General Rules below.
3. Make sure you add detailed commit info and sign your commits. For example, use `git commit -s`.
4. Add Legal information to the pull request comment.
5. Send your PR.

## General Rules

1. Includes `README.md` file (s) with technical details to communicate the intention of the tutorial, including detailed descriptions of the build flows, all necessary tools with version information, and development system setup information.
2. Only source code and scripts are allowed. Binary files or compressed big-size files should be uploaded to a file server with a link added in the tutorial.
3. Make the tutorial user-oriented, robust, and reusable for end users, even at the cost of writing more code in the background.
4. Provide necessary information for a general legal review, make sure the code allows the license that Vitis tutorials support.

## Developer Contributed Tutorials

<table style="width:100%">
<tr>
<td width="35%" align="center"><b>Tutorial</b>
<td width="65%" align="center"><b>Description</b>
</tr>
<tr>
<td align="center"><a href="./01-Versal_Custom_Thin_Platform_Extensible_System/">Versal Custom Thin Platform Extensible System</a></td>
<td>Versal VCK190 System Example Design based on a thin custom platform (Minimal clocks and AXI exposed to PL) including HLS/RTL kernels and AI Engine kernel using a full Makefile build-flow.</td>
</tr>
<td align="center"><a href="./02-AIE_DSP_with_Makefile_and_GUI/">DSP Design on AI Engine with GUI and Makefile Flows</a></td>
<td>This tutorial implements a FIR filter from the Vitis DSP Library into the AI Engine domain, either with Makefile or GUI based flows. The design runs on the VCK190 board. The host application applies XRT APIs and Petalinux.</td>
</tr>
<td align="center"><a href="./03-HLS_Code_Optimization/">Vitis HLS Optimization Techniques on Embedded Boards</a></td>
<td>This tutorial illustrates some C/C++ code optimization techniques for high performance with Vitis HLS. Some HLS are also implemented into ZCU102 or VCK190 target boards with the Vitis HW Acceleration flow. </td>
</tr>
</table>



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
