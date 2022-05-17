<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Getting Started with RTL Kernels

***Version: Vitis 2022.1***

## Tutorial Overview

In the Vitis application acceleration development flow, as explained in [Kernel Properties](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/PL-Kernel-Properties), RTL kernels can be either XRT-managed kernels adhering to XRT requirements of the `ap_ctrl_chain` or `ap_ctrl_hs` control protocols for execution control, or can be user-managed kernels that do not meet this standard but rather implement any number of user-defined control schemes specified in the RTL design. You can use an existing execution mechanism in an existing RTL design, or design your RTL to support the XRT control scheme. 

In this tutorial you will examine the process of packaging an existing RTL design with a user-managed control scheme into an RTL kernel, and review the host application requirements to integrate that kernel into your accelerated application. 

This tutorial includes an RTL design containing a simple vector accumulation example that performs a `B[i] = A[i]+B[i]` operation, which you will build into a Xilinx compiled object file (.xo) for use in the Vitis flow. It also contains a host application using the [XRT native API](https://xilinx.github.io/XRT/master/html/xrt_native_apis.html) which interacts with the kernel. The host application:

1. loads the device executable (`.xclbin`) into the Xilinx device on the accelerator platform
2. identifies the kernel and implements the user-designed control structure
3. creates read/write buffers to transfer data between global memory and the Xilinx device
4. starts the RTL kernel on the accelerator target by writing to a register, and waits for the kernel to signal its completion by reading a register
5. reads back the data to examine the results

Using these reference files, the tutorial guides you through the process of building and running your project.

## Before You Begin

The labs in this tutorial use:

* BASH Linux shell commands
* 2022.1 Vitis core development kit release and the *xilinx_u200_gen3x16_xdma_2_202110_1* platform. 
* If necessary, the tutorial can be easily extended to other versions and platforms.

>**IMPORTANT:**  
>
> * Before running any of the examples, make sure you have the Vitis core development kit installed as described in [Installation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installation).
>* If you run applications on Xilinx® Alveo™ Data Center accelerator cards, ensure the card and software drivers have been correctly installed by following the instructions on the [Alveo Portfolio page](https://www.xilinx.com/products/boards-and-kits/alveo.html).

### Accessing the Tutorial Reference Files

1. To access the reference files, type the following into a terminal: `git clone https://github.com/Xilinx/Vitis-Tutorials`
2. Navigate to `Vitis-Tutorials/Hardware_Acceleration/Feature_Tutorials/01-rtl_kernel_workflow/reference-files` directory.

## Requirements for Using an RTL Design as an RTL Kernel

To use an RTL kernel within the Vitis IDE, it must meet both the Vitis core development kit execution model and the hardware interface requirements as described in [RTL Kernels](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/RTL-Kernels) in the Vitis Application Acceleration Development Flow documentation (UG1393).

RTL kernels can use the same software interface and execution model as C/C++ software functions. They are seen by the host application as functions with a void return value, scalar and pointer arguments.

- It must start when called.
- It is responsible for processing data to provide the necessary results.
- It must send a notification when processing is complete.

For instance, the function signature for the RTL kernel in this tutorial:

```
void vadd_A_B(int *a, int *b, int scalar)
```

### Vector-Accumulate RTL IP

For this tutorial, the Vector-Accumulate RTL IP performing `B[i]=A[i]+B[i]` meets all the requirements described above and has the following characteristics:

- An AXI4-Lite slave interface (`s_axilite`) used to access programmable registers (control registers, scalar arguments, and pointer base addresses).
  - Offset `0x00` - Control Register: Controls and provides kernel status
    - Bit `0`: **start signal**: Asserted by the host application when kernel can start processing data. Must be cleared when the **done** signal is asserted.
    - Bit `1`: **done signal**: Asserted by the kernel when it has completed operation. Cleared on read.
    - Bit `2`: **idle signal**: Asserted by this signal when it is not processing any data. The transition from Low to High should occur synchronously with the assertion of the **done** signal.
  - Offset `0x10` Register for the scalar `size` argument
  - Offset `0x18` Register specifying the base address for pointer argument `A`
  - Offset `0x24` Register specifying the base address for pointer argument `B`
- Two AXI4 memory mapped interfaces for the pointer arguments, `A` and `B`, used to communicate with global memory.
    - All AXI4 master interfaces must have 64-bit addresses.
    - The RTL designer is responsible for partitioning global memory spaces to specify the kernel arguments. The base address (memory offset) for each partition must be set by a control register programmable through the `s_axilite` interface as described above.

>**TIP:** If your original RTL design uses a different execution model or hardware interfaces, you must customize your host application to address the kernel as required or add logic to the kernel to ensure that the design behaves in an expected manner.

## Next Steps

This tutorial demonstrates how to package RTL IPs as Vitis kernels (`.xo`), and use them in the Vitis core development kit. The tutorial uses the [Package IP/Package XO](./package_ip.md) flow to package an existing RTL module as a Vivado IP, and package that IP as a Vitis kernel (`.xo`). 

</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="/README.md">Return to Main Page</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2021-2022 Xilinx</sup></p>
