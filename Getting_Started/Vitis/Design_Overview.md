<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Design Overview
 
 In this section we will discuss a design conceptually and later in this tutorial we will try to use the Vitis tools to implement the design on a Versal device.
 The arrow in the below diagram represents the control path:
 ```mermaid
flowchart LR
    A[APU] -.-> B[MM2S]
    A[APU] -.-> C[MM2S]
    B --> D[MatMul]
    C --> D[MatMul]
    D --> E[S2MM]
    E --> F[DDR]
    A -.-> F
```

The plan is to send the data from APU/host application to AI Engine to perform Matrix Multiplication and save the output result in DDR, then use the APU to read the data from DDR and compare the results to print whether the test is a "Pass" or "Fail".

We will use the AMD provided platform instead of a custom platform to steer away from platform creation flow and to keep the tutorial as a beginner’s guide. 

Here is the step-by-step guide to design a Versal design from scratch:

1. [**Prepare AI Engine kernel**](./AIE_Kernel/README.md)
2. [**Prepare HLS kernels**](./HLS_Kernels/README.md)
3. [**Create a host application using AMD VCK190 platform**](./Host_srcs/README.md)
4. [**Use Vitis Linker and Packager to build the design**](./vitis_dir/README.md)


<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
