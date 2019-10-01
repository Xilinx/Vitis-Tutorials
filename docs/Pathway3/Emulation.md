<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">See other versions</a>
   </td>
 </tr>
 <tr>
 <td align="center"><h1>Essential Concepts for Building and Running the Accelerated Application </h1>
 </td>
 </tr>
</table>

# 2. Running Software and Hardware Emulation

## Introduction

Emulation is essential for design debugging and optimization. The Vitis core development kit provides both software and hardware emulation.

This tutorial details the steps to perform both software and hardware emulation.

## Before You Begin

Review and run the [Building an Application](./BuildingAnApplication.md) lab before to running this lab.  

## Running Software and Hardware Emulation

To help you better debug and optimize your applications, the Vitis core development kit allows you to run your design in both software and hardware emulation. Each provides a different level of insight into the design and are described below. For emulation and optimization details, refer to *Profiling, Optimizing, and Debugging the Application*.

* In software emulation (`sw_emu`), both the host application code and the kernel code are compiled to run on the x86 processor. This allows iterative algorithm refinement through fast build-and-run loops. This target is useful for identifying syntax errors, performing source-level debugging of the kernel code running together with application, and verifying the behavior of the system. For RTL kernels, software emulation can be supported if a C model is associated with the kernel.

* For hardware emulation (`hw_emu`), the kernel code is compiled into a hardware model, which is run in a hardware simulator, while the rest of the system uses a C simulator. Building and running takes longer but provides a detailed, cycle-aware, view of kernel activity. This target is useful for testing the functionality of the logic that will run in the FPGA and for getting initial performance estimates.

## Requirements for Running Emulation

To run emulation, it is necessary to:

1. Build the design.
2. Generate an emulation configuration file.
3. Set the XCL_EMULATION_MODE environment variable.

### Building the Design

Before running emulation, build the application with the desired emulation mode (software or hardware). The emulation mode is specified when building the hardware through the `-t <build_target>` `v++` command option.

* To run software emulation, the hardware must be built with *<build_target>* set to **sw_emu**.  
* Similarly, to run hardware emulation, the hardware must be built with *<build_target>* set to **hw_emu**.

The [Building an Application](./BuildingAnApplication.md) lab describes building the host software and hardware and how to specify the *<build_target>*.

>**IMPORTANT:** Before running any of the examples, ensure you have set up the Vitis core development kit by running the following commands.
>
>  ```bash
>  #setup Xilinx Vitis tools. XILINX_VITIS and XILINX_VIVADO will be set in this step.
>  source <VITIS_install_path>/settings64.sh
>  #Setup runtime. XILINX_XRT will be set in this step
>  source <XRT_install_path>/setup.sh
>  ```

### Emulation Configuration Files

In addition to building the application and targeting a specific emulation mode, it is also necessary to generate an emulation configuration file.

The emulation configuration file, `emconfig.json`, is generated using the `emconfigutil` command and provides information used by the Xilinx runtime (XRT) library during emulation. The `emconfigutil` command is provided in the Vitis core development kit installation.

At a minimum you need to specify the target platform through the `-f` or `-–platform` option to generate the required `emconfig.json`. The specified *platform_target* must be identical to that used during the host and hardware builds. A single `emconfig.json` file can be used for both software and hardware emulation.

```
emconfigutil -–platform <platform_target>
```

For a list of options, run `emconfigutil --help`, or refer to the _Vitis Envrionment Reference Materials_.

### Define the XCL_EMULATION_MODE Environment Variable

Finally, before running either the software or hardware emulation, define the XCL_EMULATION_MODE environment variable. This variable is used by the runtime library to correctly execute the desired emulation.

* For software emulation, set the variable to `sw_emu`.

   ```
   export XCL_EMULATION_MODE=sw_emu
   ```

* For hardware emulation, set the variable to `hw_emu`.

   ```
   export XCL_EMULATION_MODE=hw_emu
   ```

## Running Software Emulation

Using what you now know, put it all together, and run software emulation on a design by following the steps in the provided source files.

1. Ensure the software and hardware have been built with *build_target* set to `sw_emu`.  

   >**NOTE:** Refer to the [Building an Application](./BuildingAnApplication.md) module for how to build the host software and hardware.

