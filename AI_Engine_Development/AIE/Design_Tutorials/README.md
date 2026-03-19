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

# Design Tutorials

The AI Engine development design tutorials showcase the two major phases of AI Engine application development: designing the application and developing the kernels. These tutorials demonstrate both phases.

The **[README](../README.md)** of AI Engine development contains important information including tool version, environment settings, and a table describing the platform, operating system, and supported features or flows of each tutorial. AMD recommends that you review details before starting to use the AIE tutorials.

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
<tr>
 <td align="center"><a href="../../../Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System/">Versal Custom Thin Platform Extensible System</a></td>
 <td>This is an AMD Versal™ system example design based on a VCK190 thin custom platform (minimal clocks and AXI exposed to PL) that includes HLS/RTL kernels and AI Engine kernel using a full Makefile build-flow.</td>
 </tr>
 <tr>
 <td align="center"><a href="./01-aie_lenet_tutorial/">LeNet Tutorial</a></td>
 <td>This tutorial implements a system-level design to perform image classification using the LeNet algorithm on the AI Engine and PL logic, including block RAM. The design demonstrates functional partitioning between the AI Engine and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (block RAM) and AI Engine memory.</td>
 </tr>
 <tr>
 <td align="center"><a href="./02-super_sampling_rate_fir/">Super Sampling Rate FIR Filters</a></td>
 <td>This tutorial provides a methodology to enable you to make appropriate choices depending on the filter characteristics. It also provides examples on how to implement Super Sampling Rate (SSR) FIR Filters on a Versal™ adaptive SoC AI Engine processor array.</td>
 </tr>
 <tr>
 <td align="center"><a href="./03-beamforming/">Beamforming Design</a></td>
 <td>This tutorial implements a beamforming system running on the AI Engine, PL, and PS, and validates the design running on this heterogeneous domain.</td>
 </tr>
 <tr>
 <td align="center"><a href="./04-Polyphase-Channelizer/">Polyphase Channelizer</a></td>
 <td>This tutorial implements a system-level design (such as Polyphase Channelizer) using a combination of AI Engine and PL/HLS kernels.</td>
 </tr>
 <tr> 
 <td align="center"><a href="./05-Prime-Factor-FFT/">Prime Factor FFT-1008</a></td>
 <td>This Versal system example implements a 1008-pt FFT using the Prime Factor Algorithm. The design uses both AI Engine and PL kernels working cooperatively. AI Engine elements are hand-coded using AIE API. PL elements use Vitis HLS. The new v++ Unified Command Line flow manages system integration in the Vitis platform. </td>
 </tr> 
 <tr>
 <td align="center"><a href="./06-fft2d_AIEvsHLS/">2D-FFT</a></td>
 <td>This tutorial performs two implementations of a system-level design (2D-FFT): one with AI Engine, and the other with HLS using the DSP Engines. </td>
 </tr>
 <tr>
 <td align="center"><a href="./07-firFilter_AIEvsHLS/">FIR Filter</a></td>
 <td>This tutorial implements a system-level design (FIR Filter) using AI Engines and HLS with DSP Engines. It uses the Versal device plus PL resources including lookup tables, flip-flops, and block RAMs.</td>
 </tr> 
 <tr>
 <td align="center"><a href="./08-n-body-simulator/">N-Body Simulator</a></td>
 <td>It is a system-level design that uses the AI Engine, PL, and PS resources to showcase the following features:<li>A Python model of an N-Body Simulator run on x86 machine
   <li>A scalable AI Engine design that can use up to 400 AI Engine tiles
   <li>AI Engine packet switching
   <li>AI Engine single-precision floating point calculations
   <li>AI Engine 1:400 broadcast streams
   <li>Codeless PL HLS datamover kernels from the Vitis™ Utility Library
   <li>PL HLS packet switching kernels
   <li>PS Host Application that validates the data coming out of the AI Engine design
   <li>C++ model of an N-Body Simulator
   <li>Performance comparisons between Python x86, C++ Arm A72, and AI Engine N-Body Simulators
   <li>Effective throughput calculation (GFLOPS) vs. Theoretical peak throughput of AI Engine
 </tr>
 <tr>
 <td align="center"><a href="./09-ddc_chain/">Digital Down-conversion Chain</a></td>
 <td>This tutorial demonstrates the steps to upgrade a 32-branch digital down-conversion chain (XAPP1351) to the latest recommended tools and coding practice, including conversion of most AI Engine Intrinsics to APIs. The upgraded AIE API version achieves the same throughput performance as the original code base, while being easier to read and maintain.</td>
 </tr>
 <tr>
 <td align="center"><a href="./10-GeMM_AIEvsDSP/">Versal GeMM Implementation</a></td>
 <td>This tutorial performs two implementations of a system-level design: one with AI Engine, and the other with RTL using the DSP Engines. In each implementation, the tutorial takes you through the hardware emulation and hardware flow in the context of a complete Versal adaptive SoC system design.</td>
 </tr>
 <tr>
 <td align="center"><a href="./11-Bilinear_Interpolation/">Bilinear Interpolation</a></td>
 <td>This tutorial implements a bilinear interpolation algorithm using AI Engines. It also provides guidance for customizing the design to function with varying image resolutions, and to take advantage of multicore processing on the AI Engine array to achieve desired throughput.</td>
 </tr>
 <tr>
 <td align="center"><a href="./12-IFFT64K-2D/">64K IFFT Using 2D Architecture</a></td>
 <td>This Versal system example implements a 64K-pt IFFT using a 2D architecture. It decomposes 64K = 256 x 256 and builds the transform in two dimensions using row and column FFT-256. A matrix transpose is performed in between in the PL. This alternative "divide and conquer" approach is attractive in the SSR > 1 regime.</td>
 </tr>
 <tr>
 <td align="center"><a href="./13-FFT-DFT-on-AIE/">Implementing FFT and DFT Designs on AI Engines</a></td>
 <td>This tutorial implements several techniques for mapping FFT and DFT algorithms to the AI Engine array. These include the Stockham FFT used in AMD Vitis DSPlib, hand-coded variants using the AI Engine API, and a direct form DFT using vector-matrix multiplication. It also shows how to trade off AI engine tile resource vs. throughput performance of the Stockham FFT in DSPlib using its <code>TP_CASC_LEN</code> and <code>TP_PARALLEL_POWER</code> template parameters. This is useful when configuring DSPlib FFT library instances to serve as part of a larger 2D FFT architecture.</td>
 </tr>
 <tr>
 <td align="center"><a href="./14-Bitonic-Sorting/">Bitonic SIMD Sorting on AI Engine for float Datatypes</a></td>
 <td>This tutorial implements a Bitonic SIMD sorter on AI Engine in Versal for float data types. Two examples are given. First, a small example using N=16 demonstrates the concept and identifies strategies for vectorization and management of the vector register space. These ideas are then applied to a second larger example using N=1024. Profiling and throughput performance are compared to `std::sort()`.</td>
 </tr>
 <tr>
 <td align="center"><a href="./15-farrow_filter/">Fractional Delay Farrow Filter</a></td>
 <td>This Versal system example implements a variable fractional delay algorithm using the Farrow Filter structure. It explains common AI Engine design optimization techniques. The design uses both AI Engine and PL kernels working cooperatively. AI Engine elements are hand-coded using AIE API. PL elements use Vitis HLS. The new v++ Unified Command Line flow manages system integration in the Vitis platform.</td>
 </tr>
 <tr>
 <td align="center"><a href="./16-1M-Point-FFT-32Gsps/">1 Million Point float FFT @ 32 GSPS on AI Engine</a></td>
 <td>This tutorial implements a 1M-point FFT for `cfloat` data types that achieves an impressive throughput rate exceeding 32 GSPS using a large portion of the AI Engine array for compute and PL URAM resources to implement a matrix transpose operation.</td>
 </tr>
 <tr>
 <td align="center"><a href="./17-Hough-Transform/">System Partitioning of a Hough Transform on AI Engine</a></td>
 <td>This tutorial explains the process of planning the implementation of a well-known image processing algorithm, mapping and partitioning it to the resources available in a Versal Adaptive SoC device. It shows this using the Hough Transform, a feature extraction technique for computer vision and image processing.</td>
 </tr>
 <tr>
 <td align="center"><a href="./18-MUSIC-Algorithm/">MUSIC Algorithm on AI Engine</a></td>
 <td>This tutorial implements the Multiple Signal Classification (MUSIC) Algorithm on the AI Engine. MUSIC is a popular algorithm for Direction of Arrival (DOA) estimation in antenna array systems. </td>
 </tr>  
 <tr>
 <td align="center"><a href="./19-Softmax-Function/">Softmax Function on AI Engine</a></td>
 <td>The softmax function is an activation function often used in the output layer of a neural network designed for multi-class classification. This tutorial provides an example of how to implement the softmax function to create custom machine learning inference applications on AI Engines. </td>
 </tr>
 <tr>
 <td align="center"><a href="./20-TDM-Mixer/">Time-Division Multiplexed Mixer Example</a></td>
 <td>This tutorial implements a time-division multiplexed (TDM) Mixer design on AI Engine. The design shows how to perform a "corner-turning" operation using the DMA hardware resources inside the AI Engine local tile, leaving core capacity available for compute workloads. The tutorial also shows how to vectorize workloads involving phase or frequency generation without lookup tables.</td>
 </tr>
 <tr>
 <td align="center"><a href="./21-Back-Projection-SAR/">Back-Projection Synthetic Aperture Radar on AI Engine</a></td>
 <td>This tutorial builds an example design for Synthetic Aperture Radar using Vitis Libraries and custom API coding for use with the GOTCHA data set. The design achieves ~2.5 frames per second for 512 x 512 images and 586 radar pulses with fewer than 32 tiles. A large design with eight engine instances achieves close to 20 frames per second.</td>
 </tr>
 </table>

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
