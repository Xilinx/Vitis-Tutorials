<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ Getting Started Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>


# Compilation of HLS Kernels

There are three important factors to understand the requirement of HLS kernels: Memory Map to Stream (MM2S) and Stream to Memory Map (S2MM)
- AI Engine kernel used in this design uses PLIO which expects the AXI-Stream as an input
- APU and DDR connectivity is AXI-MM (memory map)
- The MM2S and S2MM kernels are used as data movers to convert the data from AXI-MM to AXIS and AXIS to AXI-MM

In this chapter we will compile the C++ code to create HLS kernels.
1. Compile of MM2S HLS kernel
2. Compile of S2MM HLS kernel

## 1. Compile of MM2S HLS kernel
The MM2S code (mm2s.cpp) is simple and as follows:
```
void mm2s(ap_int<32>* mem, hls::stream<ap_axis<32, 0, 0, 0>  > &s, int size) {  
	for(int i = 0; i < size; i++) {    
		ap_axis<32, 0, 0, 0> x;  
		x.data = mem[i];  
		s.write(x);  
```
The "mem" is the AXI-MM input to HLS kernel and "s" is the AXI-Stream output from the kernel.
"for" loop input the data AXI-MM data and generates AXI-Stream data.

To compile the above code, we will use v++ -c --mode hls command:
```
v++ -c --mode hls --platform $(PLATFORM) --config mm2s.cfg
```
The above command will generate a HLS kernel mm2s.xo.

## 2. Compile of S2MM HLS kernel
The S2MM code (s2mm.cpp) is simple and as follows:
```
void s2mm(ap_int<32>* mem, hls::stream<ap_axis<32, 0, 0, 0>  >& s, int size) {
	for(int i = 0; i < size; i++) {
		ap_axis<32, 0, 0, 0> x = s.read();
		mem[i] = x.data;
	}
```
The "s" is the AXI-Stream input to HLS kernel and "mem" is the AXI-MM output from the kernel.
"for" loop input the data AXI-Stream data and generates AXI-MM data.

To compile the above code, we will use v++ -c --mode hls command:
```
v++ -c --mode hls --platform $(PLATFORM) --config s2mm.cfg
```
The above command will generate a HLS kernel s2mm.xo.


Next Chapter: [Create a host application using AMD VCK190 platform](../Host_srcs/README.md)


<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
