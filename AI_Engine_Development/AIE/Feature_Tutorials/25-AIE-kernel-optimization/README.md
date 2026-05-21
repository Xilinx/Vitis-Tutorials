<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">Refer to the Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">Refer to the Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# AI Engine Algorithm Performance Optimization

***Version: Vitis 2026.1***

## Table of Contents

- [AI Engine Algorithm Performance Optimization](#ai-engine-algorithm-performance-optimization)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Understanding Microcode](#understanding-microcode)
    - [Instruction Formats and Decoding](#instruction-formats-and-decoding)
    - [Load and Store Instructions](#load-and-store-instructions)
    - [Scalar Instructions](#scalar-instructions)
    - [Move Instructions](#move-instructions)
    - [Vector Instructions](#vector-instructions)
    - [Microcode Review](#microcode-review)
  - [Using the Vitis Analyzer](#using-the-vitis-analyzer)
    - [Locating Microcode](#locating-microcode)
    - [Measuring Performance](#measuring-performance)
  - [Common Optimization Opportunities](#common-optimization-opportunities)
    - [Bilinear Interpolation Example](#bilinear-interpolation-example)
    - [Register Spilling](#register-spilling)
    - [Pipelining Delay](#pipelining-delay)
    - [Restrict Pointers](#restrict-pointers)
    - [API Limitations](#api-limitations)
    - [Mapping to SIMD](#mapping-to-simd)
  - [Hands-On Application](#hands-on-application)
  - [References](#references)
  - [Support](#support)

## Introduction

When mapping DSP algorithms to an AI Engine array, processing is divided and allocated to computational units called kernels. A kernel is simply a program that runs on a single AI Engine tile and is specified using C++ source code. For this tutorial, the term "algorithm" refers to a program composed of a single AI Engine kernel. You can also optimize applications running on an AI Engine array at graph level, but that level of optimization is not addressed here.

Typical performance measures of interest are throughput and latency. There is also a relationship between runtime and required number of kernels or AI Engine tiles, so improving performance can also impact resource utilization. Using the AI Engine API to create C++ code that compiles and runs your algorithm is a straightforward process. However, what if the resulting performance is not adequate? As a DSP algorithm developer, there are questions that you must answer:

- Why is the provided kernel performance observed?
- Can performance be improved?
- How can the number of processing cycles be minimized?

You can find answers to these questions in the microcode. The following figure shows an example of the microcode.

![figure1](images/va_micro.png)

*Figure 1 - Example of AI Engine Microcode*

At first glance, it seems like an incomprehensible string of characters. However, this tutorial teaches you to analyze it. The two primary objectives of this tutorial are to help you to:

* Understand the fundamentals of reading microcode to extract relevant information
* Use this information to diagnose and improve compute efficiency.
Also, the tutorial provides two example labs, encouraging further hands-on experience with the process of using microcode to optimize AI Engine kernel performance.

## Understanding Microcode

To create effective kernel code using C++ and to understand the resulting microcode, it is important to have a rudimentary understanding of AI Engine architecture. Many AI Engine training courses provide this, but a valuable reference to have on hand is the Versal Adaptive SoC AI Engine Architecture Manual ([AM009](https://docs.amd.com/r/en-US/am009-versal-ai-engine "AM009")).

Before digging into the specifics of using microcode for performance optimization, there are three things you need to keep in mind:

- **Deciphering instruction mnemonics is a straightforward process**: Microcode instructions present in a mnemonic form representing their designated functions. By contextualizing these instructions, it is straightforward to determine their purpose. The following figure shows Some examples.

   ![figure2](images/mnemonics.png)

   *Figure 2 - Microcode Instruction Mnemonics*

   If a mnemonic remains a mystery, using an AI based search engine can likely provide a correct interpretation. This is vecause many mnemonics are commonly used with different processing platforms.

- **You do not need to understand every detail of the microcode**: While microcode contains much detailed information, for the purpose of performance optimization it is helpful to learn how to focus only on what is relevant. To explain this concept further. Refer to the following figure.

   ![figure3](images/micro_details.png)

   *Figure 3 - Focus on Relevant Microcode Details*

   The left side of the figure presents a detailed snippet of microcode. With familiarity and practice, you can mentally omit sections, indicated by the red line on the image’s right side. This selective approach lets you to focus on identifying cycles wasted between the loading of data into a register and the subsequent multiply-accumulate (MAC) operation. To show how you can use this microcode in performance optimization, the thought process to follow is:

  1. Notice that cycles are wasted between the time data is loaded and when it is used.
  2. Examine C++ source code to determine why this is occurring.
  3. Make required modifications to C++ source code to eliminate wasted cycles.

- **Learn to associate microcode with C++ source code**: Compiled C++ kernel code often has a direct association to generated microcode. An example is shown in Figure 4, where C++ source code consists of a loop containing a single `MAC` operation.

   ![figure4](images/source_micro.png)

   *Figure 4 - Associate C++ Source Code with Generated Microcode*

   The red rectangles indicate data access by iterators in C++ source code. These operations are associated with the AI Engine memory interface and directly correspond to load instructions in microcode, also highlighted in red. Similarly, output is written using an iterator highlighted with a blue rectangle. This corresponds to a memory store operation and is reflected in microcode by the vector store operation highlighted in blue. Instructions highlighted with a green rectangle represent accessing data by the cascade interface. In microcode, cascade and stream interface operations result in move instructions, and as this tutorial explains, the mnemonic SCD refers to the input cascade interface. Finally, the `fpmac` AI Engine intrinsic function highlighted with a purple rectangle directly corresponds to the VFPMAC microcode instruction.

   By understanding AI Engine hardware architecture, you can create C++ kernel code and make direct associations to the generated microcode.

### Instruction Formats and Decoding

AI Engines provide instruction-level parallelism through a 'Very Long Instruction Word' (VLIW) architecture. Full VLIW instructions are 128-bits in length and comprise seven individual instruction slots (six slots in AIE-ML architecture). The following figure shows the general form of all possible full VLIW instructions.

![figure5](images/vliw_format.png)

*Figure 5 - VLIW Instruction Format*

***Note*:** There is support for two simultaneous vector load instructions. They appear in the first two slots. Single store operations appear in the third slot. A single scalar operation occupies the fourth slot. Slots 5 and 6 can contain move operations. Finally, vector processor instructions occur in slot 7.

Full VLIW instructions mostly occur in processing loops which take advantage of instruction-level parallelism. For other instructions in the main program, control code, preamble and postamble code, and outer loops, shorter instructions are supported. This is because not all slots are required. As well as the full 128-bit VLIW format, you can also observe 96-bit (5 slots), 64-bit (3 slots), 32-bit (2 slots), and 16-bit (no slots) instructions. Using shorter instructions where full VLIW parallelism is not required helps reduce program memory size. Memory size is constrained to less than 16 kB. The following figure shows an illustration of how the AI Engine processes VLIW instructions.

![figure6](images/vliw_decode.png)

*Figure 6 - Decoding VLIW Instructions*

The instruction fetch and decode unit accesses the content of the program counter (PC) register and uses it to address program memory. It then expands the instruction returned from program memory into a full 128-bit VLIW instruction. The unit decodes individual slots comprising the VLIW instruction into control signals and forwards to corresponding functional units of the AI Engine. The following sections provide more detail on components comprising a VLIW instruction.

### Load and Store Instructions

AI Engines support two simultaneous load instructions from memory along with a single store instruction. Memory locations can be kernel input or output buffers, user-defined buffers in AI Engine tile memory, or the stack. The following figure shows an example of kernel code written in C++ that results in AI Engine load and store instructions.

![figure7](images/load_store_code.png)

*Figure 7 - Kernel Code Resulting in Memory Load and Store*

Pointers shown are iterators assigned to memory locations. The dereferenced pointer highlighted in red provides input data to the vector multiplication operator, and directly corresponds to a vector load instruction. The pointer highlighted in blue accepts output of the cast operation and corresponds to a vector store instruction. Microcode generated from C++ source code can vary significantly due to different addressing modes supported. The key information to focus on is when an operator occurs and possibly what the source and destinations are for the associated data. The following figure shows an example.

![figure8](images/load_ex1.png)

*Figure 8 - Load Instruction Example 1*

Microcode shown on the left starts with `VLDA`, which indicates a vector load operation on unit A. The first argument following a microcode instruction is usually the destination of the operation output. Tables provided for this and following examples are from the Versal Adaptive SoC AI Engine Architecture Manual ([AM009](https://docs.amd.com/r/en-US/am009-versal-ai-engine)). In this case the destination of the loaded data is a 256-bit vector register `wr1`. The next argument specifies memory used to access data. In this case `sp` indicates that memory is accessing the stack. In many microcode instructions you can notice numbers such as `#10`, shown here. These are 'immediate values' and are numeric constants coded into the instruction. For the purpose of optimizing performance these are mostly irrelevant. In this example, the number is likely related to incrementing the stack pointer. Figure 9 shows another load instruction example.

![figure9](images/load_ex2.png)

*Figure 9 - Load Instruction Example 2*

In this case `VLDB` indicates the vector load occurring is using unit B. The destination is a 256-bit vector register `wd0` and the content of a pointer register `p0` specifies the memory source. Here, there is another immediate value likely used to adjust the pointer but is not relevant for optimization purposes. The following figure shows an example of a vector store operation.

![figure10](images/store_ex.png)

*Figure 10 - Store Instruction Example*

Microcode instruction `VST` indicates a vector store operation. This mnemonic is different because it has a number appended to it. By examining the destination, notice that it is an accumulator register `aml3`. According to the Versal Adaptive SoC AI Engine Architecture Manual ([AM009](https://docs.amd.com/r/en-US/am009-versal-ai-engine)), AI Engine accumulator lanes can be 48-bits or 80-bits wide. Therefore, the instruction `VST.48` indicates that data is stored in an accumulator register with 48-bit wide lanes. The accumulator register `aml3.lo` has an extra term appended to it. The table shows that this accumulator register is 384-bits wide. Because there is a limit of 256-bit on the width of the memory interface, only half of the accumulator content can be written in a single clock cycle. Therefore, the destination `aml3.lo` indicates the lower half of the accumulator register. Finally, a pointer register `p6` indicates the data memory source.

### Scalar Instructions

The VLIW instruction provides a slot for operations executed on the scalar processor of the AI Engine. However, scalar operations are not prevalent when focus is on single instruction, multiple data (SIMD) processing. The following figure shows examples of scalar operations supported by AI Engine.

![figure11](images/scalar_inst.png)

*Figure 11 - Scalar Instruction Examples*

The format for standard ALU functions is at the top-left, and a list of supported functions is in the table to the right. All ALU functions have a single cycle of latency. A scalar multiplication operator is provided for 32-bit numbers and provides a result with 3-cycle latency. The scalar processor also supports several non-linear functions. These operators have floating-point inputs and outputs, so type conversion is required when using fixed-point numbers. This is illustrated in the `SQRT` example, where fixed-point input data is provided in general purpose register `r5`. The appended suffix `FX2FLT` indicates the conversion from fixed to floating-point. Conversion between fixed and floating-point numbers requires a shift value to specify the location of the binary point. The shift control register `s2` provides this value in this example. The result is written to register `r1.FLT2FX` which indicates the required data type conversion back to fixed-point. Register `s3` provides the associated shift value. The `SINCOS` operator accepts a 32-bit argument from register `r10` and writes computed sin() and cos() in each 16-bit part of the 32-bit output register `r1`. Non-linear scalar functions have a 4-cycle latency. Refer to the Versal Adaptive SoC AI Engine Architecture Manual ([AM009](https://docs.amd.com/r/en-US/am009-versal-ai-engine)) for more details on the scalar processor.

### Move Instructions

The largest category of VLIW components is the set of move instructions. One of the potential performance bottlenecks is getting data into and out of the AI Engine tile. The memory interface used with load and store instructions is one method. There are two additional possibilities which use move operations. The following figure provides some examples of move operations associated with the stream interface.

![figure12](images/move_stream_ex.png)

*Figure 12 - Stream Interface Move Examples*

Each AI Engine tile has two 32-bit input and two 32-bit output stream interfaces. However, the AI Engine itself can use 128-bit interfaces as highlighted in yellow in the diagram. The examples show this, where `MOV` operations are used for the 32-bit interface and `VMOV` (V for vector) is used when the wider 128-bit interface is specified. There is also a nomenclature associated with the interfaces using letters M and S, likely derived from the master-slave interface idiom.

- `MS` refers to a 32-bit output stream
- `WMS` refers to a 128-bit (wide) output stream
- `SS` refers to a 32-bit input stream
- `WSS` refers to a 128-bit (wide) input stream

Because there are two streams in each direction to choose from, selection is specified in the `md0` mode control register. Finally, when writing to a stream, the register in the final argument specifies the value of `TLAST`. Taking all this information into account, you can understand what the examples of microcode in the figure accomplish.

- Example 1: Send content of register `p4` to 32-bit output stream specified in `md0[10]`, with TLAST specified in register `r3`.
- Example 2: Send content of vector register `vrh2` to 128-bit output stream specified in `md0[10]`, with TLAST specified in register `r3`.
- Example 3: Read 32-bit input stream specified in `md0[8]` and store value in register `r6`.
- Example 4: Read 128-bit input stream specified in `md0[9]` and store value in vector register `vrh3`.

The final option for moving data into and out of an AI Engine tile are cascade interfaces. The following figure shows cascade intefcaes highlighted in yellow, along with two examples of microcode.

![figure13](images/move_cascade_ex.png)

*Figure 13 - Cascade Interface Move Examples*

`MCD` specifies the output cascade stream and `SCD` specifies the input stream. Using this information, you can deduce what the microcode examples accomplish.

- Example 1: Send content of accumulator register `amh3` to the output cascade interface.
- Example 2: Read input cascade interface and store content in vector register `wc1`.

Performing shift-round-saturate (SRS), vector shifts, register splits, setting and getting register bits, and accessing and modifying vector lanes, involves move operations.

When using microcode to optimize performance, you can identify the occurrence of register spilling. Register spilling occurs when register capacity is exceeded and data needs to be written to or read from the stack. The following figure shows microcode examples showing evidence of spilling.

![Figure14](images/reg_spil.png)

*Figure 14 - Examples of Register Spilling*

While these mnemonics appear to be vector load and store operations, they are move operations and involve the stack rather than memory interfaces. These spilling operations also appear in the move operation slots of a VLIW instruction.

Register spilling is not always problematic. For example, you can store a small lookup table on the stack, and accessing this data can result in a `VLDA.SPIL` instruction. However, if both load and store spilling occur within a single computational loop when register capacity is exceeded, this introduces additional cycles, negatively impacting performance.

### Vector Instructions

Vector instructions are by far the most complex looking instructions. This is because many options are available in the vector processing units such as lane permutations and pre-adders. It is not necessary to determine all these details when using microcode to optimize performance. You only need to determine the current operation, and possibly identify the source and destination of the data. The following figure shows some examples illustrating typical vector processing instructions.

![Figure15](images/vec_inst_ex.png)

*Figure 15 - Vector Instruction Examples*

The first example shows vector multiplication. Recalling that accumulator lanes can be 48-bits or 80-bits wide, the instruction `VMUL.80` indicates 80-bit accumulator lanes. The accumulator register `bm0` stores the product and vector registers `ya` and `wc0` store the operands. You can ignore the suffixes on the registers and other included terms in the expression.

The second example uses the mnemonic `VMAC.48.SCD`. Based on previous information, you can determine that this is a vector multiply and accumulate operation. The accumulator uses 48-bit wide lanes and `SCD` indicates the input cascade interface is in use. Examining the rest of the expression notice the result is written to accumulator `bm0` and a single vector register `ya` holds an input to the multiplier. Where does the other input to the multiplier come from? From the `SCD` mnemonic, you can deduce that the input cascade interface provides it.

The final example is a vector floating-point multiply and accumulate operation. Vector registers `wr2` and `wc1` provide inputs. Because the floating-point vector unit does not have a separate set of accumulator registers, another vector register `wd0` specifies the accumulator.

### Microcode Review

You now have enough microcode knowledge to extract relevant information and optimize your kernel code performance. As a review, imagine encountering the line of microcode shown in the following figure. Consider how to use it to determine what is occurring in the AI Engine tile.

![figure16](images/mc_review.png)

*Figure 16 - Microcode Review*

 Semi-colons delineate individual slots of the VLIW instruction. Observe there are seven slots, so this is a full 128-bit VLIW instruction. You can analyze one slot at a time.

| Slot Number | VLIW Instruction | Description |
| ----------- | ---------------- |-------------|
| Slot 1      | `VLDA wd1, [p0], #32` | A vector load is occurring using unit A. Data is written to vector register `wd1`. The content of pointer register `p0` determines the memory source. You can make the link to which particular memory is being accessed by referring to the C++ source code. |
| Slot 2 | `VLDB wd0, [p5], #32` | A simultaneous vector load is occurring using unit B. Data is written to vector register `wd0` and the pointer register `p5` specifies the memory source.|
| Slot 3 | `NOP` | The third slot in the VLIW instruction is for store operations. `NOP` indicates no store operation is occurring.|
| Slot 4 | `NOP` | The fourth slot is for scalar processor instructions. `NOP` indicates no scalar operations are occurring.|
| Slot 5 | `MOV.s12 lc, #26` | A move operation is occurring. Referring to the Versal Adaptive SoC AI Engine Architecture Manual ([AM009](https://docs.amd.com/r/en-US/am009-versal-ai-engine)), you can determine that a loop count register is involved.|
| Slot 6 | `MOV.s20 r12, #-5984` | Another move operation is in progress.|
| Slot 7 | `VFPMAC wd1, r2, wd1, ya, r8, cl0, wc1, #0, cl0, #1, cl1` | A vector floating-point multiply-accumulate operation is in operation. The content of vector registers `ya` and `wc1` are being multiplied and added to the content of vector register `wd1`. Vector register `wd1` stores the result|

Now you can interpret microcode, the next step is to examine how to apply it to identify opportunities to optimize kernel code performance.

## Using the Vitis Analyzer

While there are different places to access AI Engine microcode, AMD Vitis™ Analyzer [[2]] is a good choice when the goal is to optimize performance. The Vitis Analyzer can be opened as a standalone application or by using the Vitis IDE. Prior to using hte Vitis Analyzer for optimization, run aiesimulator with the `--profile` option enabled. This counts the number of times VLIW instructions execute during simulation and is necessary to determine where time is spent during computation.

### Locating Microcode

After you launch the Vitis Analyzer, you can display microcode by selecting **Profile > Profile Details**, highlighted in yellow in the following figure.

![figure17](images/va_info.png)

*Figure 17 - Locating Microcode in Vitis Analyzer*

Sections highlighted in orange show organization of information. Locating the kernel name isolates the section of microcode which performs the computations of your algorithm. The first column on the left shows how the program counter (PC) increments by the VLIW instruction length measured in bytes. VLIW instructions expressed in hex format display to the right of the program counter. The length of these hex strings corresponds to VLIW instruction length, where a full instruction corresponds to 16-bytes. The next section contains microcode which you can analyze to perform optimization. One line of microcode executes each clock cycle. Also importantly are the columns containing execution and cycle counts for each VLIW instruction. These derive performance measures of interest.

### Measuring Performance

Much of the vector processing performed on AI Engines occur in loops. This means you can consider performance optimization as the process of identifying these loops and minimizing the number of clock cycles required to execute them. The following figure shows a method of identifying loops in microcode.

![figure18](images/va_lat_ii.png)

*Figure 18 - Measuring Latency and Initiation Interval (II)*

This kernel was executed 100 times. Therefore, many of the VLIW instructions have execution and loop counts of 100 because they were only executed one time for every kernel invocation. Notice how there is a section where execution and loop counts have a much larger value of 12700. This block of VLIW instructions comprises a loop that executes several times on each kernel invocation. After identifying  loop boundaries, you can derive performance measures.

One of the performance measures shown is latency. In this case, latency is the number of cycles from the beginning of kernel execution until the first output of the loop is available. If this were the only loop in the kernel, latency would be interpreted as the number of cycles from the beginning of execution until the first kernel output is available. The other performance measure is initiation interval, often abbreviated as II. II is the number of clock cycles between the launch of successive loop iterations. Another way of viewing kernel performance optimization is the process of reducing II of your computational loops.

## Common Optimization Opportunities

The following sections introduce common issues that can impact performance when developing AI Engine kernels. Here you work through a typical optimization process using an example of bilinear interpolation. It is from the perspective of an algorithm developer who is perhaps getting started working with AI Engines.

### Bilinear Interpolation Example

Bilinear interpolation is a method of interpolating functions of two variables by using repeated application of linear interpolation in each dimension. It is commonly used in many applications such as image processing, computer vision, and geographic information systems [[3]]. The following figure shows the process of bilinear interpolation. As shown on the left, a computed pixel value (red dot) is determined from surrounding reference pixels (green dots).

![figure19](images/bli_eqn.png)

*Figure 19 - Computing Bilinear Interpolation*

A two-step process is performed by first interpolating in one dimension then the other. The first step, shown in the center, computes intermediate values (blue dots) using linear interpolation in the x-coordinate. The second step, shown on the right, uses intermediate results to perform a linear interpolation in the y-coordinate and derive the final interpolated value. Simplification of the interpolation equations shown may be realized by assuming spacing between reference pixels in a given dimension has a value of 1. This sets $x_1 = y_1 = 0$, $x_2 = y_2 = 1$, and the query point coordinates $(x_q, y_q)$` specified by the fractional values $(x_{frac}, y_{frac})$. This results in the complete interpolation process being represented by a single dot product equation shown at the top of Figure 20.

![figure20](images/bli_vect_1.png)

*Figure 20 - Initial Attempt at Vectorization of Bilinear Interpolation*

To realize advantages of AI Engine processing, equations must be efficiently mapped to operations performed by a vector processor. The center of the preceding figure shows an initial attempt that uses a vector multiplication (MUL) twice. This is an element-wise vector multiplication that is first performed between a vector depending on $x_{frac}$ and another depending on $y_{frac}$. The resulting product is then used in a second vector multiplication with the reference pixel values. Finally, a summing operation over all vector elements obtains the interpolated pixel value.

Another issue to consider is how to obtain the first two vectors from values of $x_{frac}$ and $y_{frac}$. Modifying individual elements of a vector has a negative impact on performance, as it likely involves the scalar processor. The bottom of the preceding figure shows how to derive these vectors using a multiply-accumulate (MAC) vector operation.

AI Engines can use either a fixed-point or floating-point vector processor. This example uses floating-point data types. As shown in the Versal Adaptive SoC AI Engine Architecture Manual ([AM009](https://docs.amd.com/r/en-US/am009-versal-ai-engine)), the floating-point vector processor can perform eight parallel multiplications. Also, a vector of eight floating-point values is 256-bits wide, which is a natural match to the memory interface. For these reasons, kernel processing uses vectors of size 8. Becausee the vectors shown in the preceding figure are of size 4, two pixel interpolations can run simultaneously using the floating-point vector processor. To get data into the kernel for processing, the format shown in the following figure is assumed.

![figure21](images/bli_input_fmt_1.png)

*Figure 21 - Data Input for Bilinear Interpolation Kernel*

Each interpolated pixel requires six numbers for computation, namely $x_{frac}$, $y_{frac}$, and four reference pixel values. Because the smallest vector size that accommodates this is eight, two of the vector lanes remain unused. Based on this mapping of the equation to the vector processor and memory interfaces, kernel code was created. The following figure shows the kernel header file.

![Figure 22](images/bli_kernel_h_v1.png)

*Figure 22 - Initial Bilinear Interpolation Kernel Header File*

The header file defines the kernel class along with a member function that performs interpolation. It also defines constants used to compute vectors from $x_{frac}$ and $y_{frac}$ using a vector MAC operation, as shown in Figure 20. The following figure shows implementation of the kernel function which performs interpolation.

![Figure 23](images/bli_kernel_v1.png)

*Figure 23 - Initial Bilinear Interpolation Kernel Code*

A summary of the kernel code is as follows:

- Lines 19-20: Defines iterators for input and output buffers. The input iterator reads vectors of size 16 which comprise two rows of the input format shown in Figure 21. It allows two pixels to be interpolated at a time. The output iterator writes a single interpolated pixel at a time.
- Lines 24-27: Load constant arrays defined in the header file into registers to compute vectors from $x_{frac}$ and $y_{frac}$.
- Line 29: Loop through input array, computing two interpolated pixels per loop invocation.
- Line 34: Read input data used to compute two interpolated pixels.
- Line 37: Compute vector based on $x_{frac}$. This is done using a floating-point MAC intrinsic function. Hex strings specify which vector lanes to use in computation. In this case, value `0xCCCC4444` extracts $x_{frac}$ values from input for each pixel and assigns them to vector lanes for multiplication.
- Line 40: Similar computation as line 37 to derive vector based on $y_{frac}$.
- Line 43: Multiplication of vectors derived from $x_{frac}$ and $y_{frac}$ to compute $xy$ product.
- Line 44: Multiplication of $xy$ product with reference pixels.
- Lines 47-48: Extract half of vector corresponding to a single pixel and sum values. Resulting interpolated pixel value is written to output.

Simulation of this kernel with profiling was performed. Examine the resulting microcode in the following section.

### Register Spilling

The following figure shows microcode generated from bilinear kernel C++ source code.

![figure24](images/micro_v1.png)

*Figure 24 - Microcode Indicating Register Spilling*

Examining the execution and cycle count columns, you can easily identify a computational loop and determine it to have an II of 41. Because two pixels process in every loop invocation, computational efficiency is 20.5 cycles/pixel. Also highlighted is evidence of register spilling indicated by the `VLDA.SPIL` mnemonic in one of the move slots of the VLIW instruction. In this case, it is moving data from the stack to a vector register. Notice there is no store register spilling operation. Referring back to the C++ source code, you might deduce that the constant arrays used in computing vectors from $x_{frac}$ and $y_{frac}$ using a vector MAC operation is possibly the cause. Because register capacity is limited, each loop iteration these vectors are retrieved as needed for computation. Because register spilling is occurring, it is worth examining the source code. The following figure shows the source code.

![figure25](images/bli_kernel_reg_spil.png)

*Figure 25 - Kernel Code with Indication of Register Use*

Highlighted variables show when vector registers are assigned values and when they are used in computation. In the case of floating-point vector operations there are no accumulator registers, so results are also written back to vector registers. Considering only eight 256-bit vector registers are available, you can examine the size of variables and their persistence. Then you can determine the cause of spilling and find different ways of performing the computation to avoid it. Variables such as `xy` are assigned (line 43) then used on the next line, making efficient use of register space. In contrast, `vin` takes up the equivalent of two 256-bit registers and persists due to its use in several computations. In this particular example, register spilling does not seem to be an issue by requiring extra cycles to execute. However, if both load and store spilling operations occur in a loop, it is worth examining register usage to try to remove them.

### Pipelining Delay

Referring to Figure 24, one of the most noticeable aspects of the microcode is the presence of groups of lines with only a `NOP` assigned. This means no new operations are executing. The presence of these lines of microcode negatively impacts performance, and it is useful to understand the cause. The following figure shows one of these sections of code.

![figure26](images/micro_mul_pipe_delay.png)

*Figure 26 - Microcode Indicating Pipeline Delay*

Notice that a vector floating-point multiplication is executing, with the result written to vector register `wd1`. The content of `wd1` transfers to register `wc1` then used as input to the next vector multiplication. Why is there a 7-cycle gap between multiplications? Examining the floating-point processor architecture shown in the following figure answers this.

![figure27](images/tah1526498032789.image)

*Figure 27 - AI Engine Floating-Point Vector Processor*

This diagram shows a 7-stage pipeline. Recall that for each pixel the kernel first computes the product `xy` between vectors derived from $x_{frac}$ and $y_{frac}$. The kernel then uses this result in a follow-on multiplication with the reference pixel values. Therefore, 7-cycle gaps observed in microcode are due to the vector processor waiting for the first product to make it through the pipeline before computing the next product. So, when you observe lines containing `NOP` between successive vector operations, it is likely an indication of pipeline delays. Often this is seven cycles for the floating-point processor. When using the fixed-point vector processor, there is a 6-stage pipeline.

To fill the pipeline, you can perform partial computation of your result for many input values. You then store them in a temporary buffer in local data memory. A follow-on loop reads that data and uses it in the next stage of computation of the algorithm. Not only does this improve compute efficiency, but it often reduces the amount of register space used which eliminates register spilling.

Sometimes kernel processing requires input data multiple times. If this data is in an input buffer, then it can be read multiple times as needed. If a kernel has stream inputs, then data can send multiple times as needed. To apply this to the bilinear interpolation kernel, four loops share computation. The following figure shows the modified kernel header file.

![figure28](images/kernel_header_v2.png)

*Figure 28 - Modified Kernel Header*

Some changes to make note of are:

- Lines 18-19: The constant vectors are changed so that a single `MAC` operation uses $x_{frac}$ and $y_{frac}$ values for a single pixel instead one `MAC` using $x_{frac}$ for two pixels and another `MAC` using $y_{frac}$ values for two pixels.
- Lines 21-22: Buffers are created in local data memory to hold intermediate results.

The following figure shows the kernel code up to the end of the first computational loop.

![figure29](images/kernel_code_v2_loop1.png)

*Figure 29 - Modified Kernel Code Loop 1*

Points to note are:

- Line 19: The input iterator is of size 8 instead of 16. It now reads input data for a single pixel and uses less register space.
- Line 22: An iterator is defined to access a memory buffer to hold intermediate data.
- Line 28: This loop processes one pixel per iteration.
- Line 36: The `fpmac` intrinsic function computes vectors derived from $x_{frac}$ and $y_{frac}$ and writes the result to local memory.

The following figure shows kernel processing up to the end of the second loop.

![figure30](images/kernel_code_v2_loop2.png)

*Figure 30 - Modified Kernel Code Loop 2*

Point to note are:

- Lines 45-46: Iterators are define for both data memory buffers. The buffer containing data from the first loop is now of size 16, which means it can access two pixels worth of data.
- Line 48: This loop computes results for two pixels with each invocation
- Line 56: The permutation network of the floating-point processor is used through the `fpmac` intrinsic function to compute the first vector product for two pixels at a time. Data memory stores the results.

The third loop computes the next vector product using kernel code shown in Figure 31.

![figure31](images/kernel_code_v2_loop3.png)

*Figure 31 - Modified Kernel Code Loop 3*

Points to note are:

- Line 64: The input iterator is now defined to be of size 16. This is necessary to read the reference pixel values for two interpolated pixels
- Line 66: Computation for two interpolated pixels are performed with each loop invocation.
- Line 77: The next vector product is performed between the `xy` products stored in local memory and the reference pixel values retrieved from the input buffer. The vector product is written back to local memory.

The following figure shows the remainder of the kernel code containing the final loop.

![figure32](images/kernel_code_v2_loop4.png)

*Figure 32 - Modified Kernel Code Loop 4*

There is no significant change to the original kernel code except that it now retrieves the input for summing vector components from data memory. Each loop invocation computes the final interpolated value for two pixels and writes them to the output buffer. Compiling and profiling this version of the kernel results in microcode containing four loops. The following figure shows the content of these loops.

![figure33](images/micro_pipeline.png)

*Figure 33 - Microcode After Improving Pipelining*

The first two loops achieve excellent II values of 1 and 2, respectively. The third and fourth loops are candidates for further optimization if necessary. Because the first loop computes one pixel per iteration and the remaining loops compute two pixels per iteration, the compute efficiency of this implementation with improved pipelining is 16.5 cycles per pixel. This is an improvement over the original kernel code which required 20.5 cycles per pixel. Also, notice that the resulting microcode no longer shows signs of register spilling.

### Restrict Pointers

In the third loop in the preceding figure, you can observe several consecutive lines of `NOP`. This time, notice the inactivity between the occurrence of load instructions into vector registers and when those registers are used in computation. This is because memory transactions take several cycles to complete, and the compiler creates code to wait for the completion of memory operations to avoid corruption due to multiple conflicting accesses. In this application you do not require this level of caution, so a C++ qualifier can be used to inform the compiler that the assigned pointer is the only way to access data and to perform more aggressive optimization. Specifically, this refers to the `__restrict` C++ qualifier which is applied to the kernel code shown in the following figure.

![figure34](images/kernel_restrict.png)

*Figure 34 - Kernel Code with Restrict Pointers*

The red rectangles show the `__restrict` qualifier applied to input and output data buffers. Also, shown with by yellow rectangles are restrict pointers created using an AI Engine API function. These `begin_restrict_vector` iterators are assigned to buffers created in local data memory. When using restrict pointers it is your responsibility to make sure multiple pointers do not access the same resource. Using restrict pointers in the kernel code results in microcode shown in the following figure.

![figure35](images/micro_restrict_ptr.png)

*Figure 35 - Microcode After Adding Restrict Pointers*

In this case there is a significant improvement in II for the third loop. Computational efficiency is now 10.5 cycles/pixel compared to the original kernel which required 20.5 cycles/pixel. At this point, if you require any further optimization, the fourth loop provides the best opportunity for gains in computational efficiency.

### API Limitations

The AI Engine API is the most reliable and portable approach to creating custom kernel code. Usually there is no difference in performance between API and intrinsics functions. However, sometimes a programmer attempts to use API functions in a way that results in non-optimal performance. Most likely this occurs when applying the API to data from a larger vector. When these events happen, it worth re-examining how compute is performed to determine if there is a more optimal way. Sometimes using intrinsic functions provide benefits. As an example, examine the fourth loop of the bilinear interpolation kernel in the following figure, along with generated microcode.

![figure36](images/kernel_reduce_add.png)

*Figure 36 - Kernel Code with Extract and Reduce Add*

The kernel extracts four elements from a vector of size 8 then adds the elements and sends the result to output. This operation occurs twice per loop invocation. Microcode indicates the following:

- `VFPMAC` operations perform the addition.
- A 7-cycle gap between successive `VFPMAC` operations indicates a pipelining delay.
- It appears that a pair of `VFPMAC` operations sum components for one interpolated pixel. This repeats for the second interpolated pixel.

Giving the objective of adding vector components some thought, the following figure shows an alternative solution, based on intrinsic functions.

![figure37](images/micro_reduce_add.png)

*Figure 37 - Microcode After Using Intrinsics for Reduce Add*

Intrinsic functions enable access to the permutation network of the floating-point vector processor. The diagram shows the objective of the code. In the upper right corner where the vector on the left is the input with eight different elements represented by different colors. The kernel source code performs the following:

- Line 94: Add adjacent pairs of vector elements resulting in the vector shown in the center with four different color. After adding adjacent pairs, the elements with the same color have the same value.
- Line 95: Perform a second addition so the top 4 and bottom 4 elements have the same value. The value of these elements is the interpolated pixel value, so a single element from each group of four is selected to send to output.

The resulting microcode is also displayed. It indicates the II reduced from 15 to 10. Combining this with the first three loops, the computational efficiency of the kernel code is now eight cycles per pixel, compared to the initial value of 20.5.

### Mapping to SIMD

One of the most important things to consider is how algorithms get mapped to take advantage of SIMD processing. Considering the bilinear interpolation example up to this point, a mapping was initially assumed, and different optimizations applied, but how the algorithm made use of the SIMD processor remained unchanged. Revisit this mapping based on the vectorization shown in the following figure.

![Figure38](images/bli_eqn_2.png)

*Figure 38 - Original Vectorization for Bilinear Interpolation*

This approach attempts to use SIMD processing to compute individual interpolated pixels faster. To do so, data is first expanded across lanes ($x_{frac}$, $y_{frac}$) then SIMD processing is used for computation. To obtain the final result, data must be combined across lanes. In this example, the expansion of data did not have much impact, but the addition across lanes at the end did.

An alternative approach is to use individual vector lanes to process individual pixels. This pixel per lane approach does not speed up the processing for an individual pixel, but instead, several pixels are computed simultaneously. The following sigure shows mapping the bilinear interpolation equations to a pixel per lane approach.

![Figure39](images/bli_eqn_3.png)

*Figure 39 - Pixel Per Lane Vectorization for Bilinear Interpolation*

The top three equations are the original linear interpolations performed over x and y coordinates. Expanding and rearranging terms in these equations leads to the lower set of equations. The reason this set of equations is attractive is that the first two terms in each equation represents a MAC operation. The result of this MAC is then used with the final term in a multiply and subtract from accumulator (MSC) operation. Using these equations with a pixel per lane means we can interpolate eight pixels using six vector operations. To enable this computation, change the format of the input data as shown in the following figure.

![figure40](images/BLI_input_format_ppl.png)

*Figure 40 - Pixel Per Lane Input Format*

This format uses six 256-bit vectors for a group of eight pixels. Each group consists of a row for $x_{frac}$, a row for $y_{frac}$, and a row for each of the reference pixels. Note that this input format is also more efficient that the one introduced in Figure 21 because all the vector elements are in use. The following figure shows kernel code based on the pixel per lane approach, with the first set of interpolations over the x coordinate shown.

![figure41](images/kernel_pxlperlane_1.png)

*Figure 41 - Pixel Per Lane Kernel Code - Part 1*

Point to note are as follows:

- Lines 19-20: Input iterator used to read in eight values at a time, or a single row shown in Figure 40. Output iterator used to write eight interpolated pixels at a time.
- Lines 23-26: Four work buffers are defined in local data memory.
- Lines 29-39: Perform MAC operations in x coordinate. Eight pixels processed per loop invocation. Results written to local data buffers.
- Lines 41-43: Iterators are reset before the next processing loop.
- Lines 48-57: Perform MSC operations in x coordinate. Eight pixels processed per loop invocation. Results written to local data buffers.
- Lines 59-62: Iterators are reset before the next processing loop.

Also worth noting is that although this uses intrinsic functions, there is no use of lane permutations. So, using AI Engine API functions is simpler in this case. The following figure shows the remaining kernel processing.

![figure42](images/kernel_pxlperlane_2.png)

*Figure 42 -  Pixel Per Lane Kernel Code - Part 2*

Points to note are:

- Lines 67-74: Perform MAC operation in y coordinate. Eight pixels processed per loop invocation. Results written to local data buffer.
- Lines 76-78: Iterators are reset before the next processing loop.
- Lines 83-89: Perform MSC operation in y coordinate. Eight pixels processed per loop invocation. Eight interpolated pixel values written to output buffer.

Examining the kernel code also shows that the previously introduced principles of optimization apply. Compiling and profiling this kernel code results in microcode comprising the four loops, as the following figure shows.

![figure43](images/micro_ppl.png)

*Figure 43 - Microcode for Pixel Per Lane Vectorization*

In this case the loops obtain II of 5, 4, 2, and 2. Each loop now processes eight pixels per invocation, so the compute efficiency is now 1.625 cycles per pixel. Compare this to 20.5 cycles per pixel you started with. That amounts to a 12.6 times improvement.

## Hands-On Application

As you gain experience working with AI Engine architecture and applying these principles of optimization, notice that the C++ kernel code you develop often performs well without microcode examination. Whenever the need arises to analyze and improve performance, the knowledge gained through this tutorial should be sufficient to help achieve your objective. This process becomes more natural with experience, so to get hands on practice, consider trying the [Optimization Practice Lab](OptimizationPracticeLab.md "Optimization Practice").

## References

[[1]]: Versal Adaptive SoC AI Engine Architecture Manual ([AM009](https://docs.amd.com/r/en-US/am009-versal-ai-engine)).

[[2]]: Vitis Reference Guide ([UG1702](https://docs.amd.com/r/en-US/ug1702-vitis-accelerated-reference)).

[[3]]: Bilinear interpolation. December 16 2024, 22:13 UTC. In Wikipedia: The Free Encyclopedia. Wikimedia Foundation Inc. Encyclopedia online. Available from [wikipedia.org/wiki/Bilinear_interpolation](https://en.wikipedia.org/wiki/Bilinear_interpolation). Internet. Retrieved December 19 2024.

## Support

GitHub issues are used to track requests and bugs. For questions, go to [support.xilinx.com](http://support.xilinx.com/).

<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2024–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
