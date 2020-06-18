Code and files for **module 1**

## Organizing the C code
In order to be able to use the hardware based acceleration, we need to split the code between an host and a kernel.  The host runs on the server and uses APIs we'll discuss below to manage the data and command operations for the kernel.

Take now a look at the host code and notice the several APIs that are used.
Notice how the data is tranferred back and forth to the kernel an back.

Take a look at the kernel code.  This code will be compiled inside the Vitis tools and transformed into an hardware description that the Vivado tool can implement onto Xilinx devices. As we'll the goal on that code is to make it as efficient as possible in regards to the available hardware.

You could either import this design as a pre-setup project in the Vitis GUI or run make.
## Build and execute your first Vitis design
The Vitis GUI is based on Eclipse and allows you to customize the design.
<details>
  <summary>Click to expand! (instructions for GUI)</summary>
  
    ### Using Vitis via the **GUI**
    1. Launch Vitis
    2. Import the project
       * Browse to the zip file located under the project directory
 
</details>

<details>
  <summary>Click to expand! (instructions for make)</summary>
  
    ## Using **make**
    1. Open a terminal
    2. Here are the main options:
       * Default (no options) show the help
       * sw_emu runs the sw emulation flow
 
</details>
