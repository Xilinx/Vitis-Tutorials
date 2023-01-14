<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

Code, files and instructions for **module 1**

> **_In this module..._**  
_1> Review the OpenCL API for the host/kernel paradigm  
2> Run Vitis via the GUI or make to execute software and hardware emulation  
3> Run Vitis Analyzer to review the application timeline  
4> Run Vitis HLS to identify the throughput bottleneck  
5> Insert a testbench into HLS to iterate quickly and verify kernel code changes_

## Understanding Code Setup with Host and Kernel

The source code for each module is located under their local <code>./src</code> directory:
  + Take a look at the host code and notice the several APIs that are used. Notice how the data is transferred back and forth to the kernel an back. The execution model can be broken down into the following steps:
     + The host program writes the data needed by a kernel into the global memory of the attached device through the PCIe interface on an Alveo Data Center accelerator card, or through the AXI bus on an embedded platform.
     + The host program sets up the kernel with its input parameters.
     + The host program triggers the execution of the kernel function on the FPGA.
     + The kernel performs the required computation while reading data from global memory, as necessary.
     + The kernel writes data back to global memory and notifies the host that it has completed its task.
     + The host program reads data back from global memory into the host memory and continues processing as needed
  + Take a look at the kernel code.  This code is compiled by the Vitis tools and transformed into an hardware description that the Vivado tool implements onto Xilinx devices. Since the the host and kernel code are developed and compiled independently, wrap the kernel function declaration with the extern "C" linkage in the header file, or wrap the whole function in the kernel code.
    ```cpp
    extern "C" {
                 void kernel_function(int *in, int *out, int size);
               }
    ```

***
## Build and Emulate with Vitis
For each module of this tutorial, Vitis can be run via the GUI **or** at the command line (more efficient in the context of this tutorial):
<details>
  <summary><b>Click to expand! (<code>Vitis GUI</code> instructions)</b></summary>

   ### Using Vitis via the GUI
   TIP: when following the links below, use right-click "open in another tab" to avoid losing the context for these instructions...
   1. Open a terminal
   2. Setup and launch Vitis
   3. "File" menu -> "Import..." <a href="../images/gui1_import.png">(image)</a>
   4. Accept the default of the Vitis project exported zip file and click "Next" <a href="../images/gui2_next.png">(image)</a>
   5. Click "Browse" on the next window and navigate to the ./docs/module1_baseline/project directory <a href="../images/gui3_browse.png">(image)</a>
   6. Select the vitis_export_archive.ide.zip and click "Okay" <a href="../images/gui4_zip.png">(image)</a>
   7. Next window click "Finish" <a href="../images/gui5_finish.png">(image)</a>

   ### Setting up the Platform
   1. On the center window pane, click on the triple dot icon <code>[...]</code> right after the platform link <a href="../images/gui6_platform.png">(image)</a>
   2. Navigate to specify the platform accessible from your system

   ### Building and Emulating the Design
   Notice the "Assistant" window pane with its 3 main flow steps
   1. <code>Emulation-SW</code>: to validate the design at the functional level
   2. <code>Emulation-HW</code>: compile the kernel into a cycle accurate representation to better gauge metrics
   3. <code>Hardware</code>: to launch the full compilation and generate a bitstream

 Run the software emulation (takes a couple of minutes)...

 Run the hardware emulation (takes between 10 to 20 minutes)

 Once hardware emulation has completed, go to the Vitis Analyzer section below...

</details>

***OR***

<details>
  <summary><b>Click to expand! (instructions for <code>make</code>)</b></summary>

   ### Using **make**
   1. Open a terminal
   2. Setup Vitis
   3. Navigate to `./build`
   4. Run: `make run TARGET=sw_emu` (for a functional emulation)
   5. Run: `make run TARGET=hw_emu` (for a more detailed emulation - takes 10 minutes or more)
      * For now don't run "hw" target as it would take over an hour to run

</details>

***
## Vitis Analyzer for Application End-to-end Timeline Analysis

Vitis Analyzer is a graphical tool which lets you browse many aspects of the design starting from the whole system down to the details of the kernel.

