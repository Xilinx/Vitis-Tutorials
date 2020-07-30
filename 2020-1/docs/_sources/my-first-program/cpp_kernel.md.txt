<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">See 2019.2 Vitis Application Acceleration Development Flow Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 1. Coding My First C++ Kernel

This lab explains the coding requirements for a Vitis™ core development kit kernel. In this lab, you will look at a simple vector addition kernel (`vadd.cpp`, found in the `reference-files/src` folder.

As mentioned previously, a Vitis core development kit application consists of a software program running on a host CPU, interacting with one or more accelerators running on a Xilinx FPGA. The host program uses API calls to transfer data to and from the FPGA and interact with the accelerators. Data transfers occur by copying data from the host memory to device memory in the FPGA acceleration card. Device memory is subdivided into different memory banks. Accelerators (also referred to as kernels) have memory interfaces that allow them to connect to these memory banks and access data shared with the host program.

In this lab, you will:

1. Learn the interface requirements necessary for a kernel.
2. Understand how function arguments are mapped to the interfaces.
3. Edit the VADD function to turn it into a kernel compatible with the Vitis core development kit.

## Kernel Interface Requirements

Before you can turn a function into a kernel, you must first understand the kernel interface requirements. Every C++ function needs to meet certain interface requirements to be used as a kernel in the Vitis core development kit. All kernels require the following hardware interfaces:

- A single [AXI4-Lite](https://www.xilinx.com/products/intellectual-property/axi.html#details) slave interface used to access control registers (to start and stop the kernel) and to pass scalar arguments.
- At least one of the following interfaces (the kernel can have both interfaces):
  - [AXI4 Master](https://www.xilinx.com/products/intellectual-property/axi.html#details) interface to communicate with memory.
  - [AXI4-Stream](https://www.xilinx.com/products/intellectual-property/axi.html#details) interface for transferring data between kernels, or between the host and kernel.

Kernel executions rely on the following mechanics and assumptions:

- Scalar arguments are passed to the kernel through its AXI4-Lite slave interface.
- Pointer arguments are passed to the kernel through its AXI4-Lite slave interface.
- Pointer data resides in global memory (DDR or PLRAM).
- Kernels access pointer data in global memory through one or more AXI4 memory mapped interfaces.
- Kernels are started by the host program through control signals on the AXI4-Lite interface of the kernel.

## VADD Example

### C Code Structure

The following code shows the kernel signature of the VADD function in `vadd.cpp`, which uses only pointer and scalar arguments.

```C++
        void vadd(
                const unsigned int *in1, // Read-Only Vector 1 - Pointer arguments
                const unsigned int *in2, // Read-Only Vector 2 - Pointer arguments
                unsigned int *out,       // Output Result      - Pointer arguments
                int size                 // Size in integer    - Scalar arguments
                )
        {
```

### Mapping the Function Arguments

**IMPORTANT:** By default, kernels modeled in C/C++ do have automatic mapping to the physical interfaces that will be used to transport the function arguments. Additionally, the compiler relies on pragmas embedded in the code to determine which physical interface to generate for each port. For the function to be treated as a valid C/C++ kernel, each function argument should have a valid interface (default or specified through pragmas).

#### Pointer Arguments

A pointer argument on the function interface results in two distinct kernel ports. Consequently, each pointer argument requires two interface pragmas.

* In the first port, the kernel will access data in the global memory. This port must be mapped to an AXI Master interface (`m_axi`).
* In the second port, the base address of the data is passed by the host program to the kernel. This port must be mapped to the AXI4-Lite slave interface (`s_axilite`) of the kernel.
* By default, the arguments `in1`, `in2`, and `out` will be mapped to an `m_axi` interface for the first port and an `s_axilite` interface for
the second port.

This default assignment is the same as specifying the following pragmas for the arguments `in1`, `in2`, and `out`. If you want to change this default assignment, you will have to specify and modify the following pragmas with your changes.

   ```C++
   #pragma HLS INTERFACE m_axi     port=in1 offset=slave bundle=gmem
   #pragma HLS INTERFACE m_axi     port=in2 offset=slave bundle=gmem
   #pragma HLS INTERFACE m_axi     port=out offset=slave bundle=gmem
   #pragma HLS INTERFACE s_axilite port=in1              bundle=control
   #pragma HLS INTERFACE s_axilite port=in2              bundle=control
   #pragma HLS INTERFACE s_axilite port=out              bundle=control
   ```

The `m_axi` interface pragmas are used to characterize the AXI Master ports.

* `port`: Specifies the name of the argument to be mapped to the AXI memory mapped interface.
* `offset=slave`: Indicates that the base address of the pointer is made available through the AXI-Lite slave interface of the kernel.
* `bundle`: Specifies the name of the `m_axi` interface. In this example, the three arguments are mapped to a single AXI interface called `gmem`.

The `s_axilite` interface pragmas are used to characterize the AXI4-Lite port.

#### Scalars

A scalar argument on the function interface results in a port which must to be mapped to the AXI4-Lite interface (`s_axilite`) of the kernel. Note that the function's `return` value must be mapped to the AXI4-Lite interface in a similar manner.

In the VADD design, the `size` argument and the `return` value are automatically mapped to `s_axilite` interfaces. If you need to change the default assignment, specify and change the following pragmas:

   ```C++
   #pragma HLS INTERFACE s_axilite port=size              bundle=control
   #pragma HLS INTERFACE s_axilite port=return            bundle=control
   ```

Because a kernel can have only one AXI-Lite interface, all `s_axilite` pragmas must use the same `bundle` name. In this example, use the name `control` for the AXI4-Lite interface.

For more information on the HLS INTERFACE pragmas, refer to [HLS Pragmas](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=hlspragmas.html) in the Vitis HLS flow of the (UG1416).

### Using C Linkage

In addition to adding the interface pragmas, you need to add extern "C" { ... } around the functions to be compiled as kernels. The use of extern "C" instructs the compiler/linker to use the C naming and calling conventions.

```C++
extern "C" {
void vadd(
        const unsigned int *in1, // Read-Only Vector 1
        const unsigned int *in2, // Read-Only Vector 2
        unsigned int *out,       // Output Result
        int size                 // Size in integer
        )
 {
```

The `vadd` function can now be compiled into a kernel using the Vitis toolchain.

## Next Step

The next step in this tutorial is [coding the host program](./host_program.md).
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">Return to Getting Started Pathway</a> — <a href="/docs/my-first-program/README.md">Return to Start of Tutorial</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