2. Generate the emulation configuration file.  
    The generated `emconfig.json` file must be in the same directory as the host executable; ensure you are in the same directory as the host executable, then run the following command.

   ```bash
   emconfigutil --platform xilinx_u200_xdma_201830_2
   ```

   The platform_target is *xilinx_u200_xdma_201830_2*. It must match that *platform_target* used during the software and hardware build process.

3. Set the XCL_EMULATION_MODE environment variable to `sw_emu` to run in software emulation.

   ```bash
   export XCL_EMULATION_MODE=sw_emu
   ```

4. With the configuration file `emconfig.json` generated and XCL_EMULATION_MODE set, use the following command to execute the host program and kernel in software emulation mode.

   ```bash
   ./host mmult.sw_emu.xilinx_u200_xdma_201830_2.xclbin
   ```

  After successfully running software emulation, you will see an output similar to the following in the Console.

  ```
  Loading: 'mmult.sw_emu.xilinx_u200_xdma_201830_2.xclbin'
  TEST PASSED
  ```

## Running Hardware Emulation

Now that you have run software emulation, run hardware emulation on the same design. This will let you see the differences between software and hardware emulation.

1. Ensure the software and hardware have been built with *build_target* set to *hw_emu*.  

   >**NOTE:** For instructions on how to build the host software and hardware, refer to the [Building an Application](./BuildingAnApplication.md) module.  

2. Generate the emulation configuration file.

   >**NOTE:** This step can be omitted if the file was already generated in the "Running Software Emulation" section.  

   The generated `emconfig.json` file must be in the same directory as the host executable, so ensure you are in the same directory as the host executable, then run the following command.

   ```bash
   emconfigutil --platform xilinx_u200_xdma_201830_2
   ```

   Here, the platform_target is *xilinx_u200_xdma_201830_2*.  It must match that platform_target used during the software and hardware build process.

3. Set the XCL_EMULATION_MODE environment variable to `hw_emu` to run in hardware emulation.

   ```bash
   export XCL_EMULATION_MODE=hw_emu
   ```

4. With the configuration file `emconfig.json` generated and XCL_EMULATION_MODE variable set, use the following command to execute the host program and kernel in hardware emulation mode.

   ```bash
   ./host mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin
   ```

After successfully running hardware emulation, you will see output similar to the following in the Console.

  ```
  Loading: 'mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin'
  INFO: [Vitis-EM 01] Hardware emulation runs simulation underneath. Using a large data set will result in long simulation times. It is recommended that a small dataset is used for faster execution. This flow does not use cycle accurate models and hence the performance data generated is approximate.
  TEST PASSED
  INFO: [Vitis-EM 22] [Wall clock time: 19:16, Emulation time: 0.00651464 ms] Data transfer between kernel(s) and global memory(s)
  mmult_1:m_axi_gmem-DDR[1]          RD = 2.000 KB               WR = 1.000 KB
  ```

## Putting it All Together

The following steps summarize the steps necessary to configure and run emulations.

1. Set up the core development kit.

   ```bash
   source <Vitis_install_path>/settings64.sh
   source <XRT_install_path>/setup.sh
   cd ./reference-files/run
   ```

2. Generate the emulation configuration file.

   ```bash
   emconfigutil --platform xilinx_u200_xdma_201830_2
   ```

3. Run software emulation.

   ```bash
   export XCL_EMULATION_MODE=sw_emu
   ./host mmult.sw_emu.xilinx_u200_xdma_201830_2.xclbin
   ```

4. Run hardware emulation.

   ```bash
   export XCL_EMULATION_MODE=hw_emu
   ./host mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin
   ```

## Next Step

After you perform emulation, generate the Profile Summary and Timeline Trace reports for further debugging and optimization. For details on how to generate and view these reports, refer to the [Generating Profile and Trace Reports](./ProfileAndTraceReports.md) lab.

</br>
<hr/>
<p align="center"><b><a href="/docs/vitis-getting-started/">Return to Getting Started Pathway</a></b> — <b><a href="./README.md">Return to Start of Tutorial</a></b></p>

<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
