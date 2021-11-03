<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2021.1 Versal AI Engine/HLS FIR Filter Tutorial (HLS Implementation)</h1>
   </td>
 </tr>
</table>

# Table of Contents
[Building the Design](#building-the-design)

[Hardware Design Details](#hardware-design-details)

[Software Design Details](#software-design-details)

[References](#references)

[Revision History](#revision-history)

# Building the Design

<details>
<summary>Design Build</summary>

## Design Build
In this section, you will build and run the FIR filter design using the DSP implementation. The difference between this implementation and the AI Engine implementation, where users compile the AI Engine design and integrate it into a larger system design (including the programmable logic (PL) kernels and processing system (PS) host application), is that the FIR filter is now implemented in PL using DSP Engines.  

At the end of this section, the design flow will generate a new directory (called `build/`). Underneath are sub-directories named `fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps` (for example, fir_dsp_1firs_15taps) depending on value of `N_FIR_FILTERS` and `N_FIR_TAPS` chosen in the build. Each sub-directory contains the `hw_emu/` and `hw/` subfolders. The `hw_emu/` subfolder contains the build for hardware emulation. The `hw/` subfolder contains the build for the hardware run on a VCK190 board.   

</details>

<details>
<summary>Make Steps</summary>

## Make Steps
To run the following `make` steps (e.g. `make kernels`, `make graph`, etc), you must be in the `Makefiles/` folder.
```bash
cd Makefiles
```

The following options can be specified in the make steps. See the make steps for instructions how to apply them.

* TARGET: it can be set to "hw" or "hw_emu" to build the design in hardware or hardware emulation flow. Default is "hw_emu".

* N_FIR_FILTERS: specifies the number of FIR filters in the chain. Default is 1.

* N_FIR_TAPS: specifies the number of FIR filter taps. Default is 15.

* EN_TRACE: Flag to enable trace data to be captured. 0 is disabled and 1 is enabled. Default is 0.

</details>

<details>
<summary>Build the Entire Design with a Single Command</summary>

## Build the Entire Design with a Single Command
If you are already familiar with the AI Engine and Vitis™ accelerated kernel compilation flows, you can build the entire design with one command:

```bash
make run (default hardware emulation, 1 filter 15 taps, no trace enabled)
```
or
```bash
make run TARGET=hw N_FIR_FILTERS=5 N_FIR_TAPS=15 EN_TRACE=1   (hardware, 5 FIR filters, each with 15 taps, enable tracing)
```

This command will run the `make kernels`,`make xclbin`,`make application`,`make package` and `make run_emu` for hardware emulation or to run on hardware (VCK190 board) depending on the `TARGET` you specify. The default `TARGET` without specification is hw_emu. The settings also apply to the following individual make steps.

Note: Simulation takes considerably longer to execute the application than when running on actual hardware, so it is recommended to simulate with a smaller data set to have it complete in a reasonable time. The hardware implementation uses a much larger data set to reduce measurement effects.
In the file `\<project>/DSP/design/app_src/fir_dsp_app.c`, un-comment the following, as appropriate:
```
#define REPEAT_OFFSET   4096
#define REPETITIONS      509     <-- use this for TARGET=hw;     will produce a 2M sample data set
//#define REPETITIONS        3   <-- use this for TARGET=hw_emu; will produce an 8K sample data set
#define FLUSH_SAMPLES   4096
```

**Note**

1) The generated files for a particular build are placed under individual directory: build/fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps
2) See the specification in each make step for options used and location of input and output files.

</details>

The individual make steps to build the design with the options that applied to them are specified as follows.

<details>
<summary>make kernels: Compile PL Kernels</summary>

## make kernels: Compile PL Kernels
In this step, the Vitis compiler takes any kernels (RTL or HLS C) in the PL region of the target platform (`xilinx_vck190_base_202110_1`) and compiles them into their respective XO files.

The following commands compiles the kernels (default TARGET=hw_emu, N_FIR_FILTERS=1, N_FIR_TAPS=15, EN_TRACE=0):

```
make kernels
```

The expanded command is as follows:
```
mkdir -p ../build/fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps/hw_emu

cd ../build/fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps/hw_emu

v++ 	--target hw_emu					\
        --hls.pre_tcl ./directives/hls_pre.tcl		\
	--hls.clock 300000000:s2mm 			\
	--platform xilinx_vck190_base_202110_1		\
	--include ../../../design/pl_src 		\
	--save-temps 					\
	--temp_dir _x 					\
	--verbose 					\
	-g -c 						\
	-k fir_dsp 					\
	../../../design/pl_src/fir_dsp.cpp 		\
	-o fir_dsp.hw_emu.xo   

v++ 	--target hw_emu					\
	--hls.clock 300000000:s2mm 			\
	--platform xilinx_vck190_base_202110_1		\
	--save-temps 					\
	--temp_dir _x 					\
	--verbose 					\
	-g -c 						\
	-k s2mm 					\
	../../../design/pl_src/s2mm.cpp 		\
	-o s2mm.hw_emu.xo   

v++ 	--target hw_emu					\
	--hls.clock 300000000:mm2s 			\
	--platform xilinx_vck190_base_202110_1		\
	--save-temps 					\
	--temp_dir _x 					\
	--verbose 					\
	-g -c 						\
	-k mm2s 					\
	../../../design/pl_src/mm2s.cpp 		\
	-o mm2s.hw_emu.xo   

 ```
Summary of the switches used:
|Switch|Description|
|  ---  |  ---  |
|--target \| -t [hw\|hw_emu]|Specifies the build target.|
|--hls.clock | Specifies a frequency in Hz at which the listed kernel(s) should be compiled by Vitis HLS. |
|--platform \| -f|Specifies the name of a supported acceleration platform as specified by the $PLATFORM_REPO_PATHS environment variable or the full path to the platform XPFM file.|
|--save-temps \| -s|Directs the Vitis compiler command to save intermediate files/directories created during the compilation and link process. Use the `--temp_dir` option to specify a location to write the intermediate files to.|
|--temp_dir <string>|This allows you to manage the location where the tool writes temporary files created during the build process. The temporary results are written by the Vitis compiler, and then removed, unless the `--save-temps` option is also specified.|
|--verbose|Display verbose/debug information.|
| -g | Generates code for debugging the kernel during software emulation. Using this option adds features to facilitate debugging the kernel as it is compiled. |
|--compile \| -c|Required for compilation to generate XO files from kernel source files.|
|--kernel \<arg\>\|-k \<arg\>|Compile only the specified kernel from the input file. Only one -k option is allowed per Vitis compiler command.|
|--output \| -o|Specifies the name of the output file generated by the V++ command. The compilation process output name must end with the XO file suffix.|

[Detailed Description of All Vitis Compiler Switches](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Compiler-Command)

|Input|Description|
|  ---  |  ---  |
|fir_dsp.cpp|The FIR filter chain PL kernel source code.|
|s2mm.cpp|The stream-to-memory-mapped data-mover PL kernel source code.|
|mm2s.cpp|The memory-mapped-to-stream data-mover PL kernel source code.|

|Output|Description|
|  ---  |  ---  |
|fir_dsp.hw_emu.xo|The FIR filter chain PL kernel object file.|
|s2mm.hw_emu.xo|The stream-to-memory-mapped data-mover kernel object file.|
|mm2s.hw_emu.xo|The memory-mapped-to-stream data-mover kernel object file.|

</details>

<details>
<summary>make xclbin: Use Vitis Tools to Link HLS Kernels with the Platform</summary>

## make xclbin: Use Vitis Tools to Link HLS Kernels with the Platform
After the PL HLS kernels have been compiled, you can use the Vitis compiler to link them with the platform to generate an XCLBIN file.

The Vitis tools allow you to integrate the HLS kernels into an existing extensible platform. This is an automated step from a software developer perspective where the platform chosen is provided by the hardware designer (or you can opt to use one of the many extensible base platforms provided by Xilinx and the Vitis tools build the hardware design and integrate the PL kernels into the design).

To test this feature in this tutorial, use the base VCK190 platform to build the design.

The command to run this step is shown as follows (default TARGET=hw_emu, N_FIR_FILTERS=1, N_FIR_TAPS=15, EN_TRACE=0):
```
make xclbin
```

The expanded command is as follows:
```
cd ../build/fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps/hw_emu

v++ 	-l 						\
	--platform xilinx_vck190_base_202110_1 		\
	--include ../../../design/pl_src 		\
	--save-temps 					\
	--temp_dir _x 					\
	--verbose 					\
	-g 						\
	--clock.defaultTolerance 0.001 			\
	--clock.freqHz 300000000:mm2s_0 		\
	--clock.freqHz 300000000:s2mm_0 		\
	--clock.freqHz 300000000:fir_dsp_0 		\
	--vivado.synth.jobs 16				\
	--vivado.impl.jobs 16				\
	--config ../../../Makefiles/system.cfg 		\
	-t hw_emu 					\
	-o vck190_dsp_fir.hw_emu.xclbin  		\
	s2mm.hw_emu.xo					\
	mm2s.hw_emu.xo					\
        fir_dsp.hw_emu.xo

```

If EN_TRACE is enabled, the following `v++` flags are also set:
```
	--profile.trace_memory DDR			\
	--profile.data s2mm:s2mm_0:s			\
	--profile.data mm2s:mm2s_0:s			\
	--profile.data fir_dsp:dir_dsp_0.StreamIn	\
	--profile.data fir_dsp:dir_dsp_0.StreamOut

```
This will capture trace data for the ports specified.

Summary of the Switches used:

|Switch|Description|
|  ---  |  ---  |
|--platform \| -f|Specifies the name of a supported acceleration platform as specified by the $PLATFORM_REPO_PATHS environment variable or the full path to the platform XPFM file.|
|--save-temps \| -s|Directs the `v++` command to save intermediate files/directories created during the compilation and link process. Use the `--temp_dir` option to specify a location to write the intermediate files to.|
|--temp_dir <string>|This allows you to manage the location where the tool writes temporary files created during the build process. The temporary results are written by the Vitis compiler, and then removed, unless the `--save-temps` option is also specified.|
|--verbose|Display verbose/debug information.|
| -g | Generates code for debugging the kernel during software emulation. Using this option adds features to facilitate debugging the kernel as it is compiled. |
|--clock.freqHz \<freq_in_Hz\>:\<cu\>\[.\<clk_pin\>\]|Specifies a clock frequency in Hz and assigns it to a list of associated compute units (CUs) and optionally specific clock pins on the CU.|
|--config <config_file>|Specifies a configuration file containing `v++` switches.|
|--target \| -t [hw\|hw_emu]|Specifies the build target.|
|--output \| -o|Specifies the name of the output file generated by the `v++` command. The linking process output file name must end with the .xclbin suffix|
|--profile.data [<kernel_name>\|all]:[<cu_name>\|all]:[<interface_name>\|all]\(:[counters\|all]\)|Enables monitoring of data ports through the monitor IPs. This option needs to be specified during linking. [Detailed Profiling Options](https://docs.xilinx.com/search/all?query=--profile+Options&content-lang=en-US) |
|--profile.trace_memory \<FIFO\>:\<size\>\|\<MEMORY\>[\<n\>]|When building the hardware target \(-t=hw\), use this option to specify the type and amount of memory to use for capturing trace data. [Detailed Profiling Options](https://docs.xilinx.com/search/all?query=--profile+Options&content-lang=en-US) |


[Detailed Description of All Vitis Compiler Switches](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Compiler-Command)
[Linking the Kernels in Vitis](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Linking-the-Kernels)

|Inputs Sources|Description|
|  ---  |  ---  |
|s2mm.hw_emu.xo|The stream-to-memory-mapped data-mover kernel object file.|
|mm2s.hw_emu.xo|The memory-mapped-to-stream data-mover kernel object file.|
|fir_dsp.hw_emu.xo|The FIR filter chain PL kernel object file.|

|Output Objects|Description|
|  ---  |  ---  |
|vck190_dsp_fir.hw_emu.xclbin|Compiled Platform Binary Container|

</details>

 <details>
<summary>make application: Compile the Host Application</summary>

## make application: Compile the Host Application
You can compile the host application by following the typical cross-compilation flow for the Cortex-A72. To build the application run the following command (default TARGET=hw_emu, N_FIR_FILTERS=1, N_FIR_TAPS=15, EN_TRACE=0):
```
make application
```

The expanded command is as follows:
```
aarch64-linux-gnu-g++ 	-O 					\
			-c -std=c++14 				\
			-D__linux__ 				\
			-DXAIE_DEBUG				\
			-I$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/sysroots/aarch64-xilinx-linux/usr/include/xrt 		\
			-I$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/sysroots/aarch64-xilinx-linux/usr/include		\
			-I$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/sysroots/aarch64-xilinx-linux/usr/lib			\
			-I../../../design/app_src		\
			-I../../../design/pl_src		\
			../../../design/app_src/fir_aie_app.cpp \
			-o ../fir_aie_app.o 			\
			--sysroot=$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/sysroots/aarch64-xilinx-linux 			\
			-L$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/sysroots/aarch64-xilinx-linux/usr/lib 			\
			-lxrt_coreutil

aarch64-linux-gnu-g++ 	../fir_dsp_app.o			\
			--sysroot=$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/sysroots/aarch64-xilinx-linux			\
			-L$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/sysroots/aarch64-xilinx-linux/usr/lib 			\
			-lxrt_coreutil 				\
			-o fir_dsp_xrt.elf
```
Summary of the Switches used:
|Switch|Description|
|  ---  |  ---  |
|-O \| Optimize| Optimizing compilation takes somewhat more time, and a lot more memory for a large function. With -O, the compiler tries to reduce code size and execution time, without performing any optimizations that can take a great deal of compilation time.|
|-c |Compile or assemble the source files, but do not link.|
|-std=<\standard\>|Set the language standard.|
|-D__linux__| |
|-DXAIE_DEBUG|Enable debug interface capabilities where certain core status, event status, or stack trace can be dumped out.|
|-D\<Pre-processor Macro String\>=\<value\>|Pass Pre-processor Macro definitions to the cross-compiler.|
|-I \<dir\>|Add the directory `dir` to the list of directories to be searched for header files.|
|-o \<file\>|Place output in file `<file>`. This applies regardless of the output being produced, whether it be an executable file, an object file, an assembler file or preprocessed C code.|
|--sysroot=\<dir\>|Use `dir` as the logical root directory for headers and libraries. For example, if the compiler would normally search for headers in `/usr/include` and libraries in `/usr/lib`, it will instead search `dir/usr/include` and `dir/usr/lib`. This is automatically set by the `env_setup.sh` script|
|-l\<library\>|Search the library named `library` when linking. The 2D-FFT tutorial requires `adf_api_xrt` and `xrt_coreutil` libraries.|
|-L \<dir\>|Add directory `<dir>` to the list of directories to be searched for -l.|

[XRT Documentation](https://xilinx.github.io/XRT/2021.1/html/index.html)
[Details of Host Application Programming](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Host-Programming-for-Bare-metal-Systems)

|Inputs Sources|Description|
|  ---  |  ---  |
|fir_dsp_app.cpp|Host processor application source code file that will run on an A72 processor.|

|Intermediate Objects|Description|
|  ---  |  ---  |
|fir_dsp_app.o|Compiled host processor application object.|


|Output Objects|Description|
|  ---  |  ---  |
|fir_dsp_xrt.elf|The executable that will run on an A72 processor.|

</details>

<details>
<summary>make package: Package the Design</summary>

## make package: Package the Design
With the HLS kernel outputs created, as well as the new platform, you can now generate the programmable device image (PDI) and a package to be used on an SD card. The PDI contains all executables, bitstreams, configurations of the device. The packaged SD card directory contains everything to boot Linux, the generated applications, and `.xclbin`.

The command to run this step is as follows (default TARGET=hw_emu, N_FIR_FILTERS=1, N_FIR_TAPS=15, EN_TRACE=0):
```
make package
```

or
```
cd ../build/fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps/hw_emu

v++	-p  							\
	-t hw_emu						\
	--save-temps						\
	--temp_dir ../build/fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps/hw_emu/_x						\
	-f xilinx_vck190_base_202110_1												\
	--package.sd_dir $(PLATFORM_REPO_PATHS)/sw/versal/xrt 									\
	--package.rootfs $(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/rootfs.ext4 						\
	--package.kernel_image $(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/Image 						\
	--package.boot_mode=sd													\
	--package.out_dir ../build/fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps/hw_emu/package	        		\
	--package.image_format=ext4												\
	--package.sd_file	../build/fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps/hw_emu/fir_dsp_xrt.elf     		\
				../build/fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps/hw_emu/vck190_dsp_fir.hw_emu.xclbin 	\
```
If EN_TRACE is enabled, the following `v++` flags are also set:
```
	--package.sd_file ./xrt.ini
```
This will include the XRT ini file which includes tracing parameters.

|Switch|Description|
|  ---  |  ---  |
|--package \| -p|Packages the final product at the end of the Vitis compile and link build process.|
|--target \| -t [hw\|hw_emu]|Specifies the build target.|
|--save-temps \| -s|Directs the V++ command to save intermediate files/directories created during the compilation and link process. Use the `--temp_dir` option to specify a location to write the intermediate files to.|
|--temp_dir <string>|This allows you to manage the location where the tool writes temporary files created during the build process. The temporary results are written by the Vitis compiler, and then removed, unless the `--save-temps` option is also specified.|
|--platform \| -f|Specifies the name of a supported acceleration platform as specified by the $PLATFORM_REPO_PATHS environment variable or the full path to the platform XPFM file.|
|--package.sd_dir \<arg\>|Where <arg> specifies a folder to package into the sd_card directory/image. The contents of the directory are copied to a sub-folder of the sd_card folder.|
|--package.rootfs \<arg\>|Where \<arg\> specifies the absolute or relative path to a processed Linux root file system file. The platform RootFS file is available for download from xilinx.com. Refer to the Vitis Software Platform Installation for more information.|
|--package.kernel_image \<arg\>|Where \<arg\> specifies the absolute or relative path to a Linux kernel image file. Overrides the existing image available in the platform. The platform image file is available for download from xilinx.com. Refer to the Vitis Software Platform Installation for more information.|
|--package.boot_mode \<arg\>|Where \<arg\> specifies <ospi\|qspi\|sd> Boot mode used for running the application in emulation or on hardware.|
|--package.image_format|Where \<arg\> specifies \<ext4\|fat32\> output image file format. `ext4`: Linux file system and `fat32`: Windows file system|
|--package.sd_file|Where \<arg\> specifies an ELF or other data file to package into the `sd_card` directory/image. This option can be used repeatedly to specify multiple files to add to the `sd_card`.|


[Detailed Description of All Vitis Compiler Switches](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Compiler-Command)
[Details of Packaging the System](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Packaging-the-System8)

|Inputs Sources|Description|
|  ---  |  ---  |
|$(PLATFORM_REPO_PATHS)/sw/versal/xrt|The PS Host Application needs the XRT headers in this folder to execute.|
|$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/rootfs.ext4|The Root Filesystem file for Petalinux.|
|$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/Image|The pre-built Petalinux Image the processor boots from.|
|$(BUILD_TARGET_DIR)/fir_dsp_xrt.elf|The PS Host Application executables created in the `make application` step.|
|$(BUILD_TARGET_DIR)/vck190_dsp_fir.hw_emu.xclbin|The XCLBIN file created in the `make xclbin` step.|

The output of the V++ Package step is the package directory that contains the contents to run hardware emulation.

|Output Objects|Description|
|  ---  |  ---  |
|$(BUILD_TARGET_DIR)/package|The hardware emulation package that contains the boot file, hardware emulation launch script, the PLM and PMC boot files, the PMC and QEMU command argument specification files, and the Vivado® tools simulation folder.|

</details>

<details>
<summary>make run_emu: Run Hardware Emulation</summary>

## make run_emu: Run Hardware Emulation
After packaging, everything is set to run emulation or hardware.
To run emulation use the following command (default TARGET=hw_emu, N_FIR_FILTERS=1, N_FIR_TAPS=15, EN_TRACE=0):
```
make run_emu
```
or
```
cd ../build/fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps/hw_emu/package
./launch_hw_emu.sh
```
When launched, you will see the QEMU simulator load. Wait for the autoboot countdown to go to zero, and after a few minutes, you will see the root Linux prompt come up:
```bash
root@versal-rootfs-common-2021_1:~#
```

In some cases, the following error might come up on the screen:
```
root@versal-rootfs-common-2021_1:~# xinit: giving up
xinit: unable to connect to X server: Connection refused
xinit: server error
Enabling notebook extension jupyter-js-widgets/extension...
      - Validating: OK
[C 13:46:09.233 NotebookApp] Bad config encountered during initialization:
[C 13:46:09.239 NotebookApp] No such notebook dir: ''/usr/share/example-notebooks''
```
The error can be ignored. Press <enter> to return to the root prompt.

After the root prompt comes up, run the following commands to run the design:  
```
mount /dev/mmcblk0p1 /mnt
cd /mnt
export XLC_EMULATION_MODE=hw_emu
export XILINX_XRT=/usr
./fir_dsp_xrt.elf a.xclbin
```
The `fir_dsp_xrt.elf` should execute, and after a few minutes, you should see the output with *TEST PASSED* on the console. When this is shown, run the following keyboard command to exit the QEMU instance:

```
#To exit QEMU Simulation
Press Ctrl-A, let go of the keyboard, and then press x
```

To run with waveform do the following:
```
cd ../build/fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps/hw_emu/package
./launch_hw_emu.sh -graphic -xsim
```
The XSIM Waveform Viewer is launched. Drag and drop the signals into the Viewer and click Play to start the emulation. Go back to the terminal and wait for the Linux prompt to show up.

In the XSIM Waveform Viewer, you will see the signals you added to the waveform adjusting over the execution of the design. Once done, hit the pause button and close the window to end the emulation.

</details>

<details>
<summary>TARGET=hw: Run on Hardware</summary>

## Run on Hardware

To run the design in hardware, re-run the following "make" steps with TARGET=hw and other applicable options (see the make steps above)
```
make kernels     TARGET=hw
make xclbin      TARGET=hw
make application TARGET=hw
make package     TARGET=hw
```
this can also be done is a single step as follows:
```
make build TARGET=hw
```

Now follow **Steps 1-9** to run the `lenet_xrt.elf` executable on your VCK190 board.

**Step 1.** Ensure your board is powered OFF.

**Step 2.** Use an SD card writer (such as balenaEtcher) to flash the `sd_card.img` file onto an SD card.

**Step 3.** Plug the flashed SD card into the top slot of the VCK190 board.

**Step 4.** Set the switch SW1 Mode\[3:0\]=1110 = OFF OFF OFF ON.

**Step 5.** Connect your computer to the VCK190 board using the included USB cable.

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

**Step 7.** Power ON the board.

**Step 8.** Wait until you see the `root@versal-rootfs-common-2021_1` Linux command prompt. Press enter a few times to get past any `xinit` errors.

**Step 9.** Run the following commands into the TeraTerm terminal:
```
cd /mnt/sd-mmcblk0p1
export XILINX_XRT=/usr
./init.sh
./fir_dsp_xrt.elf a.xclbin
```

After execution completes and the testcase passes data integrity check, 'TEST PASSED' should appear on the terminal.

</details>

# Hardware Design Details
<details>
<summary>FIR Filter HLS Implementation Architecture</summary>

## FIR Filter HLS Implementation Architecture
The following figure shows a high level block diagram of the design. The test harness consists of the compute kernels, data mover kernels and DDR memory to store input and output vectors. This setup is maintained in the two implementations (using HLS with DSP engines in this section of the tutorial and AI Engine in the other). In this setup, the interface between the data mover kernels and DDR is memory mapped AXI4 and it is AXI4-stream between data mover kernel and the FIR filter chain PL kernel. The mm2s kernel moves data from the DDR memory into the FIR Filter and the s2mm kernel moves the data from FIR filter back to DDR memory. The data widths of both the kernels is 128 bits wide, and they run at 300 MHz, providing a transfer rate of up to 1.2 Gsamples/sec.

![Image of FIR Filter DSP implementation architecture](images/fir_dsp_block_diagram.png)

</details>

<details>
<summary>Design Details</summary>

## Design Details
The design in this tutorial starts with a base platform containing the control interface and processing system (CIPS), NoC, and AI Engine and the interfaces among them. The v++ linker step builds on top of the base platform by adding the PL kernels. To add the various functions in a system level design, PL kernels are added to the base platform depending on the application, that is, the PL kernels present in each design may vary. In the design, the components are added by v++ -l step (make XCLBIN in the tool flow section above) and include the following:
* FIR Filter Chain kernel (`fir_dsp.[hw|hw_emu].xo`)
* data mover kernel (`mm2s.[hw|hw_emu].xo` and `s2mm.[hw|hw_emu].xo`)
* connections interfaces defined in system configuration file (system.cfg)

To see a schematic view of the design with the extended platform as shown in the following figure, open in Vivado tools:

`build/fir_dsp_$(N_FIR_FILTERS)firs_$(N_FIR_TAPS)taps/[hw|hw_emu]/_x/link/vivado/vpl/prj/prj.xpr`

![Image of FIR Filter DSP Platform schematic](images/fir_dsp_vivado.png)

The actual FIR filter chain itself is implemented in a HLS PL kernel, which connects the specified number of filters together in a chain.  For purposes of simplicity in comparing the designs, all the filters in the chain are identical, though it is unlikely such a chain would be used in a practical application.

Notice the system debugging and profiling IP (DPA) is added to the PL region of the device to capture AI Engine run-time trace data if the EN_TRACE option is enabled in the design. The mm2s/s2mm kernels and the AI Engine array interface are both operating at 300 MHz.

</details>

<details>
<summary>HLS PL Kernels</summary>

## HLS PL Kernels
In the HLS implementation of the FIR Filter design, the AI Engine is not used and therefore there are no AI Engine-related kernels and graphs. The compute and datamover functions are implemented as HLS kernels in the PL region.

The PL kernel `fir_dsp` implements the FIR filter chain.  It contains a single AXI-stream input port and a single AXI-stream output port.  Since the FIR function requires no initialization, no additional control/status ports are required.

The PL-based data movers consist of MM2S and S2MM kernels. The MM2S move data from DDR memory through the NoC to the FIR Filter kernel and the final FIR output is moved back to DDR memory through the NoC by the S2MM kernel. In either data mover kernel, the side facing NoC uses a memory mapped AXI4 interface (MM-AXI4) and the side facing the AI Engine array uses an AXI4-Stream interface.
Some additional details regarding the data mover kernels include:

**MM2S**
* The data width is 128 bits
* To avoid bandwidth limitation resulting in back pressure which causes performance degradation, the HLS pragma `max_read_burst_length` is set higher than the default to 256 bits.

**S2MM**
* The data width is 128 bits
* To avoid bandwidth limitation resulting in back pressure which causes performance degradation, the HLS pragma `max_write_burst_length` is set higher than the default to 256 bits.

</details>

# Software Design Details
The software design in the FIR Filter HLS implementation consists of the following sections:

<details>
<summary>PL Kernels</summary>

## PL Kernels
For the HLS implementation of this design, the data mover kernels and the FIR filter chain are all implmented in HLS.

### fir_dsp (fir_dsp.cpp)
The fir_filter kernel consists of a single AXI-stream input and AXI-stream output.  The kernel makes use of the FIR Compiler IP, the same one that can be instantiated as an IP in Vivado tools. In HLS, it is instantiated as an object in the HLS code, and then cascaded together into a chain by the design.

The following include allows us to utilize the FIR Compiler interface provided in the HLS IP libraries from the Vitis HLS Libraries Reference:
```
#include <hls_fir.h>
```   

This header files provides a parameterization struct (`hls::ip_fir::params_t`) that sets the static parameters of the filter:
```
struct fir_params : hls::ip_fir::params_t {
    static const unsigned num_coeffs    = N_FIR_TAPS;
    static const double   coeff_vec[N_FIR_TAPS];
    static const unsigned coeff_width   = 16;
    static const unsigned input_width   = 16;
    static const unsigned output_width  = 16;
    static const unsigned output_rounding_mode = hls::ip_fir::truncate_lsbs;
    static const unsigned input_length  = WINDOW_LENGTH;
    static const unsigned output_length = WINDOW_LENGTH;
    static const unsigned sample_period = 1;
    static const unsigned coeff_structure = hls::ip_fir::symmetric;
};
```
Here we set key non-default values for the filter, the number of taps, the tap vectors (in `coeff_vec`), data widths, truncation mode, and filter structure.

Note that the FIR filter wrapper has the concept of an input/output length, which is called WINDOW_LENGTH. This is unrelated to FIR_WINDOW_SIZE in the AI Engine version of the design. In AI Engine graph design, data is processed in fixed size batches (windows), and FIR_WINDOW_SIZE specifies the size of these physical buffers. Here the buffer size will directly impact latency.

In the HLS (DSP) implementation, arrays (windows) are a means of passing data to functions, but these data arrays are ultimately translated into AXI-streams. For this implementation, WINDOW_SIZE is made to be 64k.

The following section instantiates arrays of filter objects (one for real values, one for imaginary):
```
static hls::FIR<fir_params> fir_real[N_FIR_FILTERS];
static hls::FIR<fir_params> fir_imag[N_FIR_FILTERS];
```

The `complex_split` function is used to take the incoming array (stream) of 32-bit data, and split each word into two 16-bit word streams:
```
void complex_split (hls::stream<ap_axiu<32, 0, 0, 0> >& StreamIn,
                    DataWindow_t DataRealIn,
                    DataWindow_t DataImagIn
                   )
{
    for (int ix = 0 ; ix < WINDOW_LENGTH; ix++)  {
        #pragma HLS PIPELINE II=1
        ap_axiu<32, 0, 0, 0> StreamWord = StreamIn.read();
        uint32_t word = StreamWord.data;
        DataRealIn[ix] = (word >> 16) & 0xFFFF;
        DataImagIn[ix] =  word        & 0xFFFF;
    }
}
```

The `complex_merge` function is the inverse of complex_split, and used to merge the words from wo incoming 16-bit streams into one 32-bit stream:
```
void complex_merge (DataWindow_t DataRealOut,
                    DataWindow_t DataImagOut,
                    hls::stream<ap_axiu<32, 0, 0, 0> >& StreamOut
                )
{
    for (int ix = 0 ; ix < WINDOW_LENGTH; ix++)  {
        #pragma HLS PIPELINE II=1
        uint32_t word = (((uint32_t) DataRealOut[ix]) << 16) | (DataImagOut[ix] & 0xFFFF);
        ap_axiu<32, 0, 0, 0> StreamWord;
        StreamWord.data = word;
        StreamOut.write(StreamWord);
    }
}
```

The `dsp_run` function takes to two 16-bit streams, and directs them to two FIR filters which are run in parallel:
```
void dsp_run (int N,
              DataWindow_t DataRealIn,
              DataWindow_t DataImagIn,
              DataWindow_t DataRealOut,
              DataWindow_t DataImagOut
             )
{
    #pragma HLS dataflow
    fir_real[N].run(DataRealIn, DataRealOut);
    fir_imag[N].run(DataImagIn, DataImagOut);
}
```
The #pragma HLS dataflow directive instructs the compile to run the two processes to run in parallel, much as would be done in RTL.

The function `fir_wrap` is used to construct the filter chain. It uses a series of #if/#elif preprocessor directives to enable the code, since it was not possible to generate it iteratively using a loop. (The limitation being synthesis of arrays of arrays).
```
void fir_wrap (hls::stream<ap_axiu<32, 0, 0, 0> >& StreamIn,
               hls::stream<ap_axiu<32, 0, 0, 0> >& StreamOut
              )
{
    #pragma HLS dataflow

    DataWindow_t DataRealIn,  DataImagIn;
    DataWindow_t DataRealOut, DataImagOut;
    #pragma HLS stream variable=DataRealIn   depth=16
    #pragma HLS stream variable=DataRealOut  depth=16
    #pragma HLS stream variable=DataImagIn   depth=16
    #pragma HLS stream variable=DataImagOut  depth=16

    complex_split(StreamIn, DataRealIn, DataImagIn);

#if (N_FIR_FILTERS == 1)
    dsp_run(0, DataRealIn,  DataImagIn,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 1)
    DataWindow_t DataReal_0,  DataImag_0;
    #pragma HLS stream variable=DataReal_0  depth=16
    #pragma HLS stream variable=DataImag_0  depth=16
    dsp_run(0, DataRealIn,  DataImagIn,  DataReal_0, DataImag_0);
#endif

...etc
```

Finally, the `fir_dsp` function it a top-level module / kernel available to be linked together to the other HLS kernels.

#### Arguments
The FIR kernel takes the following arguments:
* `hls::stream<ap_axiu<32, 0, 0, 0>>` is a data type defined in `ap_axi_sdata.h`. It is a special data class used for data transfer when using a streaming platform. The parameter `<D>` is the data width of the streaming interface which is set to 32. The remaining three parameters should be set to 0.

The fir_dsp kernel also specifies the following pragmas to help optimize the kernel code and adhere to interface protocols:
#### pragma HLS interface ap_ctrl_none port=return
This kernel requires no additional control/status interfaces.


### mm2s (mm2s.cpp)
The mm2s kernel reads data from a Memory Mapped AXI4 (MM-AXI4) interface and writes it to an AXI4-Stream Interface
#### Arguments
The mm2s kernel takes the following arguments:
* `ap_int<N>` is an arbitrary precision integer data type defined in `ap_int.h` where `N` is a bit-size from 1-1024. In this design, the bit-size is set to 128.
* `hls::stream<qdma_axis<D,0,0,0>>` is a data type defined in `ap_axi_sdata.h`. It is a special data class used for data transfer when using a streaming platform. The parameter `<D>` is the data width of the streaming interface which is set to 128. The remaining three parameters should be set to 0.

The mm2s kernel also specifies the following pragmas to help optimize the kernel code and adhere to interface protocols
#### pragma HLS INTERFACE s_axilite
The mm2s kernels has one `s_axilite` interface (specifying a AXI4-Lite slave I/O protocol) with `bundle=control` associated with all the arguments (`mem`,`s`, and `size`). This interface is also associated with `return`.
##### pragma HLS INTERFACE m_axi
The mm2s kernel has one `m_axi` interface (specifying a AXI4 master I/O protocol) with `offset=slave bundle=gmem`. This interface also has `max_read_burst_length=256`. Part of this AXI4 interface is the Read Address Channel containing the signals `ARBURST` and `ARLEN`. This interface has a burst type `ARBURST=INCR` and can support burst length `ARLEN` of 1-256 read transfers. In an incrementing burst, the address for each transfer in the burst is an increment of the previous transfer address. The `max_read_burst_length=256` sets the burst length `ARLEN=256` transfers, meaning that in every transaction (burst), there are 256 transfers of data. The address of each transfer with a size of 16 bytes (128-bits from the `mem` argument) is the previous address plus 16.
#### pragma HLS INTERFACE axis
The mm2s kernel has one `axis` interface (specifying a AXI4-Stream I/O protocol)
#### pragma HLS PIPELINE II=1
The mm2s kernel has a `for` loop that is a candidate for burst read because the memory addresses per loop iteration is consecutive (`ARBURST=INCR`). To pipeline this `for` loop, you can use this pragma by setting the initiation interval (`II`) = 1.

### s2mm (s2mm.cpp)
The s2mm kernel reads 128 bits of data from an AXI4-Stream interface and writes it to an AXI Memory mapped interface.
#### Arguments
The s2mm kernel takes the following arguments:
* `ap_int<N>` is an arbitrary precision integer data type defined in `ap_int.h` where `N` is a bit-size from 1-1024. For the `mem` argument, the bit-size is set to 128.
* `hls::stream<qdma_axis<D,0,0,0>>` is a data type defined in `ap_axi_sdata.h`. It is a special data class used for data transfer when using a streaming platform. The parameter `<D>` is the data width of the streaming interface and is set to 128 (same as the `mem` argument). The remaining three parameters should be set to 0.

The s2mm kernel also specifies the following pragmas to help optimize the kernel code and adhere to interface protocols
#### pragma HLS INTERFACE s_axilite
The s2mm kernel has one `s_axilite` interface  (specifying a AXI4-Lite slave I/O protocol) with `bundle=control` associated with all the arguments (`mem`,`s`, and `size`). This interface is also associated with `return`.
#### pragma HLS INTERFACE m_axi
The s2mm kernel has one `m_axi` interface (specifying an AXI4 master I/O protocol) with `offset=slave bundle=gmem`. This interface also has `max_write_burst_length=256`. Part of this AXI4 interface is the Write Address channel containing the signals `AWBURST` and `AWLEN`. This interface has a burst type `AWBURST=INCR` and can support burst length `AWLEN` of 1-256 read transfers. In an incrementing burst, the address for each transfer in the burst is an increment of the previous transfer address. The `max_write_burst_length=256` sets the burst length `AWLEN=256` transfers, meaning that in every transaction (burst), there are 256 transfers of data. The address of each transfer with a size of 16 bytes (128-bits from the mem argument) is the previous address plus 16.
#### pragma HLS INTERFACE axis
The s2mm kernel has one `axis` interface (specifying an AXI4-Stream I/O protocol)
#### pragma HLS PIPELINE II=1
The s2mm kernel has a `for` loop that is a candidate for burst write because the memory addresses (mem\[i]) are contiguous (memory accesses across loop iterations are consecutive). To pipeline this `for` loop, you can use this pragma by setting the initiation interval (`II`) = 1.  

</details>

<details>
<summary>PS Host Application</summary>

## PS Host Application
The FIR filter HLS(DSP) tutorial uses the embedded PS as an external controller to control the AI Engine graph and data mover PL kernels. Review [Programming the PS Host Application Section in the AI Engine Documentation](#ai-engine-documentation) to understand the process to create a host application. Note that unlike the AI Engine implementation, there are no AI Engine graphs and associated control code.

Within the PS host application, two classes are defined ((mm2s_class and s2mm_class), which defines methods used to control and monitor the corresponding kernels.

The main sections of the PS host application code is described in the following subsections:

### Define Input and Output Files
A single data file is provides data to stimulate the filter chain.  However, the output data will depend on the value of the makefile parameters N_FIR_FILTERS and N_FIR_TAPS. Data files have been generated for the four corner cases (1 FILTERS / 15 TAPS, 10 FILTERS / 15 TAPS, 1 FILTERS / 240 TAPS, 10 FILTERS / 240 TAPS).  For other configurations, the data is not checked.
```
#include "input_data.h"

#if (N_FIR_FILTERS == 1) && (N_FIR_TAPS == 15)
#include "golden_data_1f_15t.h"
#elif (N_FIR_FILTERS == 10) && (N_FIR_TAPS == 15)
#include "golden_data_10f_15t.h"
#elif (N_FIR_FILTERS == 1) && (N_FIR_TAPS == 240)
#include "golden_data_1f_240t.h"
#elif (N_FIR_FILTERS == 10) && (N_FIR_TAPS == 240)
#include "golden_data_10f_240t.h"
#else
#include "golden_data_1f_15t.h"
#endif
```

### Define Data Sizes
To enable comparing of the 2 implementations of this design, i.e. AIE and HLS implementation, it is desirable to have a small data set to be able to run it through simulation, and a large data set to run through hardware to minimize the effects of measurement errors on determining the performance metrics.  This has been done by providing a small 8k sample of input data (I and Q samples) in which the data repeats twice. The application code then copies the data into potentially much larger buffer, using REPEAT_OFFSET to determine where the data begins to repeat itself, and REPETITIONS to copy from this point forward to the end of the buffer the specified number of times.  Having two cycles of data and a fixed offset (REPEAT_OFFSET) is necessary to allow the filter's start-up transient to settle out and reach a steady state for subsequent cycles. Likewise, FLUSH_SAMPLES specifies the number of zero samples to add to the end of the buffer to clear out the FIR filter, so the application can be run multiple times.

```
#define SAMPLES_PER_WORD   4

#define REPEAT_OFFSET   4096
#define REPETITIONS      509
//#define REPETITIONS        3
#define FLUSH_SAMPLES   4096
```

### load_xclbin Function
This function is responsible for loading the XCLBIN file into the device.

### mm2s Class
This class provides the following methods for controlling / monitoring this kernel:
* init(): allocates the input data buffer object (BO), opens the kernel, and sets the kernel parameters (location of the buffer object, and its length).
* run(): starts execution of the mm2s kernel
* run_wait(): waits for the mm2s kernel to finish
* close(): closes the input data buffer object and kernel
* load(): loads data from the input file into the data buffer, using REPEAT_OFFSET, REPETITIONS and FLUSH_SAMPLES to potentially generate a much larger data set than the input file (see #define-data-sizes).

### s2mm Class
This class provides the following methods for controlling / monitoring this kernel:
* init(): allocates the output data buffer object (BO), opens the kernel, and sets the kernel parameters (location of the buffer object, and its length).
* run(): starts execution the s2mm kernel
* run_wait(): waits for the s2mm kernel to finish
  Note: This call will only return once it receives the number of samples specified computed in the init function. If the application code hangs at this point, it is waiting from data from the filter chain.
* close(): closes the output data buffer object and kernel
* golden_check():  Compare data in the output data buffer object with the data from the output file, using REPEAT_OFFSET and REPETITIONS to compare the data correctly(see #define-data-sizes).

### Main Function
This is the main PS application code that controls the kernels and runs data through the design. The various steps this code goes through is described in the following subsections.

#### 1. Check Command Line Argument
The beginning of the A72 application is represented by the main function. It takes in one command line argument: an XCLBIN file.

#### 2. Open XCLBIN
The A72 application loads the XCLBIN binary file and creates the data mover kernels to be executed on the device.

#### 3. Create and Initialize Data Mover Kernels
Create the kernel objects, initialize them, and load the input data from the constant array into the input buffer.

#### 4. Run Data Mover Kernels
Start execution of the mm2s/s2mm kernels.

#### 5. Wait for Data Mover Kernels to Complete
Wait for the mm2s and s2mm kernels to complete.

#### 6. Verify Output Results
Compare data in `output buffer` object with the reference golden data.

#### 7. Release Allocated Resources
Close the mm2s and s2mm kernel objects.

</details>

# References
The following documents provide supplemental information for this tutorial.

### [AI Engine Documentation](https://docs.xilinx.com/search/all?filters=Document_ID~%2522UG1076%2522_%2522UG1079%2522&content-lang=en-US)
Contains sections on how to develop AI Engine graphs, how to use the AI Engine compiler, AI Engine simulation, and performance analysis.

### [ FIR Compiler v7.2](https://www.xilinx.com/support/documentation/ip_documentation/fir_compiler/v7_2/pg149-fir-compiler.pdf)
Describes the FIR Compiler IP describes all of the parameters and settings and how they control the final filter implementation.

# Revision History
* Jul 2021 - Initial Release

# Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )



Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD061 | &copy; Copyright 2021 Xilinx, Inc.</sup></p>