<details>
  <summary><b>Click to expand! (instructions for <code>Vitis Analyzer</code></b>)</summary>

   1. Open a terminal and setup Vitis
   2. Run: <code>vitis_analyzer &</code>
   3. File menu -> Open Summary...
   4. Browse to <code>./build</code>
   5. Select cholesky_kernel_hw_emu_xclbin_<b>run</b>_summary (prefixed with the blue "play" pictogram)
   6. Navigate around by yourself watch this 45 seconds [looping gif](../images/analyzer_anim.gif) to see how to go around in Vitis Analyzer.

      Make sure to check:
      1. Profile summary
      2. Guidance reports - indicates area of improvement
      3. Application timeline - more information just below

The application timeline has the following structure:

* *Host*

  * **OpenCL API Calls:**
    All OpenCL API calls are traced here. The activity time is measured from the host perspective.

   * **General:**
     All general OpenCL API calls such as clCreateProgramWithBinary, clCreateContext, and clCreateCommandQueue, are traced here.

   * **Queue:**
     OpenCL API calls that are associated with a specific command queue are traced here. This includes commands such as clEnqueueMigrateMemObjects, and clEnqueueNDRangeKernel. If the user application creates multiple command queues, then this section shows all the queues and activities.

   * **Data Transfer:**
     In this section the DMA transfers from the host to the device memory are traced. There are multiple DMA threads implemented in the OpenCL runtime and there is typically an equal number of DMA channels. The DMA transfer is initiated by the user application by calling OpenCL APIs such as clEnqueueMigrateMemObjects. These DMA requests are forwarded to the runtime which delegates to one of the threads. The data transfer from the host to the device appear under Write as they are written by the host, and the transfers from device to host appear under Read.

   *  **Kernel Enqueues:**
    The kernels enqueued by the host program are shown here. The kernels here should not be confused with the kernels/CUs on the device. Here kernel refers to the NDRangeKernels and tasks created by the OpenCL commands clEnqueueNDRangeKernels and clEnqueueTask. These are plotted against the time measured from the host's perspective. Multiple kernels can be scheduled to be executed at the same time, and they are traced from the point they are scheduled to run until the end of the kernel execution. Multiple entries would be shown in different rows depending on the number of overlapping kernel executions.

* *Device "name"*

   **Binary Container "name":**
   Simply the binary container name.

   * **Accelerator "name":**
       Name of the compute unit (a.k.a., Accelerator) on the FPGA.

</details>

***
## Vitis HLS for Kernel Optimizations

The C++ kernels destined implemented onto the device LUTs and flops (a.k.a the "fabric") are automatically compiled with the high-level synthesis tool Vitis HLS. In this tutorial we run Vitis HLS "manually" to gain additional insights about the underlying synthesis technology and the Cholesky kernel algorithm.

<details>
  <summary><b>Click to expand! (instructions for <code>Vitis HLS</code>)</b></summary>

   1. Open a terminal and setup Vitis
   2. Navigate to <code>./build/cholesky_kernel_hw_emu/cholesky_kernel</code>
      * There should be yet another cholesky_kernel directory at that level
   3. Run: <code>vitis_hls -p cholesky_kernel &</code> (to start the Vitis high-level synthesis GUI)
   4. Vitis HLS now shows the high-level synthesis report
   5. In the GUI expand the **Synthesis Summary Report** window
   6. Expand the loops and function in the **Performance & Resources** section
   7. Right click on the **II violation** as shown in this clip to locate it in the code: [**50s HLS looping GIF**](../images/HLS_anim.gif)

   Note: you can restore the original Vitis HLS window layout via the "Window" menu -> "Reset Perspective".

#### Initiation Interval

   We see an II violation of 8 for two loops in this function.
   One of them looks like this:
   ```cpp
   // Loop only takes one element every 8 clock cycles!!!
   // We expected one every cycle (II of 1)
   for (int k = 0; k < j; k++)
   {
       tmp += dataA[j][k] * dataA[j][k];
   }
   ```
Since this version of the algorithm uses double data types with an accumulation, the silicon needs 8 cycles at 300MHz to perform and complete the operation before starting the next. So we can only compute samples one after another by intervals of 8 cycles...  This is the first bottleneck that we'll tackle in the next module.

#### Kernel Latency

Let's now look at latency.

