<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Implementing an IIR Filter on the AI Engine - Part 1a

***Version: Vitis 2023.1***

## Preliminaries

We use the [transposed direct form II](https://www.dsprelated.com/freebooks/filters/Transposed_Direct_Forms.html) (TDF2) of an IIR filter for [numerical robustness](https://www.dsprelated.com/freebooks/filters/Numerical_Robustness_TDF_II.html) and focus on implementing a [biquad](https://en.wikipedia.org/wiki/Digital_biquad_filter) section to allow cascading multiple sections to form higher-order IIR filters.

The block diagram for a TDF2 [IIR biquad filter](https://www.dsprelated.com/showarticle/1137.php) is as follows:

![Fig. 1](./images/iir_biquad.png "IIR Biquad Filter Block Diagram")

TDF2 [IIR biquad filter](https://www.dsprelated.com/showarticle/1137.php) has the following difference equation:

![Eqn. 1](./images/eqn1.PNG "Eqn. 1")

Starting with the *first* eight input samples (that is, *x*[0], *x*[1],…, *x*[7]), the four previous states are initialized to *x*[-1] = *x*[-2] = *y*[-1] = *y*[-2] = 0. Thus, the corresponding eight outputs can be expressed as:

![Eqn. 2](./images/eqn2.PNG "Eqn. 2")

This system of equations may be solved sequentially or in parallel. To fully utilize the SIMD capabilities of the vector processor, we need to parallelize this system of equations [[2](https://raphlinus.github.io/audio/2019/02/14/parallel-iir.html)].

***Note*** We can express all the outputs in terms of the current inputs and the four previous states. For example, substituting the expression for *y*[*n* + 0] to solve for *y*[*n* + 1] in terms of the current inputs (*x*[*n*]~*x*[*n* + 1]) and the four states (*x*[*n*-1], *x*[*n*-2], *y*[*n*-1] and *y*[*n*-2]):

![Eqn. 3](./images/eqn3.PNG "Eqn. 3")

Using a symbolic solver like [wxMaxima](https://wxmaxima-developers.github.io/wxmaxima/index.html) or [Mathematica](https://www.wolfram.com/mathematica/), we find that:

![Eqn. 4](./images/eqn4.PNG "Eqn. 4")

where

```
*************************************
Ky0_ym2 = -a2;
Ky0_ym1 = -a1;
Ky0_xm2 = b2*K;
Ky0_xm1 = b1*K;
Ky0_x0 = b0*K;
*************************************
Ky1_ym2 = a1*a2;
Ky1_ym1 = a1^2 - a2;
Ky1_xm2 = -(a1*b2*K);
Ky1_xm1 = (-(a1*b1) + b2)*K;
Ky1_x0 = (-(a1*b0) + b1)*K;
Ky1_x1 = b0*K;
*************************************
Ky2_ym2 = a2*(-a1^2 + a2);
Ky2_ym1 = -a1^3 + 2*a1*a2;
Ky2_xm2 = (a1^2 - a2)*b2*K;
Ky2_xm1 = (a1^2*b1 - a2*b1 - a1*b2)*K;
Ky2_x0 = (a1^2*b0 - a2*b0 - a1*b1 + b2)*K;
Ky2_x1 = (-(a1*b0) + b1)*K;
Ky2_x2 = b0*K;
*************************************
Ky3_ym2 = a1*(a1^2 - 2*a2)*a2;
Ky3_ym1 = a1^4 - 3*a1^2*a2 + a2^2;
Ky3_xm2 = -(a1*(a1^2 - 2*a2)*b2*K);
Ky3_xm1 = (-(a1^3*b1) + 2*a1*a2*b1 + a1^2*b2 - a2*b2)*K;
Ky3_x0 = -((a1^3*b0 - a1^2*b1 + a2*b1 + a1*(-2*a2*b0 + b2))*K);
Ky3_x1 = (a1^2*b0 - a2*b0 - a1*b1 + b2)*K;
Ky3_x2 = (-(a1*b0) + b1)*K;
Ky3_x3 = b0*K;
*************************************
Ky4_ym2 = -(a2*(a1^4 - 3*a1^2*a2 + a2^2));
Ky4_ym1 = -(a1*(a1^4 - 4*a1^2*a2 + 3*a2^2));
Ky4_xm2 = (a1^4 - 3*a1^2*a2 + a2^2)*b2*K;
Ky4_xm1 = (a1^4*b1 - 3*a1^2*a2*b1 + a2^2*b1 - a1^3*b2 + 2*a1*a2*b2)*K;
Ky4_x0 = (a1^4*b0 - a1^3*b1 + 2*a1*a2*b1 + a2*(a2*b0 - b2) + a1^2*(-3*a2*b0 + b2))*K;
Ky4_x1 = -((a1^3*b0 - a1^2*b1 + a2*b1 + a1*(-2*a2*b0 + b2))*K);
Ky4_x2 = (a1^2*b0 - a2*b0 - a1*b1 + b2)*K;
Ky4_x3 = (-(a1*b0) + b1)*K;
Ky4_x4 = b0*K;
*************************************
Ky5_ym2 = a1*a2*(a1^4 - 4*a1^2*a2 + 3*a2^2);
Ky5_ym1 = a1^6 - 5*a1^4*a2 + 6*a1^2*a2^2 - a2^3;
Ky5_xm2 = -(a1*(a1^4 - 4*a1^2*a2 + 3*a2^2)*b2*K);
Ky5_xm1 = (-(a1^5*b1) + 4*a1^3*a2*b1 - 3*a1*a2^2*b1 + a1^4*b2 - 3*a1^2*a2*b2 + a2^2*b2)*K;
Ky5_x0 = (-(a1^5*b0) + a1^4*b1 - 3*a1^2*a2*b1 + a2^2*b1 + a1^3*(4*a2*b0 - b2) + a1*a2*(-3*a2*b0 + 2*b2))*K;
Ky5_x1 = (a1^4*b0 - a1^3*b1 + 2*a1*a2*b1 + a2*(a2*b0 - b2) + a1^2*(-3*a2*b0 + b2))*K;
Ky5_x2 = -((a1^3*b0 - a1^2*b1 + a2*b1 + a1*(-2*a2*b0 + b2))*K);
Ky5_x3 = (a1^2*b0 - a2*b0 - a1*b1 + b2)*K;
Ky5_x4 = (-(a1*b0) + b1)*K;
Ky5_x5 = b0*K;
*************************************
Ky6_ym2 = a2*(-a1^6 + 5*a1^4*a2 - 6*a1^2*a2^2 + a2^3);
Ky6_ym1 = -a1^7 + 6*a1^5*a2 - 10*a1^3*a2^2 + 4*a1*a2^3;
Ky6_xm2 = (a1^6 - 5*a1^4*a2 + 6*a1^2*a2^2 - a2^3)*b2*K;
Ky6_xm1 = (a1^6*b1 - 5*a1^4*a2*b1 + 6*a1^2*a2^2*b1 - a2^3*b1 - a1^5*b2 + 4*a1^3*a2*b2 - 3*a1*a2^2*b2)*K;
Ky6_x0 = (a1^6*b0 - a1^5*b1 + 4*a1^3*a2*b1 - 3*a1*a2^2*b1 + 3*a1^2*a2*(2*a2*b0 - b2) + a1^4*(-5*a2*b0 + b2) + a2^2*(-(a2*b0) + b2))*K;
Ky6_x1 = (-(a1^5*b0) + a1^4*b1 - 3*a1^2*a2*b1 + a2^2*b1 + a1^3*(4*a2*b0 - b2) + a1*a2*(-3*a2*b0 + 2*b2))*K;
Ky6_x2 = (a1^4*b0 - a1^3*b1 + 2*a1*a2*b1 + a2*(a2*b0 - b2) + a1^2*(-3*a2*b0 + b2))*K;
Ky6_x3 = -((a1^3*b0 - a1^2*b1 + a2*b1 + a1*(-2*a2*b0 + b2))*K);
Ky6_x4 = (a1^2*b0 - a2*b0 - a1*b1 + b2)*K;
Ky6_x5 = (-(a1*b0) + b1)*K;
Ky6_x6 = b0*K;
*************************************
Ky7_ym2 = a1*a2*(a1^6 - 6*a1^4*a2 + 10*a1^2*a2^2 - 4*a2^3);
Ky7_ym1 = a1^8 - 7*a1^6*a2 + 15*a1^4*a2^2 - 10*a1^2*a2^3 + a2^4;
Ky7_xm2 = -(a1*(a1^6 - 6*a1^4*a2 + 10*a1^2*a2^2 - 4*a2^3)*b2*K);
Ky7_xm1 = (-(a1^7*b1) + 6*a1^5*a2*b1 - 10*a1^3*a2^2*b1 + 4*a1*a2^3*b1 + a1^6*b2 - 5*a1^4*a2*b2 + 6*a1^2*a2^2*b2 - a2^3*b2)*K;
Ky7_x0 = -((a1^7*b0 - a1^6*b1 + 5*a1^4*a2*b1 - 6*a1^2*a2^2*b1 + a2^3*b1 + 2*a1^3*a2*(5*a2*b0 - 2*b2) + a1^5*(-6*a2*b0 + b2) + a1*a2^2*(-4*a2*b0 + 3*b2))*K);
Ky7_x1 = (a1^6*b0 - a1^5*b1 + 4*a1^3*a2*b1 - 3*a1*a2^2*b1 + 3*a1^2*a2*(2*a2*b0 - b2) + a1^4*(-5*a2*b0 + b2) + a2^2*(-(a2*b0) + b2))*K;
Ky7_x2 = (-(a1^5*b0) + a1^4*b1 - 3*a1^2*a2*b1 + a2^2*b1 + a1^3*(4*a2*b0 - b2) + a1*a2*(-3*a2*b0 + 2*b2))*K;
Ky7_x3 = (a1^4*b0 - a1^3*b1 + 2*a1*a2*b1 + a2*(a2*b0 - b2) + a1^2*(-3*a2*b0 + b2))*K;
Ky7_x4 = -((a1^3*b0 - a1^2*b1 + a2*b1 + a1*(-2*a2*b0 + b2))*K);
Ky7_x5 = (a1^2*b0 - a2*b0 - a1*b1 + b2)*K;
Ky7_x6 = (-(a1*b0) + b1)*K;
Ky7_x7 = b0*K;
*************************************
```


***Note*** The matrix of constants **C** in (4) has eight rows and 12 columns.

`aie_iir_1a.jl` is a Julia script included in this tutorial which:

* Defines the filter characteristics.
* Breaks down the filter into second-order sections.
* Generates the SIMD coefficients for each stage using the [Double64](https://juliamath.github.io/DoubleFloats.jl/stable/) datatype.

The vector processor on the AI Engine can perform eight multiply-accumulate operations on [binary32](https://en.wikipedia.org/wiki/Single-precision_floating-point_format) variables in *one cycle*. If there were eight *independent* inputs to be processed simultaneously by eight *independent* IIR biquad filters, then the kernel code would be straightforward, and from (1), those signals would be processed with an *ideal* latency of five cycles:

* cycle 1: y<sub>i</sub>[n]  = *K* * *b*<sub>0</sub> * *x*<sub>i</sub>[*n*]
* cycle 2: y<sub>i</sub>[n] += *K* * *b*<sub>1</sub> * *x*<sub>i</sub>[*n*-1]
* cycle 3: y<sub>i</sub>[n] += *K* * *b*<sub>2</sub> * *x*<sub>i</sub>[*n*-2]
* cycle 4: y<sub>i</sub>[n] +=      *-a*<sub>1</sub> * *y*<sub>i</sub>[*n*-1]
* cycle 5: y<sub>i</sub>[n] +=      *-a*<sub>2</sub> * *y*<sub>i</sub>[*n*-2]

An actual implementation would require populating the registers with data; performing the calculation; going through the pipeline; updating the internal states; extracting the data, and so on, thereby incurring more than five cycles for latency.

We can use (4) to calculate eight sequential outputs for one input signal using *one* AI Engine core. One way to visualize the calculation is to process one column of the matrix of constants **C** at every clock cycle. Since **C** has 12 columns, it would *ideally* take 12 cycles to generate eight outputs.

## Kernel Code

The kernel code (using the AI Engine APIs) for one second-order stage is as follows:

`kernel.hpp`

```C++
#ifndef __KERNEL_HPP__	// include guard to prevent multiple inclusion

	#define __KERNEL_HPP__

	#include <adf.h>			// Adaptive DataFlow header
	#include <aie_api/aie.hpp>	// header files for AIE API

	using Vector8f = aie::vector<float, 8>;		// vector of 8 floating-point elements
	using Vector16f = aie::vector<float, 16>;	// vector of 16 floating-point elements
	using VAcc8f = aie::accum<accfloat, 8>;		// accumulator with 8 floating-point elements

	template<unsigned id>
	void SecondOrderSection(
		adf::input_buffer<float> & __restrict idata,	// 8 input samples per iteration
		adf::output_buffer<float> & __restrict odata,	// 8 output samples per iteration
		const float (&C)[96]	// run-time parameter: SIMD matrix of coefficients
	);

#endif // __KERNEL_HPP__
```

`kernel.cpp`

```C++
#include <aie_api/aie_adf.hpp>

// !!! Need to separate C++20 portions for kernel from C++14-processed files (e.g. graph.hpp and tb.cpp)
#include "kernel.hpp"

template<unsigned id>
void SecondOrderSection(
	adf::input_buffer<float> & __restrict idata,	// 8 input samples per iteration
	adf::output_buffer<float> & __restrict odata,	// 8 output samples per iteration
	const float (&C)[96]	// run-time parameter: SIMD matrix of coefficients
) {

	static Vector8f state_reg = aie::zeros<float, 8>();	// clear states

	// input/output iterators
	auto inIter = aie::begin_vector<8>(idata);
	auto outIter = aie::begin_vector<8>(odata);

	Vector8f xreg_hi = *inIter++;		// fetch input samples
	Vector16f xreg = aie::concat(state_reg, xreg_hi);	// xreg[4]: ym2; xreg[5]: ym1; xreg[6]: xm2; xreg[7]: xm1; xreg[8:15]: x0:x7
	Vector8f coeff = aie::load_v<8>(&C[0]);
	VAcc8f acc = aie::mul(coeff, xreg[4]);				// do 1st multiplication instead of zeroing

	for (auto i = 1; i < 12; i++) {
		coeff = aie::load_v<8>(&C[8 * i]);
        float xval = xreg[i + 4];
		acc = aie::mac(acc, coeff, xval);
	}

	Vector8f yout = acc;

	// update states
	state_reg[4] = yout[6];
	state_reg[5] = yout[7];
	state_reg[6] = xreg_hi[6];
	state_reg[7] = xreg_hi[7];

	*outIter++ = yout;

} // end SecondOrderSection()

```

***Notes:***

* The kernel code is compiled with a C++20-compliant compiler, while the rest of the code (`graph.hpp` and simulation testbench) is compiled with a C++14-compliant compiler. Since the kernel header is included in `graph.hpp`, it cannot contain any C++20 constructs.
* The template parameter `id` is used to instantiate multiple instances of the `SecondOrderSection()` function.
* The function accepts an input buffer containing a predetermined number of elements defined in `graph.hpp`, and generates an output buffer.
* The input and output use the `__restrict` keyword to facilitate compiler optimization (see [UG1079](https://docs.xilinx.com/r/en-US/ug1079-ai-engine-kernel-coding/Overview?tocId=OerrcATBJkz9SuXKjosb1w) for details).
* The filter coefficients are passed as a 1-D array via the `C` argument.
* The filter states (`state_reg`) need to be kept between function calls and thus are declared `static`.
* Instead of doing a regular matrix-vector multiplication as indicated by (4), each iteration of the `for` loop takes the *n<sup>th</sup>* column of the **C** matrix and multiplies all the elements of that column with the *n<sup>th</sup> element* of the **x** vector, that is, a vector scaling operation.

## Julia Script Notes

To check the functionality of the kernel code, we use `aie_iir_1a.jl` to generate the coefficients for *one* second-order section, and the impulse response. The script also generates a [unit sample function](https://en.wikipedia.org/wiki/Kronecker_delta) as an input to the kernel.

Some of the user-settable parameters in the Julia script are as follows:

```julia
# --- begin user parameters

fp = 10.0e6             # passband frequency
fs = 100.0e6            # sampling frequency
p = 2                   # no. of poles
rp = 0.1                # passband ripple (dB)
rs = 80.0               # stopband attenuation (dB)
N = 256                 # no. of samples for impulse response
show_plots = true       # display plots?
write_cmatrix = true    # write C matrix to files?
write_impulse = true    # write impulse response?

# --- end user parameters
```

To run the script, start Julia, and run the following commands at the prompt.

```
julia> cd("path_to_aie_iir_1a.jl")
julia> include("aie_iir_1a.jl")
```

***Notes:***

* Replace `path_to_aie_iir_1a.jl` with the actual path to the `aie_iir_1a.jl` script.
* The path must be enclosed in double quotes.
* The initial startup may seem slow as it loads several packages.
* This generates five plots:
  * original filter's frequency response.
  * original filter's impulse response.
  * SOS frequency response.
  * SOS impulse response.
  * impulse response error.
* The following files are also generated:
  * `C1.h` - an array of coefficients to be passed to the kernel.
  * `input.dat` - unit sample function to be used as an input signal for the kernel.
  * `impresponse.dat` - calculated impulse response for comparison with AI Engine result.
* Copy `C1.h` to the `src` directory and `input.dat` and `impresponse.dat` to the `data` directory of the AI Engine project.

## Adaptive Dataflow Graph

The adaptive dataflow graph file looks something like this.

`graph.hpp`

```C++
#ifndef __GRAPH_H__			// include guard to prevent multiple inclusion

	#define __GRAPH_H__

	#include <adf.h>		// Adaptive DataFlow header
	#include "kernel.hpp"

	using namespace adf;

	// dataflow graph declaration
	class the_graph : public graph {	// inherit all properties of the adaptive     dataflow graph

		private:
			kernel section1;

		public:
			input_plio in;		// input port for data to enter the kernel
			input_port cmtx1;	// input port for SIMD matrix coefficients
			output_plio out;	// output port for data to leave the kernel

			// constructor
			the_graph() {

				// associate the kernel with the function to be executed
				section1 = kernel::create(SecondOrderSection<1>);

				// declare data widths and files for simulation
				in = input_plio::create(plio_32_bits, "data/input.dat");
				out = output_plio::create(plio_32_bits, "output.dat");

				const unsigned num_samples = 8;

				// establish connections
				connect(in.out[0], section1.in[0]);
				dimensions(section1.in[0]) = {num_samples};
				connect<parameter>(cmtx1, adf::async(section1.in[1]));
				connect(section1.out[0], out.in[0]);
				dimensions(section1.out[0]) = {num_samples};

				// specify which source code file contains the kernel function
				source(section1) = "kernel.cpp";

				// !!! temporary value: assumes this kernel dominates the AI Engine tile !!!
				runtime<ratio>(section1) = 1.0;

			} // end the_graph()

	}; // end class the_graph

#endif // __GRAPH_H__
```

***Notes:***

* `section1 = kernel::create(SecondOrderSection<1>)` tells the tools that the AI Engine kernel program uses the templated function `SecondOrderSection` with a template parameter of `1`.
* `[input|output]_plio` declare input or output ports *for simulation* located in the programmable logic (PL) portion of the device.
* `[input|output]_plio::create()` declares the width of the data bus used in the PL and the associated input/output file used during the simulation.
* `connect(in.out[0], section1.in[0])` tells the tools that kernel input is connected to the `in` port.
* `dimensions(section1.in[0])` declares the number of samples required to be collected before the executing the kernel.
* `connect<parameter>(cmtx1, adf::async(section1.in[1]))` tells the tools that an *asynchronous* run-time parameter is required for the *first* execution of the kernel. Subsequent executions use the *latest* runtime parameter available, that is if the asynchronous parameter is only sent *once*, then that parameter is/ reused for the remaining life of the kernel.
* `connect(section1.out[0], out.in[0])` tells the tools that the kernel output is connected to the `out` port.
* `dimensions(section1.out[0])` declares the number of samples the kernel generates during each invocation.
* `source(section1) = "kernel.cpp"` tells the tools where to find the source code for the kernel.
* `runtime<ratio>(section1) = 1.0` tells the tools that only this kernel can be placed in the AI Engine. At this time, the actual execution time is unknown.

## Testbench Code

The testbench code looks something like this.

`tb.cpp`

```C++
#include "kernel.hpp"
#include "graph.hpp"
#include "C1.h"

using namespace std;
using namespace adf;

// specify the DFG
the_graph my_graph;

const unsigned num_pts = 256;	// number of sample points in "input.dat"
const unsigned num_iterations = num_pts/8;	// number of iterations to run

// main simulation program
int main() {

	my_graph.init();				// load the DFG into the AI Engine array, establish     connectivity, etc.

	my_graph.update(my_graph.cmtx1, C1, 96);
	my_graph.run(num_iterations);	// run the DFG for the specified number of iterations

	my_graph.end();					// housekeeping

	return (0);

} // end main()
```

***Notes:***

* `my_graph.update(my_graph.cmtx1, C1, 96)` sends the 96 coefficients for the filter to the kernel.
* `my_graph.run(num_iterations)` runs the kernel 256/8 = 32 times to allow comparison with the impulse response from the Julia script.

## Build and Run the Program

* Copy the files in `src` and `dat` to your project. Set the `Top-Level File` to `src/tb.cpp`.
* Since we are only interested in functional verification now, we use `Emulation-SW` to build and run the program.
* If the program builds and runs without errors, the output should be `Emulation-SW/x86simulator_output/output.dat`.
* Copy the generated `impresponse.dat` file to the `data` directory.
* We can use Julia to verify the kernel output.

```
julia> using PyPlot
julia> using DelimitedFiles
julia> ref = readdlm("{specify_directory}/impresponse.dat");
julia> dut = readdlm("{specify_directory}/output.dat");
julia> err = ref - dut;
julia> plot(err);
julia> grid("on");
julia> title("Impulse Response Error");
julia> xlabel("Sampling Index");
julia> ylabel("Error");
julia> eps(Float32)
1.1920929f-7
julia> maximum(abs.(err))
1.0517072768223557e-8
```

The resulting Julia plot of the impulse response error is as follows:
![Fig. 2](./images/impresp_error.PNG "Impulse Response Error")

You may also try modifying and running `check.jl`.

Since the maximum absolute error is less than the [machine epsilon](https://en.wikipedia.org/wiki/Machine_epsilon) for [binary32](https://en.wikipedia.org/wiki/Single-precision_floating-point_format) (`Float32` in Julia), we can conclude that the kernel code is working as expected.

The complete design is included in the `data` and `src` directories. Refer to the aie_exp/Part1 tutorial if you are unfamiliar with building an AMD Vitis&trade; design from scratch.

## Conclusion

We used `Emulation-SW` to verify the functionality of the design. We provided a Julia script that generates the required coefficients for a second-order section of an IIR filter and kernel code to implement it. The AI Engine kernel code implementation shows negligible differences from the original filter.

In Part 1b, we show the process of creating the adaptive dataflow graph for an arbitrary number of cascaded second-order sections.

## References

[1] Julius O. Smith III. "[Introduction to Digital Filters with Audio Applications](https://www.dsprelated.com/freebooks/filters/)"

[2] [IIR filters can be evaluated in parallel](https://raphlinus.github.io/audio/2019/02/14/parallel-iir.html)

# Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
