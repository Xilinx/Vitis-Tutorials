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


# AI Engine Kernel Compilation

In this chapter, we will start with a basic Matrix Multiplication example to understand the kernel code and its compilation using **Vitis AIE compiler**

> **Important Note:** This tutorial aims to explain the Vitis tool flow and uses a very simple AI Engine kernel. Please use the https://github.com/Xilinx/Vitis-Tutorials/ AI Engine designs to better leverage the AI Engine tiles.

1. [Basics of Matrix Multiplication](#basics-of-matrix-multiplication)
2. [AI Engine Kernel Overview](#ai-engine-kernel-overview)
3. [AI Engine Standalone Compilation](#ai-engine-standalone-compilation)
4. [AI Engine Standalone Simulation](#ai-engine-standalone-simulation)

### 1. Basics of Matrix Multiplication
In this tutorial we will use a simple AI Engine Kernel to perform Matrix multiplication of a [4x4] matrix (let's call it A) with a [4x1] matrix (let's call it B).

```
A = | a11  a12  a13  a14 |    B = | b1 |
    | a21  a22  a23  a24 |        | b2 |
    | a31  a32  a33  a34 |        | b3 |
    | a41  a42  a43  a44 |        | b4 |
```
A x B will result in a [4x1] matrix (let's call it C).

```
C[1][1] = | a11 * b1 + a12 * b2 + a13 * b3 + a14 * b4 |
C[2][1] = | a21 * b1 + a22 * b2 + a23 * b3 + a24 * b4 |
C[3][1] = | a31 * b1 + a32 * b2 + a33 * b3 + a34 * b4 |
C[4][1] = | a41 * b1 + a42 * b2 + a43 * b3 + a44 * b4 |

```

Here is an example of simple matrix multiplication:
```
A = | 1  2  3  4  |  b = | 1 |     A x B =   | 1*1  + 2*2  + 3*3  + 4*4  = 1  + 4  + 9  + 16 = 30  |
    | 5  6  7  8  |      | 2 |               | 5*1  + 6*2  + 7*3  + 8*4  = 5  + 12 + 21 + 32 = 70  | 
    | 9  10 11 12 |      | 3 |               | 9*1  + 10*2 + 11*3 + 12*4 = 9  + 20 + 33 + 48 = 110 |
    | 13 14 15 16 |      | 4 |               | 13*1 + 14*2 + 15*3 + 16*4 = 13 + 28 + 45 + 64 = 150 |

C = | 30  |
    | 70  |
    | 110 |
    | 150 |
```

### 2. AI Engine Kernel Overview

We will use the below function to implement the matrix multiplication logic in the AI Engine:
For more information on MMUL API, please refer to the [UG1079](https://docs.amd.com/r/en-US/ug1079-ai-engine-kernel-coding/Matrix-Multiplication-mmul)

Function Code:

``` 	using MMUL = aie::mmul <M,N,k, int32,int32> ;
	const int32* __restrict  pA=(int32*) in1->ptr;
	const int32* __restrict pB=(int32*) in2->ptr;
	int32* __restrict pC =(int32*) out_put->ptr;

	aie::vector<int32,MMUL::size_A> A0 = aie::load_v<MMUL::size_A>(pA);
	aie::vector<int32,MMUL::size_B> B0 = aie::load_v<MMUL::size_B>(pB);
    
        MMUL C00;
	C00.mul(A0,B0);
	aie::store_v(pC, C00.template to_vector<int32>())
```

Graph Code:
```
Simple_Graph()
	{
		in0 = adf::input_plio::create("DataIn1", adf::plio_32_bits, "../data_srcs/MatrixA.txt");
		in1 = adf::input_plio::create("DataIn2", adf::plio_32_bits, "../data_srcs/MatrixB.txt");
		out_put = adf::output_plio::create("DataOut1", adf::plio_32_bits, "../data_srcs/ouput.txt");

		k1 = kernel::create(MatMul);
		source(k1) = "../design_srcs/MatMul.cpp";
		runtime<ratio>(k1) = 0.9;
			
		connect(in1.out[0],k1.in[0]);
                connect(in2.out[0],k1.in[1]);
                connect(k1.out[0],out_put.in[0]);

                dimensions(k1.in[0])={16};
                dimensions(k1.in[1])={4};
                dimensions(k1.out[0])={4};

	}
```

### 3. AI Engine Standalone Compilation:
 
AI Engine Compiler (v++ -c --mode aie) takes the graph.cpp as an input, breaks the logic into smaller pieces, and assigns these pieces to different AI Engine cores to work simultaneously. It also optimizes how data moves between cores and memory, and generates a libadf.a file and Work directory as an output. Below is a sample reference command. Please refer [AIE_Kernel/Makefile](./Makefile) to access the full command.

```
v++ --compile --mode aie --target=hw /<path_to_Graph>/graph.cpp --part=<fpga_part_for_DUT> -I <data_srcs> -I <aie_srcs>
```
### 4. AI Engine Standalone Simulation:
AI Engine Simulator (aiesimulator) uses the Work directory to simulate the design.
```
aiesimulator --profile --pkg-dir Work
```

The above command will use the MatrixA.txt and MatrixB data as an input to pass stimulas to the simulation. Here is the sample output of the aiesimulator:
```
Enabling core(s) of graph mygraph
Waiting for core(s) of graph mygraph to finish execution ...
A0_matrix: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 
B0_matrix: 1 2 3 4 
C00_matrix: 30 70 110 150 
core(s) are done executing
Exiting!
Cores are done executing but the simulation will run for some more cycles to allow PLIO to be flushed
Stopping Simulator.

Info: /OSCI/SystemC: Simulation stopped by user.
--------------------------------------------------------------------------------------------------
Port Name           | Type              | Average Throughput
--------------------------------------------------------------------------------------------------
DataIn1             | IN                | 1333.333333 MBps  
DataIn2             | IN                | 1666.666667 MBps  
DataOut1            | OUT               | 2222.222222 MBps  
--------------------------------------------------------------------------------------------------

```
C00_matrix is the output generated by the AI Engine simulation model.

#### AI Engine commands and its output:

| Commands            | Expected Output | Log file | Vitis Analyzer Supported File | 
|---------------|---------------|---------------|---------------|
| v++ --compile --mode aie --target=x86sim | Work directory to functionaly simulate the AIE kernel | ./AIECompiler.log | ./Work/graph.aiecompile_summary |
|x86simulator --pkg-dir= Work | Use the --target=x86sim generated Work directory to run the functional simulation | ./x86simulator.log | x86simulator_output/x86sim.aierun_summary |
| v++ --compile --mode aie --target=hw | Work directory to target the AI Engine on hardware and also to run cycle approximate simulation of the AIE kernel | ./AIECompiler.log | ./Work/graph.aiecompile_summary |
| aiesimulator --profile --pkg-dir Work | Use the --target=hw generated Work directory to run cycle approximate simulation | ./AIESimulator.log| ./aiesimulator_output/default.aierun_summary |



Next Chapter: [Prepare HLS kernels](../HLS_Kernels/README.md)


<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
