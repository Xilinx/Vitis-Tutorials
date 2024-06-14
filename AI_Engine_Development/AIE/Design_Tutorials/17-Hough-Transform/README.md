<!--
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
Author: Mark Rollins
-->
﻿<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# System Partitioning of a Hough Transform on AI Engine

***Version: Vitis 2024.1***

## Table of Contents

1. [Introduction](#introduction)
2. [Hough Transform Matlab Model](#hough-transform-matlab-model)
3. [System Partitioning](#system-partitioning)
4. [Conclusions](#conclusions)

[References](#references)

[Support](#support)

[License](#license)

## Introduction

This tutorial walks through the process of planning the implementation of a well-known image processing algorithm, mapping and partitioning it to the resources available in a Versal Adaptive SoC device. The goal is to partition the compute workloads of the application across the heterogeneous compute domains of the device, namely the Processor Subsystem (PS), the Programmable Logic (PL) and the AI Engine (AIE). We then identify a "data flow" through the device to pass data between storage locations and these compute domains. This requires analysis of the Compute, Storage, and Bandwidth requirements of the system algorithms and aligning those to the available device resources to discover a workable system solution. We call this "System Partitioning", and we will illustrate this process using the well-known Hough Transform.

### What is the Hough Transform?

The Hough Transform is a feature extraction technique for computer vision & image processing. It was invented in 1959 to detect lines in the machine analysis of monochromatic bubble chamber photographs, was patented in 1962, and was popularized & extended in support from lines to other objects by Duda & Hart in 1972 [1]. We only consider the line detection style of Hough Transform in this tutorial. 

The Hough Transform detects lines in an image using a parameteric representation by transforming the line in 2D from its normal $(x,y)$ coordinates into a new $(\rho,\theta)$ domain where $\rho$ represents the line drawn from the origin to where it meets the line at a 90 degree angle, and $\theta$ identifies the angle that perpendicular line makes to the x-axis. This is shown in the diagram below. Notice how all points on the red line have the same $(\rho,\theta)$ values. Consequently, if every pixel in the image is associated with a $(\rho,\theta)$ pair, then lines in the original image may be identified in the $(\rho,\theta)$ plane by observing the relative occurance of these pairs -- via a histogram of the $(\rho,\theta)$ data. These histogram statistics are collected by the Hough Transform in the $(\rho,\theta)$ plane to identify lines in the original image. 

![figure](images/parametric_line_format.png)

### What is System Partitioning?

We use System Partitioning to architect an embedded system for hetergeneous compute by partitioning the application workload across the various compute elements in the device. Once partitioned, a workable "data flow" identifies the path taken between subsystems in the device as data moves between storage and compute resources. Along the way, I/O bandwidth is managed carefully within interface limits. Data storage locations are identified based on suitability of interface bandwidths, storage depths, and compute requirements. In the end, we identify a workable system solution that eliminates risk from the device selection. This is all accomplished using a combination of analysis and prototyping work without implementing the full design. This solution scope is outlined in the diagram below.

![figure](images/what-is-system-partitioning.png)

### System Partitioning Methodology

We define the following methodology to steer our system partitioning analysis activities:
1. Requirements Gathering -- an analysis step to compare system/algorithm requirements against device resources
2. Solution Synthesis -- a conceptual step to identify possible solutions based on specific partitions and their resulting data flows
3. Partitioning Validation -- a  feasibility assessment driven by detailed analysis & prototyping to identify shortcomings and reduce risk
4. Iterate to System Feasibility -- revise, rework, and re-envision until a suitable low-risk solution is identified

Some aspects of each phase is outlined in the diagram below.

![figure](images/system-partitioning-methodology.png)

### Hough Transform Matlab Model

A proper algorithm model is required for system partitioning. We start with the Matlab model shown below. A detailed study of this model identifies key aspects of the system design that impact its solution. For example:

* The overall compute load complexity is driven by the image size through $R$ and $C$ dimensions.
* The resolution adopted for $\theta$ through the `theta_res` parameter drives complexity, bandwidth, and histogram storage.
* The algorithm exhibits a "dual-nested for-loop" character.
* The algorithm employs lookup tables through `cos_theta` and `sin_theta`
* The algorithm has been quantized to use `int16` data types.
* There are multiple compute workloads: for `rho_i`, `addr` and histogram update `H`

```
function [H,theta,rho,rho_calc] = hough_model( BW, theta_res )
   if     (nargin == 1) theta_res = 180; 
   elseif (nargin ~= 2) error('hough_model(BW,theta_res)'); end
   [R,C] = size(BW);
   rho_max = ceil(sqrt((R-1)^2 + (C-1)^2));
   fprintf(1,'rho_max: %g\n',rho_max);
   % Initialize:
   tmp = linspace(-90,90,1+theta_res);
   theta = tmp(1:end-1);
   cos_theta = double(fi(cos(theta*pi/180),1,16,15,'RoundingMethod','Round','OverflowAction','Saturate'));
   sin_theta = double(fi(sin(theta*pi/180),1,16,15,'RoundingMethod','Round','OverflowAction','Saturate'));
   rho = [-rho_max:1:rho_max];
   Nt = numel(theta);
   Nr = numel(rho);
   H = zeros(Nr,Nt); 
   rho_calc = zeros(R,C,Nt);
   % Compute transform:
   for yy = 0 : R-1
     for xx = 0 : C-1
       % Compute the 'rho' value:
       rho_i = double(fi(xx*cos_theta + yy*sin_theta,...
                         1,16,0,'RoundingMethod','Round','OverflowAction','Saturate'));
       rho_calc(1+yy,1+xx,:) = rho_i;
       % Add offset to turn onto 'rho' address:
       addr = rho_i + rho_max;
       for ii = 1 : Nt
         H(1+addr(ii),ii) = H(1+addr(ii),ii) + BW(1+yy,1+xx);
     end % yy
   end %xx
end
```

The Matlab model may be run and its performance compared to the built-in Matlab function `hough` found in the Image Processing Toolbox. Here, we run with a $216\times 240$ image of the AMD Instinct and show "heat maps" of the 2D Hough Transform output histograms for both the Matlab function and our AMD 16-bit Matlab model. 

![figure](images/hough_model.png)

## System Partitioning

This section illustrates the details of system partitioning for the Hough Transform. We consider the ultimate system goals, techniques to parallelize the algorithm over multiple AI Engine tiles, analyzing storage, compute, and bandwidth and their impacts on the partitioning choices. A spreadsheet analysis steers us to some early conclusions on what may be feasible, but some detailed prototyping work is required to refine these estimates to finally identify a more accurate scoping of how many AI Engine resources are required to achieve the design objectives. 

### Goals

Our goal here for this tutorial is to identify the "best we can do" using only AI Engine resources to implement the Hough Transform. To this end, we set a target throughput requirement of 220 Mpixel/sec (or Mpps) and pose the question: "how many AI Engine tiles are required"? From our Matlab model above, we know the image size and $\theta$ resolution will be key parameters driving compute, bandwidth, and storage. With this in mind, we brainstorm solutions for how we might parallelize the Hough Transform algorithm across multiple AI Engine tiles.

### Parallelizing Over "Image Tiles"

One obvious way to increase throughput is to parallelize the image over AI Engine tiles directly such that each tile sees only a small portion of the original image. This is shown in the diagram below. In this way, each tile computes a full Hough transform for the portion of the image that it sees. This yields a linear reduction in its compute workload and a similar reduction in the input bandwidth delivered to each tile. There is no reduction in tile memory for histogram storage. One consequence of this approach is that we must combine the histogram outputs from each tile, resulting in an additional compute workload to combine together all tile results.

![figure](images/parallelize-over-tiles.png)

### Parallelizing Over "Theta"

An alternative strategy partitions the Hough Transform such that each tile sees the full image but computes only a partial transform over a subset of the $\theta$ range. This also leads to a linear reduction in compute, but does not achieve any input bandwidth reduction. This scheme benefits from a linear reduction in the tile histogram storage. We must "collect" the histogram outputs from each tile, but there is no extra compute workload involved with this approach.

![figure](images/parallelize-over-theta.png)

### Analyzing Storage Requirements

Having identified some possible parallelization schemes, we next dive into the Requirements Gathering phase of System Partitioning to analyze storage requirements. Details of this analysis are outlined in the diagram below. Following the "Parallelizing over Theta" strategy, we find a single full histogram cannot fit into the 32 KB local tile memory. We must partition over multiple tiles to make storage feasible. We find the total histogram storage for our image size is 162 KB, and we require at least 8 tiles to reduce this to manageable levels.

![figure](images/analyzing-storage.png)

### Analyzing Compute Requirements

Next, we use a spreadsheet analysis to assess compute requirements. We load the system input parameters on the left side of the spreadsheet shown below and we analyze compute parameters on the right side. We find it useful to tabulate the numbers of processor cycles required by each loop body in our original Matlab model of the Hough Transform. Based on the AI Engine compute capacity of 32 MACs/cycle for `int16` data types, we realize we can process 2 MACs/pixel per $\theta$ value in real time. Based on these vector estimates, the spreadsheet tells us we must process 5.7 cycles per pixel to meet our 220 Mpps throughput objective. This is equivalent to 45 cycles for the vector processor with its 8 lanes SIMD execution. Our compute bound for the vector processor is very high at 5000 Mpps. However, assuming an 8-cycle "read-modify-write" instruction for updating the histogram tables in the third compute workload, we find our throughput limited by the scalar processor to 39 Mpps if using 32 tiles. When projected to more tiles, we fail to reach our 220 Mpps target with even 128 tiles. 

![figure](images/analyzing-compute.png)

### Analyzing I/O Bandwidth Requirements

The bandwidth analysis for this design is straightforward. The input bandwidth is satisfied with a single PLIO. The output histogram size per tile is small, and we have not established an output bandwidth nor latency target. The computed frame duration is 235 us assuming our given image size and 220 Mpps target throughput. A single output PLIO stream can transfer the full histogram result in 1 us -- this is less than 1% of our image frame and seems reasonable.

![figure](images/analyzing-bandwidth.png)

### SIMD / Vectorization

An important aspect of System Partitioning for AI Engine is to consider how the SIMD vector data path may be leveraged for high performance compute. This usually involves investigating strategies for "vectorization" or assigning signal samples to lanes. For the $\rho$ computation of the Hough Transform, a workable scheme involves using `mac16()` intrinsics to process four pixels at a time. 

* In one vector register, we load four copies of four different $\theta$ values to allows us to process four $\theta$ values for four different pixels in a single instruction. This produces sixteen histogram outputs per cycle and we schedule two such computes per loop body.
* In a second vector register, we load four $(x,y)$ pixel values into lanes aligned properly to their $\theta$ counterparts in the other register. 

### Solution Synthesis

Based on our early spreadsheet analysis, we anticipate a 32-tile AI Engine design may be limited to ~39 Mpps throughput due to the read-modify-write updates of the histogram counts on the scalar processor. It is difficult to nail down more accurately a means to achieving our 220 Mpps throughput objective from this early analysis. We need to do some accurate prototyping work on a proposed solution to validate assumptions and obtain more accurate performance projections.

Based on the early spreadsheet analysis work, we define a Solution Proposal as follows:
* Assume a 32-tile solution where each tile computes four of the 128 $\theta$ values
* Each tile uses local tile memory for storage of $\cos$ and $\sin$ LUTs
* We use the `mac16()` vectorization outlined above operating at four pixels per cycle
* We expect a 5.1 KB histogram LUT in each tile as predicted from storage analysis above

From early spreadsheet work, we anticipate a throughput limited to ~39 Mpps and we target II=45 for our vectorized compute, but expect performance to be limited by the histogram updates. We now dive in and code this early prototype to validate and accurately quantify these assumptions.

The diagram below shows the AI Engine graph view for a single tile of this prototype design. All 32 tiles are identical. The floor plan view of the composite design is also shown. This design was profiled on the given image to tabulate accurately the its throughput performance and to obtain the cycle count performance of each function.

![figure](images/prototype-aie-design.png)

### Partitioning Validation

Coding up the Hough Transform prototype yields additional insight into the design and a deeper understanding of its performance limitations. Indeed, as predicting, the histogram update code (shown below) illustrates the exact "read-modify-write" form anticipated from the start. It is difficult to identify any means to vectorize it and remove this performance bottleneck. 

```
template <int COUNT_NUM>
  inline void update_countsA( TT_COUNT (&COUNTS)[COUNT_NUM],
                              aie::vector<TT_COUNT,16>& rho, aie::vector<TT_COUNT,8>& pixels )
  {
    COUNTS[rho[ 0]] += pixels[0];
    COUNTS[rho[ 1]] += pixels[0];
    COUNTS[rho[ 2]] += pixels[0];
    COUNTS[rho[ 3]] += pixels[0];
    COUNTS[rho[ 4]] += pixels[1];
    COUNTS[rho[ 5]] += pixels[1];
    COUNTS[rho[ 6]] += pixels[1];
    COUNTS[rho[ 7]] += pixels[1];
    COUNTS[rho[ 8]] += pixels[2];
    COUNTS[rho[ 9]] += pixels[2];
    COUNTS[rho[10]] += pixels[2];
    COUNTS[rho[11]] += pixels[2];
    COUNTS[rho[12]] += pixels[3];
    COUNTS[rho[13]] += pixels[3];
    COUNTS[rho[14]] += pixels[3];
    COUNTS[rho[15]] += pixels[3];
  }
```

The diagram below tabulates the profiling data generated by the compiler for the 32-tile Hough Transform prototype design when run on the $216\times 240$ image. The important conclusions are quite clear:
* The `update_countsA/B()` routines require ~183 cycles each representing ~90% of the total cycles
* The `theta_compute()` routine requires ~277,204 cycles representing ~10% of the total cycles, and equivalent to ~42 cycles per II (very close to our original spreadsheet estimate of 45)
* The overall throughput is $216\times 240/(0.8\times2,650,436) = ~24$ Mpps

From detailed prototyping, we have now quantified accurately the throughput performance of the Hough Transform, and these results may be used to revise our original spreadsheet estimates to produce accurate projections of how we might achieve our 220 Mpps throughput target. 

![figure](images/partitioning-validation.png)

### Iterating to System Feasibility

Now having an accurate cost model of the Hough Transform from prototyping, we are in a position to explore alternate solutions via scaling. Here, we consider "Image Partitioning" in addition to "Theta Partitioning" as a means to scale up the throughput. In this scheme, we partition out portions of the images to 32-tile clusters, where each cluster is computing partial histograms for four $\theta$ values (as in our prototype). Using this approach, we are able to scale throughput linearly higher. As shown in the diagram below, we can achieve ~ 220 Mpps with ~275 tiles.

![figure](images/system-feasibility.png)

## Conclusions

This tutorial uses the Hough Transform as a working example to illustrate the concepts of System Partitioning, and how this methodology may be used to scope new AI Engine designs and to more generally partition hetergeneous compute workloads across the resources available in Versal Adaptive SoCs. Through requirements gathering, solution synthesis, prototype validation, and iterative refinement, system applications may be successfully partitioned to these devices using common analysis tools such as spreadsheets and targeted prototyping & profiling. 

## References

[1]: [Use of the Hough Transformation to Detect Lines and Curves in Pictures](https://dl.acm.org/doi/10.1145/361237.361242)

## Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](http://support.xilinx.com/).

## License

Components: xilinx-images

images in the documentation

Components: xilinx-files

The MIT License (MIT)

Copyright (c) 2024 Advanced Micro Devices, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

<p class="sphinxhide" align="center">  &copy; Copyright 2024 Advanced Micro Devices, Inc.</p>
<p class="sphinxhide" align="center">  &copy; Copyright 2021 Xilinx Inc.</p>

