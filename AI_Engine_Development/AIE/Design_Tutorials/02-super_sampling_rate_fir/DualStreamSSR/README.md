<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Super Sampling Rate FIR Filter with Dual-Stream Input

***Version: Vitis 2025.2***

The purpose of this fourth part of the tutorial is to understand how to improve upon the performance already achieved using the two input and output stream connections to the AI Engine.

Navigate to the `DualStreamSSR` directory to continue.

## Dual-Stream Input Impact

The last two sections (Multi-kernel and Single-Stream SSR) showed that when using a single input stream, the balance between stream bandwidth and compute performance for `cint16 x cint16` is obtained for an 8-tap filter implementation in an AI Engine. This can be easily computed. For the device on the VCK190 speed grade of the AMD Versal™ AI Core Series, the entire AI Engine array (processors, AXI-Stream connections, memory modules) is clocked at 1.25 GHz. The input stream can transfer 32 bits per clock and a `cint16` variable is 32-bit wide; hence, a rate of 1.25 GSPS (giga samples per second). The processor by itself is capable of eight `cint16xcint16` operations per clock cycle. The result is that the processor can perform 8-tap filter processing per clock cycle.

If two input streams operate in an efficient way, the input sample rate can increase to 2.5 GSPS (1.25 GSPS per stream). As the processor performance does not change, it is able to process only four taps per clock cycle at the input sample rate.

This means that in the case of a single-stream implementation, the filter length should be a multiple of eight to extract maximum performance from the AI Engine array. In the case of a dual-stream implementation, the filter length must be a multiple of four to achieve this maximum performance. This lower granularity allows more freedom in the filter length. Take a 12 tap filter as an example, with an input sample rate at 2.5 GSPS.

1. Single-stream implementation: The input sample rate (2.5 GSPS) requires splitting the coefficients and input data into two phases (1.25 GSPS each). Because it has two phases, this implementation requires four kernels (2 x 2) in a grid. 12 taps divided into two phases results in six taps per phase. Each kernel handles six taps, but the maximum performance is eight taps. Single-stream input data use four AI Engines at 75 percent of their maximum performance.

2. Dual-stream implementation: This case is simpler. The input interface can handle a 2.5 GSPS input data sampling rate, but can process only four taps per kernel. This implementation requires three kernels (3 kernels x 4 taps = 12 taps) running at 100 percent of their compute performance.

A major impact is the way the data flows to the AI Engine. The AI Engine alternatively reads four samples on both the streams (or eight samples at the same time). The resulting stream must be equivalent to a 2.5 GSPS data stream. Suppose you have the following 2.5 GSPS data stream: `d0, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15, d16, d17, d18, d19, ...`

The AI Engine read sequence must be:

```text
- Read Stream 0 : `d0, d1, d2, d3`
- Read Stream 1 : `d4, d5, d6, d7`
- Read Stream 0 : `d8, d9, d10, d11`
- Read Stream 1 : `d12, d13, d14, d15`
- Read Stream 0 : `d16, d17, d18, d19`
- ...
```

So the content of each stream must be:

```text
- Stream 0: `d0, d1, d2, d3, d8,d9, d10, d11, d16, d17, d18, d19, ...`
- Stream 1: `d4, d5, d6, d7, d12, d13, d14, d15, ...`
```

The stream content is dependent on the number of samples (bits), which the system reads as a block on each stream.

In single-stream implementation, some kernels had to discard one sample before the first invocation. The initialization function accomplished this easily, discarding one sample or blocks of eight samples if the coefficient phases were longer than eight coefficients. In dual-stream implementation, this is complex because if one sample comes from Stream 0 beforehand, the stream combination becomes completely disorganized. To avoid changing the stream content, reorganize the computation and start computing one sample after (change the *Start* parameter of the `mul4/mac4` intrinsic).

On top of this, if the coefficient phase is longer than four, you must discard four elements. If it is longer than eight, discard it. Stream 0 provides the first four elements, Stream 1 provides the next four, and then Stream 0 again. If more blocks of four elements come from Stream 0 than from Stream 1, the first stream to read within the kernel is Stream 1.

## Designing the Graph

