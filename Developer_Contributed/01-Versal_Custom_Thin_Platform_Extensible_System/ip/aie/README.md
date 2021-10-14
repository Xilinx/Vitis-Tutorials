<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>

# AI Engine Datamover Examples
These AI Engine kernel examples demonstrate a few different coding techniques using data movers as case study.
They also show how to optimize the kernel processing to allow for efficient processing of data with the inner loops.
By modifying the graph and constraints file, the kernel examples can be put in different comparison contexts for learning purpose.

## Description of the AIE kernels
The current graph create 4 individual data paths moving data from PL through an instance of datamover kernel back to PL.
The kernels are indexed from 0 to 3 using the following kernels:
```
  0. datamover_scalar.cc      : Uses AIE Scalar processing to move 32 bit data (cint16) each clock cycle.
  1. datamover_vector_reg.cc  : Uses AIE Vector processing to move 256 bit data (8 lanes of cint16) each clock cycle.
                                The vector registers is used for temporary storage as a circular buffer.
  2. datamover_mul_one.cc     : Similar as vector data mover, except that the 8 lanes is passed through the DSP MUL
                                by multiplying with 1. This mimics the pipeline delay vector MUL/MAC signal processing.
  3. stream_datamover.cc      : Based on direct stream access of the 32 bit AXI Stream. As with the Vector data mover,
                                the vector register is used as a circular buffer. To align with the 128 bit width of the 
                                vector register, the 4:1 conversion from 32 to 128 bit in the stream API is used.
                                In practice this means we can read 128 bits of data every 4th clock cycle, corresponding to
                                4 cint16 samples. Similar argument for the output, we write 128 bit data every 4th clock cycle
                                and the 1:4 conversion to 32 bit stream.
```

## Short description of Vector datamover optimizations
To make efficient use vector read and write operations, MUL/MAC operations and scalar commands some considerations are need.
The AI Engine core have capability of processing two 256 bit reads, one 256 bit write, one DSP MUL/MAC operation and a Scalar
instruction in one clock cycle using Very Long Instruction Word. The core also have dedicated registers to manage counter for 
inner loop as a Zero Overhead Loop.
For this to be scheduled properly, first consider the inner loop in terms of data flow. As this case is bound by the 256 bit writes
we only need to refresh 256 bits of input data. To give room for the input data to be stored on the vector register, it will have
to use a 256 bit vector ahead of the output 256 bit vector. This also avoids read and write access on the same address of the 
vector register.

### Special note on preparing the loop
The inner loop is where the main work takes place with VLIW keeping the AI Engine busy with processing.
Upon entering the loop, it's important that the first operation have valid data to process. For the vector reg datamover,
this means we need to preload the first 256 bits of data before entering the loop. Similar argument for the vector multiplication
data mover, but in this case we require 2 x 256 bits.
This is because we process 8 lanes of data each clock cycle and the permutation for each lane offsets the data in the vector 
register according to the figure below.

<img src="../../documentation/readme_files/datamover_mul_one_8_lanes.png">

To complete the full circle, the inner loop is unrolled manually so the for loop always start in same position.
This is possible due to that the vector register will automatically wrap around according to it's type declaration.

## Compile and build AIE design
For the global project settings and paths to work, always build the AIE ip from design root directory.
Use build instructions in step 7.

## Simulating and checking the AIE kernels
Simulating the kernel is done from the ip/aie directory. For convenience this command can be issued from root dir:
```
  cd ip/aie;make sim_analyzer;cd -
```
Open the AI Engine Simulation summary with Vitis Analyzer
```
  vitis_analyzer ip/aie/aiesimulator_output/default.aierun_summary
```

## References
The following documents provide supplemental information for this tutorial.

### [AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/yii1603912637443.html)
Contains sections on how to develop AI Engine graphs, how to use the AI Engine compiler, AI Engine simulation, and performance analysis.

### [Vitis Unified Software Development Platform 2021.2 Documentation](https://www.xilinx.com/html_docs/xilinx2021_2/vitis_doc/index.html)

## Revision History
* October   2021 - Updated AI Engine Datamovers
* September 2021 - Initial Release

 
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

<p align="center"><sup>XD106 | © Copyright 2021 Xilinx, Inc.</sup></p>

