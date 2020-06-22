Code, files and instructions for **module 1**

## Organizing the C code
To enable hardware acceleration, the code is split between the host and the kernel.  The host program, written in C/C++ uses API abstractions like OpenCL, runs on a host processor, an x86 server or an Arm processor for embedded platforms.  The hardware accelerated kernels run onto the programmable logic of a Xilinx device.

The API calls, processed by Xilinx runtime (XRT), manage process transactions between the host program and the hardware accelerators. Communication between the host and the kernel, including control and data transfers, occurs across the PCIe® bus or an AXI bus for embedded platforms.

In a typical application, the host first transfers data to be operated on by the kernel from host memory into global memory. The kernel subsequently operates on the data, storing results back to the global memory. Upon kernel completion, the host transfers the results back into the host memory. Data transfers between the host and global memory introduce latency, which can be costly to the overall application. To achieve acceleration in a real system, the benefits achieved by the hardware acceleration kernels must outweigh the added latency of the data transfers.

The source code for each module is located under its module in the ./src directory:
  + Take a look at the host code and notice the several APIs that are used. Notice how the data is tranferred back and forth to the kernel an back.
  + Take a look at the kernel code.  This code will be compiled inside the Vitis tools and transformed into an hardware description that the Vivado tool can implement onto Xilinx devices. As we'll the goal on that code is to make it as efficient as possible in regards to the available hardware.

You could either import this design as a pre-setup project in the Vitis GUI or run make.
## Build and Emulate with Vitis
The Vitis GUI is based on Eclipse and allows you to customize the design.
<details>
  <summary>Click to expand! (instructions for GUI)</summary>
  
   ### Using Vitis via the GUI
   1. Open a terminal
   2. Setup and launch Vitis  
   3. "File" menu -> "Import..." <a href="../images/gui1_import.png">(image)</a>
   4. Accept the default of the Vitis project exported zip file and click "Next" <a href="../images/gui2_next.png">(image)</a>
   5. Click "Browse" on the next window and navigate to the ./docs/module1_baseline/project directory <a href="../images/gui3_browse.png">(image)</a>
   6. Select the vitis_export_archive.ide.zip and click "Okay" <a href="../images/gui4_zip.png">(image)</a>
   7. Next window click "Finish" <a href="../images/gui5_finish.png">(image)</a>
    
   ### Setting up the Platform
   1. On the center window pane, click on the triple dot icon (<code>...</code>) right after the platform link <a href="../images/gui6_platform.png">(image)</a>
   2. Navigate to specify the platform accessible from your system
    
   ### Building and emulating the design
   Notice the "Assistant" window pane with its 3 main flow steps
   1. Emulation-SW: to validate the design at the fucntional level
   2. Emulation-HW: compile the kernel into a cycle accurate representation to better gauge metrics
   3. Hardware: to launch the full compilation and generate a bitstream
 
</details>

<details>
  <summary>Click to expand! (instructions for command line flow with <code>make</code>)</summary>
  
    ## Using **make**
    1. Open a terminal
    2. Setup Vitis
    3. Navigate to ./build
    4. Run make
       * make build TARGET=sw_emu|hw_emu|hw
         + Builds for software or hardware emulation. The "hw" option runs the full compilation.
       * make run TARGET=sw_emu|hw_emu|hw
         + Executes for software or hardware emulation with "hw" running on the card after full compilation
       * make clean
	 + delete files to start from a clean context
         
</details>
