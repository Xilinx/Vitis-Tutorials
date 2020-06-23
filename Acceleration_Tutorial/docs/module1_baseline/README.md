Code, files and instructions for **module 1**

## Understanding code setup with host / kernel
To enable hardware acceleration, the code is split between the host and the kernel.  The host program, written in C/C++ uses API abstractions like OpenCL, runs on a host processor, an x86 server or an Arm processor for embedded platforms.  The hardware accelerated kernels run onto the programmable logic of a Xilinx device.

The API calls, processed by Xilinx runtime (XRT), manage process transactions between the host program and the hardware accelerators. Communication between the host and the kernel, including control and data transfers, occurs across the PCIe® bus or an AXI bus for embedded platforms.

In a typical application, the host first transfers data to be operated on by the kernel from host memory into global memory. The kernel subsequently operates on the data, storing results back to the global memory. Upon kernel completion, the host transfers the results back into the host memory. Data transfers between the host and global memory introduce latency, which can be costly to the overall application. To achieve acceleration in a real system, the benefits achieved by the hardware acceleration kernels must outweigh the added latency of the data transfers.

The source code for each module is located under the <code>./src</code> directory:
  + Take a look at the host code and notice the several APIs that are used. Notice how the data is transferred back and forth to the kernel an back. The execution model can be broken down into the following steps:
     + The host program writes the data needed by a kernel into the global memory of the attached device through the PCIe interface on an Alveo Data Center accelerator card, or through the AXI bus on an embedded platform.
     + The host program sets up the kernel with its input parameters.
     + The host program triggers the execution of the kernel function on the FPGA.
     + The kernel performs the required computation while reading data from global memory, as necessary.
     + The kernel writes data back to global memory and notifies the host that it has completed its task.
     + The host program reads data back from global memory into the host memory and continues processing as needed
  + Take a look at the kernel code.  This code will be compiled inside the Vitis tools and transformed into an hardware description that the Vivado tool can implement onto Xilinx devices. As we'll the goal on that code is to make it as efficient as possible in regard to the available hardware.
***
## Build and Emulate with Vitis
For each module of this tutorial, Vitis can be run via the GUI **or** at the command line:
<details>
  <summary>Click to expand! (<code>Vitis GUI</code> instructions)</summary>
  
   ### Using Vitis via the GUI
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
    
   ### Building and emulating the design
   Notice the "Assistant" window pane with its 3 main flow steps
   1. <code>Emulation-SW</code>: to validate the design at the functional level
   2. <code>Emulation-HW</code>: compile the kernel into a cycle accurate representation to better gauge metrics
   3. <code>Hardware</code>: to launch the full compilation and generate a bitstream
 
 Run the software emulation...
 
 Run the hardware emulation (takes over 10 minutes)
 
 After hardware emulation has completed, right click on the <code>Emulation_HW</code> and select "Run HLS" (this will spawn the Vitis HLS GUI)
 
 For instructions about Vitis HLS, please see below...
 
</details>

***OR***

<details>
  <summary>Click to expand! (instructions for <code>make</code>)</summary>
  
   ### Using **make**
   1. Open a terminal
   2. Setup Vitis
   3. Navigate to ./build
   4. Run: make run TARGET=sw_emu (for a functional emulation)
   5. Run: make run TARGET=hw_emu (for a more detailed emulation - takes 10 minutes or more)  
      * Avoid the "hw" which would take over an hour to run
   6. Navigate to ./build/cholesky_kernel_hw_emu/cholesky_kernel
      * There should be another cholesky_kernel directory at that level
   7. Run: vitis_hls -p cholesky_kernel (started the high-level synthesis GUI)
   8. Now that HLS is started, see instructions below for the rest...
      
</details>

***
## Vitis Analyzer (end-to-end timeline analysis)

<details>
  <summary>Click to expand! (instructions for <code>Vitis Analyzer</code>)</summary>
  
   1. Open a terminal and setup Vitis
   2. Run: <code>vitis_analyzer &</code>
   3. Navigate to the module area compile files
   4. File menu...
      
The report has the following structure:

*Host*

OpenCL API Calls
        All OpenCL API calls are traced here. The activity time is measured from the host perspective.

*General*
            All general OpenCL API calls such as clCreateProgramWithBinary, clCreateContext, and clCreateCommandQueue, are traced here.

*Queue*
            OpenCL API calls that are associated with a specific command queue are traced here. This includes commands such as clEnqueueMigrateMemObjects, and clEnqueueNDRangeKernel. If the user application creates multiple command queues, then this section shows all the queues and activities.

*Data Transfer*
    In this section the DMA transfers from the host to the device memory are traced. There are multiple DMA threads implemented in the OpenCL runtime and there is typically an equal number of DMA channels. The DMA transfer is initiated by the user application by calling OpenCL APIs such as clEnqueueMigrateMemObjects. These DMA requests are forwarded to the runtime which delegates to one of the threads. The data transfer from the host to the device appear under Write as they are written by the host, and the transfers from device to host appear under Read.
      
</details>

***
## Vitis HLS (kernel optimizations)

<details>
  <summary>Click to expand! (instructions for <code>Vitis HLS</code>)</summary>
  
   1. Open a terminal and setup Vitis
   2. Navigate to ./build/cholesky_kernel_hw_emu/cholesky_kernel
      * There should be another cholesky_kernel directory at that level
   3. Run: vitis_hls -p cholesky_kernel (started the high-level synthesis GUI)
   4. Now that HLS is started, see instructions below for the rest...
      
</details>

***
