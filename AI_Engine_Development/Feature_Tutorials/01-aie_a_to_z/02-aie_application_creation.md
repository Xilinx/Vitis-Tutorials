﻿<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Introduction

In this section of the tutorial, you will learn how to create a new AI Engine application project from a template and run through the ***Emulation-AIE*** process.

### Step 1. Create a new AI Engine Application Project

1. Open the Vitis IDE if you have closed it. You can use the same workspace as the previous stage or create a new one.

2. On the Welcome Page, click on ***Create Application Project*** or click ***File → New →  Application Project***.

3. On the platform page, select the ***base_pfm_vck190*** platform you just created in last step. If you do not use the same workspace from last step, click on ***Add*** and select the folder `base_pfm_vck190/export` to add the platform into this workspace.

4. Set the application name as ***simple_application*** and select ***ai_engine*** for the processor.

      ![missing image](images/aie_app_pg1.png)

5. Keep the default setting for the next page and click ***Next***.

6. In the Template page, select the ***Simple*** template. Click ***Finish*** to create the application project.

      ![missing image](images/aie_app_pg2.png)

>**Note**: In the description of the template it says that the template works only for AI Engine Emulation and SW (x86) simulation. In the following steps you will learn how to make it work on hardware.  

7. The template imports two folders:

* `src` contains the sources for the kernels and the graph.
* `data` contains the data for the simulation input (`input.txt`) and the golden reference for the output (`golden.txt`).

    ![missing image](images/aie_app_folder.png)

8. Open the file `project.h` to see the graph.
You can see that the graph (simpleGraph) has one input and one output and implements two kernels with the same function. The output of the first kernel feeds the second one.

```
first = kernel::create(simple);
second = kernel::create(simple);
connect< window<128> > net0 (in, first.in[0]);
connect< window<128> > net1 (first.out[0], second.in[0]);
connect< window<128> > net2 (second.out[0], out);
```

This is the representation of the graph.

  ![missing image](images/aie_app_graph.png)

9. Open the file `kernels/kernels.cc` to see what function will be implemented in the kernels.
You can see that this is a simple operation which is doing the sum of the real and imaginary parts of the input to create the real part of the output and the subtraction the real and imaginary part of the input to create the imaginary part of the output.
```
void simple(input_window_cint16 * in, output_window_cint16 * out) {
  cint16 c1, c2;
  for (unsigned i=0; i<NUM_SAMPLES; i++) {
    window_readincr(in, c1);
    c2.real = c1.real+c1.imag;
    c2.imag = c1.real-c1.imag;
    window_writeincr(out, c2);
  }
}
```

### Step 2. Build the Project and Run Through Emulation-AIE

1. Click on the arrow next to the hammer icon and make sure that ***Emulation-AIE*** is selected.

2. Click on the hammer icon to build the project.

3. To run System C simulation (called ***Emulation-AIE***), right-click on the ***simple_application*** project and select ***Run as*** -> ***Launch AIE Emulator*** from the pop-up menu.

4. You should see the simulation running successfully in the console.
The output data is written to the file `Emulation-AIE/aiesimulator_output/data/output.txt`. You can compare the file `output.txt` with the `golden.txt` file. The output data should match the golden value.

      ![missing image](images/aie_app_emu.png)

5. Select the files `simple/data/golden.txt` and `simple/Emulation-AIE/aiesimulator_output/data/output.txt` and right-click on one of them and then click ***Compare With > Each Other After Transformation***.

      ![missing image](images/aie_app_comp.png)

6. Then click on the left-most icons on the right (Pre-defined Filters).

      ![missing image](images/aie_app_comp_2.png)

7. Select ***Remove timestamp*** and click ***OK***.

      ![missing image](images/aie_app_comp_3.png)  

8. You can see that a grep command will be executed to remove the timestamps from the output file. Click ***OK***.

      ![missing image](images/aie_app_comp_4.png)  

A pop-up window should appear saying that the two files match which means that the graph is behaving as expected. You can move forward and integrate the graph in the full system.


<p align="center"><b><a href="./01-custom_base_platform_creation.md">Return to Step 1</a> — <a href="./03-pl_application_creation.md">Go to Step 3</a></b></p>




Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020–2021 Xilinx</sup><br><sup>XD018</sup></br></p>
