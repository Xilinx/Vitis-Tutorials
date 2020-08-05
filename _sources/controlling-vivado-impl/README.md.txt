<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">See 2019.2 Vitis Application Acceleration Development Flow Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Controlling Vivado Implementation

## Introduction

The Vitis™ compiler creates kernel objects from the source code, links the kernels with the targeted shell, and runs the assembled design through the Vivado® tool implementation flows. It generates the platform file (`xclbin`) needed to program the FPGA-based acceleration cards. Sometimes, it is essential to use the advanced Vivado synthesis and implementation options to achieve your desired results, including timing closure. The topics discussed here are documented in [Managing FPGA Synthesis and Implementation Results in the Vivado Tool](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=buildingdevicebinary.html;a=hnw1523048617934) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).

## Tutorial Overview

This tutorial shows you how to control the Vivado tool flows to implement your project and guides you through the steps in the command line flow.

The Vitis core development kit provides two methods to control the Vivado tool flow:

1. When linking the system for the hardware build, you can pass Vivado synthesis and implementation options at the `v++` command line or use configuration files.

2. If your application has already been compiled, then you can:
   * Optimize the design with the Vivado Design Suite.
   * Generate a new routed checkpoint.
   * Reuse this checkpoint to generate a new device binary file (`.xclbin`).

