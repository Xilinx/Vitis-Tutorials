<table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Application Acceleration Tutorials</h1>

   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

<!-- 
#/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#*/
-->

# 1. Creating a Vitis HLS Project

The Vitis HLS tool lets you specify C/C++ code for synthesis into Vitis core development kit kernels (`.xo`) or RTL IP for implementation in the PL region of AMD devices. The first step in creating a new project is identifying the C/C++ source code for synthesis.

In this tutorial, you are working with a simple discrete cosine transform (DCT) algorithm that processes an input matrix of values, applies fixed coefficients, and returns a matrix of modified values. In the `reference_files/src` folder, the `dct.cpp` contains the top-level DCT function.

1. Enter the following command to launch Vitis HLS in GUI mode.

   `vitis_hls`

   Vitis HLS opens.

2. Select **File** > **New Project**. 

   The New Vitis HLS Project wizard is displayed.

    ![Create Project New](./images/create_project-new.png)

3. Create the Vitis HLS project type:
   1. In the Project name field, enter `dct_prj`.
   2. In the Location field, click **Browse** to select the location for the project.
   3. Click **Next>**.

   The Add/Remove Files page of the New Project wizard displays.

   ![Add Sources](./images/create_project-add_source.png)

4. Make the following selections:
   1. Select **Add Files** to specify source files for the project.
      1. Navigate to the **./reference-files/src** folder location, and select **dct.cpp**.
   2. In the Top Function field at the top of the New Project wizard, select the **Browse...** button to open the Select Top Function dialog box, as shown below. 
      1. Select the **dct (dct.cpp)** function and click **OK**.

            ![Select Top Function](./images/create_project-select_top_function.png)

   3. Click **Next>**.

         The Add/Remove Testbench Files page of the New Project wizard displays.

         Writing a good testbench can greatly increase your productivity because C functions execute in orders of magnitude faster than RTL simulations. Using C to develop and validate the algorithm before synthesis is much faster than developing and debugging RTL code. For more information, refer to [Writing a Testbench](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/Writing-a-Test-Bench) in the Vitis HLS Flow of the Vitis Unified Software Platform Documentation (UG1416).

      ![Add Sources](./images/create_project-add_testbench.png)

5. Select **Add Files** to specify the testbench, and additional files for the project. 
   1. Navigate to the `./reference-files/src` folder, and select **dct_test.cpp, in.dat** and **out.golden.dat**. 
      * `dct_test.cpp` is a test bench for the design that iterates through the kernel multiple times. 
      * `in.dat` provides the input values to be processed by the kernel. 
      * `out.golden.dat` provides known output results to use for comparing the output of the dct function against.
   2. Click **Next>**.

      The Solution Configuration page of the New Project wizard is opened as shown below.

      The Solution Configuration creates and defines a solution which is a specific build configuration that the tool uses to build against. The solution includes definitions for the clock period, and clock uncertainty, and specifies a platform, with an AMD device, to build against. The solution provides you a framework for building your RTL code, and testing different optimizations by using different directives (possibly in different solutions).

      ![Add Sources](./images/create_project-solution_config.png)

6. Make the following selections:
      1. Specify the **Solution Name**, or use the default name. 
      2. Specify the **Period** for the clock as the default period of 10 ns.
      3. Leave the clock uncertainty blank. The default clock uncertainty, when it is not specified, is 27% of the clock period. For more information, refer to [Specifying the Clock Frequency](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/Specifying-the-Clock-Frequency) in the Vitis HLS Flow of the Vitis Unified Software Platform Documentation (UG1416).
      4. For **Part Selection** accept the default value.
      5. For **Flow Target** select the **Vitis Kernel Flow Target** from the dropdown menu.

   This enables the creation of a Vitis application acceleration hardware kernel (.xo) as an output for the project. For more information on how enabling the Vitis kernel flow configures the default behavior of the tool, refer to [Vitis HLS Flow Overview](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/Vitis-HLS-Flow-Overview) in the Vitis HLS Flow of the Vitis Unified Software Platform Documentation (UG1416).

7. With your project settings now configured, click **Finish**.
    Vitis HLS opens the new project in the default view.

      ![Add Sources](./images/create_project-default_perspective.png)

## Conclusion

You created the DCT project, targeted your AMD device or board, and configured the solution characteristics. You are ready to move to the next lab, [Running Simulation, Synthesis and Analyzing Results](./synth_and_analysis.md).
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="/README.md">Return to Main Page</a> — <a href="./README.md">Return to Start of Tutorial</a></b></p>


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>