`cholesky_kernel/solution/syn/report/cholesky_kernel_csynth.rpt`

    * Loop:
    +--------------------+--------+---------+-------------+-----------+-----------+------------+----------+
    |                    | Latency (cycles) |  Iteration  |  Initiation Interval  |    Trip    |          |
    |       Loop Name    |  min   |   max   |   Latency   |  achieved |   target  |    Count   | Pipelined|
    +--------------------+--------+---------+-------------+-----------+-----------+------------+----------+
    |- VITIS_LOOP_32_..  |       ?|        ?|            3|          1|          1|           ?|    yes   |
    |- Loop_first_col    |       ?|        ?|           34|          1|          1|           ?|    yes   |
    |- Loop_col          |       ?|        ?|            ?|          -|          -|           ?|    no    |
    | + Loop_diag        |      17|  2097161|           18|          8|          1| 1 ~ 262144 |    yes   |
    | + Loop_row         |       ?|        ?| 61 ~ 2097205|          -|          -|           ?|    no    |
    |  ++ Loop_vec_mul   |      17|  2097161|           18|          8|          1| 1 ~ 262144 |    yes   |
    |- VITIS_LOOP_67_..  |       ?|        ?|            4|          1|          1|           ?|    yes   |
    +--------------------+--------+---------+-------------+-----------+-----------+------------+----------+

Notice that:
  - The <code>VITIS</code> prefixed loops: these are loops automatically labeled by Vitis HLS since none were applied in the source code for them.  The other loops did have a label, it's shown in the table.
  - The question marks (?), they denote a metric that cannot be calculated because dependent on scalar input to the function and indeed in this example the matrix size is configurable and latency will vary depending on the size.
  - The last "Pipeline" column indicates if a loop was constrained to process its inputs at each cycle. The simple loops or most inner nested loops are the ones generally "pipelined" automatically by the tool

 As an input to the Cholesky function the user passes the size of the matrix N (in the example we ran it was 64).

 The first loop requires N iterations at II=1 so it takes Nx3 to complete since the iteration latency is 3.
 The <code>Loop_first_col</code> loop takes Nx34
 The <code>Loop_col</code> loop runs N times ( (<code>Loop_diag</code> is N * 18) + (<code>Loop_row</code> is N * (N + 18))
 Last loop also requires N iterations like the first one.

 Some we can roughly estimate the duration to be:
<code> N(18N+N(18N+residual1)+residual2) = 18N<sup>3</sup> + (18+residual1)N<sup>2</sup> + residual2.N </code>

So essentially the algorithm latency goes by the cube of N, the size of the matrix.

#### Adding a C++ testbench for the kernel

For this tutorial we provide a pre-made C++ "main" program to wrap around the kernel and simulate in the Vitis HLS environment.

**Instructions:**
1. In a terminal, from the <code>docs</code> directory:

       cp -r ./hls_tb ./module1_baseline/build/cholesky_kernel_hw_emu/cholesky_kernel
       cp ./module1_baseline/src/cholesky_kernel.hpp ./module1_baseline/build/cholesky_kernel_hw_emu/cholesky_kernel/hls_tb
       
2. If the Vitis HLS GUI was closed, open it again:

       cd ./module1_baseline/build/cholesky_kernel_hw_emu/cholesky_kernel
       vitis_hls -p cholesky_kernel &
       
3. In the "Explorer" window left pane of the GUI, locate "Test Bench" under "Source".
   Right-click -> "Add file...", select test_hls.cpp. Right-click again and use "Add folder...", select ./hls_tb/tb_data
4. Now select "Project"-> "Run C simulation" in main menu.
   This runs a purely functional simulation called "Csim", none of what HLS synthesizes is involved.
5. Select "Project"-> "Run C simulation"
6. Select "Solution" -> "Run C Synthesis" -> "Active Solution"
7. Run "Solution" -> "Run C/RTL Cosimulation".  In the popup window select Okay.

The Vitis HLS Cosimulation runs a cycle accurate RTL simulation which shows the actual latency in clock cycles.  In the test bench the matrix is a 16x16.

</details>

***
## Wrap-up for module 1

**Key points:**
 - To accelerate an algorithm on an Alveo card, the program needs a host and a kernel design unit
 - Vitis helps build the application, provides drivers to enable host-kernel communications
 - Vitis provides an analyzer tool to help understand the sequence of operations
 - Vitis HLS is a compiler technology that transforms the C code algorithm into an hardware language to implement onto the Xilinx device
***

#### Next

You can now proceed to [**module 2**](../module2_pipeline/README.md).
***


<p align="center"><sup>Copyright&copy; 2020-2022 Xilinx</sup></p>
