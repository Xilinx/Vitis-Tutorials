<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Keywords

### What is XSA?

XSA is a Shell Archive, a container containing below information:

- One or more .hwh files
- AMD Vivado™ tool version, part, and board tag information
- IP - instance, name, VLNV, and parameters
- Memory Map information of the processors
- Internal Connectivity information (including interrupts, clocks, etc.) and external ports information
- BMM/MMI and BIT files
- User and HLS driver files
- Device tree files
- Other meta-data files

Please refer to [UG1725](https://docs.amd.com/r/en-US/ug1725-xsdb-reference-guide/XSA) for more details.

### What is Kernel?

Kernel = Computational Unit = Hardware Accelerators 
Kernels can be AI Engine kernels or PL (HLS) kernels. 
To explain in simple language, a kernel is a computational function offloaded to the AI Engine or to a PL logic using AI Engine and HLS tools.

Please refer to [AIE-Kernel](https://docs.amd.com/v/u/en-US/aie-kernel) for more details.


### What is XRT/AMD Runtime Library?

XRT is the AMD Runtime Library to help application development. 
The XRT helps to facilitate the hardware commumication from the user application development. The XRT manages the hardware access using API and keeps the hardwarwe complexities away from the application developer. 
Helps the application developer to bridge the gap between the software and hardware. Supports Linux only.
ForExample: Kernel Management, Memory Management, Profiling and Debugging. 

How does it help? 
Take an example of accessing memory in Baremetal application, the user need to open the platform to make a note of memory addres to manually manage the memory.
Use of XRT APIs can help to allocate and manage the memory without getting into the low-leve hardware details. 

Please refer [link](https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis/vitis-runtime-library.html) to better understand the XRT.

<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
