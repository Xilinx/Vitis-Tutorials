Code and files for **module 1**

## Organizing the C code
In order to be able to use the hardware based acceleration, we need to split the code between an host and a kernel.  The host runs on the server and uses APIs we'll discuss below to manage the data and command operations for the kernel.

Take now a look at the host code and notice the several APIs that are used.
Notice how the data is tranferred back and forth to the kernel an back.

Take a look at the kernel code.  This code will be compiled inside the Vitis tools and transformed into an hardware description that the Vivado tool can implement onto Xilinx devices. As we'll the goal on that code is to make it as efficient as possible in regards to the available hardware.

## Running the design with Vitis

You could either open this design (already setup) in the Vitis GUI or run make.
## Running the design with Vitis
You could either open this design (already setup) in the Vitis GUI or run make.
<details>
  <summary>Click to expand!</summary>
  
  ## Running Vitis from the GUI
  1. A numbered
  2. list
     * With some
     * Sub bullets
  ## Running Vitis with make
  1. A numbered
  2. list
     * With some
     * Sub bullets
</details>
