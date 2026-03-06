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

# Bilinear Interpolation

***Version: Vitis 2025.2***

## Table of Contents

- [Bilinear Interpolation](#bilinear-interpolation)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Computing Interpolated Values](#computing-interpolated-values)
  - [Design Assumptions](#design-assumptions)
  - [AI Engine Code Vectorization](#ai-engine-code-vectorization)
  - [Data Interface](#data-interface)
    - [Programmable Logic Component](#programmable-logic-component)
    - [PLIO Interface](#plio-interface)
    - [AI Engine Test Vectors](#ai-engine-test-vectors)
  - [AI Engine Kernel Processing](#ai-engine-kernel-processing)
    - [Kernel Data Interface](#kernel-data-interface)
    - [Kernel Code](#kernel-code)
  - [Running the Example](#running-the-example)
    - [Generating Test Vectors](#generating-test-vectors)
    - [Running x86 Simulation](#running-x86-simulation)
    - [Running AI Engine Simulation](#running-ai-engine-simulation)
  - [Analyzing Results](#analyzing-results)
    - [Vitis Analyzer](#vitis-analyzer)
    - [Test Vector Comparison](#test-vector-comparison)
  - [Customizing the Example](#customizing-the-example)
    - [Specifying a Test Image and Output Resolution](#specifying-a-test-image-and-output-resolution)
    - [Multicore Processing](#multicore-processing)
  - [References](#references)
  - [Support](#support)

[References](#references)

[Support](#support)

[License](#license)

## Introduction

Bilinear interpolation is a method for interpolating functions of two variables using repeated linear interpolation. It is commonly used in the following applications:

- Image processing and computer vision: For resampling images and textures. For example, when zooming in or out of an image, use bilinear interpolation to estimate color values of new pixels based on color values of the original pixels. [[1]] [[2]]
- Finite element analysis: For estimating values of variables such as stress and strain between nodes in a finite element mesh. [[3]]
- Geographic information systems: For interpolating elevation or other spatial data from a grid of points. [[4]]
- Computer graphics: For mapping textures onto 3D surfaces or perform texture filtering. [[5]]

Bilinear interpolation is one of the simplest and fastest interpolation methods, but it can also introduce some artifacts such as blurring or aliasing. More advanced interpolation methods, such as bicubic interpolation or spline interpolation, can produce smoother and more accurate results, but they are also more computationally expensive.

## Computing Interpolated Values

The following figure shows the problem of bilinear interpolation. It is assumed you know the values of a function at points $(x_1, y_1)$, $(x_1, y_2)$, $(x_2, y_1)$, and $(x_2, y_2)$ defined on a grid, which for practical purposes, can be assumed to be rectilinear. The goal is to estimate the function value at a point with coordinates $(x_q, y_q)$ by using the known values at the surrounding points. In the following figure, green dots represent known values, and the red dot represents the value to be estimated.

![figure1](images/points_1.png)

*Figure 1 - Bilinear Interpolation Problem*

Bilinear interpolation is a two-step process, where linear interpolation is first performed over one dimension then the other. The following figure shows the first step of the process. Here, the function values at the blue dots are computed from the known values at the green dots, using linear interpolation over the variable $x$.

![figure2](images/points_2.png)

*Figure 2 - First Linear Interpolation*

These two intermediate points can be expressed in terms of the known values as

$$f(x_q,y_1) = \frac{(x_2-x_q)}{(x_2-x_1)}f(x_1,y_1) + \frac{(x_q-x_1)}{(x_2-x_1)}f(x_2,y_1)$$

and

$$f(x_q,y_2) = \frac{(x_2-x_q)}{(x_2-x_1)}f(x_1,y_2) + \frac{(x_q-x_1)}{(x_2-x_1)}f(x_2,y_2).$$

Figure 3 shows the second step of the process, where the desired value at the red dot is derived from the computed values at the blue dots using linear interpolation over the variable $y$.

![figure3](images/points_3.png)

*Figure 3 - Second Linear Interpolation*

The resulting interpolated point is

$$f(x_q,y_q) = \frac{(y_2-y_q)}{(y_2-y_1)}f(x_q,y_1) + \frac{(y_q-y_1)}{(y_2-y_1)}f(x_q,y_2).$$

Combining equations, the bilinear interpolation formula can be expressed as

$$f(x_q,y_q) = \frac{1}{(x_2-x_1)(y_2-y_1)} \begin{bmatrix} x_2-x_q & x_q-x_1 \end{bmatrix} \begin{bmatrix} f(x_1,y_1) & f(x_1,y_2) \\ 
f(x_2,y_1) & f(x_2,y_2) \end{bmatrix} \begin{bmatrix} y_2-y_q \\
y_q-y_1 \end{bmatrix}.$$

## Design Assumptions

While you can apply bilinear interpolation to various applications, this tutorial uses an image processing example. In this case, function values correspond to pixel values in the range [0, 255]. Single precision, floating-point numerical format is assumed for interpolated pixel values and interpolation coordinates $(x_q,y_q)$.

A reference image generates a lookup table which provides input to the AI Engine. An input image with resolution $x_{res} \times y_{res}$ is assumed to have pixels defined on a grid with unit spacing. You can combine the $x$ and $y$ pixel coordinates using the equation $I = x \times y_{res} + y$ to derive a LUT index $I$, as shown in the following figure.

![figure4](images/image2lut.png)

*Figure 4 - Image as a Lookup Table*

For any query point, $(x_q,y_q)$, the floating point coordinates can separate into integer and fractional parts, where $x_q = x_{int}.x_{frac}$ and $y_q = y_{int}.y_{frac}$. The integer parts extract pixel values used in the interpolation equation. The four values required for interpolation derive from the LUT using the following relations:

$$
\begin{aligned}
&f(x_1,y_1) = LUT(x_{int} * y_{res} + y_{int}) \\
&f(x_1,y_2) = LUT(x_{int} * y_{res} + y_{int} + 1) \\
&f(x_2,y_1) = LUT((x_{int} + 1) * y_{res} + y_{int}) \\
&f(x_2,y_2) = LUT((x_{int} + 1) * y_{res} + y_{int} + 1).
\end{aligned}
$$

The preceding figure shows an example of LUT indexing using the pixels marked with X. When the four pixel values required for interpolation are obtained, the integer parts of the coordinates $(x_q,y_q)$ are no longer needed and you can assume these to be zero. This simplifies the interpolation equation to

$$
f(x_q,y_q) = \begin{bmatrix} 1-x_{frac} & x_{frac} \end{bmatrix} \begin{bmatrix} f(x_1,y_1) & f(x_1,y_2) \\
f(x_2,y_1) & f(x_2,y_2) \end{bmatrix} \begin{bmatrix} 1-y_{frac} \\
y_{frac} \end{bmatrix}
$$

or expressed as an inner product

$$
f(x_q,y_q) = \begin{bmatrix} (1-x_{frac})(1-y_{frac}) & (1-x_{frac})y_{frac} & x_{frac}(1-y_{frac}) & x_{frac}y_{frac} \end{bmatrix}
\begin{bmatrix} f(x_1,y_1) \\
f(x_1,y_2) \\
f(x_2,y_1) \\
f(x_2,y_2) \end{bmatrix}.
$$

## AI Engine Code Vectorization

To realize advantages of AI Engine processing, you must vectorize computations. Applying this to pixel interpolation, the calculation can be restated as:

$$f(x_q,y_1) = x_{frac}f(x_2,y_1) + f(x_1,y_1) - x_{frac}f(x_1,y_1)$$

and

$$f(x_q,y_2) = x_{frac}f(x_2,y_2) + f(x_1,y_2) - x_{frac}f(x_1,y_2)$$

for the first two interpolations in the x coordinate, and

$$f(x_q,y_q) = y_{frac}f(x_q,y_2) + f(x_q,y_1) - y_{frac}f(x_q,y_1)$$

for the final interpolation in the y coordinate. By reformulating the computation in this way, the first two terms in each equation represent a multiply-accumulate (MAC) operation. You can use this in a follow-on multiply and subtract from accumulator (MSC) operation to obtain the result. Each interpolated pixel requires 3 MAC plus 3 MSC operations.

This example uses single precision floating-point for computation. Figure 5 shows the floating-point vector unit of an AI Engine. Observe that the multiply and accumulator units process eight lanes in parallel. SIMD parallelism uses a pixel-per-lane approach. Because each pixel requires 3 MAC plus 3 MSC operations, each of which may execute in a single clock cycle, a lower limit on computation requirement would be 0.75 cycles per pixel. Consider this bound on computation as a ballpark estimate on expected performance, which is likely unachievable due to overhead, bandwidth limitations, and pipelining inefficiencies.

![figure5](images/fp_vector_unit.png)

*Figure 5 - Floating-Point Vector Unit*

## Data Interface

When mapping algorithms to AI Engines, the process often becomes a tradeoff between computational efficiency, data bandwidth, and memory utilization. In the previous section, a value of 0.75 cycles per pixel was derived as a lower bound on computational efficiency. Next, examine data interfaces to determine limitations they impose and to provide guidance on a suitable choice.

### Programmable Logic Component

When considering system partitioning, tasks such as retrieving data from lookup tables and extracting integer and fractional parts of floating-point numbers are better suited for programmable logic. AI Engines are most efficient if you program them for continual vector processing on a steady stream of input data.

Data necessary to process a single pixel comprises four reference pixels and fractional parts of the $x_q$ and $y_q$ coordinates. Assume each of the six data values uses a 32-bit, single-precision, floating-point format. Figure 6 shows a conceptual illustration of how input data is derived in programmable logic for each pixel. This example design does not include a programmable logic component but it assumes such a component has generates test input data for AI Engine processing.

![figure6](images/vin_fmt.png)

*Figure 6 - Derivation of Input Pixel Data*

### PLIO Interface

Considering that input requires six floating-point values per pixel while output is a single floating-point value, it is apparent that the input stream places a more restrictive limit on achievable pixel processing rate. The PLIO interface supports transfer rates of one floating-point value per cycle, which depending on speed grade of the AMD Versal™ device, amounts to transfer rates of 1.0 to 1.25 billion floating-point values per second. Because the input requires six floating-point values per pixel, the input PLIO restricts the rate to 6.0 cycles per pixel. To more closely match the input limitation to computational efficiency, three input PLIO interfaces are used. This brings the limitation down to 2.0 cycles per pixel. Figure 7 shows the data format for each of the input PLIO interfaces.

![figure7](images/pl_if_streams.png)

*Figure 7 - Input PLIO Data Format*

### AI Engine Test Vectors

When AI Engines graphs are simulated apart from programmable logic and processing systems, text files provide input data. This example uses MATLAB® to generate test vectors, which are sequences of `int32` numbers. Although actual data is single precision floating-point, it is difficult to express such numbers in text format. To capture full precision, the 32 bits used to represent a floating-point number (sign, exponent, mantissa) are written as equivalent `int32` values. Files containing output data use a similar format.

## AI Engine Kernel Processing

### Kernel Data Interface

The kernel example presented here uses buffered I/O for input and output. This enables more efficient VLIW parallelism, where load and store instructions can execute in the same clock cycle as vector processor instructions. The tradeoff is that there is an increased initial latency. Also, the compiler inserts ping pong buffers for each I/O allocated from AI Engine tile memory. Because this example has three inputs and a single output, it requires a total of eight memory banks. This means additional AI Engine tiles accommodate the memory requirement.

Another option for I/O is to use direct streaming to or from the AI Engine. There are two 32-bit input and two 32-bit output streams available. Although this removes the need for ping pong buffers, the kernel code would use additional cycles to shift vector data. For example, if a `float` vector of size eight is sent to an output stream, eight clock cycles are required.

A final option for kernel I/O is possible if the data source or destination is another AI Engine tile. In this case, you can use the cascade interface. A `float` vector of size eight can transfer on each clock cycle using the cascade interface.

### Kernel Code

To improve compute efficiency, kernel code takes advantage of VLIW instructions that perform simultaneous vector multiply, load, and store operations. Each invocation of the kernel processes 256 pixels, but this can vary when generating test vectors for simulation. The kernel code shown here processes two interpolations over the x coordinate followed by an interpolation over the y coordinate for each loop using vectors of size 8. AI Engine vector instrinsic functions performs computation.

```cpp
void bilinear_kernel::interp(input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in_A, 
                             input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in_B, 
                             input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in_C, 
                             output_buffer<int32, extents<BUFFER_SIZE_OUT>>& __restrict out)
{
    // iterators for input & output buffers
    auto pInA = aie::begin_vector<8>(in_A);
    auto pInB = aie::begin_vector<8>(in_B);
    auto pInC = aie::begin_vector<8>(in_C);
    auto pOut = aie::begin_vector<8>(out);

    for (unsigned i = 0; i < PXLPERGRP/8; i++)
        chess_prepare_for_pipelining
        chess_loop_count(PXLPERGRP/8)
    {
        // get data for first x interpolation
        auto xfrac = (*pInA++).cast_to<float>();
        auto p11 = (*pInB++).cast_to<float>();
        auto p21 = (*pInC++).cast_to<float>();

        // compute first x interpolation
        auto tempy1 = fpmac(p11,xfrac,0,0x76543210,p21,0,0x76543210);
        auto pxy1 = fpmsc(tempy1,xfrac,0,0x76543210,p11,0,0x76543210);

        // get data for second x interpolation
        auto p12 = (*pInB++).cast_to<float>();
        auto p22 = (*pInC++).cast_to<float>();

        // compute second x interpolation
        auto tempy2 = fpmac(p12,xfrac,0,0x76543210,p22,0,0x76543210);
        auto pxy2 = fpmsc(tempy2,xfrac,0,0x76543210,p12,0,0x76543210);

        // get data for y interpolation
        auto yfrac = (*pInA++).cast_to<float>();

        // compute y interpolation
        auto tempxy = fpmac(pxy1,yfrac,0,0x76543210,pxy2,0,0x76543210);
        auto pxy = fpmsc(tempxy,yfrac,0,0x76543210,pxy1,0,0x76543210);

        // write interpolated pixels to output
        *pOut++ = as_v8int32(pxy);
    }
}
```

## Running the Example

Running the example requires that you install and correctly configure both MATLAB and AMD Vitis™ tools. After downloading the files, cd into the ``.../11-Bilinear_Interpolation/aie/`` directory and use the make build process.

### Generating Test Vectors

Running AI Engine graph simulation requires test vectors to provide input. Files are also provided to compare with AI Engine simulator output for verification. To generate the vectors, run the command:

```bash
$ make gen_vectors
```

### Running x86 Simulation

To perform a functional x86 simulation, enter the following sequence of commands:

```bash
$ make x86com
$ make x86sim
$ make check_x86sim
```

The first command compiles graph code for simulation on an x86 processor. The second command runs the simulation. The final command invokes MATLAB to compare the simulator output with test vectors.

### Running AI Engine Simulation

To perform AI Engine emulation using the SystemC simulator, enter the following sequence of commands:

```bash
$ make aiecom
$ make aiesim
$ make check_aiesim
```

The first command compiles graph code for the SystemC simulator. The second command runs the simulation. The final command invokes MATLAB to compare simulation output with test vectors. To generate trace and profile data during simulation, use the sequence:

```bash
$ make aiecom
$ make profile
$ make check_aiesim
```

## Analyzing Results

### Vitis Analyzer

Vitis Analyzer is an essential tool for accessing information on compilation, simulation, and implementation of AI Engine graphs. Use the tool to obtain a summary on profiling data and to graphically display trace events. Launch the tool using the ``vitis_analyzer`` command, or for this example, by entering:

```bash
$ make analyze
```

The Graph view displays connectivity of the AI Engine graph. Figure 8 shows connectivity for this example. This simple example shows the kernel along with ping pong buffers associated with input and output ports.

![figure8](images/va_graph.png)

*Figure 8 - Vitis Analyzer Graph View*

The Array view displays how the AI Engine graph maps to the AI Engine array for the specified device. This example uses a VC1902 Versal AI Core device which contains 400 AI Engine tiles. As shown in Figure 9, this example uses one tile for kernel processing and two additional tiles for ping pong buffer and system memory. If you require more control over memory placement, specify design constraints.

![figure9](images/va_array.png)

*Figure 9 - Vitis Analyzer Array View*

Figure 10 contains information from the Profile view. The highlighted fields show that the bilinear interpolation kernel takes 537 cycles to process 256 pixels of data. For lowest speed Versal devices, this translate to a peak processing rate of ~476.7 MP/s. Highest speed devices have a peak processing rate of ~595.9 MP/s. Kernel computation can be further improved by coding to take advantage of more efficient software pipelining. This has shown that a rate of 667 MP/s is achievable.

![figure10](images/va_profile.png)

*Figure 10 - Vitis Analyzer Profile View*

Data bandwidth limits this specific example, so it does not achieve the rates mentioned. Figure 11 shows part of the Vitis Analyzer trace view. The cursors show that the time between the end of one kernel invocation to the end of the next is 484.0 ns. During this duration 256 pixels are processed, resulting in a rate of 528.9 MP/s.

![figure11](images/va_analyze.png)

*Figure 11 - Vitis Analyzer Trace View*

### Test Vector Comparison

When comparing simulation results with test vectors, a MATLAB script performs the processing. Figure 12 shows an example of a successful comparison.

![figure12](images/check_sim.png)

*Figure 12 - Simulation Verification*

The output provides three different indications of simulation performance.

The first is an indication of whether the simulation output matched the corresponding test vector. There is one comparison for each kernel simulated. The script compares `int32` values which represent floating-point interpolated pixel values. Because there can be slight variations in floating point calculations, the comparison enables for mismatch in the least significant mantissa bits of the floating-point number. You can specify this in the comparison script.

The second comparison indicates maximum pixel value difference between AI Engine simulation results and single precision MATLAB generated vectors. Pixels take on values in the range [0, 255], and this result provides the maximum of the differences between all pairs of corresponding pixels.

The final comparison indicates the maximum pixel value difference between AI Engine simulation results and double precision floating-point results generated by the MATLAB ``interp2`` function.

## Customizing the Example

You can customize this example to use different test images, use a specified output resolution, or to use multiple parallel processing cores for increased throughput. When generating test vectors, instead of using the Makefile, you can manually run scripts in MATLAB with the desired function arguments. For example, the equivalent to:

```bash
$ make gen_vectors
```

is to invoke MATLAB and run

```bash
>> image_transform
>> genvectors_bilinear_interp
```

### Specifying a Test Image and Output Resolution

The ``image_transform`` function uses file ``../images/epyc.jpg`` as a test image by default. You can specify a different file named ``image_file`` when invoking the function in MATLAB.

```bash
>> image_transform('image_file')
```

Default output resolution in pixels is 1024 $\times$ 1024. You can specify a different resolution of $x_{res} \times y_{res}$ by invoking:

```bash
>> image_transform('image_file', [ xres  yres ])
```

The ``image_transform`` function reads the test image file, performs a sequence of linear transformations on image pixel coordinates, and stores these transformations in a file named ``bli_coords.mat`` along with the test image pixel array. When this function executes in MATLAB, it displays a sequence of image transformations.

### Multicore Processing

The ``genvectors_bilinear_interp`` function generates test vector files assuming a single AI Engine kernel is used for processing. To use multiple kernels in parallel, invoke the function as:

```bash
>> genvectors_bilinear_interp(N)
```

where, N is the number of desired kernels. This function extracts one of the coordinate transformations from file ``bli_coords.mat``, performs bilinear interpolation on the pixels, and creates input and output test vectors for AI Engine simulation. The function also generates a file named ``config.h`` used by the AI Engine compiler and simulator. This specifies the number of kernels and number of kernel invocations required to process the entire test image. This function also performs bilinear interpolation in double precision using the MATLAB ``interp2`` function and stores the result in file ``golden_ref.mat``.

As an example, specifying four kernels results in the MATLAB Console Window displaying something like:

```
>> genvectors_bilinear_interp(4)
Number of output pixels interpolated = 453221 out of 1048576 (43.22252 %)
Maximum pixel error is 0.000052940
Mean square pixel error is 0.000000000
```

along with the image shown in Figure 13.

![figure13](images/gen_vec.png)

*Figure 13 - MATLAB genvectors_bilinear_interp Output*

After these MATLAB scripts run, the rest of the AI Engine build and simulation process proceeds in the same manner. The following figure shows how four kernels are assigned to tiles in the AI Engine array.

![figure14](images/va_array_4.png)

*Figure 14 - Multicore Kernel Placement in AI Engine Array*

Figure 15 shows the result of comparing multicore AI Engine simulation output with test vectors. Based on profile results, four kernels support peak processing rates in the range of approximately 1.7 to 2.1 GP/s. This depends on device speed grade.

![figure15](images/check_sim_4.png)

*Figure 15 - Multicore Simulation Verification*

## References

[1]:<https://en.wikipedia.org/wiki/Bilinear_interpolation> "Bilinear interpolation"

[[1]]: Bilinear interpolation. May 17 2023, 14:05 UTC. In Wikipedia: The Free Encyclopedia. Wikimedia Foundation Inc. Encyclopedia online. Available from https://en.wikipedia.org/wiki/Bilinear_interpolation. Internet. Retrieved August 4 2023.

[2]: <https://ieeexplore.ieee.org/document/1409828> "A novel approach to real-time bilinear interpolation"
[[2]]: K. T. Gribbon and D. G. Bailey, "A novel approach to real-time bilinear interpolation," Proceedings. DELTA 2004. Second IEEE International Workshop on Electronic Design, Test and Applications, Perth, WA, Australia, 2004, pp. 126-131, doi: 10.1109/DELTA.2004.10055.

[3]: <https://en.thedailyscoup.news/2023/03/05/interpolation-formula-understanding-the-basics-and-applications/> "Interpolation Formula: Understanding the Basics and Applications"

[[3]] Chen, J. (2023, March 5). *Interpolation Formula: Understanding the Basics and Applications.* The Daily Scoup News. https://en.thedailyscoup.news/2023/03/05/interpolation-formula-understanding-the-basics-and-applications/

[4]: <https://ieeexplore.ieee.org/document/9943113> "Super-Resolution of Digital Elevation Model with Local Implicit Function Representation"

[[4]] P. He et al., "Super-Resolution of Digital Elevation Model with Local Implicit Function Representation," 2022 International Conference on Machine Learning and Intelligent Systems Engineering (MLISE), Guangzhou, China, 2022, pp. 111-116, doi: 10.1109/MLISE57402.2022.00030.

[5]: <https://bartwronski.com/2020/04/14/bilinear-texture-filtering-artifacts-alternatives-and-frequency-domain-analysis/> "Bilinear texture filtering–artifacts, alternatives, and frequency domain analysis"

[[5]] Wronski, B. (2020, April 14). *Bilinear texture filtering–artifacts, alternatives, and frequency domain analysis.* https://bartwronski.com/2020/04/14/bilinear-texture-filtering-artifacts-alternatives-and-frequency-domain-analysis/

## Support

GitHub issues are used to track requests and bugs. For questions, go to [support.xilinx.com](http://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
