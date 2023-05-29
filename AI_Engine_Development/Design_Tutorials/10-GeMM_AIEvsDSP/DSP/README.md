<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>2023.1 Versal Matrix Multiplication using DSP58 Tutorial (XDXX)</h1>
   </td>
 </tr>
</table>

# GeMM DSP58 Implementation

## Table of Contents

[Building the Design](#Building-the-Design)

[Hardware Design Details](#Hardware-Design-Details)

[Software Design Details](#Software-Design-Details)

[Performance Details](#Performance-Details)

## Building the Design

<details>
<summary>Design Build</summary> 
	
### Design Build

In this section, you learn to build and run the Matrix Multiplication design using the DSP58 Engines in Versal device. You will compile the design and integrate it into a larger system design (including the PS host application).

The Makefile used to build the design takes 2 user inputs from command line. These are -
TARGET (hw/hw_emu)
GEMM_SIZE (32, 64, 128, 256, 512 or 1024)

Based on these inputs, the design flow will generate a new directory (called `build/`). Underneath are subdirectories named gemm_GEMM_SIZExGEMM_SIZExGEMM_SIZE.  For example if GEMM_SIZE is given as 64, a subdirectory named gemm_64x64x64 will be created under build directory. Underneath, `hw_emu/` and/or `hw/` subfolders will be created. These folders contain a host app executable and the builds targeted to `hw` or `hw_emu` respectively. The `hw_emu/` subfolder contains the build for hardware emulation. The `hw/` subfolder contains the build for a hardware run on a VCK190 board.

</details>

<details>
<summary>Make Steps</summary> 
	
### Make Steps

To run the following `make` steps (for example, `make kernels`, `make xsa`, `make application`, and `make package`), you must be in the `gemm_dsp58/` folder. The following options can be specified in the `make` steps. Instructions for how to apply them are provided later in this section.

`TARGET:` This option can be set to `hw` or `hw_emu` to build the design in the hardware or hardware emulation flow. The default is `hw_emu`.

`GEMM_SIZE:` This option can be set to 32, 64, 128, 256, 512 or 1024

The Makefile uses the following directory references:

```
## Relative directory
RELATIVE_PROJECT_DIR := ./
PROJECT_REPO := $(shell readlink -f $(RELATIVE_PROJECT_DIR))
DESIGN_REPO  := $(PROJECT_REPO)/design
PL_SRC_REPO  := $(DESIGN_REPO)/pl_src
CONSTRAINTS_REPO  := $(PL_SRC_REPO)/constraints
HOST_APP_SRC := $(DESIGN_REPO)/host_app_src
SYSTEM_CONFIGS_REPO    := $(DESIGN_REPO)/system_configs
VIVADO_METRICS_SCRIPTS_REPO := $(DESIGN_REPO)/vivado_metrics_scripts

BASE_BLD_DIR := $(PROJECT_REPO)/build_$(PL_FREQ)
GEMM_BLD_DIR     := $(BASE_BLD_DIR)/gemm_$(MAT_DIMS)
BUILD_TARGET_DIR := $(GEMM_BLD_DIR)/$(TARGET)

VIVADO_REPORTS_REPO := $(PROJECT_REPO)/vivado_reports_dir
BLD_VIVADO_REPORTS_DIR := $(VIVADO_REPORTS_REPO)/gemm_$(MAT_DIMS)

EMBEDDED_PACKAGE_OUT := $(BUILD_TARGET_DIR)/package
EMBEDDED_EXEC_SCRIPT := run_script.sh

```

</details>

<details>
<summary>Build the Entire Design with a Single Command</summary>

### Build the Entire Design with a Single Command

If you are already familiar with Vitis kernel compilation flows, you can build the entire design with one command: 

```bash
make run (default TARGET=hw_emu, GEMM_SIZE=64) 
```

or 

```bash
make run TARGET=hw (Target is hardware, GEMM_SIZE=64)
```

This command runs the `make kernels`, `make xsa`, `make application`, `make package`, and `make run_emu` steps for hardware emulation or to run on hardware (VCK190 board) depending on the `TARGET` you specify. The settings also apply to individual make steps listed below.

The generated files are placed under an individual directory: `$(BUILD_TARGET_DIR)/`. Each `make` step to build the design is specified in the following sections. These sections also detail the options used and the location of input and output files in each case.

</details>

<details>

See [this page](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/v-Command) for a detailed description of all Vitis compiler switches. The following table provides a summary of the switches used. 


|Switch|Description|
|  ---  |  ---  |
|--target \| -t [hw\|hw_emu]|Specifies the build target.|
|--platform \| -f|Specifies the name of a supported acceleration platform as specified by the $PLATFORM_REPO_PATHS environment variable or the full path to the platform XPFM file.|
|--save-temps \| -s|Directs the Vitis compiler command to save intermediate files/directories created during the compilation and link process. Use the `--temp_dir` option to specify a location to write the intermediate files to.|
|--temp_dir <string>|This allows you to manage the location where the tool writes temporary files created during the build process. The temporary results are written by the Vitis compiler, and then removed, unless the `--save-temps` option is also specified.|
|--verbose|Display verbose/debug information.|
|--compile \| -c|Required for compilation to generate XO files from kernel source files.|
|--kernel \<arg\>\|-k \<arg\>|Compile only the specified kernel from the input file. Only one -k option is allowed per Vitis compiler command.|
|-D \| --define  \<Macro Name\>=\<value\>|Defines Macros for the compiler.|
|--output \| -o|Specifies the name of the output file generated by the V++ command. The kernel output should be XO.|

Following RTL files are used in this design

```
${PL_SRC_REPO}/rtl/BDELAY.vhd
${PL_SRC_REPO}/rtl/FIXGEMM.vhd
${PL_SRC_REPO}/rtl/SDELAY.vhd
${PL_SRC_REPO}/rtl/sfixed_pkg.vhd
${PL_SRC_REPO}/rtl/cfixed_pkg.vhd
${PL_SRC_REPO}/rtl/DSP_GW.vhd
${PL_SRC_REPO}/rtl/FIXGEMM_WRAPPER.vhd
${PL_SRC_REPO}/rtl/control_logic.sv
${PL_SRC_REPO}/rtl/gemm_top.sv
${PL_SRC_REPO}/rtl/ps_slave.sv
${PL_SRC_REPO}/rtl/DSP_data_controller.sv
${PL_SRC_REPO}/rtl/op_uram.sv
${PL_SRC_REPO}/rtl/row_uram.sv
${PL_SRC_REPO}/rtl/col_uram.sv
${PL_SRC_REPO}/rtl/gemm_large_ocm.sv
${PL_SRC_REPO}/rtl/partial_sum_bram.sv
${PL_SRC_REPO}/rtl/synchronizer.sv

```

<!---
(For hw_emu step, Row and Column URAMs are initialized to reduce simulation run time.) These files are located under $(PL_SRC_REPO)/mem_init_files/init_files_GEMM_SIZExGEMM_SIZExGEMM_SIZE folder)
--->

$(CONSTRAINTS_REPO)/gemm_dsp58.tcl provides constraints for synthesis and implementation. 

Following is the output xo file

```
$(PROJECT_REPO)/build/gemm_GEMM_SIZExGEMM_SIZExGEMM_SIZE/gemm_large_ocm.xo

```

</details>

<details>
<summary>make kernels: Generates the PL Kernels </summary> 

### make kernels: Generates the PL Kernels

This step uses the RTL and mem_init_files specified above to generate the PL kernel (gemm_large_ocm.xo)

</details>

<details>
<summary>make xsa: Using the Vitis Tools to Link PL Kernels with the Platform</summary> 
 
### make xsa: Using the Vitis Tools to Link HLS Kernels with the Platform

After the kernel is generated, you can use the Vitis compiler to link it with the platform to generate an XSA file. 

The Vitis tools allow you to integrate the kernels into an existing extensible platform. This is an automated step from a software developer perspective where the platform chosen is provided by the hardware designer (or you can opt to use one of the many extensible base platforms provided by Xilinx and the Vitis tools build the hardware design and integrate the kernels into the design).
 
The command to run this step is shown as follows 

```
make xsa TARGET=<hw/hw_emu> GEMM_SIZE=<64,128,256,512,1024>
``` 

The expanded command is as follows:

```
cd $(BUILD_TARGET_DIR);	\

v++ -l --platform xilinx_vck190_base_202310_1 --save-temps --temp_dir $(BUILD_TARGET_DIR)/_x \
   --verbose -g --clock.freqHz 500000000:gemm_large_ocm_0 --clock.defaultTolerance 0.001 \
   --config $(SYSTEM_CONFIGS_REPO)/gemm.cfg --vivado.prop fileset.sim_1.xsim.simulate.log_all_signals=true \
   --vivado.prop run.synth_1.{STEPS.SYNTH_DESIGN.ARGS.CONTROL_SET_OPT_THRESHOLD}={16} \
   --vivado.prop run.synth_1.{STEPS.SYNTH_DESIGN.ARGS.KEEP_EQUIVALENT_REGISTERS}={true} \
   --xp vivado_prop:run.impl_1.STEPS.PLACE_DESIGN.TCL.PRE=$(CONSTRAINTS_REPO)/gemm_dsp58.tcl
   -t hw_emu -o $(BUILD_TARGET_DIR)/gemm.hw_emu.xclbin $(PROJECT_REPO)/build/gemm_GEMM_SIZExGEMM_SIZExGEMM_SIZE/gemm_large_ocm.xo
```

See [this page](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Building-the-Device-Binary) for a detailed description of Vitis linking options. The following table provides a summary of the switches used. 

|Switch|Description|
|  ---  |  ---  |
|--platform \| -f|Specifies the name of a supported acceleration platform as specified by the $PLATFORM_REPO_PATHS environment variable or the full path to the platform XPFM file.|
|--save-temps \| -s|Directs the V++ command to save intermediate files/directories created during the compilation and link process. Use the `--temp_dir` option to specify a location to write the intermediate files to.|
|--temp_dir <string>|This allows you to manage the location where the tool writes temporary files created during the build process. The temporary results are written by the Vitis compiler, and then removed, unless the `--save-temps` option is also specified.|
|--verbose|Display verbose/debug information.|
|--output \| -o|Specifies the name of the output file generated by the V++ command. In this design the outputs of the HLS/DSP kernels with their interfacing with the PL kernels are in XO files.|
|--vivado.prop \<arg\>|Specifies properties for the Vivado Design Suite to be used during synthesis and implementation of the FPGA binary (xclbin). See [this page](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/vivado-Options) for detailed Vivado options.|
|--profile.data [<kernel_name>\|all]:[<cu_name>\|all]:[<interface_name>\|all]\(:[counters\|all]\)|Enables monitoring of data ports through the monitor IPs. This option needs to be specified during linking. See [this page](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/profile-Options) for detailed profiling options.|
|--profile.trace_memory \<FIFO\>:\<size\>\|\<MEMORY\>[\<n\>]|When building the hardware target \(-t=hw\), use this option to specify the type and amount of memory to use for capturing trace data. See [this page](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/profile-Options) for detailed profiling options.|
|--config <config_file>|Specifies a configuration file containing V++ switches.|

The information to tell the linker how to connect the PL kernels together is described in a configuration file, `system_configs/gemm.cfg`. The file describes the overall connection scheme of the system.

```
[connectivity]
nk=gemm_large_ocm:1:gemm_large_ocm_0

[clock]
id=0:gemm_large_ocm_0.S_AXI_ACLK

[advanced]
## Disable Profiling in hw_emu so that it is faster...
param=hw_emu.enableProfiling=false
## Export the xsa of the design..
param=compiler.addOutputTypes=hw_export
param=compiler.worstNegativeSlack=-1.0
[vivado]
prop=run.synth_1.STRATEGY=Flow_PerfOptimized_high
prop=run.impl_1.STEPS.OPT_DESIGN.is_enabled=true
prop=run.impl_1.STEPS.OPT_DESIGN.ARGS.DIRECTIVE=Explore
prop=run.impl_1.STEPS.PLACE_DESIGN.ARGS.DIRECTIVE=ExtraTimingOpt
prop=run.impl_1.STEPS.PHYS_OPT_DESIGN.is_enabled=true
prop=run.impl_1.STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE=AggressiveExplore
#prop=run.impl_1.STEPS.ROUTE_DESIGN.ARGS.MORE OPTIONS=-tns_cleanup
prop=run.impl_1.STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE=AggressiveExplore
```

See [this page](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Compiler-Configuration-File) for a detailed description of the Vitis compiler configuration file. A summary of the configuration options used is provided in the following table. 


|Switch|Comment|
|  ---  |  ---  |
|--connectivity.nk|Number of kernels. `gemm_large_ocm:1:gemm_large_ocm_0` means that the Vitis compiler should instantiate one gemm_large_ocm kernel and name the instance `gemm_large_ocm_0`.|
|param=hw_emu.enableProfiling=false - This option disables profiing during hw_emu for faster run time|
|param=compiler.addOutputTypes=hw_export| This option tells the Vitis compiler that besides creating an XCLBIN file, it also outputs an XSA file which is needed to create a post-Vivado fixed platform for Vitis software development.|
param=compiler.worstNegativeSlack=-1.0 - This parameter sets 210ps tolerance for WNS
prop=run.synth_1.STRATEGY=Flow_PerfOptimized_high - This parameter sets Synthesis streategy
prop=run.impl_1.STEPS.OPT_DESIGN.is_enabled=true - This option enables opt design directive
prop=run.impl_1.STEPS.OPT_DESIGN.ARGS.DIRECTIVE=Explore - This option sets the value of opt design stage directive
prop=run.impl_1.STEPS.PLACE_DESIGN.ARGS.DIRECTIVE=ExtraTimingOpt - This option sets the value of place design directive
prop=run.impl_1.STEPS.PHYS_OPT_DESIGN.is_enabled=true - This option enables physical optimzation directive
prop=run.impl_1.STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE=AggressiveExplore - This option sets value of physical optimization directive
prop=run.impl_1.STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE=AggressiveExplore - This option sets value of route design directive

The Vitis™ compiler calls the Vivado™ IP integrator under the hood to build the design. The platform and kernels are input to the Vivado Design Suite, which produces a simulation XSA or an XSA after running place and route on the design. The point at which the XSA is produced from Vivado depends on the `-target` option set on the Vitis compiler command line. 

You can now view the Vivado project, which is located in the `$(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj` directory. You have now generated the XCLBIN file, `$(BUILD_TARGET_DIR)/gemm.hw_emu.xclbin`, that is used to execute your design on the platform.

</details>

<details>
<summary>make application: Compile the Host Application</summary> 

### make application: Compile the Host Application

You can compile the host application by following the typical cross-compilation flow for the Cortex A72 processor. To build the application, run the following command 

```
make application 
```
or

```
cd $(BUILD_TARGET_DIR);	\

aarch64-xilinx-linux-g++ -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong \
   -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$(SDKTARGETSYSROOT) -O -c \
   -std=c++14 -D__linux__ \
   -DM_LARGE=$(GEMM_SIZE) -DN_LARGE=$(GEMM_SIZE) -DL_LARGE=$(GEMM_SIZE) \
   -I$(SDKTARGETSYSROOT)/usr/include/xrt -I$(SDKTARGETSYSROOT)/usr/include -I$(SDKTARGETSYSROOT)/usr/lib -I$(HOST_APP_SRC)/$(MAT_DIMS) \
$(HOST_APP_SRC)/main.cpp -o $(BUILD_TARGET_DIR)/gemm_top_app.o \
   -L$(SDKTARGETSYSROOT)/lib -lxrt_coreutil

aarch64-xilinx-linux-g++  -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong \
   -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$(SDKTARGETSYSROOT) \
   $(BUILD_TARGET_DIR)/gemm_top_app.o -L$(SDKTARGETSYSROOT)/usr/lib -lxrt_coreutil \
   -o $(BUILD_TARGET_DIR)/gemm_dsp_xrt.elf
```


See [this page](https://xilinx.github.io/XRT/master/html/index.html) for XRT documentation. See [this page](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Programming-the-PS-Host-Application) for details of host application programming.


|Switch|Description|
|  ---  |  ---  |
|-O \| Optimize.| Optimizing compilation takes somewhat more time, and a lot more memory for a large function. With -O, the compiler tries to reduce code size and execution time, without performing any optimizations that can take a great deal of compilation time.|
|-D__linux__|
|-DXAIE_DEBUG|Enable debug interface capabilities where certain core status, event status, or stack trace can be dumped out.|
|-D\<Pre-processor Macro String\>=\<value\>|Pass Pre-processor Macro definitions to the cross-compiler.|
|-I \<dir\>|Add the directory `dir` to the list of directories to be searched for header files.|
|-o \<file\>|Place output in file `<file>`. This applies regardless of the output being produced, whether it be an executable file, an object file, an assembler file or preprocessed C code.|
|--sysroot=\<dir\>|Use `dir` as the logical root directory for headers and libraries. For example, if the compiler would normally search for headers in `/usr/include` and libraries in `/usr/lib`, it will instead search `dir/usr/include` and `dir/usr/lib`. This is automatically set by the `env_setup.sh` script|
|-l\<library\>|Search the library named `library` when linking. The 2D-FFT tutorial requires `adf_api_xrt` and `xrt_coreutil` libraries.|
|-L \<dir\>|Add directory `<dir>` to the list of directories to be searched for -l.|

The following is a description of the input sources compiled by the cross-compiler compiler command. 

|Inputs Sources|Description|
|  ---  |  ---  |
|$(HOST_APP_SRC)/main.cpp|Source application file for the `gemm_dsp_xrt.elf` that will run on an A72 processor.|
|$(HOST_APP_SRC)/matrix_A_data.h, matrix_B_data.h|Matrix A and B Data to be used for matrix multiplication.|
|$(HOST_APP_SRC)/output_data.h|Golden data to which DUT output will be compared.|

The following is a description of the output objects that results from executing the cross-compiler command with the above inputs and options. 

|Output Objects|Description|
|  ---  |  ---  |
|$(BUILD_TARGET_DIR)/gemm_dsp_xrt.elf|The executable that will run on an A72 processor.|

</details>

<details>
<summary>make package: Packaging the Design</summary> 
 
### make package: Packaging the Design

With the Kernel outputs created, as well as the new platform, you can now generate the programmable device image (PDI) and a package to be used on an SD card. The PDI contains all the executables, bitstreams, and configurations of the device. The packaged SD card directory contains everything to boot Linux, the generated applications, and the XCLBIN.

The command to run this step is as follows (default `TARGET=hw_emu`):

```
make package
``` 

or 

```
cp $(PROJECT_REPO)/run_script.sh $(BUILD_TARGET_DIR)/
cd $(BUILD_TARGET_DIR);	\

v++ -p -t hw --save-temps --temp_dir $(BUILD_TARGET_DIR)/_x -f xilinx_vck190_base_202310_1 \
   --package.rootfs $(XLNX_VERSAL)/rootfs.ext4 --package.kernel_image $(XLNX_VERSAL)/Image --package.boot_mode=sd \
   --package.out_dir $(BUILD_TARGET_DIR)/package --package.image_format=ext4 --package.sd_file $(BUILD_TARGET_DIR)/gemm_dsp_xrt.elf \
   $(BUILD_TARGET_DIR)/gemm.hw.xclbin
```

If the `XRT_ROOT` is set, the following Vitis compiler flags are also set:

```
   --package.sd_dir $(XRT_ROOT)
```

See [this page](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/package-Options) for more details about packaging the system.


|Switch|Description|
|  ---  |  ---  |
|--target \| -t [hw\|hw_emu]|Specifies the build target.|
|--package \| -p|Packages the final product at the end of the Vitis compile and link build process.|
|--package.rootfs \<arg\>|Where \<arg\> specifies the absolute or relative path to a processed Linux root file system file. The platform RootFS file is available for download from xilinx.com. Refer to the [Vitis Software Platform Installation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Software-Platform-Installation) for more information.|
|--package.kernel_image \<arg\>|Where \<arg\> specifies the absolute or relative path to a Linux kernel image file. Overrides the existing image available in the platform. The platform image file is available for download from xilinx.com. Refer to the [Vitis Software Platform Installation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Software-Platform-Installation) for more information.|
|--package.boot_mode \<arg\>|Where \<arg\> specifies <ospi\|qspi\|sd> Boot mode used for running the application in emulation or on hardware.|
|--package.image_format|Where \<arg\> specifies \<ext4\|fat32\> output image file format. `ext4` is the Linux file system and `fat32` is the Windows file system.|
|--package.sd_file|Where \<arg\> specifies an ELF or other data file to package into the `sd_card` directory/image. This option can be used repeatedly to specify multiple files to add to the `sd_card`.|

|Inputs Sources|Description|
|  ---  |  ---  |
|$(XRT_ROOT)|The PS host application needs the XRT headers in this folder to execute. Set in the `env_setup.sh`.|
|$(XLNX_VERSAL)/rootfs.ext4|The root filesystem file for PetaLinux.|
$(XLNX_VERSAL)/Image|The pre-built PetaLinux image the processor boots from.|
|$(BUILD_TARGET_DIR)/gemm_dsp_xrt.elf|The PS host application executable created in the `make application` step.|
|$(BUILD_TARGET_DIR)/gemm.hw_emu.xclbin|The XCLBIN file created in the `make xclbin` step.|

The output of the V++ Package step is the package directory that contains the contents to run hardware emulation. 

|Output Objects|Description|
|  ---  |  ---  |
|$(BUILD_TARGET_DIR)/package|The hardware emulation package that contains the boot file, hardware emulation launch script, the PLM and PMC boot files, the PMC and QEMU command argument specification files, and the Vivado simulation folder.|

</details>

<details>
<summary>make run_emu: Running Hardware Emulation</summary>

### make run_emu: Running Hardware Emulation

After packaging, everything is set to run hardware emulation. To run emulation, use the following command (default `TARGET=hw_emu`):

```
make run_emu 
```

or

```
###########################################################################
Hardware Emulation Goto:
$(BUILD_TARGET_DIR)/package

and do:
./launch_hw_emu.sh or ./launch_hw_emu.sh -g (for waveform viewer)...

```
When hardware emulation is launched, you see the QEMU simulator load. Wait for the autoboot countdown to go to zero. After a few minutes, the root Linux prompt comes up: 

```bash
root@versal-rootfs-common-2023.1:~#
```

After the root prompt comes up, run the following commands to run the design:  

```
cd /mnt
export XILINX_XRT=/usr
./gemm_dsp_xrt.elf a.xclbin
```

The `gemm_dsp_xrt.elf` executes. After a few minutes, you should see the output with `TEST PASSED` on the console. When this is shown, run the following keyboard command to exit the QEMU instance: 

```
#To exit QEMU Simulation
Press CtrlA, let go of the keyboard, and then press x 
```

To run with waveform, do the following:

```
cd $(BUILD_TARGET_DIR)/package
./launch_hw_emu.sh -g
```
The XSIM Waveform Viewer is launched. Drag and drop the signals into the viewer and click **Play** to start the emulation. Go back to the terminal and wait for the Linux prompt to show up. In the XSIM Waveform Viewer, you will see the signals you added to the waveform adjusting over the execution of the design. When this is done, hit the pause button and close the window to end the emulation.Data Integrity mismatch due to software issue in Hardware Emulation,Design works in Hardware run.

</details>

<details>
<summary>TARGET=hw: Running on Hardware</summary>

### Running on Hardware

To run the design on hardware, rerun the following `make` steps with `TARGET=hw` and other applicable options (see the preceding `make` steps specified above).

```
make kernels TARGET=hw 
make xsa TARGET=hw 
make application TARGET=hw
make package TARGET=hw 
```

These commands create a `$(BUILD_TARGET_DIR)` folder with the kernels, xsa, and `package` for a hardware run. 

Run the following step to set up the execution file, generated images, and base images (`$(BUILD_TARGET_DIR)/package/sd_card` and `$(BUILD_TARGET_DIR)/package/sd_card.img`).

```
make run_emu TARGET=hw 
```

These commands create a `build/hw` folder with the kernels, XCLBIN, and `package` for a hardware run. Follow steps 1-9 to run the `gemm_dsp_xrt.elf` executable on your VCK190 board. 

**Step 1.** Ensure your board is powered off. 

**Step 2.** Use an SD card writer (such as balenaEtcher) to flash the `sd_card.img` file to an SD card. 

**Step 3.** Plug the flashed SD card into the top slot of the VCK190 board. 

**Step 4.** Set the switch (`SW1 Mode\[3:0\]=1110 = OFF OFF OFF ON`).

**Step 5.** Connect your computer to the VCK190 board using the USB cable included with the board. 

**Step 6.** Open a TeraTerm terminal and select the correct COM port. Set the port settings to the following: 

```
Port: <COMMXX>
Speed: 115200
Data: 8 bit
Parity: none
Stop Bits: 1 bit
Flow control: none
Transmit delay: 0 msec/char 0 msec/line
```

**Step 7.** Power on the board.

**Step 8.** Wait until you see the `root@versal-rootfs-common-2023_1` Linux command prompt. Press enter a few times to get past any `xinit` errors. 

**Step 9.** Run the following commands in the TeraTerm terminal: 

```
mount /dev/mmcblk0p1 /mnt
cd /mnt
export XILINX_XRT=/usr

./gemm_dsp_xrt.elf a.xclbin
```

</details>

## Hardware Design Details

<details>
<summary> Matrix Multiplication using DSP58 Implementation Architecture </summary>

### Matrix Multiplication using DSP58 Implementation Architecture 

In this design, Matrix Multiplication is implemented using DSP58 Systolic array of size 32x32. i.e There are 32 DSP58 cascade chains, each chain having 32 DSP58s. Thus 32x32 matrix is the basic matrix multiplication size. Larger matrices are broken down into submatrices of size 32x32. 

Basic 32x32 Multiplication is performed as follows -

Matrix A row data moves upwards along DSP A Port cascade chain. For first 32 clocks data is only shifted into the DSP chains. So after 32 clocks, Row 0 of Matrix A is populated in first DSP cascade chain,
Row 1 is populated in next cascade chain and so on, as shown in the below diagram. 
show in the below diagram

![Image of Matrix A data movement](images/Matrix_A_data_movement.png)

#### Calculating First Row of Output Matrix

After Matrix A elements are shifted into cascade chain, last row of matrix B is driven clock-by-clock to the bottom most DSP of the first cascade chain, as shown in the below diagram

![Image of Matrix B data movement](images/Matrix_B_data_movement.png)

First Row of output matrix is calculated as follows - 

Bottom most DSP calculates A[0,31] *B[31,0] and sends the output to upper DSP via PCOUT cascade port. On 2nd clock upper DSP starts receiving B[30,0], B[30,1], … B[30,31] (i.e Row 30 of Matrix B).
So,on 2nd clock, 2nd DSP calculates A[0,30]* B [30,0] + PCOUT = A[0,30] *B[30,0] + A[0,31]* B[31,0], and sends it up to the 3rd DSP. 3rd DSP starts receiving Matrix B Column 29 on 3rd clock, computes 
3rd MAC operation and send up to 4th DSP. Thus after 32nd clock, top DSP has generated Row 0 Column 0 element of the output matrix.

On 2nd clock, bottom DSP receives B[31,1] and it calculates A[0,31] * B[31,1] which is the beginning of the MAC operation for Row 0 Column 1 element of output matrix. Row 0, Column 1 calculations traverse 
upwards in a similar way, and on 33rd clock, top DSP generates Row  0 Column 1 element of the output matrix. 

Similarly for next 30 clocks, (that is, clock 34 to 63) top DSP of first cascade chain generates other 30 elements of Row 0 of the output matrix

Other rows of output matrix are calculated as follows - 

B[31,0], B[31,1], … B[31,31] elements, that is row 31 of Matrix B is shifted to next DSP chain every clock. Hence Start of driving Matrix A Rows to subsequent DSP chains is also started with one clock delay.
So bottom DSP of 2nd cascade chain starts on 2nd clock and it computes A[1,31] * B[31,0], which is beginning of the MAC operation for Row 1 Column 0 element of output matrix. Thus 2nd cascade chain is 1 
clock delayed wrt first cascade chain and it generates its 32 outputs from clock 33 to 64. These outputs are Row 1 of the output matrix. Each subsequenct cascade chain is one clock delayed wrt previous 
chain, and thus the last cascade chain generates Row 31 outputs on clock 63 to 94.

#### 32x32 Matrix Multiplication Latency

For the first 32 clocks, Matrix A Row 0 is loaded into first cascade chain. Over next 32 clocks, First cascade chain calculates first row of output matrix, and for next 32 clocks, other rows of
output matrix are generated. However after 64 clocks, first DSP cascade chain can receive first row data for next 32x32 matrix.

Larger matrices are broken down into smaller 32x32 matrices. For example, 1Kx1Kx1K Matrices are represented as follows, where each box is 32x32 matrix –

![Image of GEMM DSP Implementation Submatrices](images/submatrices.png)

Output matrix is -

![Image of GEMM DSP Implementation Output Matrix](images/output_matrix.png)

#### Data Flow for larger matrices 

Matrix A00 is first multiplied with Matrix B00, which is the basic 32x32 matrix multiplication. Over the first 96 clocks, each DSP chain produces 32 outputs, thus total 1K outputs are generated which are the partial sums for the final output. These partial sums are written to 64 partial sum BRAMs.
After 64 clocks, first cascade chain is done with A00 *B00 submatrix, and it then starts performing A00* B01 to calculate partial sums for the
next column of the output matrix. Likewise over next 32 clocks, other DSP cascade chains will also complete A00 *B00 matrix multiplication and
move to A00* B01 submatrix multiplication. This way Matrix A00 is multiplied with Matrix B00, B01, B02 … B0,31.

This completes A00 submatrix multiplications. Next, we read A01 submatrix of Matrix A, and it gets multiplied with submatrices of Matrix B. The 
partial sums are added to the partial sums previous generated, and stored back. Thus we will keep moving along the first row of Matirx A and 
multiply that submatrix with submatrices of Matirx B. This will continue for 32 iterations, and in the 32nd iteration, data is written to Output 
BRAM instead of partial Sum BRAM. This completes computation of the first row of the output matrix.

Then we will move to the next row of Matrix A and all these steps are repeated. After 32 such iterations, 1Kx1Kx1K matrix multiplication will be completed.

#### Matrix Calculation Latency for large matrices

32x32 matrix calculation requires 96 clocks. However first cascade chain in the DSP58 array is done with its computation after 64 clocks, and it 
can start receiving data for next submatrix. Thus for 32 clocks, there is overlap of previous and new submatrix calculations. So the total
number of clocks required for large matix multiplication is 64 * No. of Sbumatrices + 32.

In this design, DSP clock is operating at 750MHz (1.33ns). 

The following figure shows block diagram of the design. 

![Image of GEMM DSP Implementation Architecture](images/Gemm_DSP_Block_Diagram.png)

</details>

<details>
<summary>PL Kernel Details</summary>

### PL Kernel Details

GeMM DSP RTL design can be divided into 2 main parts -
 First one is the core matrix mutliplication functionality, gemm_top module is the top level module that implements this functionality.
 Second part involves data mover logic for writing Matrix A and B data and to read the matrix output from host application. This is implemented in ps_slave module.

 In this design, core DSP logic operates at 750MHz while rest of the logic operates at 375 MHz. There is synchronizer module to handle synchronization of signals going across these 2 clock domains

 gemm_large_ocm \
 |-gemm_top \
 |-ps_slave \
 |-synchronizer

 Underneath gemm_top module, following modules are instantiated -

 1. FIXGEMM_WRAPPER - This module implements the systolic array of 1K DSP58 Engines
 2. row_uram - These are the URAMs which store Matrix A data. Entire 1Kx1K matrix A is stored in URAMs
 3. col_uram - These are the URAMs which store Matrix B data. Entire 1Kx1K matrix B is stored in URAMs
 4. partial_sum_bram - There are 64 partial Sum BRAMs (512 x 64) to store the partial sum
 5. op_uram - These URAMs store the final output of the matrix multiplication
 6. DSP_data_controller - This module controls input data to DSP58 array and output from DSP58 array  
 7. control_logic  - This module controls writes/reads to/from URAMs

 Underneath FIXGEMM_WRAPPER, FIXGEMM entity is instantiated, underneath which there is DSP_GW instantiations.

</details>

<details>
<summary>Platform Details</summary>

### Platform Details

The base platform contains the control interface and processing system (CIPS), NoC,and the interfaces among them.
 The Vitis compiler linker step builds on top of the base platform by adding the PL kernels. To add the various 
functions in a system-level design, PL kernels are added to the base platform depending on the application (that is, the PL kernels present in each design might vary). In the design, the components are added by the Vitis 
compiler `-l` step. 

(See [make xsa](#make-xsa-using-the-vitis-tools-to-link-hls-kernels-with-the-platform)) and include the following:

* `gemm_large_ocm` DSP kernel (`gemm_large_ocm.xo`)
* Connections interfaces are defined in the system configuration file

To see a schematic view of the design with the extended platform as shown in the following figure, open the following in Vivado:

`build/gemm_GEMM_SIZExGEMM_SIZExGEMM_SIZE/[hw|hw_emu]/_x/link/vivado/vpl/prj/prj.xpr`

</details>

## Software Design Details

The software design in Matrix Multiplication tutorial consists of the following sections:

<details>
<summary>Methodology</summary>

### Methodology

#### Frequency Selection

The `gemm_large_ocm` kernel operates at 800 MHz

#### Timing Closure


For timing closure of the whole design, different implementation properties are used, as mentioned in the `make xsa` step above. These strategies are required because timing is not met for default implementation settings. Routing Congestion limits operating frequency to 800MHz. 

For more information about implementation strategies, see the _Vivado Implementation User Guide_ [UG904](https://docs.xilinx.com/r/en-US/ug904-vivado-implementation)

### Data Flow

Host ps_app writes Matrix A and B data and enables DUT. It then polls for Done signal from DUT. When DUT is done, Host app reads Output URAM and compares the URAM read data with golden data. Golden input Matrix data for Matrix A and B, and golden expected data are stored in arrays which are read by host app.

#### Top Function

The PS host application (`main.cpp`) is cross-compiled to get the executable. Flow in main.cpp is as follows -

1. Include the required headers and define the required macros:

```
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include "experimental/xrt_aie.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_bo.h"

2. Include input and output arrays

#include "matrix_A_data.h"
#include "matrix_B_data.h"
#include "output_data.h"

...
```

3. Check the command line argument. The beginning of the A72 application is represented by the `main` function. It takes in one command line argument: an XCLBIN file.

   - Open the device and load the XCLBIN:

   ```
   auto dhdl = xrtDeviceOpen(0);
   auto xclbin = load_xclbin(dhdl, xclbinFilename);
   auto top = reinterpret_cast<const axlf*>(xclbin.data());
   ```
   - Open the GEMM DSP58 kernel and obtain handles to start the kernel. 

   ```
   ...
   xrtKernelHandle gemm_top_khdl;
   xrtRunHandle gemm_top_rhdl;
   ...
   gemm_top_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, gemm_top_obj);
   gemm_top_rhdl = xrtRunOpen(gemm_top_khdl);
   ...
   ```

```
4. main Function
int main(int argc, char** argv)
```
#### Sub-Function Details
test_gemm - This function programs matrix A and B URAMs from the array data and sets other control registers and then enables the gemm kernel.

check_done - This function polls for Done signal to be set from DUT.

read_perf - This function reads the performance counter value counted by the DUT. Gemm kernel counts the number of clocks required for matrix multiplication operation. Note - this count does not include time required for input and output data movement.

golden_check - This function compares data from Output URAM with the golden data. It maintains error counter which is used to decide if test passed or failed.

gemm_soft_reset_pulse - This function generates soft reset to DUT. 

<summary>PS Host Application</summary>
	
### PS Host Application

```
void gemm_bring_up(void) 
{

unsigned int i, j;
uint32_t uram_data;
unsigned int waddr;
    printf("Writing into registers\n");
    // 1. Write to Control register with Address autoincrement bit set to 1
    //    Write to address 0x10 data = 0x2
    xrtKernelWriteRegister(gemm_top_khdl, 0x10,  0x2);
    // 2. Write to Indirect address register value of 0
    //    Write to address 0x1C, data = 0
    xrtKernelWriteRegister(gemm_top_khdl, 0x1C,  0x0);

    // 3. Write to indirect address control register, Valid bit = 1, R/W## = 0
    //    Write to address 0x18 data = 0x1
    xrtKernelWriteRegister(gemm_top_khdl, 0x18,  0x1);

    // Write 16 32x32 A Matrices into Row URAMs at adress 0x20
    // Size of each Matrix is 2KB, total size = 32KB
    // Data is arrangde in 32-bit wide entry (4Byte)
    // So total lines = 8K
     
    printf("Writing Matrix A\n");
    waddr = 0;
    for (i=0; i<NUM_ROW_URAM; i=i+1) {   // Only 8 URAMs are populated
       for (j=0; j<(MATRIX_A_SIZE/NUM_ROW_URAM); j=j+1) { // 1024 locations written to 8 URAMs
          uram_data = matrix_A_data[MATRIX_A_SIZE/NUM_COL_URAM*i+j]; 
          xrtKernelWriteRegister (gemm_top_khdl, 0x20, uram_data);
       }
       // Increment the address
       waddr += 0x8000;
       xrtKernelWriteRegister (gemm_top_khdl, 0x1c, waddr);
    }
    
    waddr = 0x200000;
    xrtKernelWriteRegister (gemm_top_khdl, 0x1c, waddr);
    printf("Writing Matrix B\n");
    for (i=0; i<NUM_COL_URAM; i=i+1) {   // Only 8 URAMs are populated
       for (j=0; j<(MATRIX_B_SIZE/NUM_COL_URAM); j=j+1) { // 1024 locations written to 8 URAMs
          uram_data = matrix_B_data[MATRIX_B_SIZE/NUM_COL_URAM*i+j]; 
          xrtKernelWriteRegister (gemm_top_khdl, 0x20, uram_data);
       }
       // Increment the address
       waddr += 0x8000;
       xrtKernelWriteRegister (gemm_top_khdl, 0x1c, waddr);
    }
    
    // Set DUT Enable bit
    // Write to address 0x10, data = 0x3
    xrtKernelWriteRegister (gemm_top_khdl, 0x10, 0x3); 

}

```


5. Data Integrity Check is performed by running golden_check function 

```
void golden_check(unsigned int *mismatch_count) 
{

uint16_t golden_data_lower;
uint16_t golden_data_upper;
uint32_t read_data;
uint16_t read_data_lower;
uint16_t read_data_upper;
uint32_t read_addr;
unsigned int i, Done;
unsigned int match_count;

    // Poll for Done bit from DUT
    //printf("entered golden_check");
    //while (Done == 0) {
        // Read address 4
    //    xrtKernelReadRegister(gemm_top_khdl, 0x14, &read_data);
    //    Done = read_data & 0x1;
    //}

    // Write to indirect address control register, Vali = 1, R/W## = 1
    // Write to address 8, data = 0x3
    // xrtKernelWriteRegister (gemm_top_khdl, 0x18, 0x3);

    // Read 16 32x32 Matrices from Output URAMs from address 0x24
    // Total data is 32KB, 2-bytes read at a time, total 16K reads
    match_count = 0;
    mismatch_count = 0;
    for (i=0; i<8192; i=i+2) {
      golden_data_lower = output_data [i];
      golden_data_upper = output_data [i+1];
      xrtKernelReadRegister (gemm_top_khdl, 0x24, &read_data);
      read_data_lower = read_data & 0xFFFF;
      read_data_upper = read_data >> 16; 
      if (golden_data_lower != read_data_lower) {
         printf ("Data mismatch Addr : 0x%x, Golden Data : 0x%x, Read Data : 0x%x\n", i, golden_data_lower, read_data_lower);
         mismatch_count++;
      } else {
         //printf ("Data match Addr : 0x%x, Golden Data : 0x%x, Read Data : 0x%x\n", i, golden_data_lower, read_data_lower);
         match_count++;
      }
      if (golden_data_upper != read_data_upper) {
         printf ("Data mismatch Addr : 0x%x, Golden Data : 0x%x, Read Data : 0x%x\n", i+1, golden_data_upper, read_data_upper);
         mismatch_count++;
      } else {
         //printf ("Data match Addr : 0x%x, Golden Data : 0x%x, Read Data : 0x%x\n", i+1, golden_data_upper, read_data_upper);
         match_count++;
      }
    }  
    printf ("Match Count : %u, Mismatch Count : %u\n", match_count, *mismatch_count);
}

```

</details>

## Performance Details

For all applications, designers must work to predefined specifications and build a system for their specific deployment by meeting their system requirements with respect to their available resources, latency, throughput, performance, and power. In this section, it is outlined how to measure those characteristics for this tutorial. 

<details>
<summary>Resource Utilization</summary> 

### Resource Utilization

Resource utilization and power are measured using Vivado, vcdanalyze, and Xilinx Power Estimator (XPE) for Versal (2023.1 version) tools.

The registers, CLB LUT, BRAM, URAM and DSP Engine utilization information can be found in the Vivado project if you perform the following steps:

1. Open the Vivado project: ``$(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr``.
2. **Open Implemented Design** then click **Report Utilization**. 
3. In the Utilization tab (shown in the following figure) select **gemm_large_ocm_0** and view the resource utilization.

** Or **

1. Do `make report_metrics TARGET=hw`, (recipe expanded below), alongwith relevant options, to generate `utilization_hierarchical.txt` under `$(BLD_REPORTS_DIR)/` directory:

Resource Utilization for 32x32x32 matrix is as follows -

1. Resource Utilization 

```
+----------------------------+-------+-------+------------+-----------+-------+
|          Site Type         |  Used | Fixed | Prohibited | Available | Util% |
+----------------------------+-------+-------+------------+-----------+-------+
| Registers                  | 60653 |    16 |          0 |   1799680 |  3.37 |
| CLB LUTs                   | 17535 |     0 |          0 |    899840 |  1.95 |
| LOOKAHEAD8                 |   142 |     0 |          0 |    112480 |  0.13 |
| RAMB36E5                   |     0 |     0 |          0 |       967 |  0.00 |
| URAM                       |   192 |     0 |          0 |       463 | 41.47 |
| DSP58                      |  1024 |     0 |          0 |      1968 | 52.03 |
+----------------------------+-------+-------+------------+-----------+-------+
```

Resource Utilization is same for rest of the matrix sizes, and is as follows -

```
+----------------------------+-------+-------+------------+-----------+-------+
|          Site Type         |  Used | Fixed | Prohibited | Available | Util% |
+----------------------------+-------+-------+------------+-----------+-------+
| Registers                  | 80480 |     1 |          0 |   1799680 |  4.47 |
| CLB LUTs                   | 25231 |     0 |          0 |    899840 |  2.80 |
| LOOKAHEAD8                 |   654 |     0 |          0 |    112480 |  0.58 |
| RAMB36E5                   |    64 |     0 |          0 |       967 |  6.62 |
| URAM                       |   192 |     0 |          0 |       463 | 41.47 |
| DSP58                      |  1024 |     0 |          0 |      1968 | 52.03 |
+----------------------------+-------+-------+------------+-----------+-------+
```

</details>

<details>
<summary>Power</summary>

### Power

Power is measured using the Vivado tool. The steps for retrieving this information from the Vivado project are as follows.

1. Open the Vivado project `$(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr`.
2. Click **Open Implemented Design** and click **Report Power**. In the Power tab shown below, select **fft_2d_0** and view the power consumed for the 1024 x 2048 point - 1 instance - cint16 design:

A summary of power utilization is given in the following table.

```
+--------------------+-------------------+------------------+-----------------+
| GeMM Configuration | Dynamic Power (W) | Static Power (W) | Total Power (W) | 
+--------------------+-------------------+------------------+-----------------+
|        32x32x32    |   4.9298          |    3.2865        |  8.2163         |   
|        64x64x64    |   4.1551          |    3.3804        |  7.5355         | 
|     128x128x128    |   4.7420          |    3.2630        |  8.0050         | 
|     256x256x256    |   5.2014          |    3.1691        |  8.3705         | 
|     512x512x512    |   5.0359          |    2.6762        |  7.7121         | 
|  1024x1024x1024    |   4.7654          |    3.2865        |  8.0519         | 
+--------------------+-------------------+------------------+-----------------+
```

</details>
             
<details>
<summary>Throughput and Latency</summary> 

### Throughput and Latency

Throughput is measured in Tera Operations Per Second (TOPS) When Host App is done writing Matrices A and B, it drives 
Start signal to DUT. When DUT is done it drives Done output. A performance counter increments for all the clocks from
Start to Done. This counts the number of clocks for which DUT is active.

For 32x32x32 configuration- In this design 2 32x32x32 matrix multiplications are done. 
For each matrix, 64K MAC operations are performed, giving
total 64K * 2 = 128K MACs.
If performance counter reaches value X, that means at operating frequency of 375MHz (period of 2.66ns), total time taken
by DUT = 2.66 x X ns
Thus TOPS = 128K MACs / (2.66 x X) ns 
For rest of the configurations 1 matrix multiplication is done.
For 64x64x64       512K     MACs Thus TOPS = 512K     MACs / (2.66 x X) ns
For 128x128x128    4096K    MACs Thus TOPS = 4096K    MACs / (2.66 x X) ns
For 256x256x256    32768K   MACs Thus TOPS = 32768K   MACs / (2.66 x X) ns
For 512x512x512    262144K  MACs Thus TOPS = 262144K  MACs / (2.66 x X) ns
For 1024x1024x1024 2097152K MACs Thus TOPS = 2097152K MACs / (2.66 x X) ns

Summary of Throughput & Latency for all Variations:

Latency of the design is given by the perf counter value read from DUT. Perf counter measures the time
taken by the DUT for matrix multiplication in terms of number of clocks. 

Latency for various matrix sizes is as shown in the below table.

```
+--------------------+--------------------+-----------------+------------------+---------------+
| GeMM Configuration | Data Transfer Size |  Latency in     | Latency (us)     | Matrices/s    | 
|                    | Per matrix (int16) |  1x clocks      |                  |               | 
+--------------------+--------------------+-----------------+------------------+---------------+
|        32x32x32    |     1024           |        48       |    0.127         |  7.87 x 10^6  |
|        64x64x64    |     4096           |       145       |    0.385         |  2.59 x 10^6  |
|     128x128x128    |    16384           |      1041       |    2.769         |  3.61 x 10^5  |
|     256x256x256    |    65536           |      8209       |   21.835         |  4.76 x 10^4  |
|     512x512x512    |   262144           |     65553       |    174.3         |  5.74 x 10^3  |
|  1024x1024x1024    |  1048576           |   524305        |   1394.6         |  7.17 x 10^2  |
+--------------------+--------------------+-----------------+------------------+---------------+

```
NB:In hw_emu, due to simulation problem expected data and read data will be off by one clock.
</details>

<details>
<summary>TOPs per Watt</summary> 

### TOPs per Watt

TOPS and power utilization for DSP based martix multiplication is more or less independent of matrix size. TOPS is 1.49 and TOPs per Watt is 0.28 to 0.31 in this design.

```
+--------------------+---------+
| GeMM Configuration | TOPs/W  |
+--------------------+---------+
|        32x32x32    |  0.285  |
|        64x64x64    |  0.355  |
|     128x128x128    |  0.315  |
|     256x256x256    |  0.288  |
|     512x512x512    |  0.297  |
|  1024x1024x1024    |  0.314  |
+--------------------+---------+

```

</details>

<details>
<summary>Consolidated Summary</summary> 

### Consolidated Summary

A summary of throughput and latency for all variations is shown in the following table.

```
+--------------------+-------------------+------------------+---------------+---------+--------------------+-------------+-------------------------+--------------+------------------+--------------------+----------------+
| GeMM Configuration |  perf (in MSPS)   | Latency(us)      |  Matrices/s   |  TOPs   | No.of Active Cores | Vector Load | No. of Active mem Banks | Mem R/W Rate | Active AIE Tiles | Dynamic Power (mW) | TOPs per Watt  |
+--------------------+-------------------+------------------+---------------+---------+--------------------+-------------+-------------------------+--------------+------------------+--------------------+----------------+
|        32x32x32    |     12000         |  0.127           | 7.874 x 10^6  | 1.407   |        NA          |    NA       |         NA              |    NA        |      NA          |      4929.8        |    0.285       | 
|        64x64x64    |     12000         |  0.385           | 2.59 x 10^6   | 1.479   |        NA          |    NA       |         NA              |    NA        |      NA          |      4155.1        |    0.355       |
|     128x128x128    |      6000         |  2.769           | 3.61 x 10^5   | 1.497   |        NA          |    NA       |         NA              |    NA        |      NA          |      4742.0        |    0.315       |
|     256x256x256    |      3000         |  21.835          | 4.76 x 10^4   | 1.499   |        NA          |    NA       |         NA              |    NA        |      NA          |      5201.4        |    0.288       |
|     512x512x512    |      1500         |  174.37          | 5.74 x 10^3   | 1.499   |        NA          |    NA       |         NA              |    NA        |      NA          |      5035.9        |    0.297       |
|  1024x1024x1024    |       750         |  1394.6          | 7.17 x 10^2   | 1.499   |        NA          |    NA       |         NA              |    NA        |      NA          |      4765.4        |    0.314       |
+--------------------+-------------------+------------------+---------------+---------+--------------------+-------------+-------------------------+--------------+------------------+--------------------+----------------+

```
</details>

## Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
