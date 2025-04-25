<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AIE-ML Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

# Matrix Compute with Vitis Libraries on AIE and AIE-ML

***Version: Vitis 2024.2***

## Introduction

In linear algebra, matrix multiplication is a binary operation that generates a matrix from two input matrices.

For this operation to occur, the number of columns in the first matrix must match the number of rows in the second matrix. The resulting matrix, called the matrix product, will have the same number of rows as the first matrix and the same number of columns as the second matrix.

![Matrix Multiplication](./images/1_Intro_Matrix_Mul.png)

The following figure visually represents the product of two matrices, A and B, highlighting how each entry in the product matrix relates to a row from A and a column from B.

For example, value at c11, is the product of  a11  * b11 + a12 * b21. Similarly, value at C33, is the product of  a31  * b13 + a32 * b23.

![Matrix Product](./images/2_Matrix_Product.png)

**IMPORTANT**: Before beginning the tutorial, make sure that you have read and followed the *Vitis Software Platform Release Notes* (v2024.2) for setting up the software and installing the VEK280 base platform.

Before starting this tutorial, run the following steps:
1. Set up your `PLATFORM_REPO_PATHS` environment variable based upon where you downloaded the platform.
2. Download the AMD Vitis™ Libraries from https://github.com/Xilinx/Vitis_Libraries
   For example: `git clone https://github.com/Xilinx/Vitis_Libraries.git` to an appropriate directory.
3. Set the DSPLIB_ROOT to the downloaded Vitis Libraries path. For example, `export DSPLIB_ROOT=/<DSP_LIBRARY_PATH>/Vitis_Libraries/dsp`

# AMD Versal Devices with AI Engine Variants

AMD Versal™ AI Core and Versal AI Edge devices come in both AIE and AIE-ML variants. You need to choose your device carefully. The following table lists the devices that have AIE and AIE-ML variants.

![AIE Variants](./images/0_AMD_Versal_Devices.png)

