<!--
/*
 * Copyright 2020-2022 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */ -->

<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>


> **NOTE:**   
In this section, the **traveling salesperson problem** (TSP) design is implemented with 4 parallel memory lookups to fetch distances.

## Load the project into Vitis HLS
Open a terminal and navigate to the `build` directory. Launch the following command which will open the graphical interface of Vitis HLS and configure the project based on the settings included in the hls_opt.tcl Tcl file:

```console
user@server:~$ cd ./build
user@server:~$ vitis_hls -p hls_opt.tcl &
```
Once the tool comes up, on the left-hand side, locate the *Explorer* pane, expand `proj->Source` and double-click on `tsp_opt.cpp` to inspect the source code.
The testbench is found below in `proj->TestBench` in the file is `tsp_TB.cpp`.

## Review Code Changes

There are now 4 arrays for the distances:
```cpp
uint16_t distances_0[N][N];
uint16_t distances_1[N][N];
uint16_t distances_2[N][N];
uint16_t distances_3[N][N];
```
The incoming distance data points are still read one at a time, but they are copied into all 4 memories:
```cpp
loop_distances: for (int i = 0; i < N*N; ++i)
  {
    uint16_t val;
    streamDistances >> val;
    distances_0[i/N][i%N] = val;
    distances_1[i/N][i%N] = val;
    distances_2[i/N][i%N] = val;
    distances_3[i/N][i%N] = val;
  }
```
The `loop_compute` main loop continuously increments by 4 and distributes the 4 values to copies of the `compute` function.  Each evaluates a route:
```cpp
loop_compute: for( unsigned long int i_ = 0; i_ < factorialN; i_ += 4 )
  {
    #pragma HLS pipeline II=1
    candidate0 = std::min(candidate0, compute(i_+0, distances_0));
    candidate1 = std::min(candidate1, compute(i_+1, distances_1));
    candidate2 = std::min(candidate2, compute(i_+2, distances_2));
    candidate3 = std::min(candidate3, compute(i_+3, distances_3));
  }
```
Final determination of the shortest distance:
```cpp
    // Determine shortest between the 4 candidates
    shortestDistance = std::min({ candidate0, candidate1,
                                  candidate2, candidate3 });
```
## Running C-simulation and C-synthesis

First you will run C simulation to confirm the optimized design works as expected. Edit the `tsp.h` file to make sure the number of cities is small (`N=5`) for faster simulation run times, and **Run C Simulation**.  

After reviewing the results of simulation, increase the number of cities again (`N=13`) in the `tsp.h` file for C synthesis so that you can compare results with the original design. **Run C Synthesis**.

The following figure shows the C synthesis report in the Vitis HLS GUI (the **Performance and Resource Estimates** section):  
<img src="./images/synthesis2.png" alt="synthesis" title="synthesis" width="900" height="200" />  

You will notice that:
- The latency for the `tsp` function is now less than a second
- The loop distance trip count is unchanged as expected since the input data is same
- The tripcount for the main loop (`loop_compute`) is now a fourth of factorial 12 (12!/4) thanks to the parallel execution of the `compute` function
- The new `Loop 3` originates from the final std::min call that returns the smallest of the 4 results obtained
