<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>

# AI Engine datamover examples
These AI Engine kernel examples demonstrate a few different coding techniques using datamovers as case study.
They also show how to optimize the kernel processing to allow for efficient processing of data with the inner loops.
By modifying the graph and constraints file, the kernel examples can be put in different comparison contexts for learning purpose.

Note that the graph have been converted to use C++ class style kernels.

## Object oriented C++ template class kernels with AI Engine API style
The current graph create 4 individual data paths each moving data from PL through an instance of a datamover kernel back to PL.
By using template C++ class kernels, the data type can be changed when instantiating the kernel in the graph.
The AI Engine API enables portable code across AI Engine device families (AIE1 and AIE2-ML) and is easier to use than intrinsics API.

Note that these examples are basic with focus on readable and compact code using helper functions like vector iterators.
For optimization, techniques used in the old function style kernels can still be applied, but may require mixing AIE API with intrinsics.
```
  0. my_scalar_dm.cpp       : Uses AIE scalar processing to move 32-bit data (cint16) each clock cycle. The data is moved
                              from input to output using scalar iterators.
  1. my_vector_dm.cpp       : Uses AIE vector processing to move 256-bit data (8 lanes of cint16) each clock cycle.
                              The vector registers is used for temporary storage declared as a vector iterator.
  2. my_mul_dm.cpp          : Similar as vector data mover, except that the 8 lanes are passed through the DSP MUL
                              by multiplying with 1. This mimics the pipeline delay vector MUL/MAC signal processing.
  3. my_stream_dm.cpp       : Based on direct stream access of the 32-bit AXI Stream. For AIE1 the native stream interface
                              is 32-bit, so for this example a temporary cint16 variable is used to pass input to output.
                              If used with vector register, the 4:1 conversion from 32 to 128-bit is better to use as this
                              aligns with the vector register. For that case we can read 128-bits of data every
                              4th clock cycle, corresponding to 4 cint16 samples. Similar argument for the output, we
                              write 128-bit data every 4th clock cycle and the 1:4 conversion to 32-bit stream.
```


## Description of the AIE kernels - C function style kernels (unused in graph)
These examples was used in earlier releases of the tutorials and are now not in use.
For comparison purpose they are kept as they represent ideas about potential optimizations.
The kernels are indexed from 0 to 3 using the following kernels:
```
  0. datamover_scalar.cc      : Uses AIE scalar processing to move 32-bit data (cint16) each clock cycle.
  1. datamover_vector_reg.cc  : Uses AIE vector processing to move 256-bit data (8 lanes of cint16) each clock cycle.
                                The vector registers is used for temporary storage as a circular buffer.
  2. datamover_mul_one.cc     : Similar as vector data mover, except that the 8 lanes are passed through the DSP MUL
                                by multiplying with 1. This mimics the pipeline delay vector MUL/MAC signal processing.
  3. stream_datamover.cc      : Based on direct stream access of the 32-bit AXI Stream. As with the vector data mover,
                                the vector register is used as a circular buffer. To align with the 128-bit width of the 
                                vector register, the 4:1 conversion from 32 to 128-bit in the stream API is used.
                                In practice this means we can read 128-bits of data every 4th clock cycle, corresponding to
                                4 cint16 samples. Similar argument for the output, we write 128-bit data every 4th
                                clock cycle and the 1:4 conversion to 32-bit stream.
```

## Short description of vector datamover optimizations
To make efficient use of vector read and write operations, MUL/MAC operations,  and scalar commands some considerations are needed.
The AI Engine core has the capability of processing two 256-bit reads, one 256-bit write, one DSP MUL/MAC operation, and a scalar
instruction in one clock cycle using Very Long Instruction Word. The core also has dedicated registers to manage the counter for
the inner loop as a Zero Overhead Loop.
For this to be scheduled properly, first consider the inner loop in terms of data flow. As this case is bound by the 256-bit writes
we only need to refresh 256-bit vector of input data. To give room for the input data to be stored on the vector register,
it will have to use a 256-bit vector ahead of the output 256-bit vector. This also avoids read and write access on the same
address of the vector register.

### Special note on preparing the loop
The inner loop is where the main work takes place with VLIW keeping the AI Engine busy with processing.
Upon entering the loop, the first operation must have valid data to process. For the vector reg datamover,
this means we need to preload the first 256-bit vector of data before entering the loop. A similar argument for the vector multiplication
data mover, but in this case, we require two 256-bit vectors of data.
This is because we process 8 lanes of data each clock cycle and the permutation for each lane offsets the data in the vector 
register according to the figure below.

<img src="../../../documentation/readme_files/datamover_mul_one_8_lanes.png">

To complete the full circle, the inner loop is unrolled manually so the for loop always starts in the same position.
This is possible due to that the vector register will automatically wrap around according to its type declaration.

## Compile and build AIE design
For the global project settings and paths to work, always build the AIE ip from the design root directory.
Use build instructions in step 7.

## Simulating and checking the AIE kernels
Simulating the kernel is done from the ip/aie directory. For convenience this command can be issued from root dir:
```
  make sim_analyzer -C vitis/ip/aie
```
Open the AI Engine Simulation summary with Vitis Analyzer
```
  vitis_analyzer vitis/ip/aie/aiesimulator_output/default.aierun_summary
```

## References
The following documents provide supplemental information for this tutorial.

### [AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/yii1603912637443.html)
Contains sections on how to develop AI Engine graphs, how to use the AI Engine compiler, AI Engine simulation, and performance analysis.

### [Vitis Unified Software Development Platform 2021.2 Documentation](https://www.xilinx.com/html_docs/xilinx2021_2/vitis_doc/index.html)

## Revision History
* March     2023 - Changed to templated C++ class kernel style and AIE API
* October   2021 - Updated AI Engine Datamovers
* September 2021 - Initial Release


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
