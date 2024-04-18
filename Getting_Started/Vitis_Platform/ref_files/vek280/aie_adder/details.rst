Artificial Intelligence Engine (AIE) Adder
==========================================

This simple design demonstrates the flow to create Programmable Logic(PL) kernels that stream to and from AIE, and validate the design running on the AI Engine, Processor System(PS), and PL.

There're 3 kernel functions here, 2 running in PL and 1 operating on AI Engines.

+---------------+---------------+----------------------------------------------------------------------------------------------------------------------------------------+
|    Kernel     |   Type        |  Description                                                                                                                           |
+===============+===============+========================================================================================================================================+
|   mm2s        | HLS           |  Memory Map to Stream kernel to feed input data from DDR to AI Engine via the PL DMA                                                   |
+---------------+---------------+----------------------------------------------------------------------------------------------------------------------------------------+
|   Adder       | AI Engine     |  Receive two stream inputs carrying 32-bit integers, perform add operation and produce a single stream output carrying 32-bit integers |
+---------------+---------------+----------------------------------------------------------------------------------------------------------------------------------------+
|   s2mm        | HLS           |  Stream to Memory Map kernel to feed output result data from AI Engine to DDR via the PL DMA                                           |
+---------------+---------------+----------------------------------------------------------------------------------------------------------------------------------------+

AIE Graph
---------

The AI Engine has two 32-bit input AXI4-Stream interfaces and two 32-bit output AXI4-Stream interfaces. Each stream is connected to a FIFO both on the input and output side, allowing the AI Engine to have a 4 word (128-bit) access per 4 cycles, or a 1 word (32-bit) access per cycle on a stream.
An AI Engine program consists of a Data Flow Graph Specification written in C++. First we need to prepare the kernels. Kernels are declared as ordinary C/C++ functions that return void and can use special data types as arguments.

In this case, we just create a single kernel with the function of aie_adder. It is recommended that a header file(aie_kernel.h) should declare the function prototypes for all kernels used in a graph.

.. code:: c++

      :number-lines: 1
           
           #include <adf.h>
 
           void aie_adder( input_stream_int32 * in0, input_stream_int32 * in1, output_stream_int32 * out) {  
            
               v4int32 a = readincr_v4 (in0); 
            
               v4int32 b = readincr_v4(in1);
            
               v4int32 c = operator+ ( a , b );   
            
               writeincr_v4(out,c);
            
           }


Adaptive Data Flow (ADF) header (adf.h) is included here. The ADF library includes all the required constructs for defining and executing the graphs on AI Engines.

Data type v4int32 is vector of 32-bit vector elements. It has 4 lanes. operator+ is vector intrinsics, performing lane by lane addition.

readincr_v4 operation reads 4-lane data from the given input stream and advance the stream on the AI Engine. writeincr_v4 operation writes 4-lane data to the given output stream and advance the stream on the AI Engine.

We can create and compile the AI Engine graph into a libadf.a file using the AI Engine compiler.

.. code:: c++

      aiecompiler --target=hw -include="$XILINX_VITIS/aietools/include" -include="./src/aie" -include="./data"-include="./" --pl-freq=100 -workdir=./Work  aie_graph.cpp


We can also invoke the AI Engine System C simulator to simulate the AI Engine application.

.. code:: c++

      aiesimulator --pkg-dir ./Work


The process (v++ linker) creates an XCLBIN file to load and run an AI Engine graph and PL kernels code in the target platform. We need to tell the linker how we want to connect them together (from the AI Engine array to PL and vice versa). This is specified in system.cfg file.

.. code:: c++

      [connectivity]
      nk=mm2s:2:mm2s_1.mm2s_2
      nk=s2mm:1:s2mm
      stream_connect=mm2s_1.s:ai_engine_0.DataIn0
      stream_connect=mm2s_2.s:ai_engine_0.DataIn1
      stream_connect=ai_engine_0.DataOut:s2mm.s
      
The nk option specifies the kernel name, the number of instances, or CUs of that kernel, and the CU name for each instance.

We specify two CUs of mm2s called mm2s_1 and mm2s_2, and one CU of s2mm.

The stream_connect (sc) options defines connections between the ports of the AI Engine graph and streaming ports of PL kernels that are not included in the graph. 

From the config file, we define connections between the streaming outputs of mm2s_1 & mm2s_2 and the DataIn0 & Datain1 input port of the AI Engine graph respectively. The output port DataOut of the AI Engine graph is connected to the streaming input port of s2mm.   