With the input data rate being 2.5 GSPS on each AI Engine and the filter with 32 taps, the data stream and coefficient can be split into eight phases as each AI Engine is capable of 4-tap filter processing. This leads to 2.5 GSPS x 8 Phases = 20 GSPS input sample rate. You can now design the maximum performance for the filter.

The same recommendations as in the previous section apply:

- Each AI Engine in a column must receive the same data.
- One row in every two has a cascade stream in the other direction, leading to a differentiated set of stream for even and odd rows.
- The kernels above the diagonal (lower left to upper right) must discard one element in the stream.

## C++ Code Analysis

The kernel definition is now different from the single-stream implementation. The class definition is close, but there is now one more parameter in the template:

```C++
template<int NSamples,int ShiftAcc,bool DiscardSample,bool SwapRead>
class FIR_MultiKernel_cout {
private:
	alignas(32) cint16 weights[4];
	alignas(32) cint16 delay_line[16];

public:
	FIR_MultiKernel_cout(const cint16 (&taps)[4])
	{
		for(int i=0;i<4;i++) weights[i] = taps[i];
		for(int i=0;i<16;i++) delay_line[i] = (cint16){0,0};
	};

	void filter(input_stream_cint16*  restrict sin1,input_stream_cint16*  restrict sin2,output_stream_cacc48*  cout);

	static void registerKernelClass()
	{
		REGISTER_FUNCTION(FIR_MultiKernel_cout::filter);
	};
};
```

The template contains two more boolean parameters: `DiscardSample`and `SwapRead`. These parameters provide control over whether or not the computation is started at sample 1 or 2 (`DiscardSample`), and whether the stream read should start with Stream 0 or Stream 1 (`SwapRead`).

```C++
template <int NSamples,int ShiftAcc,bool DiscardSample,bool SwapRead>
void DoubleStream::FIR_MultiKernel_cout<NSamples,ShiftAcc,DiscardSample,SwapRead>::filter(input_stream_cint16* restrict sin1,input_stream_cint16* restrict sin2,output_stream_cacc48* cout)
{
	v8cint16 taps =  *(v8cint16*) weights;
	v16cint16 *ptr_delay_line = (v16cint16 *)delay_line;
	v16cint16 data = *ptr_delay_line;

	v4cacc48 acc = undef_v4cacc48();

    // These values must be constants in the intrinsic, hence these pre-declaration
	static const int Start_1 = (DiscardSample?2:1);
	static const int Start_3 = (DiscardSample?4:3);
	static const int Start_5 = (DiscardSample?6:5);
	static const int Start_7 = (DiscardSample?8:7);
	static const int Start_9 = (DiscardSample?10:9);
	static const int Start_11 = (DiscardSample?12:11);
	static const int Start_13 = (DiscardSample?14:13);
	static const int Start_15 = (DiscardSample?0:15);


	// Computes 16 samples per iteration
	for(int i=0;i<NSamples/16;i++)
	chess_prepare_for_pipelining
	chess_loop_range(NSamples/16,NSamples/16)
	{
        // Change read order depending on 'SwapRead' flag
		if(SwapRead)
			data = upd_w(data,1, concat(getc_wss(1),getc_wss(0)));
		else
			data = upd_w(data,1, concat(getc_wss(0),getc_wss(1)));

		acc = mul4(data,Start_1,0x3210,1,taps,0,0x0000,1);
		acc = mac4(acc,data,Start_3,0x3210,1,taps,2,0x0000,1);
		writeincr_v4(cout,acc);

		acc = mul4(data,Start_5,0x3210,1,taps,0,0x0000,1);
		acc = mac4(acc,data,Start_7,0x3210,1,taps,2,0x0000,1);
		writeincr_v4(cout,acc);


		if(SwapRead)
			data = upd_w(data,0, concat(getc_wss(1),getc_wss(0)));
		else
			data = upd_w(data,0, concat(getc_wss(0),getc_wss(1)));

		acc = mul4(data,Start_9,0x3210,1,taps,0,0x0000,1);
		acc = mac4(acc,data,Start_11,0x3210,1,taps,2,0x0000,1);
		writeincr_v4(cout,acc);

		acc = mul4(data,Start_13,0x3210,1,taps,0,0x0000,1);
		acc = mac4(acc,data,Start_15,0x3210,1,taps,2,0x0000,1);
		writeincr_v4(cout,acc);

	}

	*ptr_delay_line = data;
}
```