For more information, please visit the *[AMD Versal Adaptive SoCs page](https://www.amd.com/en/products/adaptive-socs-and-fpgas/versal.html)*. 

# Table of Contents
- [Introduction to DSP Library](#introduction-to-dsp-library) 
- [List of Parameters in General Matrix Multiply (GEMM)](#list-of-parameters-in-general-matrix-multiply-gemm)
- [Configuration of GEMM Parameters](#configuration-of-gemm-parameters)
- [Design Variant 1: Single Tile](#design-variant-1-single-tile)
- [Design Variant 2: 4-tile design with TP_CASC_LEN=4](#design-variant-2-4-tile-design-with-tp_casc_len4)
- [Design Variant 3: 8-tile design with TP_CASC_LEN=4 and TP_SSR=2](#design-variant-3-8-tile-design-with-tp_casc_len4-and-tp_ssr2)
- [Migrate the Design from AIE to AIE-ML and Evalute the Performance Differences](#migrate-the-design-from-aie-to-aie-ml-and-evalute-the-performance-differences)

## Objectives
- Overview of Matrix Multiply/General Matrix Multiply (GEMM) in Vitis libraries
- Configure the GEMM parameters according to your design requirements
- Explore three designs catering to various needs
- Compare the performance of the designs with AIE vs AIE-ML

## Introduction to DSP Library
The DSP library (DSPLib) contains a PL DSP library and an AI Engine DSP library. You will be focusing on the AI Engine DSP Library. This is a configurable library of graphs and kernels that can be used to develop applications on Versal AI Engines. 

This is an open-source library for DSP applications. The kernels are coded using AI Engine APIs in C++ that give access to AI Engine vector processing capabilities. These kernels can be combined to construct graphs for developing complex designs.

An example design is provided with this library for your reference. Each kernel has a corresponding graph. It is highly recommended to use the library element’s L2 graph as the entry point.

The DSPLib contains one Matrix Multiply/GEMM (General Matrix Multiply) solution for AI Engine (AIE) and AIE-ML. The GEMM has two input ports connected to two windows of data. The inputs are denoted as Matrix A (inA) and Matrix B (inB).  An output port connects to a window, where the data for the output matrix is stored. 

The data type of both input matrices can be configured, and the data type of the output is derived from the inputs.

The Matrix Multiply supports a matrix of elements of integer type (int16, cint16, int32, or cint32) multiplied by a matrix of elements of integer type. It also supports a matrix of elements of float type (float, cfloat) multiplied by a matrix of elements of float type. However, a mix of integer types and float types is not supported.

The Matrix Multiply for AIE-ML supports integer types (int16, int32, cint16, and cint32) but does not support floating-point types (float, cfloat).

The graph entry point as follows: ```xf::dsp::aie::blas::matrix_mult::matrix_mult_graph```

## List of Parameters in General Matrix Multiply (GEMM)

The parameters are organized into seven groups as shown in the following figure.
![GEMM Parameters](./images/3_GEMM_Parameters.png)

The first group defines the ***data type***:

* TT_DATA_A: which describes the type of individual data samples input of Matrix A to the GEMM function.
The data type must be one of the following: int16, cint16, int32, cint32, float, cfloat.

* TT_DATA_B: describes the type of individual data samples input of Matrix B to the GEMM function.
The data type must be one of the following: int16, cint16, int32, cint32, float, cfloat.
The following rules apply:
  - must be an integer type if TT_DATA_A is an integer type
  - must be a float type if TT_DATA_A is a float type

The second group specifies the ***matrix dimensions***:

* TP_DIM_A: is an unsigned integer which describes the number of elements along the unique dimension (rows) of Matrix A.

* TP_DIM_AB: is an unsigned integer which describes the number of elements along the common dimension of Matrix A (columns) and Matrix B (rows).

* TP_DIM_B: is an unsigned integer which describes the number of elements along the unique dimension (columns) of Matrix B.

The third group specifies the ***window size***:

* TP_INPUT_WINDOW_VSIZE_A: describes the number of samples in the window API used for input to Matrix A.
It must be of size TP_DIM_A*TP_DIM_AB.

* TP_INPUT_WINDOW_VSIZE_B: describes the number of samples in the window API used for input to Matrix B.
It must be of size TP_DIM_B*TP_DIM_AB Note, the output window will be of size TP_DIM_A * TP_DIM_B.

The fourth group details the ***data order***:

* TP_DIM_A_LEADING: describes the scheme in which the data should be stored in memory. ROW_MAJOR = 0, COL_MAJOR = 1. Note, a COL_MAJOR matrix can be transposed to become a ROW_MAJOR matrix.

* TP_DIM_B_LEADING: describes the scheme in which the data should be stored in memory. ROW_MAJOR = 0, COL_MAJOR = 1.

* TP_DIM_OUT_LEADING: describes the scheme in which the data should be stored in memory. ROW_MAJOR = 0, COL_MAJOR = 1.

The fifth group describes the ***tiling scheme***:

* TP_ADD_TILING_A / TP_ADD_TILING_B / TP_ADD_DETILING_OUT: describes wether or not to add an additional kernel to rearrange the matrix samples into their required position.
Setting this option to 0 indicates that the re-arrangement will be done externally to the AIE matrix multiply graph.

The sixth group addresses the ***parallelization***:

* TP_CASC_LEN: describes the number of AIE kernels the matrix multiplication will be divided into in series.
TP_SSR: describes the number of kernels (or cascaded kernel chains) that will compute the matrix multiplication in parallel. Each SSR rank will receive an equal sized split (along the unique dimension) of Matrix A data.

The seventh group focuses on the selection of ***shift, rounding, and saturation methods***. Various selection options can be found in the *[Vitis Libraries User Guide](https://docs.amd.com/r/en-US/Vitis_Libraries/dsp/rst/class_xf_dsp_aie_blas_matrix_mult_matrix_mult_graph.html)*.

## Configuration of GEMM Parameters

### Configure Matrix Dimension
In the coding section, the first step is to specify the graph entry: `xf::dsp::aie::blas::matrix_mul`.

You can create your own class to configure the GEMM parameters, and the class name is `gemm_16x32x8_graph`.

![Configure Matrix Dimension](./images/4_Configure_Matrix_Dim.png)

The data type and matrix dimensions are configured as follows:
- Matrix A has 16 rows, and matrix B has 8 columns.
- TP_DIM_AB indicates the number of elements along the shared dimension of Matrix A (columns) and Matrix B (rows), which is set to 32. 

Therefore, the final matrices will be:
```
- Matrix A: is 16x32
- Matrix B: is 32x8
- Resulting Matrix C: is 16x8
```
### Configure Data Ordering (LEADING) - A, B, Output

Now you need to set the data ordering for matrices A, B, and the resulting matrix C. But first, let’s clarify what data ordering is.

There are two options for data ordering: Row major and Column major.

In Row major, data is stored in row order. In the following figure, you can see how the data is organized when Row major is enabled.

In Column major, data is stored in column order. Similarly, in the following figure, you can see how the data is arranged when Column major is enabled.

![Configure Data Order](./images/5_Configure_Data_Order.png)

Now, configure the data order for matrices A, B, and C as follows:
```
- Matrix A is set to Column major, so the data should be provided in Column major format.
- Matrix B is set to Row major, meaning the data should be supplied in Row major format.
- Finally, the resulting matrix C is also set to Column major, so the data read from the output must account for this data order.
```
### Configure Buffer Size (window)

Now configure the buffer sizes.

The input buffer size for ``TP_INPUT_WINDOW_VSIZE_A`` is calculated as ***TP_DIM_A * TP_DIM_AB***, which in this case equals 512.

Likewise, the input buffer size for ``TP_INPUT_WINDOW_VSIZE_B`` is set to ***TP_DIM_B * TP_DIM_AB***, resulting in a value of 256.

![Configure Buffer Size](./images/6_Configure_Buffer_Size.png)

### Configure Parallelization (TP_CASC_LEN and TP_SSR)

In this tutorial, you will explore three design variants, with the first design focusing on the use of a single core.

To use a single core, configure both ``TP_CASC_LEN`` and ``TP_SSR`` to 1.

The number of input and output ports is determined by the values of TP_CASC_LEN and TP_SSR.

For the input port **NPORT_I**, it is calculated as ``TP_CASC_LEN * TP_SSR``, resulting in one input port.

For the output port **NPORT_N**, it is set to ``TP_SSR``, which means there will be one output port.

![Configure Parallelization](./images/7_Configure_Parallelization.png)


### Configure Tiling Scheme

**What is Tiling?**

To maximize performance, the GEMM unit requires that the input matrix data is arranged into a specific tiling pattern, where each sub-tile within the matrix is contiguous in memory. Tiler and detiler widgets are offered which can be configured to arrange the input matrix data into this tiling pattern, and also convert the tiled output data to a specified row or column major format, but this may introduce a notable performance and resource overhead. 

For optimal performance of the GEMM unit, it is recommended that the user supplies the input data, and accepts the output data, in the required tiled arrangement.

The following figure demonstrates how a 16x16 input matrix should be rearranged into a 4x4 tiling pattern.

The tiling scheme depends on the data type. In this case, for int16 matrices A and B, the appropriate tiling scheme is 4x4 for both matrices A and B.

![Tiling Scheme](./images/8_Tiling_Scheme.png)

The following table specifies the tiling scheme used for the given data type combination and the corresponding output data type for AIE and AIE-ML devices.

For instance, for the `int32` data type in both matrices A and B, the tiling scheme for `matrix A is 4x4`, while for `matrix B, it is 4x2`.

In the case of **AIE-ML**, both matrix A and matrix B have a tiling scheme of `4x4`.

![Tiling Scheme and Data Type](./images/9_Tiling_Data_Type.png)

**Tiling and Data Ordering:**

As mentioned earlier, the 16x16 matrix should be rearranged into a 4x2 tiling pattern. 

It is important to note the data order, in this case it has been configured as Row major.

The data must be stored contiguously in memory, as illustrated in the following figure.

![Tiling and Data Order](./images/10_Tiling_Data.png)

**Tiling Scheme and Data Ordering Example**

Following is an example of an 8x8 matrix.

![Tiling Scheme and Data Ordering Example](./images/11_Tiling_Data_Order_Example.png)

The **Row Major/Column Major** data order indicates the arrangement of matrices before any tiling occurs.

For an 8x8 matrix with a 4x4 tiling scheme, both column and row major modes illustrate how the 8x8 matrix is populated, while the tiles are derived from this matrix, regardless of whether it was provided in row or column major order.

In Example 1, the data order is row major, and the tiling scheme is 4x4. You can see how the data should be stored contiguously. Similarly, for matrix B, which also uses row major order with a tiling scheme of 4x2, the data is stored contiguously.

In Example 2, the data order is column major, with a tiling scheme of 4x4. Again, notice how the data is stored contiguously. For matrix B, the data order is column major and the tiling scheme is 4x2, with the data also stored contiguously.

The red arrows show the data that should be contiguous in memory as will be used by the GEMM kernel (after tiling). The tiles should be consumed left to right and top to bottom (in row major order).

![Tiling Scheme and Data Ordering Example](./images/11_2_Tiling_Data_Order_Col_Major_Example.png)

> Note: As you prepare to implement the design in AIE and AIE-ML, it is crucial to consider how the data should be stored according to the tiling scheme.

| Architecture        | Data Type | Data Type  | Tiling Scheme   |          | 
|---------------------|-----------|------------|-----------------|----------|
|                     | Matrix A  | Matrix B   | Matrix A        | Matrix B |
| AIE                 | int32     | int32      | 4x4             | 4x2      |
| AIE-ML              | int32     | int32      | 4x4             | 4x4      |      

The tiling scheme for Matrix A is ``4x4``, so the data can be the same for both AIE and AIE-ML designs. 

For Matrix B, the tiling scheme is ``4x2`` for **AIE** and ``4x4`` for **AIE-ML**. Therefore, the data should be stored in a ``4x2`` tiling scheme for **AIE** and a ``4x4`` tiling scheme for **AIE-ML**.

## Design Variant 1: Single Tile

The goal of Design 1 is to use a single tile for matrix multiplication.

Observe the parameter configuration as shown in the following figure.

![Design Variant 1 Parameter Configuration](./images/12_Design_1_Parameter_Config.png)

The data type is defined as ```int32``` for both ***matrix A*** and ***matrix B*** inputs.

**Matrix A** has dimensions of *16x32*, while **matrix B** is set to *32x8*.

The data order for **matrix A** and the resulting **matrix C** is configured as ``Column major``, whereas **matrix B** is set as ``Row major``.

The tiling scheme is configured to exclude, meaning the user will handle the re-arrangement externally to the AIE matrix multiply graph. This approach avoids adding an additional kernel to rearrange the matrix samples into their required positions.

The input buffer size for **TP_INPUT_WINDOW_VSIZE_A** is calculated as ``TP_DIM_A * TP_DIM_AB``, which in this case equals ***512***.

Likewise, the input buffer size for **TP_INPUT_WINDOW_VSIZE_B** is set to ``TP_DIM_B * TP_DIM_AB``, resulting in a value of ***256***.

Because we plan to use a single AIE core, set both **TP_CASC_LEN** and **TP_SSR** to ```1```.

The number of input and output ports is determined by the values of TP_CASC_LEN and TP_SSR.

For the input port **NPORT_I**, it is calculated as ```TP_CASC_LEN * TP_SSR```, resulting in ***one*** input port.

For the output port **NPORT_O**, it is set to ```TP_SSR```, which means there will be ***one*** output port.

### Change the Project Path
Enter the following command to navigate to the project path of the design variant 1:
```
$ cd <path-to-tutorial>/aie/gemm_16_32x8
```

### Review the gemm_16x32x8_graph.h file

Open the ```gemm_16x32x8_graph.h``` file and review the code:

- Graph entry namespace as ```dsplib = xf::dsp::aie::blas::matrix_mult;```
- Class defined as ```class gemm_16x32x8_graph : public graph {```
- GEMM parameters defined 
- Passing the defined parameters as ```using TT_GEMM = dsplib::matrix_mult_graph<>```
- Observe how the input and output port names are generated

The number of input and output ports is determined by the NPORT_I and NPORT_O parameters.

For example, using NPORT_I as the loop counter, the port for matrix A is named PLIO_A_0, PLIO_A_1, and so on, while the port for matrix B is named PLIO_B_0, PLIO_B_1, etc.

Similarly, using NPORT_O as the loop counter, the output ports are named PLIO_0_o, PLIO_1_o, and so forth.

Close the ```gemm_16x32x8_graph.h``` file after complete your review.

Similarly, review the ```gemm_16x32x8_graph.cpp``` file. After completing the review, close this file.

### Compile and Simulate the Design Variant 1: Single Tile

Enter the following command to compile (x86compile) and simulate (x86sim) to verify the functional correctness of the design:
```
$ make x86com
$ make x86sim
```
The first command compiles the graph code for simulation on an x86 processor, the second command runs the simulation. 

To verify the results, make sure you have already invoked MATLAB in your command line and run the following command:
```
$ make check_sim_output_x86
```
This command invokes MATLAB to compare the simulator output against golden test vectors. The console should output as follows:
```
Max err: 0
--- PASSED ---
```

To understand the performance of the design, you can perform AI Engine emulation using the SystemC simulator by entering the following sequence of commands:

Enter the following command to compile the design for aiesim:
```
$ make clean
$ make all
$ make profile
$ make check_sim_output_aie
```
The first first command deletes all the previously generated files, the second command compiles graph code for the SystemC simulator, the third command runs the AIE simulation, and the final command invokes MATLAB to compare the simulation output with the golden test vectors. The average throughput for the IO ports is displayed at the end of AIE simulation.
The output port PLIO_0_o throughput (MBps) is 1137.78.

After running the last command (`make check_sim_output_aie`) to verify the results, the console should output as follows: 
```
Max err: 0
--- PASSED ---
```

### Analyze the Reports
Enter the following command to launch the Vitis Analyzer and review the reports.
```
$ make analyze
```
Select the `Graph` view.

![Design 1 Graph View](./images/13_Design_1_Graph_View.png)

The Graph view shows the kernels in the graph and the input/output ports of the graph. Select the I/O tabs as shown in the preceding figure. Observe the Throughput column in the I/O tab.
Click the `Array` view, where you can see the tile placement of the kernel, the memory used in tiles, and the PLIO connections.

Close the Vitis Analyzer.

### Comparison of the Designs

| Design              | TP_CASC_LEN | TP_SSR | NPORT_I | NPORT_O | Throughput  |
|---------------------|-------------|--------|---------|---------|-------------|
| Design Variant 1    |      2      |    1   |    1    |    1    | 1137 MBPS   |


## Design Variant 2: 4-tile design with TP_CASC_LEN=4

In Design 2, the goal is to use four tiles. We need to adjust the parameters to accommodate a four-tile configuration.

All the parameters shown in the following figure are the same except the TP_CASC_LEN.

![Design Variant 2 Parameter Configuration](./images/14_Design_2_Parameter_Config.png)

Because a four AIE core is being used, set `TP_CASC_LEN` to **4** from **1**. 

The number of input and output ports is determined by the values of TP_CASC_LEN and TP_SSR.

For the input port **NPORT_I**, it is calculated as `TP_CASC_LEN * TP_SSR`, resulting in **four** input ports.

For the output port **NPORT_N**, it is set to `TP_SSR`, which means there will be **one** output port.

### Change the Project Path
Enter the following command to change the project path:
```
cd ../gemm_16x32x8_cascade
```
### Review the gemm_16x32x8_graph.h file
Open the ```gemm_16x32x8_graph.h``` file and review the code:

- Observe the `TP_CASC_LEN` is set to **4** (from **1** in previous design).  
- The number of input and output ports is determined by the NPORT_I and NPORT_O parameters which is based on `TP_CASC_LEN` and `TP_SSR`.

Close the file after completing your review.

### Compile and Simulate the Design Variant 2: 4-tile design with TP_CASC_LEN=4
To understand the performance of the design, you can perform AI Engine emulation using the SystemC simulator by entering the following sequence of commands:

Enter the following command to compile the design for aiesim:
```
$ make clean
$ make all
$ make profile
$ make check_sim_output_aie
```

The average throughput for the IO ports is displayed at the end of AIE simulation.
The output port PLIO_0_o throughput (MBps) is 2578.05.

After running the last command (`make check_sim_output_aie`) to verify the results, the console should output as follows: 
```
Max err: 0
--- PASSED ---
```
### Analyze the Reports
Enter the following command to launch the Vitis Analyzer and review the reports.
```
$ make analyze
```
Select the `Graph` view.

![Design 2 Graph View](./images/15_Design_2_Graph_View.png)

The Graph view displays the kernels within the graph along with the graph's input and output ports. It shows that four AI Engine kernels are implemented and four input ports are utilized in the design as the `TP_CASC_LEN` is set to **4**.

Select the I/O tabs as shown in the preceding figure. Observe the Throughput column in the I/O tab.

Click the `Array` view, where you can see the tile placement of the kernel, the memory used in tiles, and the PLIO connections.

Close the Vitis Analyzer.

### Comparison of the Designs

| Design              | TP_CASC_LEN | TP_SSR | NPORT_I | NPORT_O | Throughput  |
|---------------------|-------------|--------|---------|---------|-------------|
| Design Variant 1    |      2      |    1   |    1    |    1    | 1137 MBPS   |
| Design Variant 2    |      4      |    1   |    4    |    1    | 2578 MBPS   |

## Design Variant 3: 8-tile design with TP_CASC_LEN=4 and TP_SSR=2

In Design 3, the goal is to use eight tiles. You need to adjust the parameters to accommodate a eight-tile configuration.

All the parameters shown in the following figure are same except the `TP_SSR`.

![Design Variant 3 Parameter Configuration](./images/16_Design_3_Parameter_Config.png)

Because an eight AIE core is being used, set `TP_SSR` to **2** from **1** and the `TP_CASC_LEN` is the same as 4.

For the input port **NPORT_I**, it is calculated as `TP_CASC_LEN * TP_SSR`, resulting in **eight** input ports.

For the output port **NPORT_N**, it is set to `TP_SSR`, which means there will be **two** output ports.

### Change the Project Path
Enter the following command to change the project path:
```
cd ../gemm_16x32x8_cascade_ssr
```
### Review the gemm_16x32x8_graph.h file
Open the ```gemm_16x32x8_graph.h``` file and review the code:

- Observe the `TP_SSR` is set to **2** (from **1** in previous design).  
- The number of input and output ports is determined by the NPORT_I and NPORT_O parameters which is based on `TP_CASC_LEN` and `TP_SSR`.

Close the file after completing your review.

### Compile and Simulate the Design Variant 3: 8-tile design with TP_CASC_LEN=4 and TP_SSR=2
To understand the performance of the design, you can perform AI Engine emulation using the SystemC simulator by entering the following sequence of commands:

Enter the following command to compile the design for aiesim:
```
$ make clean
$ make all
$ make profile
$ make check_sim_output_aie
```

The average throughput for the IO ports is displayed at the end of AIE simulation.
The throughput for the output port is approximately 3935 MBYTES/S (i.e., PLIO_0_o + PLIO_1_o).

After running the last command (`make check_sim_output_aie`) to verify the results, the console should output as follows: 
```
Max err: 0
--- PASSED ---
```

### Analyze the Reports
Enter the following command to launch the Vitis Analyzer and review the reports.
```
$ make analyze
```
Select the `Graph` view.

It shows that eight AI Engine kernels are implemented, eight input ports for each matrix input and two output ports are used in the design as the `TP_CASC_LEN` is set to **4** and `TP_SSR` is set to **2**.

The design implements `eight` AI Engine kernels, with `eight` input ports for each matrix input and `two` output ports. This configuration is due to `TP_CASC_LEN` being set to 4 and `TP_SSR` set to 2.

![Design 3 Graph View](./images/17_Design_3_Graph_View.png)

Select the I/O tabs and observe the Throughput column in the I/O tab.

Click the `Array` view, where you can see the tile placement of the kernel, the memory used in tiles, and the PLIO connections.

![Desgin 3 Array View](./images/18_Design_3_Array_View.png)

Close the Vitis Analyzer.

### Comparison of the Designs

| Design              | TP_CASC_LEN | TP_SSR | NPORT_I | NPORT_O | Throughput  |
|---------------------|-------------|--------|---------|---------|-------------|
| Design Variant 1    |      2      |    1   |    1    |    1    | 1137 MBPS   |
| Design Variant 2    |      4      |    1   |    4    |    1    | 2578 MBPS   |
| Design Variant 3    |      4      |    2   |    8    |    2    | 3935 MBPS   |


## Migrate the Design from AIE to AIE-ML and Evalute the Performance Differences

Migrating the design from AIE to AIE-ML is straightforward. The only modification required is to update the device name.

There are no changes needed in the code. AIE and AIE-ML are compatible, making it easy to migrate the design.

### Change the Project Path
Enter the following command to change the project path:
```
cd ../../aie-ml/gemm_16x32x8
```

### Review the gemm_16x32x8_graph.h file
Open the ```Makefile``` file and review the code. The only modification is the updated platform name.

```
PLATFORM_USE      := xilinx_vek280_base_202410_1
```

### Design Variant 1: Single Tile (AIE-ML)
Enter the following commands to compile, simulate the design for aiesim and verfiy the results:
```
$ make clean
$ make all
$ make profile
$ make check_sim_output_aie
```

The average throughput for the IO ports is displayed at the end of AIE simulation.
The throughput for the output port is approximately 1529 MB/s.

After running the last command (`make check_sim_output_aie`) to verify the results, the console should output as follows: 
```
Max err: 1
--- PASSED ---
```

### Analyze the Reports
Enter the following command to launch the Vitis Analyzer and review the reports.
```
$ make analyze
```
Select the `Graph` view and observe the kernel and I/O ports. Select the I/O tabs and observe the Throughput column in the I/O tab.

Click the `Array` view and observe the implementaion.

Close the Vitis Analyzer.

### Comparison of the Designs

| Design              | TP_CASC_LEN | TP_SSR | NPORT_I | NPORT_O | Throughput (AIE)|Throughput (AIE-ML)|
|---------------------|-------------|--------|---------|---------|-----------------|-------------------|
| Design Variant 1    |      2      |    1   |    1    |    1    | 1137 MBPS       | 1529 MBPS         |
| Design Variant 2    |      4      |    1   |    4    |    1    | 2578 MBPS       |                   |
| Design Variant 3    |      4      |    2   |    8    |    2    | 3935 MBPS       |                   |

## Design Variant 2: 4-tile design with TP_CASC_LEN=4 (AIE-ML)

### Change the Project Path
Enter the following command to change the project path:
```
cd ../gemm_16x32x8_cascade
```

Enter the following commands to compile, simulate the design for aiesim and verfiy the results:
```
$ make clean
$ make all
$ make profile
$ make check_sim_output_aie
```

The average throughput for the IO ports is displayed at the end of AIE simulation.
The throughput for the output port is approximately 3080 MB/s.

After running the last command (`make check_sim_output_aie`) to verify the results, the console should output as follows: 
```
Max err: 1
--- PASSED ---
```

### Analyze the Reports
Enter the following command to launch the Vitis Analyzer and review the reports.
```
$ make analyze
```
Select the `Graph` view and observe the kernels and I/O ports. Select the I/O tabs and observe the Throughput column in the I/O tab.

Click the `Array` view and observe the implementaion.

Close the Vitis Analyzer.

### Comparison of the Designs

| Design              | TP_CASC_LEN | TP_SSR | NPORT_I | NPORT_O | Throughput (AIE)|Throughput (AIE-ML)|
|---------------------|-------------|--------|---------|---------|-----------------|-------------------|
| Design Variant 1    |      2      |    1   |    1    |    1    | 1137 MBPS       | 1529 MBPS         |
| Design Variant 2    |      4      |    1   |    4    |    1    | 2578 MBPS       | 3080 MBPS         |
| Design Variant 3    |      4      |    2   |    8    |    2    | 3935 MBPS       |                   |

## Design Variant 3: 8-tile design with TP_CASC_LEN=4 and TP_SSR=2 (AIE-ML)

### Change the Project Path
Enter the following command to change the project path:
```
cd ../gemm_16x32x8_cascade_ssr
```

Enter the following commands to compile, simulate the design for aiesim and verfiy the results:
```
$ make clean
$ make all
$ make profile
$ make check_sim_output_aie
```

The average throughput for the IO ports is displayed at the end of AIE simulation.
The throughput for the output port is approximately 6481 MB/s (i.e., PLIO_0_o + PLIO_1_o).

After running the last command (`make check_sim_output_aie`) to verify the results, the console should output as follows: 
```
Max err: 1
--- PASSED ---
```

### Analyze the Reports
Enter the following command to launch the Vitis Analyzer and review the reports.
```
$ make analyze
```
Select the `Graph` view and observe the kernels and I/O ports. Select the I/O tabs and observe the Throughput column in the I/O tab.

Click the `Array` view and observe the implementaion.

Close the Vitis Analyzer.

### Comparison of the Designs

| Design              | TP_CASC_LEN | TP_SSR | NPORT_I | NPORT_O | Throughput (AIE)|Throughput (AIE-ML)|
|---------------------|-------------|--------|---------|---------|-----------------|-------------------|
| Design Variant 1    |      2      |    1   |    1    |    1    | 1137 MBPS       | 1529 MBPS         |
| Design Variant 2    |      4      |    1   |    4    |    1    | 2578 MBPS       | 3080 MBPS         |
| Design Variant 3    |      4      |    2   |    8    |    2    | 3935 MBPS       | 6481 MBPS         |


From the preceding table, for **Design Variant 3**, the AIE-ML architecture achieves a performance that is roughly **1.6** times faster than the AIE architecture.

**Why does the AIE-ML architecture outperform the AIE architecture?**

The **AIE** architecture supports ***8 MACs*** for ``int32 x int32`` operations, whereas the **AIE-ML** architecture supports ***32 MACs*** for ``int32 x int16`` operations. However, for a 32x32 multiplication in AIE-ML, two of these operations are needed, resulting in an overall capacity of 16 MACs. This theoretically provides a 2X benefit, but due to emulation performance, a 1.6X improvement is observed.

![AIE vs AIE-ML](./images/19_Vector_Data_Path_AIE_AIE-ML.png)

The improvement is attributed to the greater number of multipliers in AIE-ML compared to AIE, with the mmul() intrinsics designed specifically for matrix multiplication.

## Conclusion

This tutorial has demonstrated the following:
- A list of GEMM parameters and their usage.
- Configuration of the GEMM parameters according to your design requirements.
- Explored three designs with various configurations.
- Migrated the design from AI Engine to AIE-ML architecture.
- Compared the performance of the designs with AIE vs AIE-ML.


<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2021–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
