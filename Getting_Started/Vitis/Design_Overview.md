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

1. [**Prepare AI Engine kernel**](./Versal_w_PetaLinux/VCK190/AIE_Kernel/README.md)
2. [**Prepare HLS kernels**](./Versal_w_PetaLinux/VCK190/HLS_Kernels/README.md)
3. [**Create a host application using AMD VCK190 platform**](./Versal_w_PetaLinux/VCK190/Host_srcs/README.md)
4. [**Use Vitis Linker and Packager to build the design**](./Versal_w_PetaLinux/VCK190/vitis_dir/README.md)


<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