You can see that the stream read is done using the raw access intrinsic to the streams intrinsic. This is because if the stream name given in the parameter list is used (even with restrict), the compiler is unable to schedule them on the same clock cycle.

At the graph level, all kernels are first declared in a class:

```C++
class FIRGraph_SSR8: public adf::graph
{
private:
    kernel k[8][8];

public:
    input_port in[16]; // 8 columns, 2 streams per kernel
    output_port out[16]; // 8 columns, 2 streams per kernel
```

The constructor takes charge of the next operations. The first operation is to create the kernels. The complete grid of 8x8 kernels is defined within a nested loop. Because template parameters must be constant, there are two parts in the inner loop, one for `(DiscardSample, SwapRead) = (true, false)` and the other for `(DiscardSample, SwapRead) = (false, false)`.

The source and header locations are then defined for the AI Engine. The location of the first AI Engine in each row must also be constrained to facilitate the placer work. To shorten the place time by a few seconds, you can constrain the core location. A single one is necessary because all the others are constrained by the **cascade** connection.

The kernels must discard a specific number of elements. In this dual-stream implementation, this is handled by the kernel itself. To ensure that this is correctly done, the instantiation line can be extracted from the AI Engine source code. Navigate to `Work/aie/`. In this directory, all the AI Engines used in the design have their own directory. Open the first one: `cd 23_0/src`, and look at the source code. The instantiation of the kernel can be viewed:

```C++
// Declare Kernel objects and external arrays
#include "aie_kernels/FirDoubleStream.cpp"
DoubleStream::FIR_MultiKernel_cout<512, 0, false, false> i48({{-82, -253}, {643, 467}, {9582, 0}, {-192, 140
}});

// Declare shared memory buffers
...
```

It is the same for other columns. A simpler utility provides a shorter implementation. Navigate back to `Work/aie`, and type `GetDeclare.sh`. The output starts as follows:

```C++
Row 0
DoubleStream::FIR_MultiKernel_cout<512, 0, false, false> i48({{-82, -253}, {643, 467}, {9582, 0}, {-192, 140}});
DoubleStream::FIR_MultiKernel_cincout<512, 0, false, false> i49({{0, -204}, {984, 1355}, {7421, 2411}, {-882, 287}});
DoubleStream::FIR_MultiKernel_cincout<512, 0, false, false> i50({{11, -35}, {550, 1691}, {3936, 2860}, {-1079, 0}});
DoubleStream::FIR_MultiKernel_cincout<512, 0, false, false> i51({{-198, 273}, {0, 647}, {1023, 1409}, {-755, -245}});
DoubleStream::FIR_MultiKernel_cincout<512, 0, false, false> i52({{-642, 467}, {538, -1656}, {-200, -615}, {-273, -198}});
DoubleStream::FIR_MultiKernel_cincout<512, 0, false, false> i53({{-1026, 333}, {2860, -3936}, {0, -1778}, {22, 30}});
DoubleStream::FIR_MultiKernel_cincout<512, 0, false, false> i54({{-927, 0}, {6313, -4587}, {517, -1592}, {63, 194}});
DoubleStream::FIR_MultiKernel_cin<512, 0, false, false> i55({{-226, -73}, {9113, -2961}, {467, -643}, {0, 266}});

Row 1
DoubleStream::FIR_MultiKernel_cin<512, 0, true, false> i56({{-226, -73}, {9113, -2961}, {467, -643}, {0, 266}});
DoubleStream::FIR_MultiKernel_cincout<512, 0, false, false> i57({{-82, -253}, {643, 467}, {9582, 0}, {-192, 140}});
DoubleStream::FIR_MultiKernel_cincout<512, 0, false, false> i58({{0, -204}, {984, 1355}, {7421, 2411}, {-882, 287}});
DoubleStream::FIR_MultiKernel_cincout<512, 0, false, false> i59({{11, -35}, {550, 1691}, {3936, 2860}, {-1079, 0}});
...
```

