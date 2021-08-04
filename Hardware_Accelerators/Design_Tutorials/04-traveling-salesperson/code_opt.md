
> **NOTE:**   
In this section, the **travelling salesman problem** (TSP) design is implemented with 4 parallel memory lookups to fetch distances.

## Load the project into Vitis HLS
Open a terminal and navigate to the `build` directory. Launch the following command which will open the graphical interface of Vitis HLS and configure the project based on the settings included in the hls_opt.tcl Tcl file:

```console
user@server:~$ cd ./build
user@server:~$ vitis_hls -p hls_opt.tcl &
```
Once the tool comes up, on the left-hand side, locate the *Explorer* pane, expand `proj->Source` and double-click on `tsp_opt.cpp` to inspect the source code.
The testbench is found below in `proj->TestBench` in the file is `tsp_TB.cpp`.

## Review Code Changes

We now have 4 arrays for the distances:
```cpp
uint16_t distances_0[N][N];
uint16_t distances_1[N][N];
uint16_t distances_2[N][N];
uint16_t distances_3[N][N];
```
The incoming distance data points are still read one at a time but copied in all 4 memories:
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
Final determination of shortest distance:
```cpp
    // Determine shortest between the 4 candidates
    shortestDistance = std::min({ candidate0, candidate1,
                                  candidate2, candidate3 });
```
## Running Csim, Csynth and Cosim

First we run C simulation making sure the number of cities is small for fast runtime (e.g. N=4) to confirm the design works as expected.  
Then change N to 13 for C synthesis to compare with original design.  
This is how the C synthesis report in the Vitis HLS GUI looks like (the **Performance and Resource Estimates** section):  
<img src="./images/synthesis2.png" alt="synthesis" title="synthesis" width="900" height="200" />  

We notice that:
- The latency for the `tsp` function is now less than a second
- The loop distance trip count is unchanged as expected since the input data is same
- The tripcount for the main loop (`loop_compute`) is now a fourth of factorial 12 (12!/4) thanks to the parallel execution of the `compute` function
- The new `Loop 3` originates from the final std::min call that returns the smallest of the 4 results obtained