>**TIP:** The design used in this tutorial is taken from the `cl_gmem_2banks` example in the `ocl_kernels` category of the [Vitis Accel Examples](https://github.com/Xilinx/Vitis_Accel_Examples/tree/master/ocl_kernels/cl_gmem_2banks/src) on GitHub.

## Before You Begin

This tutorial uses:

* BASH Linux shell commands
* 2020.1 Vitis core development kit release and the *xilinx_u200_xdma_201830_2* platform.
If necessary, it can be easily ported to other versions and platforms.

>**IMPORTANT:**
>
>* Before running any of the examples, make sure you have installed the Vitis core development kit as described in [Installation](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vhc1571429852245.html) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).
>* If you run applications on Xilinx® Alveo™ Data Center accelerator cards, ensure the card and software drivers have been correctly installed by following the instructions on the [Alveo Portfolio page](https://www.xilinx.com/products/boards-and-kits/alveo.html).
>* Before running this lab, you should be familiar with the [Essential Concepts for Building and Running the Accelerated Application](../Pathway3/README.md) tutorial.

### Accessing the Tutorial Reference Files

1. To access the reference files, enter the following into a terminal: `git clone https://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to the `controlling-vivado-impl` directory, and then access `reference-files/run` directory.

### Set Up the Vitis Environment

 Set up the Vitis core development kit by running these commands:
>
>   ```bash
>    #setup Xilinx Vitis tools. XILINX_VITIS and XILINX_VIVADO will be set in this step.
>    source <VITIS_install_path>/settings64.sh
>    #Setup Xilinx runtime. XILINX_XRT will be set in this step.
>    source <XRT_install_path>/setup.sh
>   ```

## Controlling Vivado Synthesis and Implementation through the Vitis Compiler

>**NOTE:** In this tutorial, run all instructions from the `reference-files/run` directory.

The `–-vivado` switch is paired with properties or parameters to configure the Vivado tools. For instance, the `--vivado` switch  can configure optimization, placement, and timing, or set up emulation and compile options. In the command line flow, properties are specified as `--vivado.prop <object_type>.<object_name>.<prop_name>` where:

* `prop`: Required keyword that signifies the specified property.
* `object_type`: Defines the first class object type to assign the property to.
* `object_name`: Specifies the object instance to attach to.
* `prop_name`: Defines the property_name/value pair which defines both the name and the value to assign.

While you can use `--vivado` options from the `v++` command line, it makes more sense to do some from configuration files specified by the `--config` option. This is explained in [Vitis Compiler Configuration File](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vitiscommandcompiler.html;a=pni1524163195211) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416). In this tutorial, you will use the following options as examples to see how to control Vivado synthesis and implementation:

* Completely flatten the hierarchy during RTL synthesis by specifying the `FLATTEN_HIERARCHY=full` property.
  * `--vivado.prop run.my_rm_synth_1.{STEPS.SYNTH_DESIGN.ARGS.FLATTEN_HIERARCHY}={full}`

* Use the `NoTimingRelaxation` directive during the Vivado routing step by using the `STEPS.ROUTE_DESIGN` property.
  * `--vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={NoTimingRelaxation}`

In this tutorial, the `-–vivado` command options are defined in the `design.cfg` file, which will be used during the hardware build process.

>**TIP:** Because it is running Vivado synthesis and implementation, the hardware build process (`t=hw`) can take a significant amount of time to run.

1. Compile the kernel with the Vitis compiler and link it with the platform file to produce the device binary (`.xclbin`) using the following commands.

   ```bash
   v++ -t hw --config design.cfg -c -k apply_watermark -o apply_watermark.hw.xilinx_u200_xdma_201830_2.xo ../src/krnl_watermarking.cl
   v++ -t hw -s --config design.cfg -R2 -l -o apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin apply_watermark.hw.xilinx_u200_xdma_201830_2.xo
   ```

2. Open the `design.cfg` file and get familiar with different options. The following list describes some of the command options.

      >**Command Options Descriptions**
      >
      >* `-t hw`: Build target is hardware
      >* `platform=xilinx_u200_xdma_201830_2`: Specify the xilinx_u200 platform
      >* `debug=1`: Generate debug info
      >* `max_memory_ports=apply_watermark`: Generate a separate AXI4 interface for each port
      >* `-c`: Compile the kernel
      >* `-k apply_watermark`: Name the kernel
      >* `../src/krnl_watermarking.cl`: Specify source files
      >* `sp=apply_watermark_1.m_axi_gmem0:DDR[0]`: Specify DDR bank connection
      >* `--config design.cfg`: Set the Vivado Synthesis and Implementation options
      >* `-R2`: Set Report Level to 2 to generate DCPs for each implementation step
      >* `-l`: Link the kernel
      >* `nk=apply_watermark:1:apply_watermark_1`: Specify kernel instance number and name

3. To check that the Vivado synthesis and implementation options are applied, open `_x/logs/link/vivado.log`, and search for *flatten_hierarchy* and *NoTimingRelaxation*. You will find following lines indicating that the options have taken effect:

   * **Command**: `synth_design -top pfm_dynamic -part xcu200-fsgd2104-2-e -flatten_hierarchy full -mode out_of_context`
   * **Command**: `route_design -directive NoTimingRelaxation`

You have successfully applied properties to manage or influence the results of synthesis and implementation in the Vivado Design Suite.

## Optimizing the Design in the Vivado Tool

You can optimize the Vivado implementation results to achieve your goal using various optimization techniques, to achieve timing closure for example, or to improve performance. After working with the Vivado tool to optimize your design, write a design checkpoint (DCP) from the routed design to reuse in the Vitis compiler link process (`v++ --link`).

>**IMPORTANT:** Ensure that the Vitis tools and Vivado tools use the same release version.

For information on specific Vivado tool usage and optimization, refer to the *Vivado Design Suite Implementation User Guide* ([UG904](https://www.xilinx.com/cgi-bin/docs/rdoc?v=latest;d=ug904-vivado-implementation.pdf)) and the *UltraFast Design Methodology Guide for the Vivado Design Suite* ([UG949](https://www.xilinx.com/cgi-bin/docs/rdoc?v=latest;d=ug949-vivado-design-methodology.pdf)).

You can run the Vivado tool in batch mode using a Tcl script or run it interactively. In this tutorial, you will run it interactively, and then source a Tcl script inside the tool to achieve your optimization. This will give you an opportunity to familiarize yourself with the Vivado IDE.

1. From the `/reference-file/run` folder, launch the Vivado IDE.

   ```bash
   vivado
   ```

2. When the Vivado IDE is open, enter the following command in the Tcl console.

   ```bash
   source ./opt.tcl
   ```

   The `opt.tcl` file includes the following Tcl commands:

   ```bash
   #DCP files in different stages of Vivado have been written by v++ linker automatically with option "-R2"
   open_checkpoint ./_x/link/vivado/vpl/prj/prj.runs/impl_1/pfm_top_wrapper_routed.dcp
   #Run post-route physical optimization
   phys_opt_design -directive AggressiveExplore
   write_checkpoint -force ./_x/link/vivado/routed.dcp
   ```

   After the `opt.tcl` script completes, if you are not familiar with the tool, spend some time looking around the Vivado IDE. Examine the Tcl console at the bottom of the IDE to see what commands have been run. 

3. Select  **File > Exit**.

### Reuse the Optimized Checkpoint to Create the Device Binary

Next, reuse the `routed.dcp` checkpoint file generated by the `opt.tcl` script to generate a new platform file (`.xclbin`). To do this, add the `--reuse_impl` option to the `v++` command. This reruns the `--link` process using the already implemented Vivado design checkpoint file (`.dcp`). It runs in much less time than the initial hardware build required.

```bash
v++ -t hw --config design.cfg -l -o 'apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin' apply_watermark.hw.xilinx_u200_xdma_201830_2.xo --reuse_impl ./_x/link/vivado/routed.dcp
```

From the transcript in the command shell, you can see that the steps to generate the implemented design have been skipped, and the bitstream generation starts.

```bash
INFO: [VPL 60-423]   Target device: xilinx_u200_xdma_201830_2
[11:22:58] Run vpl: Step interactive: Started
Starting FPGA bitstream generation.
```

## Putting it All Together

The following section summarizes all the steps to control the Vivado tool implementation with the `v++` command, optimize design with the Vivado tools, and reuse implemented DCP to generate the platform file. You might use it as a quick reference for commands.

1. Set up the core development kit and working directory.

   ```bash
    #setup Xilinx Vitis tools. XILINX_VITIS and XILINX_VIVADO will be set in this step.
    source <VITIS_install_path>/settings64.sh
    #Setup Xilinx runtime. XILINX_XRT will be set in this step.
    source <XRT_install_path>/setup.sh
   ```

2. Generate the platform file (`xclbin`) with `v++` commands.  

   ```bash
   v++ -t hw --config design.cfg -c -k apply_watermark -o apply_watermark.hw.xilinx_u200_xdma_201830_2.xo ../src/krnl_watermarking.cl
   v++ -t hw -s --config design.cfg -R2 -l -o apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin apply_watermark.hw.xilinx_u200_xdma_201830_2.xo
   ```

3. Optimize the design with the Vivado tool, and rewrite the routed DCP.

   ```bash
   vivado -mode batch -source opt.tcl
   ```

4. Reuse the routed DCP to generate the device binary file (`.xclbin`).

   ```bash
   v++ -t hw --config design.cfg -l -o apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin apply_watermark.hw.xilinx_u200_xdma_201830_2.xo --reuse_impl ./_x/link/vivado/routed.dcp
   ```

5. This tutorial has provided host code to make it complete to run through various flows. The following commands are references to build the host and run the application.

   ```bash
   #Generate the host executable.
   g++ -I$XILINX_XRT/include/ -I$XILINX_VIVADO/include/ -Wall -O0 -g -std=c++11 -L$XILINX_XRT/lib/ -lOpenCL -lpthread -lrt -lstdc++ -o 'host' '../src/host.cpp'
   #Please set correct XCL_EMULATION_MODE manually if running sw_emu and hw_emu modes
   ./host apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin ../src/inputImage.bmp ../src/golden.bmp
   ```

## Conclusion

This tutorial illustrated how to use the `v++` command `--vivado` option to control Vivado synthesis and implementation, using a simple example to optimize the design with the Vivado tool. It also showed the steps to reuse an implemented DCP to generate the device binary file (`.xclbin`), building the Vitis accelerated application from the optimized Vivado implementation.
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="/README.md">Return to Main Page</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