In row 0, no kernel should discard any sample, in row 1, only the first kernel discards one sample, and so on.

Finally, all kernels must be connected together with the cascade stream between them, and the input streams for all them. In this example, the implementation shares the DMA FIFOs. There are two DMA FIFOs in row 0 for all the even rows, and two DMA FIFOs on row 1 for all odd rows:

```C++
// Input Streams connections
for(int row = 0;row<NPhases;row++)
for(int col=0;col<NPhases;col++)
{
    int col1 = (row%2?NPhases-col-1:col); // kernel col is inverted on odd rows
    int fiforow = row%2;

    connect<stream> n0(in[2*col],k[row][col1].in[0]);
    connect<stream> n1(in[2*col+1],k[row][col1].in[1]);
    fifo_depth(n0) = 512;
    fifo_depth(n1) = 512;

    location<fifo>(n0) = dma_fifo(aie_tile, FirstCol+col, fiforow, 0x0000, 512);
    location<fifo>(n1) = dma_fifo(aie_tile, FirstCol+col, fiforow, 0x2000, 512);
}
```

## Compilation and Analysis

Navigate to the `MultiKernel` directory. In the `Makefile`, three methods define the build process:

- `aie`
  - Compiles the graph and the kernels
- `aie_sim`
  - Runs the AI Engine SystemC simulator
- `aie_viz`
  - Runs `vitis_analyzer`on the output summary

Take a look at the source code (kernel and graph) to familiarize yourself with C++ instantiation of kernels. In `graph.cpp`, the PL AI Engine connections declare 64-bit interfaces running at 500 MHz. This allows for maximum bandwidth on the AI Engine array AXI-Stream network.

To have the simulation running, you must generate input data. There are two possibilities:

1. Type `make data`.
2. Change directory to `data` and type `GenerateStreamsGUI`. Set the following parameters for this example:

    ![missing image](../Images/generateDualStreamsSSR8.jpg)

Click **Generate** and then **Exit**. The generated files `PhaseIn_0_0.txt` through `PhaseIn_7_7.txt` should contain mainly 0s, with a few 1s and 2s. The number of samples per stream is half of the one that the C++ code declares because in the C++ code this is the length of the concatenation of both input streams.

Type `make all` and wait for the `vitis_analyzer` GUI to display. The AMD Vitis™ Analyzer can show the graph, how the device implements it, and the complete timeline of the simulation. In this specific case, the graph is simple (a single kernel) and the implementation is on a single AI Engine.

Click **Graph** to visualize the graph of the application:

![missing image](../Images/Graph8Phases.png )

The 64 kernels and their 16 independent input streams are clearly visible. The top graph is for the output phases 0, 2, 4, and 6, the phases where the cascade stream goes from left to right on the physical device, and the bottom graph is for the phases 1, 3, 5, and 7 where the cascade stream goes from right to left.

Click **Array** to visualize where the placer positioned the kernel, and how the PL feeds it:

![missing image](../Images/Array8Phases.png)

In this view, the cascade streams connecting neighboring AI Engines are key to the performance of this graph. With the four location constraints in place, the placer had only one solution for the kernel placement: this square. The router had an easy job to feed all these kernels by simply using the south-north AXI-Stream. The path back to the PL from the extremities also uses only the vertical AXI-Streams.

Finally, click **Trace** to look at how the entire simulation went through. This may be useful to track where your AI Engine stalls if the performance is not as expected:

Now you can display the output of the filter. Because the input is a set of Dirac impulses, you should recognize the impulse response of the filter throughout the waveform. Navigate to `aiesimulator_output/data` and look at the `PhaseOut_0.txt`. You can see that you have two complex outputs per line, with a time stamp prepending each line. `ProcessAIEOutput PhaseOut_*`.

![missing image](../Images/GraphOutput8Phases.jpg)

The top graph reflects the real part of the output, the bottom graph this is the imaginary part. On both, the filter impulse response is recognizable.

After simulation the simulator displays the raw throughput at the input and output ports:

