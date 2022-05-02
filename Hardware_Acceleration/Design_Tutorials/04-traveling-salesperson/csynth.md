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


> **In this section...**   
Perform C synthesis to create an RTL representation of the design and discuss the compiler hints (pragmas) used... 


## Run Vitis HLS C Synthesis
In the previous phase, the C simulation, the code execution was purely C based and although special libraries are used for streams, no RTL was yet involved.
In C synthesis, the top function `tsp` is analyzed and compiled based on the hints (called pragma or directives) passed to the HLS compiler.  Once the operations are scheduled and mapped onto hardware constructs, the final code is generated in RTL (with both Verilog and VHDL).

The code uses 3 of these "hints":
- `PIPELINE`: Requests execution of the main loop (labeled `loop_compute`) at each clock cycle as specified by the `II=1` option
- `INLINE`: Dissolves a sub-function for better optimization results.  This is used for the `compute` function
- `INTERFACE`: Specifies a protocol for a given top function argument.  This is optional and only to demonstrate how AXI-Stream can be added to an HLS stream
- `BIND_STORAGE`: Assigns an array to a specific type of on-chip memory.  Here the `distance` array is mapped onto a RAM with 1 write port and multiple read ports to allow simultaneous access to multiple city to city distances and calculate the full route quicker.

Before running synthesis open the `tsp.h` file and set the number to 13 (`N=13`). You can open the `tsp.h` file in the Vitis HLS GUI from the `tsp.cpp` file by holding the mouse over the `#include "tsp.h"` line and press `CTRL` key while clicking the mouse.

To run HLS synthesis from the GUI:  
Use the same shortcut as we used for C simulation earlier and select 'Run C Synthesis' or...  
Via the main menu, `Project` -> `Run C Synthesis` - > `C Synthesis`

Once synthesis has completed, the main window shows "Performance and Resource Estimates":  
(collapse the "General Information" and "Timing Estimates" sections by clicking on their title to make more room if necessary)   
<img src="./images/synthesis.png" alt="synthesis" title="synthesis" width="800" height="150">  
This "Performance and Resource Estimates" section shows a table in which we see the main function `tsp` and the main loops and since we gave them a label in the source code it's easy to know which they are.  
- The full latency for the `tsp` function is 479,001,957 (close to half a billion clock cycles) mainly contributed by `loop_compute`
- That latency of 479,001,600 is exactly factorial 12 (12!) which corresponds to the the scenario with 13 cities (N=13) given that the first city (the route starting point) is fixed, so the permutations are applied on the 12 remaining cities.

## Next

* [Run the RTL/C cosimulation](./cosim.md)
