<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ System Design Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>


# Versal Custom Platform Integration using Vitis Subsystem

***Version: Vivado and Vitis 2026.1***

This tutorial demonstrates key features in AMD® 2026.1 tools for designing and verifying AI Engine and HLS based DSP subsystem and deploy it on a custom platform.

To show the design, verification and integration activities, the tutorial use simple building blocks to make it easy to track the design results and processing data with visual inspection.
The example blocks are verified with basic test benches to demonstrate using the verification features, and it's advised that user complement with more advanced tests to reach acceptable coverage.

***Note:*** Please use the Github Issue reporting tool to provide feedback and report issues.

### Vitis subsystem flow
The tutorial follow the Vitis subsystem flow to completely build to hardware. Though some of the steps can be performed concurrently, the tutorial goes through the steps in sequence.

![](documentation/readme_files/vss_flow_for_tutorial.svg)


[Skip directly to the getting started](#getting-started)

### Features demonstrated in this tutorial

#### DSP development - AI Engine and Vitis Subsystem development

| Category | Feature                        | Example in this tutorial   | User guide reference
| -------- | -------------------------------|--------|-------------------------------------------------
| AIE Development   | AIE Kernel programming using AIE API        | [Datamover examples and 16 tap FIR filter with sliding mul ops](./vss/ip/aie/README.md)     | [AI Engine Kernel and Graph Programming Guide (UG1079)](https://docs.amd.com/r/en-US/ug1079-ai-engine-kernel-coding/Multiple-Lanes-Multiplication-sliding_mul)
| AIE Development   | Optimizing loops for AIE Kernel             | [AIE Optimizations](./vss/ip/aie/README_AIE_OPTIMIZATIONS.md)     | [AI Engine Kernel and Graph Programming Guide (UG1079)](https://docs.amd.com/r/en-US/ug1079-ai-engine-kernel-coding/Multiple-Lanes-Multiplication-sliding_mul)
| AIE Development   | AIE Graph programming with subgraphs        | [Graph with subgraphs](./vss/ip/aie/src/graphs/mygraph2.h)        | [AI Engine Kernel and Graph Programming Guide (UG1079)](https://docs.amd.com/r/en-US/ug1079-ai-engine-kernel-coding/Introduction-to-Graph-Programming)
| Vitis Application | Vitis Functional Simulation in Matlab       | [Simulate AIE graph in Matlab](./vss/matlab/README.md)     | [Vitis Functional Simulation User Guide (UG1684)](https://docs.amd.com/r/en-US/ug1864-vitis-functional-simulation)
| Vitis Application | Vitis Functional Simulation in Python       | [Simulate HLS kernel in Python](./vss/python/README.md)     | [Vitis Functional Simulation User Guide (UG1684)](https://docs.amd.com/r/en-US/ug1864-vitis-functional-simulation)
| Vitis Application | AIE Kernel performance with Vitis Analyzer  | [Checking datamovers with AIE simulation](./vss/ip/aie/README_AIESIM.md)     |  [Vitis Reference Guide (UG1702)](https://docs.amd.com/r/en-US/ug1702-vitis-accelerated-reference/Working-with-the-Analysis-View-Vitis-Analyzer)
| VSS Linking       | Creating a Vitis Subsystem                  | [Design and compile a VSS component](./vss/README.md)     | [Linking a VSS component with Vitis (UG1701)](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Linking-a-VSS-Component)
| VSS Verification  | Vitis Subsystem Simulation                  | [Simulate AIE+PL in XSIM with RTL testbench](./vss/cosim/README.md)     | [Simulation with Vitis Subsystem (UG1701)](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Simulation-with-the-Vitis-Subsystem)
| VSS Verification  | VSS performance with Vitis Analyzer         | [Checking AIE FIR filter after VSS simulation](./vss/cosim/README.md#Analyzing-FIR-filter-response)     |  [Vitis Reference Guide (UG1702)](https://docs.amd.com/r/en-US/ug1702-vitis-accelerated-reference/Working-with-the-Analysis-View-Vitis-Analyzer)

#### Hardware development - Creating, linking, and implementing the hardware platform

| Category | Feature    | Example in this tutorial   | User guide reference
| -------- | -----------|----------------------------|-------------------------------------------------
| Vivado Design         | Creating a custom extensible platform                 | [Vivado extensible platform](./vivado/README.md)        |  [Extensible hardware platforms (UG1701)](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Extensible-Hardware-Platforms)
| Vitis Linking         | Linking the subsystem to extensible platform          | [Vitis Linking](./vitis/README.md)                      |  [Linking the VSS component to the platform (UG1701)](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Linking-the-VSS-Component-to-the-Platform)
| Vivado Optimizations  | Importing VMA and implementing the hardware platform  | [Implement design Vivado](./vivado/Finalize_Vivado.md)  |  [Vitis export to Vivado flow detailed example (UG1701)](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Vitis-Export-to-Vivado-Flow-Detailed-Example)

#### Embedded development - Adding custom Linux, devicetree overlays, and cross-compile host application

***Important! This tutorial use Yocto/EDF by default. Petalinux option has not yet been aligned to the changes to accommodate the EDF!***

| Category | Feature    | Example in this tutorial                      | User guide reference
| -------- | -----------|-----------------------------------------------|-------------------------------------------------
| HW/SW Handoff         | Create System Device Tree handoffs with SDTGen/Lopper | [Generating device tree from implemented hardware design](./linux/sdtgen/README.md)     |  [SDTGen README](https://github.com/Xilinx/system-device-tree-xlnx/blob/xlnx_rel_v2026.1/README.md)
| Generate Machine Config | Prepare custom machine for Yocto/EDF                | [EDF Development Flows](./linux/yocto/README.md)     |  [EDF Running gen-machine-conf](https://edf.docs.amd.com/en/latest/shel/running-gen-machine-conf.html#running-gen-machine-conf)
| Prepare OS            | Add custom layers for EDF                             | [Introduction to development flows with EDF](./linux/yocto/README.md)     |  [Building Components From Source](https://edf.docs.amd.com/en/latest/yocto/building-components-from-source.html)
| Prepare OS            | Prepare and build custom Linux with Yocto/EDF         | [Introduction to development flows with EDF](./linux/yocto/README.md)     |  [](https://edf.docs.amd.com/en/latest/development-flows-how-it-all-works.html#custom-hardware-development)
| Prepare OS            | Prepare and build custom Linux with Petalinux         | [Preparing a custom Linux environment](./linux/README.md)     |  [PetaLinux reference guide (UG1144)](https://docs.amd.com/r/en-US/ug1144-petalinux-tools-reference-guide)
| Vitis SW Platform     | Create Vitis platform component including devicetree overlay **(ONLY for Petalinux or Baremetal!)** | [Vitis platform component](./vitis/README.md)     |  [Create a platform component from XSA](https://docs.amd.com/r/en-US/ug1400-vitis-embedded/Creating-a-Platform-Component-from-XSA)
| Vitis SW Application  | Create host application                       | [Create and crosscompile host applications](./ps_apps/README.md)     |  [Host application development overview (UG1701)](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Host-Application-Development)
| Vitis Package         | Integrate the system and package to SD cards  | [Package the design with Vitis](./vitis/Package.md)     |  [Integrating the System (UG1701)](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Integrating-the-System)
| Prepare SD card       | Copy files to SD card image                   | [Update the SD card files with wic](./linux/yocto/README.md#Update-SD-card)     |  [Inspecting and modifying the wic image](https://edf.docs.amd.com/en/latest/ref/disk-image-layout.html#inspecting-and-modifying-the-wic-image)


## Detailed design description

The use case for the lab is a custom extensible Vivado Block Design platform with RTL Modules combined with a Vitis Metadata Archive (VMA) containing a simple AI Engine and HLS design integrated following Vitis Export to Vivado Flow.
To headstart the signal processing algorithm development, we introduce the Vitis Subsystem (VSS) component that by targeting a device part number instead of a platform enables working independently of the platform.
Once the VSS is ready for integration, it can be added to an extensible platform, similar to linking AIE graphs and PL kernels, using v++ linker config.
To complete the design, we follow the Vitis Export to Vivado Flow, which enables the user to finalize the design in Vivado with full control of the synthesis and implementation steps before writing the fixed XSA.

The example design will go through the steps of creating a small AMD® Versal™ System Example Design consisting of:
 - Preparing a VSS Component
   - Adding AIE, HLS and RTL components
     - Showcase AIE+HLS functional verification with Matlab VFS.
     - Showcase HLS kernel functional verification with Python VFS.
 - Preparing a Custom Vivado Platform and export an extensible XSA demonstrated as build alternatives
    - Flat (non-BDC) examples targeting VCK190 and VEK280.
    - Segmented configuration examples targeting VCK190 (vck190seg) and VEK385. ***Note:*** VRK160 is early access, contact FAE for details.
 - Integrate the VSS Component and additional HLS Kernels using Vitis Linker
 - Exporting an AIE/PL VMA Subsystem
 - Import AIE/PL VMA Subsystem to Vivado
   - Run synthesis and implementation.
   - Generate a fixed XSA.
 - Configure and build embedded Linux sysroot, image and boot artifacts
   - Use Yocto to generate Linux. (Optional: Use Petalinux to generate custom Linux.)
 - Prepare Vitis Software platform
   - Use Vitis Python CLI for automation.
   - Apply devicetree overlays.
   - Configure Baremetal BSP and low level driver. (Preparation for future updates).
 - Create host applications
   - Crosscompile and link Linux userspace application executable.
 - Package to a SD Card
   - Adds boot files, image, root filesystem, and user apps to a bootable SD card.

## Conceptual description of the design and workflow arrangement used in the tutorial
To demonstrate how a system of RTL, HLS and AIE kernels can be arranged and integrated using Vitis, the tutorial use a few easy to understand building blocks. To highlight the differences between Vitis Subsystem component and Vitis kernels, the system have intentionally been decomposed for this.
The design structure and build scripts are prepared to allow for modifications, like moving, adding or removing various blocks across the Vitis Region, VSS component and Vivado block design.

***Note:*** *The AI Engine graph should reside inside the VSS component if VSS is used.* This is to enable simulation of AIE Graphs with PLIO interfaces in a RTL testbench with the VSS simulation.

### Functional description of the hardware design

As the tutorial provide several AI Engine kernel examples demonstrating various techniques, the actual configuration can deviate depending on the [my_dm_graph](./vss/ip/aie/src/graphs/dm_graph.h) setup.<br>
The user is encouraged to experiment with choosing different datamover examples by replacing the kernels in the datamover graph.

This figure illustrate a functional description of the design. It also marks which blocks are placed in the VSS component and its relation to the Vitis region.

![](documentation/readme_files/design_description.svg)


### Design arrangement from a build flow perspective
This figure describe the order of which the hardware design components are arranged and compiled.

![](documentation/readme_files/vss_design_overview.svg)


## Getting Started

### Prerequisites
To avoid tool environment interference between Vitis/Vivado and Yocto/EDF tools, each build activity loads the necessary setup where applicable. For Vitis and Vivado, this is handled by providing `XILINX_VITIS=<path Vitis install>` either as environment variable or via Makefile argument.
```
export XILINX_VITIS=<Vitis_Installation_Path>
```

For Yocto/EDF, the tutorial has the same prerequisites as [AMD Repo Manifest](https://github.com/Xilinx/Yocto-manifests).
The tools are automatically downloaded and configured by the tutorial Makefiles using to a local disk `TMP_DIR=tmp/<your_folder>`.
To avoid conflicts with other users, boards, or tool versions, the exact location is resolved by `LINUX_TMP_DIR` in [./linux/yocto/Makefile#16](./linux/yocto/Makefile#16)
More details on configuration and build options are described in [Yocto instructions](./linux/yocto/README.md)

In addition, `wic` tool requires the `mtools` package installed. This contains the `mcopy` command which is used to modify `fat32` partitions during the image deployment or SD card image creation. `mtools` can be installed through conventional Linux distribution package management.

### Build instructions
Makefiles are provided to build everything from the lab top folder. It will automatically compile all required RTL, AIE and HLS components as required by VSS during linking.

From top folder, run:
```
make all XILINX_VITIS=<Vitis_Install_Path> BOARD_NAME=<vck190 | vek280 | vck190seg | vek385 > VITIS_FLOW=<vma | vitis>
```
If BOARD_NAME is omitted, the build default to vck190.
If VITIS_FLOW is omitted, the build use vma = Vitis Export to Vivado.
All other values to `VITIS_FLOW` will pick Vitis Integrated flow which use v++ link to implement the fixed XSA.


Alternatively run step by step, by choosing from the following:
```
make vss vivado_platform vitis_ip vma_export vivado_fixed sdtgen linux ps_apps package sdcard_image XILINX_VITIS=<Vitis_Install_Path>
```

To build using prebuilt embedded strategy, the order is altered to create the embedded prebuilt boot artifacts. This reduced the build time for consecutive iterations as the first steps can be omitted.
```
# Build platform independent VSS
make vss XILINX_VITIS=<Vitis_Install_Path>
# Prebuild embedded:
make vivado_base sdtgen linux zocl_dtsi XILINX_VITIS=<Vitis_Install_Path>
# Continue build custom design
make vivado_platform vitis_ip vma_export vivado_fixed ps_apps package sdcard_image XILINX_VITIS=<Vitis_Install_Path>
```
***Note:*** Building the VSS is done first to explore the DSP side without investing effort with platforms.

#### Navigation helper to the makefile steps

All steps are supported with scripts and pre-built sources to give user time to inspect the results and explore the results.
The user is encouraged to modify/change/replace parts after first running through these steps once.
 - [1. Create a Vitis Subsystem component](./vss/README.md)
 - [2. Develop a custom Vivado extensible platform](./vivado/Vivado.md)
 - [3. Compile additional Vitis blocks (Non VSS)](./vitis/README.md)
 - [4. Importing and integrating VSS and Vitis blocks to extensible platform](./vitis/README.md)
 - [5. Import VMA and finalize the design in Vivado](./vivado/Finalize_Vivado.md)
 - [6. Update system device tree](./linux/sdtgen/README.md)
 - [7. Configure and build Linux](./linux/README.md)
 - [8. Compile and build Linux host applications](./ps_apps/README.md)
 - [9. Package design](./vitis/Package.md)
 - [10. Update SD card](./linux/yocto/README.md#Update-SD-card)
 - [11. Run the design on hardware](#run-on-hardware)



## Testing the design on a board

### Run on hardware
***Note:*** This example assumes vck190 board was used

  1. Write the image from `[project-root]/linux/my-vck190-versal/images/linux/my-vck190-versal/edf-linux-disk-image-my-vck190-versal.rootfs.wic.xz` to an SD-card
  2. Put the SD-card in the board's Versal SD-card slot (board's top SD-card slot closest to the bracket).
  3. Connect the included USB-cable between the board (Middle bottom of the bracket) and a computer:
     - Usually you will see 3 serial ports in your device manager:
       - One for the system controller device.
       - Two for Versal; however only one of the Versal serial ports are in use.
       - To see the serial ports, the board does not need to be powered-ON, the physical USB connection should be enough!
     - Connect to the serial port(s) by using a terminal emulator like Putty (Windows) with the following settings:
       - 115200 baud
       - 8 data bits
       - 1 stop bit
       - Parity none
       - Flow control XON/XOFF
     - If unsure which port to use, open all 3 serial ports to see which one is the correct Versal serial port where you can follow the Versal-boot and interact later on.
  4. Power-UP:
     - It will first boot-up up the system controller, next it will start the Versal boot. 
     - Only one of the Versal serial ports will give you the Linux login prompt after booting.
  5. Continue to "Execution & Results".

### Execution & Results
The boot process first launch U-Boot to scan for storage devices and brings up a U-Boot menu. It default to mmc 0, which refers to the Versal SD Card, with 5 second timeout.
Next follows a menu to select EDF Linux (default) or EDF Xen, again with 5 second timeout.
This launches the Linux boot and ends with a login prompt.


The first time you login with user `amd-edf` and you are instructed to change password (This is also the `sudo` password):

```
amd-edf login: amd-edf
You are required to change your password immediately (administrator enforced).
New password:
Retype new password:
AMD® Embedded Development Framework

WARNING: Embedded Development Framework is a reference Yocto Project
distribution that intended for testing and development purposes only.
```

Check that the device is ready using the `xrt-smi` tool:
```
amd-edf:~$ xrt-smi examine | tail -n 8

Device(s) Present
|BDF             |Shell  |Logic UUID  |Device ID     |Device Ready*  |
|----------------|-------|------------|--------------|---------------|
|[0000:00:00.0]  |edge   |0x0         |user(inst=0)  |Yes            |


* Devices that are not ready will have reduced functionality when using XRT tools
```

Run the host application that run basic testing using the counter values to feed the AIE data movers and reads the subtractor values for each data path.
The expected output should look like this:
```
amd-edf:~$ sudo ./aie_dly_test.exe my.xclbin

We trust you have received the usual lecture from the local System
Administrator. It usually boils down to these three things:

    #1) Respect the privacy of others.
    #2) Think before you type.
    #3) With great power comes great responsibility.

For security reasons, the password you type will not be visible.

Password:
Initializing ADF API...
INFO:    argc: 3
PASSED:  auto my_device = xrt::device(0)
XAIEFAL: INFO: Resource group Avail is created.
XAIEFAL: INFO: Resource group Static is created.
XAIEFAL: INFO: Resource group Generic is created.
PASSED:  auto xclbin_uuid = my_device.load_xclbin(my.xclbin)
PASSED:  auto my_rtl_ip = xrt::ip(my_device, xclbin_uuid, "subtractor:{subtractor_0}")
PASSED:  auto my_graph  = xrt::graph(my_device, xclbin_uuid, "mygraph_top")
PASSED:  Create my_mm2s instance
PASSED:  Create my_s2mm instance
PASSED:  Setup input buffer objects
PASSED:  Setup output buffer objects
PASSED:  Mapping input buffer objects
PASSED:  auto out_0_bo_mapped = out_0_bo.map<TYPE_DATAO*>()
PASSED:  Declare in_ref with 3072 samples
INFO:    Missing input data file, will generate default data
PASSED:  Generating input stimuli as ramp
in0 = 0+i0
in0 = 0+i1
in0 = 1+i0
in0 = 0+i2
in0 = 2+i0
in0 = 0+i3
in0 = 3+i0
in0 = 0+i4
in0 = 4+i0
in0 = 0+i5
in0 = 5+i0
in0 = 0+i6
in0 = 6+i0
in0 = 0+i7
in0 = 7+i0
in0 = 0+i8
in0 = 8+i0
in0 = 0+i9
in0 = 9+i0
in0 = 0+i10
PASSED:  in_0_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE)
PASSED:  auto in_0_run = in_0(in_0_bo, nullptr, 512)
PASSED:  auto out_run = out(out_0_bo, nullptr, 512)
PASSED:  my_graph.reset()
PASSED:  my_graph.run(-1)
Poll subtractor register
  Value Reg0:  240
  Value Reg1:  505c
  Value Reg2:  1ac
  Value Reg3:  b3
Poll subtractor register
  Value Reg0:  240
  Value Reg1:  1854
  Value Reg2:  15a
  Value Reg3:  cf
Poll subtractor register
  Value Reg0:  240
  Value Reg1:  d004
  Value Reg2:  1b0
  Value Reg3:  d7
Poll subtractor register
  Value Reg0:  240
  Value Reg1:  4d70
  Value Reg2:  1ae
  Value Reg3:  d1
Poll subtractor register
  Value Reg0:  240
  Value Reg1:  f640
  Value Reg2:  19a
  Value Reg3:  d5
Poll subtractor register
  Value Reg0:  240
  Value Reg1:  5370
  Value Reg2:  1b0
  Value Reg3:  d5
Poll subtractor register
  Value Reg0:  240
  Value Reg1:  e1e8
  Value Reg2:  1a2
  Value Reg3:  d5
Poll subtractor register
  Value Reg0:  240
  Value Reg1:  3458
  Value Reg2:  164
  Value Reg3:  d1
Poll subtractor register
  Value Reg0:  240
  Value Reg1:  9fac
  Value Reg2:  1a0
  Value Reg3:  ad
Poll subtractor register
  Value Reg0:  240
  Value Reg1:  3018
  Value Reg2:  1ac
  Value Reg3:  d5

INFO:    Waiting for DMA end...

PASSED:  in_0_run.wait()
PASSED:  out_0_run.wait()
PASSED:  out_0_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE)
out_0_bo_mapped[0] = b5+i0out_0_bo_mapped[1] = b6+i0out_0_bo_mapped[2] = b6+i0out_0_bo_mapped[3] = b7+i0out_0_bo_mapped[4] = b8+i0out_0_bo_mapped[5] = b9+i0out_0_bo_mapped[6] = b9+i0out_0_bo_mapped[7] = ba+i0out_0_bo_mapped[8] = bb+i0out_0_bo_mapped[9] = bc+i0out_0_bo_mapped[a] = bc+i0out_0_bo_mapped[b] = bd+i0out_0_bo_mapped[c] = 7e+i0out_0_bo_mapped[d] = 7f+i0out_0_bo_mapped[e] = 7f+i0out_0_bo_mapped[f] = 0+i0out_0_bo_mapped[10] = 1+i0out_0_bo_mapped[11] = 2+i0out_0_bo_mapped[12] = 2+i0out_0_bo_mapped[13] = 3+i0PASSED:  my_graph.end()

PASSED:  ./aie_dly_test.exe
vck190-versal:/run/media/BOOT-mmcblk0p1# 
```
The values presented when polling the registers show the propagation delay in clock cycles (hex value) for each data mover path.
When buffer API is used, the buffer size affects the delay. In the tutorial the buffer size is set to 256 samples.

| Poll number | Register | Value (hex) | Value (dec)
| -- | ----------- | ----- | -----
| 1  | Value Reg0  | 240   | 576
| 1  | Value Reg1  | 505c  | 20572
| 1  | Value Reg2  | 1ac   | 428
| 1  | Value Reg3  | b3    | 179
| 2  | Value Reg0  | 240   | 576
| 2  | Value Reg1  | 1854  | 6228
| 2  | Value Reg2  | 15a   | 346
| 2  | Value Reg3  | cf    | 207
| 3  | Value Reg0  | 240   | 576
| 3  | Value Reg1  | d004  | 53252
| 3  | Value Reg2  | 1b0   | 432
| 3  | Value Reg3  | d7    | 215
| 4  | Value Reg0  | 240   | 576
| 4  | Value Reg1  | 4d70  | 19824
| 4  | Value Reg2  | 1ae   | 430
| 4  | Value Reg3  | d1    | 209
| 5  | Value Reg0  | 240   | 576
| 5  | Value Reg1  | f640  | 63040
| 5  | Value Reg2  | 19a   | 410
| 5  | Value Reg3  | d5    | 213
| 6  | Value Reg0  | 240   | 576
| 6  | Value Reg1  | 5370  | 21360
| 6  | Value Reg2  | 1b0   | 432
| 6  | Value Reg3  | d5    | 213
| 7  | Value Reg0  | 240   | 576
| 7  | Value Reg1  | e1e8  | 57832
| 7  | Value Reg2  | 1a2   | 418
| 7  | Value Reg3  | d5    | 213
| 8  | Value Reg0  | 240   | 576
| 8  | Value Reg1  | 3458  | 13400
| 8  | Value Reg2  | 164   | 356
| 8  | Value Reg3  | d1    | 209
| 9  | Value Reg0  | 240   | 576
| 9  | Value Reg1  | 9fac  | 40876
| 9  | Value Reg2  | 1a0   | 416
| 9  | Value Reg3  | ad    | 173
| 10 | Value Reg0  | 240   | 576
| 10 | Value Reg1  | 3018  | 12312
| 10 | Value Reg2  | 1ac   | 428
| 10 | Value Reg3  | d5    | 213


***Note:*** The value for Reg1 is changing significantly between the polling, indicating that this path is not working as intended. This is a known bug and is being investigated.


## Troubleshooting and common issues
This tutorial will stress test that the design environment is correctly setup. To enable both Vivado/Vitis and Yocto/EDF shell environment, the `Makefile` automatically manages which environment is to be loaded. For this to work, the shell calling the `Makefile` need have as clean setup as possible.
Performing a clean build from fresh clone is most likely to succeed. When iterating builds, there are several ways to speed up the build process, however not all these shortcuts have been fully tested, so there are few situations where old artifacts interfer with the makefile build recipies and require manual tweaks.
The intent with the tutorial is to serve as a learning platform and act as inspiration. Improvement suggestions are welcome and will be used if it does not divert the design from the intent and workflow robustness.

Typical root causes to failed builds are:
 - Missing Vitis/Vivado installation components
    - Device definitons missing (caused by selective install or attempting to run EA devices). Check the install and ensure beta devices are enabled.
    - Library dependencies missing. Check [Installing the Vitis Software Platform (UG1742)](https://docs.amd.com/r/en-US/ug1742-vitis-release-notes/Installing-the-Vitis-Software-Platform)
    - License missing. Check [Licensing (UG1742)](https://docs.amd.com/r/en-US/ug1742-vitis-release-notes/Licensing)
 - Issues caused by insufficient temporary storage space
    - Wrong place for temporary Yocto build artifacts
      - Attempting to use a NFS mounted drive for Yocto will cause issues. Use local mounts like `/tmp` or `/scratch` instead, preferrably using fast drives like SSD. Refer to the `TMP_DIR` variable to control the location.
    - Yocto reports `OSError: [Errno 9] Bad file descriptor`:
      - Check if `/var` is full. Though not used directly, temporary logs are stored in `/var` and when full it cause hard to root cause issues.
 - Other environment setup and tools
    - Several steps involve specific Python versions and Python packages being installed. Using Python virtual environment provide a neat method to try out packages without involving admin privileges to install. See [Creation of virtual environments](https://docs.python.org/3/library/venv.html) for ideas.
    - The `wic` tool require mtools package to manipulate fat32 partitions, if missing this error is produced: `ERROR: Can't find executable 'mcopy'`. Install or load the mtools to the environment prior to building the tutorial.
 - Analyzing the logs
    - Due to the setup of the `Makefile` recipes and temporary folders created by the various tools, the log files are sometimes buried in directory structure. Pay attention to the error messages and logs to find the location of important outputs.
    - If `make all` option is used, check which steps completes ok and when rebuilding, use the rules one by one up to the failing step. It's possible to do a new clone and add the last known good outputs and inject to the cloned design to do deeper analysis and focus on the logs for the failing step.


## References
The following documents provide supplemental information for this tutorial.

### [Vitis Unified Software Documentation Landing Page](https://docs.amd.com/v/u/en-US/ug1416-vitis-documentation)

- [Vitis Unified Software Platform Documentation: Embedded Software Development (UG1400)](https://docs.amd.com/r/en-US/ug1400-vitis-embedded/Getting-Started-with-Vitis)
- [Vitis Accelerated Embedded User Guide (UG1701)](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Getting-Started-with-Vitis)
- [Vitis Reference Guide (UG1702)](https://docs.amd.com/r/en-US/ug1702-vitis-accelerated-reference/Navigating-Content-by-Design-Process)
- [Vitis High-Level Synthesis User Guide (UG1399)](https://docs.amd.com/r/en-US/ug1399-vitis-hls/Introduction)

### AI Engine Documentation
- [AI Engine Tools and Flows User Guide (UG1076)](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment)
- [AI Engine Kernel and Graph Programming Guide (UG1079)](https://docs.amd.com/r/en-US/ug1079-ai-engine-kernel-coding)

### [AMD® Embedded Development Framework (EDF) Landing Page](https://edf.docs.amd.com/en/latest/index.html)

- [XRT Documentation](https://xilinx.github.io/XRT/master/html/index.html): Explains general XRT API calls used in the PS Host Application.

### [Boot Architecture For AMD® Evaluation Boards](https://edf.docs.amd.com/en/latest/ref/common-specifications.html#boot-architecture-for-amd-evaluation-boards)

- [XRT Documentation](https://xilinx.github.io/XRT/master/html/index.html): Explains general XRT API calls used in the PS Host Application.
- [XRT Github Repo](https://github.com/Xilinx/XRT): Contains the XRT source code.
- [XRT AIE API](https://github.com/Xilinx/XRT/blob/master/src/runtime_src/core/include/experimental/xrt_aie.h): Documents the AI Engine XRT API calls


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