```text
--------------------------------------------------------------------------------
| Intf Type   | Port Name                          | Type  | Throughput(MBps)  |
--------------------------------------------------------------------------------
| plio        | PhaseOut_0_1                       | IN    | 4981.621622       |
|             | PhaseIn_0_1                        | IN    | 5002.171081       |
|             | PhaseIn_1_0                        | IN    | 4947.391024       |
|             | PhaseIn_1_1                        | IN    | 5002.171081       |
|             | PhaseIn_2_0                        | IN    | 4529.192058       |
|             | PhaseIn_2_1                        | IN    | 5002.171081       |
|             | PhaseIn_3_0                        | IN    | 4981.621622       |
|             | PhaseIn_3_1                        | IN    | 5002.171081       |
|             | PhaseIn_4_0                        | IN    | 4981.621622       |
|             | PhaseIn_4_1                        | IN    | 5002.171081       |
|             | PhaseIn_5_0                        | IN    | 4529.192058       |
|             | PhaseIn_5_1                        | IN    | 5002.171081       |
|             | PhaseIn_6_0                        | IN    | 4947.391024       |
|             | PhaseIn_6_1                        | IN    | 5002.171081       |
|             | PhaseIn_7_0                        | IN    | 4981.621622       |
|             | PhaseIn_7_1                        | IN    | 5002.171081       |
|             | PhaseOut_0_0                       | OUT   | 4457.018498       |
|             | PhaseOut_0_1                       | OUT   | 4455.079400       |
|             | PhaseOut_1_0                       | OUT   | 4457.018498       |
|             | PhaseOut_1_1                       | OUT   | 4455.079400       |
|             | PhaseOut_2_0                       | OUT   | 4457.018498       |
|             | PhaseOut_2_1                       | OUT   | 4455.079400       |
|             | PhaseOut_3_0                       | OUT   | 4457.018498       |
|             | PhaseOut_3_1                       | OUT   | 4455.079400       |
|             | PhaseOut_4_0                       | OUT   | 4457.018498       |
|             | PhaseOut_4_1                       | OUT   | 4455.079400       |
|             | PhaseOut_5_0                       | OUT   | 4457.018498       |
|             | PhaseOut_5_1                       | OUT   | 4455.079400       |
|             | PhaseOut_6_0                       | OUT   | 4457.018498       |
|             | PhaseOut_6_1                       | OUT   | 4455.079400       |
|             | PhaseOut_7_0                       | OUT   | 4457.018498       |
|             | PhaseOut_7_1                       | OUT   | 4455.079400       |
```

The aggregated output port throughput in MSPS (cint16) is: `17102.45 Msps`.

You can measure the performance of this architecture using the timestamped output. In the same directory (`aiesimulator_output/data`), type `StreamThroughput PhaseOut_*`:

```text
PhaseOut_0_0.txt -->  1066.67 Msps
PhaseOut_0_1.txt -->  1064.89 Msps
PhaseOut_1_0.txt -->  1072.03 Msps
PhaseOut_1_1.txt -->  1072.03 Msps
PhaseOut_2_0.txt -->  1066.67 Msps
PhaseOut_2_1.txt -->  1064.89 Msps
PhaseOut_3_0.txt -->  1072.03 Msps
PhaseOut_3_1.txt -->  1072.03 Msps
PhaseOut_4_0.txt -->  1066.67 Msps
PhaseOut_4_1.txt -->  1064.89 Msps
PhaseOut_5_0.txt -->  1072.03 Msps
PhaseOut_5_1.txt -->  1072.03 Msps
PhaseOut_6_0.txt -->  1066.67 Msps
PhaseOut_6_1.txt -->  1064.89 Msps
PhaseOut_7_0.txt -->  1072.03 Msps
PhaseOut_7_1.txt -->  1072.03 Msps

-----------------------


Total Throughput -->   17102.45 Msps

```

This architecture achieves almost 19 GSPS performance. It is less than the maximum expected (20 GSPS) because of the number of cycles spent for initialization when calling the kernels. This performance increases when you increase the frame length. For a 32K sample frame length, the performance obtained is:

```text
Total Throughput -->   19950.30 Msps
```

which is almost the expected maximum.

## Support

GitHub issues are used to track requests and bugs. For questions, go to [support.amd.com](https://adaptivesupport.amd.com/s/?language=en_US).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc</sub><br></br></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
