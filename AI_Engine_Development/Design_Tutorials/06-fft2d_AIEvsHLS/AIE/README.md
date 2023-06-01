<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Versal® ACAP AI Engine Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# 2023.1 Versal 2D-FFT Implementation Using Vitis Acceleration Library Tutorial (XD073)

# AI Engine Implementation 

## Table of Contents

[Building the Design](#Building-the-Design)

[Hardware Design Details](#Hardware-Design-Details)

[Software Design Details](#Software-Design-Details)

[Performance Details](#Performance-Details)

## Building the Design

<details>
<summary>Design Build</summary> 

### Design Build

In this section, you build and run the 2D-FFT design using the AI Engine implementation. You compile and integrate the AI Engine design into a larger system design (including the PL kernels and PS host application). Review the [Integrating the Application section in the AI Engine Documentation](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Integrating-the-Application-Using-the-Vitis-Tools-Flow) for the general flow.

At the end of this section, the design flow generates a new directory (called `build/`). Underneath are sub-directories named `(cint16_dsns-cfloat_dsns)/fft2d_$(MAT_ROWS)x$(MAT_COLS)/x$(FFT_2D_INSTS)/` (for example, `cint16_dsns/fft2d_1024x2048/x1/`) depending on the datatype `${FFT_2D_DT}`, value of matrix dimensions `${MAT_ROWS}`, `${MAT_COLS}` and the number of instances `$(FFT_2D_INSTS)` chosen in the build. Each sub-directory contains the `hw_emu/` and/or `hw/` subfolders. The respective subfolders contain `Work/` and `libadf.a`, outputs from the AI Engine compiler, the host app executable, and the builds, targeted to `hw` or `hw_emu`, respectively. The `hw_emu/` subfolder contains the build for the hardware emulation. The `hw/` subfolder contains the build for hardware running on a VCK190 board.

</details>

<details>
<summary>Make Steps</summary> 

### Make Steps

To run the following `make` steps (that is, `make kernels`, `make graph`, and so on), you must be in the `AIE/` folder. The options that can be specified in the `make` steps are as follows.

`TARGET:` This can be set to `hw` or `hw_emu` to build the design in the hardware or hardware emulation flow, respectively. The default option is `hw_emu`.

`FFT_2D_INSTS:` This can be set to 1, 5, or 10 to build the design with the number of kernel instances. The default is `1`.

`ITER_CNT:` The number of iterations the design is run. The default is `8`.

`FFT_2D_PT`: FFT 2D point. Permissible values are `64`, `128`, `256`, `512`, and `2048`.

`FFT_2D_DT`: FFT 2D Datatype. Permissible values are `0` and `1` for cint16 and cfloat datatypes.

`MAT_ROWS x MAT_COLS:` Dimensions of the matrix (number of rows in the input matrix x number of cols in the input matrix). Permissible values are `32x64`, `64x128`, `128x256`, `256x512`, and `1024x2048`. The default is `1024x2048`. Automatically configured as `FFT_2D_PT/2, FFT_2D_PT`.

`EN_TRACE:` Flag to enable trace profiling. The default is `0` (disabled). `0` is disabled, and `1` is enabled.

The Makefile uses the following directory references:

```
## Relative fft_2d directory
RELATIVE_PROJECT_DIR := ./

## Absolute fft_2d directory = <user path>/Tutorials/AI_Engine/fft_2d
PROJECT_REPO := $(shell readlink -f $(RELATIVE_PROJECT_DIR))

DESIGN_REPO  := $(PROJECT_REPO)/design
AIE_SRC_REPO := $(DESIGN_REPO)/aie_src
HOST_APP_SRC := $(DESIGN_REPO)/host_app_src
PL_SRC_REPO  := $(DESIGN_REPO)/pl_src

SYSTEM_CONFIGS_REPO    := $(DESIGN_REPO)/system_configs
PROFILING_CONFIGS_REPO := $(DESIGN_REPO)/profiling_configs
EXEC_SCRIPTS_REPO      := $(DESIGN_REPO)/exec_scripts
VIVADO_METRICS_SCRIPTS_REPO := $(DESIGN_REPO)/vivado_metrics_scripts

BASE_BLD_DIR     := $(PROJECT_REPO)/build
FFT_2D_DT_DIR    := $(BASE_BLD_DIR)/cint16_dsns
FFTPT_BLD_DIR    := $(FFT_2D_DT_DIR)/fft2d_$(MAT_ROWS)x$(MAT_COLS)
INSTS_BLD_DIR    := $(FFTPT_BLD_DIR)/x$(FFT_2D_INSTS)
BUILD_TARGET_DIR := $(INSTS_BLD_DIR)/$(TARGET)

REPORTS_REPO := $(PROJECT_REPO)/reports_dir
BLD_REPORTS_DIR := $(REPORTS_REPO)/$(FFT_2D_DT_DIR_VAL)/fft2d_$(MAT_ROWS)x$(MAT_COLS)/x$(FFT_2D_INSTS)

EMBEDDED_PACKAGE_OUT := $(BUILD_TARGET_DIR)/package
EMBEDDED_EXEC_SCRIPT := run_script.sh

WORK_DIR         := Work
```

</details>

<details>
<summary>Build the Entire Design with a Single Command</summary>

### Build the Entire Design with a Single Command

If you are already familiar with the AI Engine and AMD Vitis&trade; kernel compilation flows, you can build the entire design for each case of `FFT_2D_INSTS` with one command:

```bash
make run (default hardware emulation, cint16 datatype, 1 instance, iterations=8, matrix dimentions rows=1024 and columns=2048, no trace-profiling )
```
or 

```bash
make run TARGET=hw FFT_2D_DT=0 FFT_2D_INSTS=5 ITER_CNT=16 EN_TRACE=1 FFT_2D_PT=64 (hardware, cint16 datatype, 5 instances, 16 iterations, enable trace profiling, matrix dimentions rows=32 and columns=64 )
```

This command runs the `make kernels`, ` make graph`, `make xsa`,`make an application`, `make package`, and `make run_emu` for hardware emulation or to run on hardware (VCK190 board) depending on the `TARGET` you specify. The settings also apply to the individual make steps listed below.

Each `make` step to build the design is specified in the following sections. These sections also detail the options used and the location of input and output files in each case. The generated files for each `FFT_2D_INSTS` are placed under an individual directory: `$(BUILD_TARGET_DIR)/`.

</details>

<details>
<summary>make kernels: Compiling PL Kernels</summary> 

### make kernels: Compiling PL Kernels

In this step, the Vitis compiler takes any Vitis compiler kernels (RTL or HLS C) in the PL region of the target platform (`xilinx_vck190_base_202310_1`) and the AI Engine kernels and graphs and compiles them into their respective XO files. The following commands compile the kernels (default `TARGET=hw_emu`, `FFT_2D_INSTS=1`, `ITER_CNT=16`, `FFT_2D_DT=0` and `FFT_2D_PT=2048`).

```
make kernels
```

The expanded command is as follows (for `dma_hls`):

```
mkdir -p $(BUILD_TARGET_DIR); \

cd $(BUILD_TARGET_DIR); \

v++ --target hw_emu --hls.clock 312500000:dma_hls --platform xilinx_vck190_base_202310_1 \
   --save-temps --temp_dir $(BUILD_TARGET_DIR)/_x --verbose -g -c -k dma_hls -D FFT_2D_DT=0 \
   $(DESIGN_REPO)/pl_src/dma_hls.cpp -o $(BUILD_TARGET_DIR)/dma_hls.hw_emu.xo
```

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
|--output \| -o|Specifies the name of the output file generated by the V++ command. The DMA HLS kernels output should be XO.|

|Input|Description|
|  ---  |  ---  |
|$(PL_SRC_REPO)/dma_hls.cpp|Defines the data mover PL kernel.|

|Output|Description|
|  ---  |  ---  |
|$(BUILD_TARGET_DIR)/dma_hls.hw_emu.xo|The data mover kernel object file.|

</details>

<details>
<summary>make graph: Creating the AI Engine ADF Graph for the Vitis Compiler Flow</summary>

### make graph: Creating the AI Engine ADF Graph for Vitis Compiler Flow

An ADF graph can be connected to an extensible Vitis platform (the graph I/Os can be connected to platform ports or ports on Vitis kernels through Vitis compiler connectivity directives).

* The AI Engine ADF C++ graph of the design contains AI Engine kernels and PL kernels.
* All interconnects between kernels are defined in the C++ graph
* All interconnections to external I/O are fully specified in the C++ simulation testbench (`graph.cpp`) that instantiates the C++ ADF graph object.

To compile the graph using the Makefile flow type (default `FFT_2D_DT=0`, `TARGET=hw_emu`, `FFT_2D_INSTS=1`, `ITER_CNT=8`, `EN_TRACE=0`, `FFT_2D_PT=2048`):

```
make graph
```

The following AI Engine compiler command compiles the AI Engine design graph:

```
cd $(BUILD_TARGET_DIR); \

aiecompiler -include=$(AIE_SRC_REPO) -include=<DSPLIB_ROOT>/L1/include/aie \
   -include=<DSPLIB_ROOT>/L1/src/aie \
   -include=<DSPLIB_ROOT>/L1/tests/aie/inc \
   -include=<DSPLIB_ROOT>/L1/tests/aie/src \
   -include=<DSPLIB_ROOT>/L2/include/aie \
   -include=<DSPLIB_ROOT>/L2/tests/aie/common/inc \
   --verbose --Xpreproc="-DFFT2D_INSTS=1" --Xpreproc="-DMAT_ROWS=1024" --Xpreproc="-DMAT_COLS=2048" --Xpreproc="-DFFT_2D_DT=0" \
   --platform=<PLATFORM_REPO_PATHS/xilinx_vck190_base_202310_1>/xilinx_vck190_base_202310_1.xpfm \
   --log-level=5 --test-iterations=2 --dataflow --heapsize=7000 \
   --Xchess="main:bridge.llibs=softfloat m" --workdir=Work $(AIE_SRC_REPO)/graph.cpp 2>&1 | tee -a aiecompiler.log 

 ```

See [this page](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment) for full AI Engine programming environment documentation.

The following table provides a summary of the switches used.

|Switch|Description|
|  ---  |  ---  |
|--include=\<string\>|Specify compile-time include directory (zero or more).|
|--verbose\|-v|Verbose output of the AI Engine compiler emits compiler messages at various stages of compilation. These debug and tracing logs provide useful messages on the compilation process.|
|--Xpreproc="-D\<Pre-processor Macro String\>"|Specify compile time macro.|
|--Xchess="\<Chess Make Options\>"|Specify compile time chess make options; "main:bridge.llibs=softfloat m" enables floating point operations.|
|--heapsize=\<int\>|Heap size in bytes.|
|--log-level=\<int\>|Log level for verbose logging (default=1).|
|--workdir=\<string\>|By default, the compiler writes all outputs to a sub-directory of the current directory, called Work. Use this option to specify a different output directory.|

The following is a description of the output objects that results from executing the AI Engine compiler (`aiecompiler`) command.

|Inputs Sources|Description|
|  ---  |  ---  |
|$(AIE_SRC_REPO)/graph.cpp|Defines the row wise and col wise FFT graph objects.|

|Output Objects|Description|
|  ---  |  ---  |
|$(BUILD_TARGET_DIR)/libadf.a|Compiled AI Engine design graph.|
|$(BUILD_TARGET_DIR)/Work/|Directory that contains all outputs of the AI Engine compiler.|
</details>

<details>
<summary>make xsa: Using the Vitis Tools to Link AI Engine and HLS Kernels with the Platform</summary>

### make xsa: Using the Vitis Tools to Link AI Engine and HLS Kernels with the Platform

After the AI Engine kernels and graph and PL HLS kernels have been compiled, you can use the Vitis compiler to link them with the platform to generate aN XSA file.

The Vitis tools allow you to integrate the AI Engine, HLS, and RTL kernels into an existing extensible platform. This is an automated step from a software developer perspective where the hardware designer provides the platform chosen. Alternatively, you can use one of the many extensible base platforms provided by AMD and the Vitis tools to build the hardware design and integrate the AI Engine and PL kernels.
 
To test this feature in this tutorial, use the base VCK190 platform to build the design. The command to run this step is shown in the following example (default `TARGET=hw_emu`, `FFT_2D_INSTS=1`, `ITER_CNT=16`, `EN_TRACE=0`, `FFT_2D_PT=2048`):

```
make xsa
``` 

The expanded command is as follows:

```
cd $(BUILD_TARGET_DIR);	\

v++ -l --platform xilinx_vck190_base_202310_1 --save-temps \
   --temp_dir $(BUILD_TARGET_DIR)/_x --verbose -g --clock.freqHz 312500000:dma_hls_0 \
   --clock.defaultTolerance 0.001 --config $(SYSTEM_CONFIGS_REPO)/x1.cfg \
   --vivado.prop fileset.sim_1.xsim.simulate.log_all_signals=true \
   -t hw_emu -o $(BUILD_TARGET_DIR)/vck190_aie_fft_2d.hw_emu.xsa \
   $(BUILD_TARGET_DIR)/dma_hls.hw_emu.xo \
   $(BUILD_TARGET_DIR)/libadf.a

```

If `EN_TRACE` is enabled, the following Vitis compiler flags are also set:

```
   --profile.data dma_hls:all:all or profile.data dma_hls:all:strmInp_from_colwiseFFT (for higher instances) \
   --profile.trace_memory DDR

```
For higher values of `FFT_2D_INSTS`, only the `strmInp_from_colwiseFFT` port is profiled to avoid too much data.

See [this page](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Building-the-Device-Binary) for a detailed description of Vitis linking options.

|Switch|Description|
|  ---  |  ---  |
|--platform \| -f|Specifies the name of a supported acceleration platform as specified by the $PLATFORM_REPO_PATHS environment variable or the full path to the platform XPFM file.|
|--save-temps \| -s|Directs the V++ command to save intermediate files/directories created during the compilation and link process. Use the `--temp_dir` option to specify a location to write the intermediate files to.|
|--temp_dir <string>|This allows you to manage the location where the tool writes temporary files created during the build process. The temporary results are written by the Vitis compiler, and then removed, unless the `--save-temps` option is also specified.|
|--verbose|Display verbose/debug information.|
|--config <config_file>|Specifies a configuration file containing V++ switches.|
|--output \| -o|Specifies the name of the output file generated by the V++ command. In this design the outputs of the DMA HLS kernels and the PL kernels interfacing with the AI Engine are in XO files.|
|--profile.data [<kernel_name>\|all]:[<cu_name>\|all]:[<interface_name>\|all]\(:[counters\|all]\)|Enables monitoring of data ports through the monitor IPs. This option needs to be specified during linking. See [this page](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/profile-Options) for detailed profiling options.|
|--profile.trace_memory \<FIFO\>:\<size\>\|\<MEMORY\>[\<n\>]|When building the hardware target \(-t=hw\), use this option to specify the type and amount of memory to use for capturing trace data. See [this page](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/profile-Options) for detailed profiling options.|

The information to tell the linker how to connect the AI Engine and PL kernels together is described in a configuration file, `system_configs/x$(FFT_2D_INSTS).cfg`. The file describes the overall connection scheme of the system.

```
[connectivity]
nk=dma_hls:1:dma_hls_0

#Connections For FFT-2D Insts 0...
stream_connect=dma_hls_0.strmOut_to_rowiseFFT:ai_engine_0.DataIn0
stream_connect=ai_engine_0.DataOut0:dma_hls_0.strmInp_from_rowiseFFT
stream_connect=dma_hls_0.strmOut_to_colwiseFFT:ai_engine_0.DataIn1
stream_connect=ai_engine_0.DataOut1:dma_hls_0.strmInp_from_colwiseFFT

[advanced]
## Disable Profiling in hw_emu so that it is faster...
param=hw_emu.enableProfiling=false



```

See [this page](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Compiler-Configuration-File) for a detailed description of the Vitis compiler configuration file.


|Switch|Comment|
|  ---  |  ---  |
|--connectivity.nk|Number of kernels. `dma_hls:1:dma_hls_0` means that the Vitis compiler should instantiate one dma_hls kernel and name the instance `dma_hls_0`.|
|--connectivity.stream_connect|How the kernels will connect to IPs, platforms, or other kernels. The output of the AI Engine compiler tells you the interfaces that need to be connected. `dma_hls_0.strmOut_to_rowiseFFT:ai_engine_0.DataIn0` means that the Vitis compiler should connect the port `strmOut_to_rowiseFFT` of the `dma_hls` PL kernel to the shim channel of the AI Engine with the logical name `DataIn0`, defined in `$(AIE_SRC_REPO)/graph.cpp` as part of the PLIO instantiation.|
|param=compiler.addOutputTypes=hw_export| This option tells the Vitis compiler that besides creating an XSA file, it also outputs an XSA file which is needed to create a post-Vivado fixed platform for Vitis software development.|

The Vitis compiler calls the AMD Vivado&trade; IP integrator under the hood to build the design. The platform and kernels are input to the Vivado Design Suite, which produces a simulation XSA or an XSA after running place and route on the design. The point at which the XSA is produced from Vivado depends on the `-target` option set on the Vitis compiler command line. 

You can now view the Vivado project, which is located in the `$(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj` directory. You have now generated the XSA file that will be used to execute your design on the platform.

</details>

<details>
<summary>make application: Compiling the Host Application</summary> 

### make application: Compiling the Host Application

You can compile the host application by following the typical cross-compilation flow for the Cortex A72. To build the application, run the following command (default `FFT_2D_DT=0`, `FFT_2D_INSTS=1`, `ITER_CNT=16`, `FFT_2D_PT=2048`):

```
make application
```
or

```
cd $(BUILD_TARGET_DIR);	\

aarch64-xilinx-linux-g++ -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong\
   -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$(SDKTARGETSYSROOT) -O -c\
   -std=c++14 -D__linux__ -D__PS_ENABLE_AIE__ -DXAIE_DEBUG -DFFT2D_INSTS=1 -DITER_CNT=8 -DFFT_2D_DT=0\
   -DMAT_ROWS=1024 -DMAT_COLS=2048 -I$(SDKTARGETSYSROOT)/usr/include/xrt -I$(XILINX_VITIS)/aietools/include/\
   -I$(SDKTARGETSYSROOT)/usr/include -I$(SDKTARGETSYSROOT)/usr/lib -I$(AIE_SRC_REPO) -I$(HOST_APP_SRC)\
   -I$(DSPLIB_ROOT)/L1/include/aie -I$(DSPLIB_ROOT)/L1/src/aie -I$(DSPLIB_ROOT)/L1/tests/aie/inc\
   -I$(DSPLIB_ROOT)/L1/tests/aie/src -I$(DSPLIB_ROOT)/L2/include/aie -I$(DSPLIB_ROOT)/L2/tests/aie/common/inc\
   $(BUILD_TARGET_DIR)/$(WORK_DIR)/ps/c_rts/aie_control_xrt.cpp -o $(BUILD_TARGET_DIR)/app_control.o

aarch64-xilinx-linux-g++ -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong\
   -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$(SDKTARGETSYSROOT) -O -c\
   -std=c++14 -D__linux__ -D__PS_ENABLE_AIE__ -DXAIE_DEBUG -DFFT2D_INSTS=1 -DITER_CNT=16 -DFFT_2D_DT=0\
   -DMAT_ROWS=1024 -DMAT_COLS=2048 -I$(SDKTARGETSYSROOT)/usr/include/xrt -I$(XILINX_VITIS)/aietools/include/\
   -I$(SDKTARGETSYSROOT)/usr/include -I$(SDKTARGETSYSROOT)/usr/lib -I$(AIE_SRC_REPO) -I$(HOST_APP_SRC)\
   -I$(DSPLIB_ROOT)/L1/include/aie -I$(DSPLIB_ROOT)/L1/src/aie -I$(DSPLIB_ROOT)/L1/tests/aie/inc\
   -I$(DSPLIB_ROOT)/L1/tests/aie/src -I$(DSPLIB_ROOT)/L2/include/aie -I$(DSPLIB_ROOT)/L2/tests/aie/common/inc\
   $(HOST_APP_SRC)/fft_2d_aie_app.cpp -o $(BUILD_TARGET_DIR)/fft_2d_aie_app.o -L$(SDKTARGETSYSROOT)/usr/lib\
   -L$(XILINX_VITIS)/aietools/lib/aarch64.o -L$(XILINX_VITIS)/aietools/lib/lnx64.o -ladf_api_xrt -lxrt_coreutil

aarch64-xilinx-linux-g++ -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong
   -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$(SDKTARGETSYSROOT)\
   $(BUILD_TARGET_DIR)/app_control.o $(BUILD_TARGET_DIR)/fft_2d_aie_app.o -L$(SDKTARGETSYSROOT)/usr/lib\
   -L$(XILINX_VITIS)/aietools/lib/aarch64.o -L$(XILINX_VITIS)/aietools/lib/lnx64.o\
   -ladf_api_xrt -lxrt_coreutil -o $(BUILD_TARGET_DIR)/fft_2d_aie_xrt.elf
```

See [this page](https://xilinx.github.io/XRT/master/html/index.html) for XRT documentation. See [this page](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Best-Practices-for-Host-Programming) for details of host application programming.


|Switch|Description|
|  ---  |  ---  |
|-O \| Optimize.| Optimizing compilation takes more time and a lot more memory for a large function. With -O, the compiler tries to reduce code size and execution time, without performing any of the optimizations that can take a great deal of compilation time.|
|-D__linux__|
|-DXAIE_DEBUG|Enable debug interface capabilities where certain core status, event status, or stack trace can be dumped out.|
|-D\<Pre-processor Macro String\>=\<value\>|Pass pre-processor macro definitions to the cross-compiler.|
|-I \<dir\>|Add the directory `dir` to the list of directories to be searched for header files.|
|-o \<file\>|Place output in file `<file>`. This applies regardless of the output being produced, whether it be an executable file, an object file, an assembler file, or preprocessed C code.|
|--sysroot=\<dir\>|Use `dir` as the logical root directory for headers and libraries. For example, if the compiler normally searches for headers in `/usr/include` and libraries in `/usr/lib`, it instead searches `dir/usr/include` and `dir/usr/lib`. This is automatically set by the `env_setup.sh` script.|
|-l\<library\>|Search the library named `library` when linking. The 2D-FFT tutorial requires the `adf_api_xrt` and `xrt_coreutil` libraries.|
|-L \<dir\>|Add directory `<dir>` to the list of directories to be searched for `-l`.|

The following is a description of the input sources compiled by the AI Engine compiler command. 

|Inputs Sources|Description|
|  ---  |  ---  |
|$(HOST_APP_SRC)/fft_2d_aie_app.cpp|Source application file for the `fft_2d_aie_xrt.elf` that will run on an A72 processor.|
|$(BUILD_TARGET_DIR)/Work/ps/c_rts/aie_control_xrt.cpp|This is the AI Engine control code generated implementing the graph APIs for the Lenet graph.|

The following is a description of the output objects that results from executing the AI Engine compiler command with the above inputs and options. 

|Output Objects|Description|
|  ---  |  ---  |
|$(BUILD_TARGET_DIR)/fft_2d_aie_xrt.elf|The executable that will run on an A72 processor.|

</details>

<details>
<summary>make package: Packaging the Design</summary> 

### make package: Packaging the Design

With the AI Engine outputs created, and the new platform, you can now generate the programmable device image (PDI) and a package to be used on an SD card. The PDI contains all the executables, bitstreams, and device configurations. The packaged SD card directory contains everything to boot Linux, the generated applications, and the XCLBIN.

The command to run this step is as follows (default `TARGET=hw_emu`, `EN_TRACE=0`, `FFT_2D_DT=0`, `FFT_2D_INSTS=1`, and `FFT_2D_PT=2048`):

```
make package
``` 

or

```
cp $(PROJECT_REPO)/run_script.sh $(BUILD_TARGET_DIR)/
cd$(BUILD_TARGET_DIR);	\

v++ -p -t hw --save-temps --temp_dir $(BUILD_TARGET_DIR)/_x -f xilinx_vck190_base_202310_1\
   --package.rootfs $(XLNX_VERSAL)/rootfs.ext4\
   --package.kernel_image $(XLNX_VERSAL)/Image --package.boot_mode=sd\
   --package.out_dir $(BUILD_TARGET_DIR)/package --package.image_format=ext4\
   --package.sd_file $(BUILD_TARGET_DIR)/fft_2d_aie_xrt.elf \
   $(BUILD_TARGET_DIR)/vck190_aie_fft_2d.hw.xsa $(BUILD_TARGET_DIR)/libadf.a \
   --package.defer_aie_run \
```

If `EN_TRACE` is enabled, the following Vitis compiler flags are also set:

```
   --package.sd_file $(PROFILING_CONFIGS_REPO)/xrt.ini
```

If `XRT_ROOT` is set, the following Vitis compiler flags are also set:

```
   --package.sd_dir $(XRT_ROOT)
```

See [this page](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Packaging) for more details about packaging the system.

|Switch|Description|
|  ---  |  ---  |
|--target \| -t [hw\|hw_emu]|Specifies the build target.|
|--package \| -p|Packages the final product at the end of the Vitis compile and link build process.|
|--package.rootfs \<arg\>|Where \<arg\> specifies the absolute or relative path to a processed Linux root file system file. The platform RootFS file is available for download from xilinx.com. Refer to the [Vitis Software Platform Installation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installation) for more information.|
|--package.kernel_image \<arg\>|Where \<arg\> specifies the absolute or relative path to a Linux kernel image file. Overrides the existing image available in the platform. The platform image file is available for download from xilinx.com. Refer to the [Vitis Software Platform Installation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installation) for more information.|
|--package.boot_mode \<arg\>|Where \<arg\> specifies <ospi\|qspi\|sd>. Boot mode used for running the application in emulation or on hardware.|
|--package.image_format|Where \<arg\> specifies the \<ext4\|fat32\> output image file format. `ext4` is the Linux file system and `fat32` is the Windows file system.|
|--package.sd_file|Where \<arg\> specifies an ELF or other data file to package into the `sd_card` directory/image. This option can be used repeatedly to specify multiple files to add to the `sd_card` directory.|
|--package.defer_aie_run| Load the AI Engine application with the ELF file, but wait to run it until graph run directs it. This is required in the PS based AI Engine flow.|

|Inputs Sources|Description|
|  ---  |  ---  |
|$(PLATFORM_REPO_PATHS)/sw/versal/xrt|The PS host application needs the XRT headers in this folder to execute.|
|$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/rootfs.ext4|The root filesystem file for PetaLinux.|
|$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/Image|The pre-built PetaLinux image that the processor boots from.|
|$(BUILD_TARGET_DIR)/fft_2d_aie_xrt.elf|The PS host application executable created in the `make application` step.|
|$(BUILD_TARGET_DIR)/vck190_aie_fft_2d.hw_emu.xsa|The XSA file created in the `make xsa` step.|
|$(BUILD_TARGET_DIR)/libadf.a|The compiled AI Engine design graph created in the `make graph` step.|

The Vitis compiler package step output is the package directory containing the contents to run hardware emulation.

|Output Objects|Description|
|  ---  |  ---  |
|$(BUILD_TARGET_DIR)/package|The hardware emulation package that contains the boot file, hardware emulation launch script, PLM and PMC boot files, PMC and QEMU command argument specification files, and Vivado simulation folder.|

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
mount /dev/mmcblk0p1 /mnt
cd /mnt
./fft_2d_aie_xrt.elf a.xclbin
```

The `fft_2d_aie_xrt.elf` executes. After a few minutes, you should see the output with `TEST PASSED` on the console. When this is shown, run the following keyboard command to exit the QEMU instance:

```
#To exit QEMU Simulation
Press Ctrl+A, let go of the keyboard, and then press x 
```

To run with waveform, do the following:

```
cd $(BUILD_TARGET_DIR)/package
./launch_hw_emu.sh -g
```

The XSIM Waveform Viewer is launched. Drag and drop the signals into the viewer and click **Play** to start the emulation. Return to the terminal and wait for the Linux prompt to appear. In the XSIM Waveform Viewer, you see the signals you added to the waveform adjusting over the execution of the design. When done, hit the pause button and close the window to end the emulation.

The following figure shows a waveform view of the 32x64 - 1x design.

![Image of 2D-FFT AIE HW_EMU Run Waveform View For 32x64-1x Design](images/fft_2d_aie_hw_emu_waveform_view_32x64_x1.PNG)

</details>

<details>
<summary>TARGET=hw: Running on Hardware</summary>

### Running on Hardware

To run the design in hardware, rerun the following `make` steps with `TARGET=hw` and other applicable options (see the preceding `make` steps previously specified).

```
make kernels TARGET=hw
make xsa TARGET=hw 
make package TARGET=hw 
```

These commands create a `$(BUILD_TARGET_DIR)` folder with the kernels, XSA, and `package` for a hardware run.

Run the following step to set up the execution file, generated images, and base images (`$(BUILD_TARGET_DIR)/package/sd_card` and `$(BUILD_TARGET_DIR)/package/sd_card.img`).

```
make run_emu TARGET=hw 
```

These commands create a `build/hw` folder with the kernels, XSA, and `package` for a hardware run. Follow steps 1-9 to run the `fft_2d_aie_xrt.elf` executable on your VCK190 board.

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

**Step 8.** Wait until you see the `root@versal-rootfs-common-2023.1` Linux command prompt. Press **Enter** a few times to get past any `xinit` errors.

**Step 9.** Run the following commands in the TeraTerm terminal:

```
cd /mnt/sd-mmcblk0p1
./init.sh
./fft_2d_aie_xrt.elf a.xclbin
```

</details>

## Hardware Design Details
<details>
<summary>2D-FFT AI Engine Implementation Architecture and AI Engine/PL Function Partitioning</summary>

### 2D-FFT AI Engine Implementation Architecture and AI Engine/PL Function Partitioning

The following figure shows a high-level block diagram of the design. The test harness consists of the AI Engine and data mover HLS kernels (`dma_hls`). In this setup, there is an AXI4-Stream interface between the data mover kernels and AI Engines, with a data width of 128 bits. The data mover kernels and the AI Engine array interface run at 312.5 MHz.

The data mover is a PL-based data generator and checker. It generates impulse input and checks the output of the row-wise FFT core for its response. It then generates the transposed pattern of the row-wise FFT output, feeds that to the col-wise FFT core, and checks its output.

![Image of 2D-FFT AIE Implementation Architecture](images/fft_2d_aie_block_diagram.PNG)

</details>

<details>
<summary>Design Details</summary>

### Design Details

The design in this tutorial starts with a base platform containing the control interface and processing system (CIPS), NoC, AI Engine, and the interfaces among them. The Vitis compiler linker step builds on top of the base platform by adding the AI Engine graphs and PL kernels. To add the various functions in a system-level design, PL kernels are added to the base platform depending on the application (that is, the PL kernels present in each design might vary). An ADF graph is connected to an extensible Vitis platform. The graph I/Os is connected to the platform ports or ports on Vitis kernels through the Vitis compiler connectivity directives. In the design, the components are added by the Vitis compiler `-l` step (see [make XSA](#make-xsa-using-the-vitis-tools-to-link-ai-engine-and-hls-kernels-with-the-platform)) and include the following:


* `libadf.a`
* Data mover kernel (`dma_hls.[hw|hw_emu].xo`)
* Connection interfaces defined in the system configuration file

To see a schematic view of the design with the extended platform, as shown in the following figure, open the following in Vivado:

```
`build/fft2d_$(MAT_ROWS)x$(MAT_COLS)/x$(FFT_2D_INSTS)/[hw|hw_emu]/_x/link/vivado/vpl/prj/prj.xpr`
```

![Image of 2D-FFT AIE 1x Vivado BD](images/fft_2d_aie_1x_vivado_bd.PNG)

In this design, the 2D FFT computation happens in two stages: the first computation is across the row vectors, and the second is performed across the column vectors. The input data is accessed linearly and streamed to the AI Engines, which perform `MAT_COLS( default 2048 )` point FFT. The data from the AI Engines is streamed to a PL kernel, where it is checked against the expected pattern (the first row should be one, and the remaining should be 0). If there is a mismatch, it is recorded in the variable `stage0_errCnt`. The transposed pattern of the output of the row vectors is then linearly streamed into another AI Engine, which performs `MAT_ROWS( default 1024 )` point FFT. The output is streamed into a data mover kernel again and is checked against the expected pattern (all values should be 1). If there is a mismatch, it is stored in the variable `stage1_errCnt`. Finally, the sum of `stage0_errCnt` and `stage1_errCnt` is returned from the kernel, which is read in the host app to determine whether the test has passed or failed.

The system debugging and profiling IP (DPA) is added to the PL region of the device to capture AI Engine runtime trace data if the `EN_TRACE` option is enabled in the design. The `dma_hls` kernel and the AI Engine array interface operate at 312.5 MHz. Unlike the HLS/DSP implementation, this design has no clock domain crossing in the PL region.

</details>

<details>
<summary>AI Engine and PL Kernels</summary>

### AI Engine and PL Kernels

The top-level AI Engine graph, `graph.cpp`, contains two sub-graphs: `FFTrows_graph` and `FFTcols_graph`. Each sub-graph contains the individual AI Engine kernel, `*FFTrow_gr.getKernels()`, and `*FFTcol_gr.getKernels()`, which performs `MAT_COLS` and `MAT_ROWS` point FFT respectively.

#### dma_hls

The PL-based data movers consist of the `dma_hls` kernel, which generates impulse input and checks the output of each FFT stage for the expected pattern.

* It internally comprises four loops (`mm2s0`, `s2mm0`, `mm2s1`, and `s2mm1`), with `s2mm0` - `mm2s1` sequenced one after the other and wrapped into the `dmaHls_rowsToCols` function. `mm2s0`, `dmaHls_rowsToCols`, and `s2mm1` are concurrently scheduled.
* The data width is 128 bits at both the AXI4-stream I/O sides, running at 312.5 MHz.

</details>

## Software Design Details

The software design in the AI Engine 2D-FFT tutorial consists of the following sections:

<details>
<summary>Methodology</summary>

### Methodology

The following figure elaborates on the AI Engine implementation methodology.

![Image of 2D-FFT AIE Implementation Methodology](images/fft_2d_aie_block_diagram_methodology.PNG)

#### AI Engine

##### Independent Cores

Both AI Engine graphs for `FFTrows_graph` and `FFTcols_graph` are to be configured to be independent, with runtime ratios set to >= 0.6 so that each can be run independently of the other.

```
...
runtime<ratio>(*FFTrow_gr.getKernels()) = 0.6;
...
runtime<ratio>(*FFTcol_gr.getKernels()) = 0.6;
...
```

##### Window Streaming Buffer Config

The `FFTrows_graph` graph performs the `MAT_COLS` point FFT and runs for the `MAT_ROWS` number of iterations. For the `FFTcols_graph` graph, increase the `TP_WINDOW_VSIZE` to `MAT_COLS` instead of `MAT_ROWS,` and it does `MAT_ROWS` point FFT, but runs for `MAT_ROWS` number of iterations instead of `MAT_COLS`. This reduces the ping-pong overhead, which improves the overall throughput. 

Large windows may result in mapper errors due to excessive memory usage. The increased `TP_WINDOW_VSIZE` reduces ping-pong overhead but increases the utilization of AIE cores and, thereby, the power consumption. In this design, due to the rows to cols ratio being 1:2, the `TP_WINDOW_VSIZE` of both graphs are also in the same ratio, which gives an additional increase in throughput with minimal increase in utilization.

```
...
// TP_WINDOW_VSIZE for FFTrows_graph...
#define FFT_ROW_TP_WINDOW_VSIZE MAT_COLS

// TP_WINDOW_VSIZE for FFTcols_graph
// Increasing the "TP__WINDOW _VSIZE" so that the ping-pong overhead is less
// Assigning it as MAT_COLS instead of MAT_ROWS...
#define FFT_COL_TP_WINDOW_VSIZE MAT_COLS

////////////////////////////////////////////////////////
// FFT_2D Datatype related Macros
// Datatype can be:
// cint16 (Default) or cfloat...
// 0=cint16(Default)
// 1=cfloat
#if FFT_2D_DT == 0

   // Input data type...
   #define FFT_2D_TT_DATA cint16
   // Twiddle Factor data type...
   #define FFT_2D_TT_TWIDDLE cint16
   
   // FFTrows_graph I/O WINDOW BUFF SIZE IN BYTES...
   #define FFT_ROW_WINDOW_BUFF_SIZE (FFT_ROW_TP_WINDOW_VSIZE * 4)
   // FFTcols_graph I/O WINDOW BUFF SIZE IN BYTES...
   #define FFT_COL_WINDOW_BUFF_SIZE (FFT_COL_TP_WINDOW_VSIZE * 4)
   
#elif FFT_2D_DT == 1

   // Input data type...
   #define FFT_2D_TT_DATA cfloat
   // Twiddle Factor data type...
   #define FFT_2D_TT_TWIDDLE cfloat
   
   // FFTrows_graph I/O WINDOW BUFF SIZE IN BYTES...
   #define FFT_ROW_WINDOW_BUFF_SIZE (FFT_ROW_TP_WINDOW_VSIZE * 8)
   // FFTcols_graph I/O WINDOW BUFF SIZE IN BYTES...
   #define FFT_COL_WINDOW_BUFF_SIZE (FFT_COL_TP_WINDOW_VSIZE * 8)
   
#endif
...
```

#### Data Mover

##### Data Generation/Checking and Sequencing

The data mover comprises four loops: `mm2s0`, `s2mm0`, `mm2s1`, and `s2mm1`. The `s2mm0` and `mm2s1` functions are wrapped into a single function, `dmaHls_rowsToCols`. Within that, the execution sequence, `s2mm0` is followed by `mm2s1`. The `s2mm0` and `s2mm1` functions check the row-wise and col-wise FFT output, respectively, against the expected golden output.

##### Concurrent Scheduling

Concurrent scheduling is required so that each function runs independently and the execution of one function is not blocking the other. The concurrent scheduling of the three functions `mm2s0`, `dmaHls_rowsToCols`, and `s2mm1` is achieved using `#pragma HLS DATAFLOW` as shown in the following example.

```
#pragma HLS DATAFLOW
...
LOOP_ITER_MM2S0:for(int i = 0; i < iterCnt; ++i)
{
   #pragma HLS loop_tripcount min=1 max=8
   
   mm2s0(strmOut_to_rowiseFFT, matSz);
}

LOOP_ITER_S2MM0_TO_MM2S1:for(int i = 0; i < iterCnt; ++i)
{
   #pragma HLS loop_tripcount min=1 max=8
   
   dmaHls_rowsToCols(strmInp_from_rowiseFFT, strmOut_to_colwiseFFT, \
                     matSz, rows, cols, stg0_errCnt, goldenVal);
}

LOOP_ITER_S2MM1:for(int i = 0; i < iterCnt; ++i)
{
   #pragma HLS loop_tripcount min=1 max=8
   
   s2mm1(strmInp_from_colwiseFFT, matSz, stg1_errCnt, goldenVal);
}
...
```

##### Vitis HLS Scheduling and Dataflow View

The following figure shows the data mover scheduler view.

![Image of Datamover Scheduler View](images/dma_hls_scheduler_view.PNG)

The following figure shows the data mover dataflow view.

![Image of Datamover Dataflow View](images/dma_hls_dataflow_view.PNG)

The following figure shows the data mover functional call graph view.

![Image of Datamover functional call graph View](images/fft_2d_aie_functional_call_graph_view.PNG)

#### Streaming Interface Data Width

The streaming interface data width is kept at 128-bits to reduce read/write overhead while processing data.

#### Frequency Selection

AI Engine kernels are configured for `cint16 / 4bytes`, and the streaming interface is at `128bit / 16bytes`. The frequency of the AI Engine array is 1.25 GHz, and the data mover is kept at 312.5 MHz, maintaining a 1:4 ratio.

</details>

<details>
<summary>AI Engine Kernels and Graph Representation</summary>

### AI Engine Kernels and Graph Representation

An AI Engine kernel is a C/C++ program written using specialized intrinsic calls that target the VLIW vector processor. The AI Engine compiler compiles the kernel code to produce an executable ELF file for each AI Engines used in the design. These kernels can be stitched together as AI Engine graphs written in C++. In this design, the AI Engine compiler summarizes compilation results. Review the [AI Engine Kernel Programming](https://docs.xilinx.com/r/en-US/ug1079-ai-engine-kernel-coding) section in the AI Engine documentation for a high-level overview of kernel programming. You can view the graph by running the following command:

`vitis_analyzer $(BUILD_TARGET_DIR)/Work/graph.aiecompile_summary`

The following figures show the graph representation of the AI Engine kernels (default FFT 2048 point and FFT 1024 point, `FFT_2D_INSTS=1`). In addition to the compute units, there are the twiddle factor LUTs (`fft_lut_tw*`) and temporary buffers for FFT stages (`fft_2048/1024_tmp*`).

![Image of 2D-FFT AI Engine 2K point Graph](images/fft_2d_aie_fft_rows_graph_for_2kpt_1x.PNG)
![Image of 2D-FFT AI Engine 1K point Graph](images/fft_2d_aie_fft_cols_graph_for_1kpt_1x.PNG)

</details>

<details>
<summary>Adaptive Data Flow (ADF) Graph</summary>

### Adaptive Data Flow (ADF) Graph

This section describes the overall data flow graph specification of the 2D-FFT design using AI Engine, compiled by the AI Engine compiler.

The overall graph definition of the design is contained in the `graph.cpp` file. The top-level graph contains two sub-graphs, `FFTrows_graph` and `FFTcols_graph`, each with an `FFT_2D_INSTS` number of objects. The following describes the definition of the sub-graphs (the `FFTrows_graph` is used as an illustration).

#### Defining the Graph Class

Define the graph classes using objects defined in the appropriate name space. It must include the ADF library and [Vitis DSP Library](https://docs.xilinx.com/r/en-US/Vitis_Libraries/dsp/index.html) for FFIT. A general specification is put in for the ADF namespace:

```
// FFTrows_graph FFT point size...
#define FFT_ROW_TP_POINT_SIZE MAT_COLS
// FFTcols_graph FFT point size...
#define FFT_COL_TP_POINT_SIZE MAT_ROWS

// 1 (FFT) or 0 (IFFT)...
#define FFT_2D_TP_FFT_NIFFT 1
// 0 Bit Shift before output, change based on input...
#define FFT_2D_TP_SHIFT 0
// FFT divided over 1 FFT Kernel...
#define FFT_2D_TP_CASC_LEN 1
// Dynamic FFT Point Size is disabled...
#define FFT_2D_TP_DYN_PT_SIZE 0

// TP_WINDOW_VSIZE for FFTrows_graph...
#define FFT_ROW_TP_WINDOW_VSIZE MAT_COLS

// TP_WINDOW_VSIZE for FFTcols_graph
// Increasing the "TP__WINDOW _VSIZE" so that the ping-pong overhead is less
// Assigning it as MAT_COLS instead of MAT_ROWS...
#define FFT_COL_TP_WINDOW_VSIZE MAT_COLS

////////////////////////////////////////////////////////
// FFT_2D Datatype related Macros
// Datatype can be:
// cint16 (Default) or cfloat...
// 0=cint16(Default)
// 1=cfloat
#if FFT_2D_DT == 0

   // Input data type...
   #define FFT_2D_TT_DATA cint16
   // Twiddle Factor data type...
   #define FFT_2D_TT_TWIDDLE cint16
   
   // FFTrows_graph I/O WINDOW BUFF SIZE IN BYTES...
   #define FFT_ROW_WINDOW_BUFF_SIZE (FFT_ROW_TP_WINDOW_VSIZE * 4)
   // FFTcols_graph I/O WINDOW BUFF SIZE IN BYTES...
   #define FFT_COL_WINDOW_BUFF_SIZE (FFT_COL_TP_WINDOW_VSIZE * 4)
   
#elif FFT_2D_DT == 1

   // Input data type...
   #define FFT_2D_TT_DATA cfloat
   // Twiddle Factor data type...
   #define FFT_2D_TT_TWIDDLE cfloat
   
   // FFTrows_graph I/O WINDOW BUFF SIZE IN BYTES...
   #define FFT_ROW_WINDOW_BUFF_SIZE (FFT_ROW_TP_WINDOW_VSIZE * 8)
   // FFTcols_graph I/O WINDOW BUFF SIZE IN BYTES...
   #define FFT_COL_WINDOW_BUFF_SIZE (FFT_COL_TP_WINDOW_VSIZE * 8)
   
#endif

#include "adf.h"
#include "fft_ifft_dit_1ch_graph.hpp"

using namespace adf;
namespace dsplib = xf::dsp::aie;
```

All user graphs are defined from the class graph: for example, in the `FFTrows_graph` design:

```
class FFTrows_graph: public graph
{
   public:
   	port<input>   in;
   	port<output> out;
      
   	// Constructor - with Rowise FFT graph class initialization...
   	FFTrows_graph()
      {
         dsplib::fft::dit_1ch::fft_ifft_dit_1ch_graph<FFT_2D_TT_DATA, FFT_2D_TT_TWIDDLE, FFT_ROW_TP_POINT_SIZE,
         FFT_2D_TP_FFT_NIFFT, FFT_2D_TP_SHIFT, FFT_2D_TP_CASC_LEN, FFT_2D_TP_DYN_PT_SIZE, FFT_ROW_TP_WINDOW_VSIZE> FFTrow_gr;
         
         runtime<ratio>(*FFTrow_gr.getKernels()) = 0.6;
         
         connect< window<FFT_ROW_WINDOW_BUFF_SIZE> > (in,   FFTrow_gr.in[0]);
         connect< window<FFT_ROW_WINDOW_BUFF_SIZE> > (FFTrow_gr.out[0], out);
   	}
};
```

#### Top-Level Application

Define a top-level application file (`graph.cpp` in this design) that contains an instance of the graph class and connect the graph to a simulation platform to provide file input and output (in the case of `FFT2D_INSTS = 1` to the two sub-graphs):

```
#include "graph.h"

uint8_t fftCols_grInsts = 0, fftRows_grInsts = 0;

FFT2D_graph fft2d_graph;
```

The `main` function is called under the guard bounds of `\_\_AIESIM\_\_` as shown below, to avoid conflict with the `main` function in the host application:

```
#ifdef __AIESIM__

   int main(int argc, char ** argv)
   {
      fft2d_graph.init();

      fft2d_graph.run(ITER_CNT * MAT_ROWS);

      fft2d_graph.end();
      
      return 0;
   }

#endif
```

</details>

<details>
<summary>PL Data Mover Kernel</summary>

### PL Data Mover Kernel

In addition to the kernels operating in the AI Engine array, this design specifies a data mover kernel to run in the PL region of the device (written in HLS C++). The data mover kernel is brought into the design during the Vitis kernel compilation, which is further replicated based on the `FFT_2D_INSTS` value. The software design of the data mover kernel is described in the following sections.

#### dma_hls (dma_hls.cpp)

The `dma_hls` kernel reads data from a memory mapped AXI4 (MM-AXI4) interface and writes it to an AXI4-Stream interface.

##### Top Function Declaration

The `dma_hls` kernel takes the following arguments:

```
int dma_hls(
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_rowiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_rowiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_colwiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_colwiseFFT,
      int matSz, int rows, int cols, int iterCnt
     );
```

- `ap_int<N>` is an arbitrary precision integer data type defined in `ap_int.h` where `N` is a bit size from 1-1024. In this design, the bit size is set to 128.
- `hls::stream<qdma_axis<D,0,0,0>>` is a data type defined in `ap_axi_sdata.h`. It is a special data class used for data transfer when using a streaming platform. The parameter `<D>` is the data width of the streaming interface, which is set to 128. The remaining three parameters should be set to 0.

##### Top Function Definition

Use the `dataflow` pragma for concurrently scheduling the three functions `mm2s0`, `dmaHls_rowsToCols`, and `s2mm1`.

```
int dma_hls(
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_rowiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_rowiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_colwiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_colwiseFFT,
      int matSz, int rows, int cols, int iterCnt
     )
{
   #pragma HLS INTERFACE axis port=strmOut_to_rowiseFFT
   #pragma HLS INTERFACE axis port=strmInp_from_rowiseFFT
   #pragma HLS INTERFACE axis port=strmOut_to_colwiseFFT
   #pragma HLS INTERFACE axis port=strmInp_from_colwiseFFT
   
   #pragma HLS INTERFACE s_axilite port=matSz bundle=control
   #pragma HLS INTERFACE s_axilite port=rows bundle=control
   #pragma HLS INTERFACE s_axilite port=cols bundle=control
   #pragma HLS INTERFACE s_axilite port=iterCnt bundle=control
   #pragma HLS INTERFACE s_axilite port=return bundle=control  
   
   #pragma HLS DATAFLOW
   
   int stg0_errCnt = 0, stg1_errCnt = 0;
   
   ap_uint<128> goldenVal;

   goldenVal.range(127, 64) = 0x0000000100000001;
   goldenVal.range( 63,  0) = 0x0000000100000001;
   

   LOOP_ITER_MM2S0:for(int i = 0; i < iterCnt; ++i)
   {
      #pragma HLS loop_tripcount min=1 max=8
      
      mm2s0(strmOut_to_rowiseFFT, matSz);
   }
   
   LOOP_ITER_S2MM0_TO_MM2S1:for(int i = 0; i < iterCnt; ++i)
   {
      #pragma HLS loop_tripcount min=1 max=8
      
      dmaHls_rowsToCols(strmInp_from_rowiseFFT, strmOut_to_colwiseFFT, \
                        matSz, rows, cols, stg0_errCnt, goldenVal);
   }
   
   LOOP_ITER_S2MM1:for(int i = 0; i < iterCnt; ++i)
   {
      #pragma HLS loop_tripcount min=1 max=8
      
      s2mm1(strmInp_from_colwiseFFT, matSz, stg1_errCnt, goldenVal);
   }

   return (stg0_errCnt + stg1_errCnt);
}
```

The `dma_hls` kernel also specifies HLS pragmas to help optimize the kernel code and adhere to interface protocols. See [this page](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/HLS-Pragmas) for detailed documentation of all HLS pragmas. A summary of the HLS pragmas used in the kernel is provided in the following table:

|Switch|Description|
|  ---  |  ---  |
|#pragma HLS INTERFACE|In C/C++ code, all input and output operations are performed, in zero time, through formal function arguments. In an RTL design, these same input and output operations must be performed through a port in the design interface and typically operate using a specific input/output (I/O) protocol. For more information, see [this page](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/pragma-HLS-interface).|
|#pragma HLS PIPELINE II=1|Reduces the initiation interval (II) for a function or loop by allowing the concurrent execution of operations. The default type of pipeline is defined by the config_compile -pipeline_style command, but can be overridden in the PIPELINE pragma or directive. For more information, see [this page](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/pragma-HLS-pipeline).|
|#pragma HLS dataflow|The DATAFLOW pragma enables task-level pipelining, allowing functions and loops to overlap in their operation, increasing the concurrency of the RTL implementation and the overall throughput of the design. See [this page](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/pragma-HLS-dataflow) for more information.|
|#pragma HLS loop_tripcount|When manually applied to a loop, it specifies the total number of iterations a loop performs. The `LOOP_TRIPCOUNT` pragma or directive is for analysis only and does not impact the synthesis results. See [this page](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/pragma-HLS-loop_tripcount) for more information.|
 
</details>

<details>
<summary>PS Host Application</summary>

### PS Host Application

The 2D-FFT AI Engine tutorial uses the embedded processing system (PS) as an external controller to control the AI Engine graph and data mover PL kernels. Review the [Programming the PS Host Application](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Programming-the-PS-Host-Application) section in the AI Engine documentation to understand the process of creating a host application.

In addition to the PS host application (`fft_2d_aie_app.cpp`), the AI Engine control code must also be compiled. The AI Engine compiler generates this control code (`aie_control_xrt.cpp`) when compiling the AI Engine design graph and kernel code. The PS host application uses the AI Engine control code for the following purposes:

* Controlling the initial loading of the AI Engine kernels.
* Running the graph for several iterations, updating the runtime parameters associated with the graph, exiting, and resetting the AI Engine tiles.

The steps to run the A72 application are as follows:

1. Include `graph.cpp` and other required headers. Define the required macros. The `graph.cpp` AI Engine application file contains the instantiation of the AI Engine 2D-FFT data flow graph object.

   ```
   #include "graph.cpp"
   
   #include <stdio.h>
   #include <stdlib.h>
   #include <stdint.h>
   #include <fstream>
   #include <iostream>
   #include <string>
   
   #include "adf/adf_api/XRTConfig.h"
   
   #include "experimental/xrt_aie.h"
   #include "experimental/xrt_kernel.h"
   #include "experimental/xrt_bo.h"
   
   #define MAT_SIZE (MAT_ROWS * MAT_COLS)
   
   /////////////////////////////////////////////////
   // Due to 128bit Data Transfer all dimensions,
   // to be given as by 4 for cint16
   // since 4 samples of cint16 are passed 
   /////////////////////////////////////////////////
   #if FFT_2D_DT == 0
      #define MAT_SIZE_128b (MAT_SIZE / 4)
      #define MAT_ROWS_128b (MAT_ROWS / 4)
      #define MAT_COLS_128b (MAT_COLS / 4)
   /////////////////////////////////////////////////
   // Due to 128bit Data Transfer all dimensions,
   // to be given as by 2 for cfloat
   // since 2 samples of cfloat are passed 
   /////////////////////////////////////////////////
   
   #elif FFT_2D_DT == 1
      #define MAT_SIZE_128b (MAT_SIZE / 2)
      #define MAT_ROWS_128b (MAT_ROWS / 2)
      #define MAT_COLS_128b (MAT_COLS / 2)
   
   #endif
   ```

2. Check the command line argument. The beginning of the A72 application is represented by the `main` function. It takes in one command line argument: an XCLBIN file.

```
int main(int argc, char** argv)
```

3. Open the XCLBIN and create data mover kernel handles. The A72 application loads the XCLBIN binary file and creates the data mover kernels to be executed on the device. The steps are:

   * Open the device and load the XCLBIN:

   ```
   auto dhdl = xrtDeviceOpen(0);
   auto xclbin = load_xclbin(dhdl, xclbinFilename);
   auto top = reinterpret_cast<const axlf*>(xclbin.data());
   ```
   * Open the data mover kernel and obtain handles to start the HLS PL kernels (see the following example for the `dma_hls` PL kernel):

   ```
   xrtKernelHandle dma_hls_khdl;
   xrtRunHandle dma_hls_rhdl;

   // Open kernel handle exclusively to read the ap_return register later for reporting error...
   dma_hls_khdl = xrtPLKernelOpenExclusive(dhdl, top->m_header.uuid, dma_hls_obj);
   dma_hls_rhdl = xrtRunOpen(dma_hls_khdl);
   ```

4. Open the graph, obtain the handle, and execute the graph: 

   * The A72 processor opens and obtains its handle using the ` xrtGraphOpen` function.
   * The A72 processor resets the graph using the `xrtGraphReset` function and runs the graph execution using the `xrtGraphRun` function for the 2K point and 1K point sub-graphs.

5. Execute the data mover kernels and generate the output results:

   * Set the `dma_hls` kernel arguments using the `xrtRunSetArg` function.
   * Start the `dma_hls` kernels using the `xrtRunStart` function.
   * Wait for `dma_hls` execution to finish using the `xrtRunWait` function.

6. Verify the output results by reading the `ap_return` in `$(BUILD_TARGET_DIR)/_x/dma_hls.$(TARGET)/dma_hls/dma_hls/ip/drivers/dma_hls_v1_0/src/xdma_hls_hw.h` using the `xrtKernelRegister` API, as follows:

   ```
   void golden_check(uint32_t *errCnt)
   {
      //////////////////////////////////////////
      // Compare results
      //////////////////////////////////////////
      
      // Reading the error count for the ap_return reg of the hls kernel...
      xrtKernelReadRegister(dma_hls_khdl, 0x10, &instance_errCnt);
      std::cout << "fft_2d_" << instsNo << " " << (instance_errCnt ? "Failed!..." : "Passed!...") << "\n" << std::endl;
      
      // Adding instance error to the total error count...
      *errCnt += instance_errCnt;
   }
   ```

7. Release allocated resources. After post-processing the data, release the allocated objects and handles using the `xrtRunClose`, `xrtKernelClose`, `xrtGraphClose`, and `xrtDeviceClose` functions.

</details>

## Performance Details

For all applications, designers must work to predefined specifications and build a system for their specific deployment by meeting their system requirements for their available resources, latency, throughput, performance, and power. This section outlines how to measure those characteristics for the AI Engine implementation in this tutorial.

<details>
<summary>Resource Utilization and Power</summary> 

#### Resource Utilization and Power

Resource utilization and power are measured using Vivado, vcdanalyze, and Xilinx Power Estimator (XPE) for AMD Versal&trade; (2023.1 version) tools.

The registers and CLB LUT utilization information can be found in the Vivado project if you perform the following steps:

1. Open the Vivado project: ``$(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr``.

2. Go to **Open Implemented Design**, click **Report Utilization**. In the Utilization tab shown in the following figure, select **ai_engine_0** and view the **Registers** and **CLB LUTs** for the 32 x 64 point, 1-instance, and cint16 design:

![Image of 2D-FFT AIE Utilization](images/fft_2d_aie_vivado_resources.PNG)

** Or **

1. Do `make report_metrics TARGET=hw`, (recipe expanded as follows), alongwith relevant options, to generate `utilization_hierarchical.txt` under `$(BLD_REPORTS_DIR)/` directory:

```
report_metrics:
ifeq ($(TARGET),hw_emu)
	@echo "This build target (report-metrics) not valid when design target is hw_emu"

else
	rm -rf $(BLD_REPORTS_DIR)
	mkdir -p $(BLD_REPORTS_DIR)
	cd $(BLD_REPORTS_DIR); \
	vivado -mode batch -source $(VIVADO_METRICS_SCRIPTS_REPO)/report_metrics.tcl $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr

endif
```

The vcdanalyze tool generates a `graph.xpe` file, which can be input to XPE for viewing the AI Engine resource utilization and power. The steps are as follows:

1. Run `make vcd` (recipe expanded below) to create the `graph.xpe` file under `$(BUILD_TARGET_DIR)/aiesim_xpe/`:

```
cd $(BUILD_TARGET_DIR); \
aiesimulator --pkg-dir $(WORK_DIR)/ --dump-vcd x$(FFT_2D_INSTS) 2>&1 | tee -a vcd.log
cd $(BUILD_TARGET_DIR); \
vcdanalyze --vcd x$(FFT_2D_INSTS).vcd --xpe
```

2. If you do not already have it installed, download and install [XPE for Versal Version 2023.1](https://www.xilinx.com/products/technology/power/xpe.html). For full documentation of XPE, see [this page](https://www.xilinx.com/products/technology/power/xpe.html).

3. AI Engine power comsumption and resource utilization shown in below images for the 32 x 64 point 1-instance design:

![Image of 2D-FFT AIE XPE](images/fft_2d_aie_xpe_pow.PNG)
A summary of resource utilization and power for all variations is given in the following table.

##### cint16 Design
| Number of Instances | FFT Configuration            | Number of Compute Cores | Vector Load | Number of Active Memory Banks | Mem R/W Rate | Active AI Engine Tiles | Interconnect Load | FF (Regs) | CLB LUTS  | Dynamic Power<br/>(in mW) | 
|:----------------:|:----------------------------:|:-----------------------:|:-----------:|:-----------------------------:|:------------:|:----------------:|:-----------------:|:---------:|:---------:|:-------------------------:|
| 1                | 64 point<br/>(32 x 64)       | 2                       | 11%          | 28                            | 5%           | 5                | 7%                | 1136     | 369       | 840                       |
| 1                | 128 point<br/>(64 x 128)     | 2                       | 29%          | 28                            | 11%          | 5                | 7%                | 1136     | 380       | 925                       |
| 1                | 256 point<br/>(128 x 256)    | 2                       | 45%          | 28                            | 17%          | 5                | 7%                | 1136     | 369       | 999                       |
| 1                | 512 point<br/>(256 x 512)    | 2                       | 59%          | 30                            | 20%          | 5                | 7%                | 1136     | 369       | 1066                      |
| 1                | 2048 point<br/>(1024 x 2048) | 2                       | 78%          | 42                            | 19%          | 6                | 7%                | 1136     | 369       | 1180                      |
| 5                | 64 point<br/>(32 x 64)       | 10                      | 11%          | 140                           | 5%           | 24               | 7%                | 5680     | 1829      | 1833                      |
| 5                | 128 point<br/>(64 x 128)     | 10                      | 29%          | 140                           | 9%           | 24               | 7%                | 5680     | 1829      | 2257                      |
| 5                | 256 point<br/>(128 x 256)    | 10                      | 45%          | 140                           | 17%          | 24               | 7%                | 5680     | 1829      | 2623                      |
| 5                | 512 point<br/>(256 x 512)    | 10                      | 59%          | 150                           | 20%          | 24               | 7%                | 5680     | 1829      | 2954                      |
| 5                | 2048 point<br/>(1024 x 2048) | 10                      | 78%          | 210                           | 19%          | 32               | 6%                | 5680     | 1829      | 3594                      |
| 10               | 64 point<br/>(32 x 64)       | 20                      | 11%          | 280                           | 5%           | 42               | 7%                | 11360    | 3654      | 2926                      |
| 10               | 128 point<br/>(64 x 128)     | 20                      | 29%          | 280                           | 11%          | 42               | 7%                | 11360    | 3800      | 3773                      |
| 10               | 256 point<br/>(128 x 256)    | 20                      | 44%          | 280                           | 17%          | 42               | 7%                | 11360    | 3654      | 4504                      |
| 10               | 512 point<br/>(256 x 512)    | 20                      | 58%          | 300                           | 20%          | 42               | 7%                | 11360    | 3654      | 5165                      |
| 10               | 2048 point<br/>(1024 x 2048) | 20                      | 77%          | 420                           | 19%          | 72               | 6%                | 11360    | 3654      | 6781                      |

##### cfloat Design
| Number of Instances | FFT Configuration            | Number of Compute Cores | Vector Load | Number of Active Memory Banks | Mem R/W Rate | Active AI Engine Tiles | Interconnect Load | FF (Regs) | CLB LUTS  | Dynamic Power<br/>(in mW) | 
|:----------------:|:----------------------------:|:-----------------------:|:-----------:|:-----------------------------:|:------------:|:----------------:|:-----------------:|:---------:|:---------:|:-------------------------:|
| 1                | 64 point<br/>(32 x 64)       | 2                       | 43%         | 24                            | 10%          | 5                | 7%                |1136       |367        |  949                      |
| 1                | 128 point<br/>(64 x 128)     | 2                       | 60%         | 24                            | 14%          | 5                | 7%                |1136       |367        |  1013                     |
| 1                | 256 point<br/>(128 x 256)    | 2                       | 73%         | 24                            | 16%          | 5                | 7%                |1136       |367        |  1060                     |
| 1                | 512 point<br/>(256 x 512)    | 2                       | 82%         | 26                            | 17%          | 5                | 7%                |1136       |367        |  1091                     |
| 1                | 2048 point<br/>(1024 x 2048) | 2                       | 89%         | 52                            | 9%           | 7                | 6%                |1136       |367        |  1164                     |
| 5                | 64 point<br/>(32 x 64)       | 10                      | 43%         | 120                           | 10%          | 24               | 7%                |5680       |1828       |  2372                     |
| 5                | 128 point<br/>(64 x 128)     | 10                      | 60%         | 120                           | 14%          | 24               | 7%                |5680       |1828       |  2686                     |
| 5                | 256 point<br/>(128 x 256)    | 10                      | 73%         | 120                           | 16%          | 24               | 7%                |5680       |1828       |  2925                     |
| 5                | 512 point<br/>(256 x 512)    | 10                      | 81%         | 130                           | 17%          | 24               | 7%                |5680       |1828       |  3080                     |
| 5                | 2048 point<br/>(1024 x 2048) | 10                      | 88%         | 260                           | 9%           | 24               | 5%                |5680       |1828       |  3644                     |
| 10               | 64 point<br/>(32 x 64)       | 20                      | 43%         | 240                           | 10%          | 42               | 7%                |11360      |3654       |  4007                     |
| 10               | 128 point<br/>(64 x 128)     | 20                      | 60%         | 240                           | 14%          | 42               | 7%                |11360      |3654       |  4631                     |
| 10               | 256 point<br/>(128 x 256)    | 20                      | 72%         | 240                           | 16%          | 41               | 7%                |11360      |3654       |  5084                     |
| 10               | 512 point<br/>(256 x 512)    | 20                      | 81%         | 260                           | 17%          | 42               | 7%                |11360      |3654       |  5411                     |
| 10               | 2048 point<br/>(1024 x 2048) | 20                      | 88%         | 520                           | 9%           | 80               | 6%                |11360      |3654       |  6606                     |

##### Power from XPE vs HW

**cint16**
| Number of Instances | FFT Configurations       | XPE Load(in A) | HW Load(in A) |
|:-----------------:|:--------------------------:|:--------------:|:-------------:|
| 10                | 512 point (256x512)        |  7.671         |   4.59305     |
| 10                | 2048 point (1024x2048)     |  9.224         |   5.85561     |

**cfloat**
| Number of Instances | FFT Configurations       | XPE Load(in A) | HW Load(in A) |
|:-----------------:|:--------------------------:|:--------------:|:-------------:|
| 10                | 512 point (256x512)        | 7.527          |   4.4594      |
| 10                | 2048 point (1024x2048)     | 10.027         |   6.00331     |

</details>

<details>
<summary>Throughput and Latency</summary> 

#### Throughput and Latency

Throughput is measured in mega-samples transferred per second (MSPS). It is measured by viewing the runtime-generated trace texts using the Vitis analyzer. Latency is defined as the time between the first sample being sent by the data mover into the row-wise FFT kernel and the first sample from the col-wise FFT kernel being received by the data mover. The steps to measure throughput and latency are listed below:

1. Compile the design using `EN_TRACE=1`. It automatically includes a `xrt.ini` file while packaging, which comprises the following:

   ```
   [Debug]
   xrt_trace=true
   data_transfer_trace=fine
   trace_buffer_size=500M
   ```

   For more information, refer to the [xrt.ini](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/xrt.ini-File) documentation.  

2. After execution on the board, transfer the generated `device_trace_0.csv`, `hal_host_trace.csv`, and `xrt.run_summary` files back to your system.

3. Open `xrt.run_summary` using `vitis_analyzer`: `vitis_analyzer xrt.run_summary`.

4. The snapshot of the timeline trace for the AI Engine 32 x 64 point 1-instance design run with `ITER_CNT=16` is shown in the following figure:

![Image of 2D-FFT AI Engine implementation 1x Timeline Trace](images/fft_2d_aie_trace_32x64_iter16.PNG)

5. The profiling setup in the Makefile measures the execution time and all the interfaces. For higher instance designs only, `strmInp_from_colwiseFFT` is profiled.

The throughput and latency calculations for the 32 x 64 point, 1-instance, and cint16 design based on the `hw` run is as follows:

```
Processing Time = (End of Processing Timestamp of Stream `strmInp_from_colwiseFFT`) - (Start of Processing Timestamp of Stream `strmInp_from_colwiseFFT`)

Processing Time (with 156.250MHZ)    =  513.965 us
Processing Time (scaled to 312.5MHZ) =  (513.965 * (156.25/312.5)) us
                                     =   256.9825   us

Latency = (Start of Execution Timestamp of Stream `strmOut_to_rowiseFFT`) - (Start of Execution Timestamp of Stream `strmInp_from_colwiseFFT`)

Latency (with 156.250MHz)    = 0.909us
Latency (scaled to 312.5MHz) = (0.909 * (156.25/312.5)) us
		             = 0.4545 us

Throughput = (Samples transferred) / processing time
           = (MAT_ROWS x MAT_COLS x Iterations) / processing time
           = (32 x 64 x 16) / 256.9825 us
           = 127.510 x 2 (Since we have two kernels namely rowise_fft and colwise_fft, we're multiplying the Throughput with factor 2)

           = 255.021 MSamples/s
           = 255.021 x 4 MB/s (As each sample is 4bytes)
           = 1020.085 MB/s
```
The throughput and latency calculations for the 32 x 64 point, 1-instance, and cint16 design based on the `hw_emu` run are as follows, along with the snapshot of the timeline trace of hw emulation as follows:
![Image of 2D-FFT AI Engine implementation 32x64 hw_emu](images/fft_2d_aie_hw_emu_waveform_view_32x64_x1.PNG)
```
Processing Time
   = (End of Processing Timestamp of Stream `strmInp_from_colwiseFFT`) -
     (Start of Processing Timestamp of Stream `strmInp_from_colwiseFFT`)
   = 261.28us

Latency:
   = Difference between strmOut_to_rowiseFFT beginning and execution beginning of strmInp_to_colwise
   = (Start of Execution Timestamp of Stream `strmOut_to_rowiseFFT`) -
     (Start of Execution Timestamp of Stream `strmInp_from_colwiseFFT`)
   = 7.87 us

Throughput = (Samples transferred) / processing time
           = (MAT_ROWS x MAT_COLS x Iterations) / processing time
           = (32 x 64 x 16) / 261.28 us
           = 125.4133 x 2 (Since we have two kernels namely rowise_fft and colwise_fft, we're multiplying the Throughput with factor 2)
           = 250.83 MSamples/s
           = 250.83 x 4 MB/s (As each sample is 4bytes)
           = 1003.306 MB/s
```

A summary of throughput and latency for all variations is shown in the following table.

##### cint16 Design
| Number of Instances | FFT Configuration            | Data Transfer Size | Aggregate Throughput<br/>(in MSPS) | Average Throughput<br/>(in MSPS) | Average Latency<br/>(in μs) | Minimum Latency<br/>(in μs) |
|:----------------:|:----------------------------:|:------------------:|:----------------------------------:|:--------------------------------:|:---------------------------:|:---------------------------:|
| 1                | 64 point<br/>(32 x 64)       | 32768              | 250.829                            |   250.829                       | 7.868                        | 7.868                       |
| 1                | 128 point<br/>(64 x 128)     | 131072             | 356.855                            |   356.855                       | 20.4832                      | 20.4832                     |
| 1                | 256 point<br/>(128 x 256)    | 524288             | 486.6468                           |   486.6468                      | 63.2080                      | 50.372                      |
| 1                | 512 point<br/>(256 x 512)    | 2097152            | 572.2053                           |   572.2053                      | 227.593                      | 184.090                     |
| 1                | 2048 point<br/>(1024 x 2048) | 33554432           | 621.5528                           |   621.5528                      | 3546.4672                    | 2873.847                    |
| 5                | 64 point<br/>(32 x 64)       | 32768              | 1254.1365                          |   250.827                       | 7.868                        | 6.457                       |
| 5                | 128 point<br/>(64 x 128)     | 131072             | 1784.2756                          |   356.855                       | 20.4832                      | 16.493                      |
| 5                | 256 point<br/>(128 x 256)    | 524288             | 2434.9722                          |   486.646                       | 63.20800                     | 50.365                      |
| 5                | 512 point<br/>(256 x 512)    | 2097152            | 2861.0269                          |   572.2053                      | 227.5936                     | 184.080                     |
| 5                | 2048 point<br/>(1024 x 2048) | 33554432           | 3106.654                           |   621.5528                      | 3546.4672                    | 2873.834                    |
| 10               | 64 point<br/>(32 x 64)       | 32768              | 2508.2792                          |   250.829                       | 7.868                        | 6.455                       |
| 10               | 128 point<br/>(64 x 128)     | 131072             | 3568.5513                          |   356.855                       | 20.4832                      | 16.493                      |
| 10               | 256 point<br/>(128 x 256)    | 524288             | 4866.9111                          |   486.6468                      | 63.20800                     | 50.365                      |
| 10               | 512 point<br/>(256 x 512)    | 2097152            | 5721.85427                         |   572.2053                      | 227.5936                     | 184.083                     |
| 10               | 2048 point<br/>(1024 x 2048) | 33554432           | 6215.5280                          |   621.5528                      | 3546.4672                    | 2873.110                    |
                                                                                                                                                         
##### cfloat Design
| Number of Instances | FFT Configuration            | Data Transfer Size | Aggregate Throughput<br/>(in MSPS) | Average Throughput<br/>(in MSPS) | Average Latency<br/>(in μs) | Minimum Latency<br/>(in μs) |
|:----------------:|:----------------------------:|:------------------:|:----------------------------------:|:--------------------------------:|:---------------------------:|:---------------------------:|
| 1                | 64 point<br/>(32 x 64)       | 32768              |  159.560                           |   159.560                        | 13.088                      |   13.088                    |
| 1                | 128 point<br/>(64 x 128)     | 131072             |  186.427                           |   186.427                        | 45.244                      |   45.244                    |
| 1                | 256 point<br/>(128 x 256)    | 524288             |  197.008                           |   197.008                        | 174.364                     |   174.364                   |
| 1                | 512 point<br/>(256 x 512)    | 2097152            |  195.077                           |   195.077                        | 713.74                      |   713.74                    |
| 1                | 2048 point<br/>(1024 x 2048) | 33554432           |  174.650                           |   174.650                        | 12861.968                   |   12861.968                 |
| 5                | 64 point<br/>(32 x 64)       | 32768              |  797.800                           |   159.560                        | 13.088                      |   13.088                    |
| 5                | 128 point<br/>(64 x 128)     | 131072             |  932.135                           |   186.427                        | 45.244                      |   45.244                    |
| 5                | 256 point<br/>(128 x 256)    | 524288             |  985.041                           |   197.008                        | 174.364                     |   174.364                   |
| 5                | 512 point<br/>(256 x 512)    | 2097152            |  975.3879                          |   195.077                        | 713.74                      |   713.74                    |
| 5                | 2048 point<br/>(1024 x 2048) | 33554432           |  873.2685                          |   174.650                        | 12861.968                   |   12861.968                 |
| 10               | 64 point<br/>(32 x 64)       | 32768              |  1595.601                          |   159.560                        | 13.088                      |   13.088                    |
| 10               | 128 point<br/>(64 x 128)     | 131072             |  1864.271                          |   186.427                        | 45.244                      |   45.244                    |
| 10               | 256 point<br/>(128 x 256)    | 524288             |  1970.083                          |   197.008                        | 174.364                     |   174.364                   |
| 10               | 512 point<br/>(256 x 512)    | 2097152            |  1950.775                          |   195.077                        | 713.74                      |   713.74                    |
| 10               | 2048 point<br/>(1024 x 2048) | 33554432           |  1746.537                          |   174.650                        | 12861.968                   |   12861.968                 |

</details>

<details>
<summary>Performance per Watt</summary> 

#### Performance per Watt

Performance per Watt is represented as throughput in MSPS/power in Watts. The following example shows the calculation for the 32 x 64 point, 1-instance, and cint16 design:

```
Performance per Watt = Throughput(MSPS) / Power(Watt)
                     = (250.829  / 0.840) MSPS/Watt
                     = 298.606 MSPS/Watt
```

A summary of performance per Watt for all variations is shown in the following table.

##### cint16 Design
| Number of Instances | FFT Configuration     | Performance per Watt (in MSPS/Watt)  |
|:----------------:|:------------------------:|:-----------------------------------: |
| 1                | 64 point (32 x 64)       |      298.606                         |
| 1                | 128 point (64 x 128)     |      385.7892                        |
| 1                | 256 point (128 x 256)    |      487.133                         |
| 1                | 512 point (256 x 512)    |      536.778                         |
| 1                | 2048 point (1024 x 2048) |      526.778                         |
| 5                | 64 point (32 x 64)       |      684.1988                        |
| 5                | 128 point (64 x 128)     |      790.5519                        |
| 5                | 256 point (128 x 256)    |      928.3157                        |
| 5                | 512 point (256 x 512)    |      968.315                         |
| 5                | 2048 point (1024 x 2048) |      864.4003                        |
| 10               | 64 point (32 x 64)       |      857.2383                        |
| 10               | 128 point (64 x 128)     |      945.8127                        |
| 10               | 256 point (128 x 256)    |      1080.5752                       |
| 10               | 512 point (256 x 512)    |      1107.813025                     |
| 10               | 2048 point (1024 x 2048) |      916.60936                       |

##### cfloat Design
| Number of Instances | FFT Configuration     | Performance per Watt (in MSPS/Watt)  |
|:----------------:|:------------------------:|:------------------------------------:|
| 1                | 64 point (32 x 64)       | 168.135086                           |
| 1                | 128 point (64 x 128)     | 184.0346884                          |
| 1                | 256 point (128 x 256)    | 185.8569065                          |
| 1                | 512 point (256 x 512)    | 178.8061921                          |
| 1                | 2048 point (1024 x 2048) | 150.0429553                          |
| 5                | 64 point (32 x 64)       | 336.3407605                          |
| 5                | 128 point (64 x 128)     | 347.0349327                          |
| 5                | 256 point (128 x 256)    | 336.7663605                          |
| 5                | 512 point (256 x 512)    | 316.6843877                          |
| 5                | 2048 point (1024 x 2048) | 239.6455821                          |
| 10               | 64 point (32 x 64)       | 398.203577                           |
| 10               | 128 point (64 x 128)     | 402.5635195                          |
| 10               | 256 point (128 x 256)    | 387.5065321                          |
| 10               | 512 point (256 x 512)    | 360.5203259                          |
| 10               | 2048 point (1024 x 2048) | 264.3864672                          |

</details>

<details>
<summary>Consolidated Summary</summary> 

#### Consolidated Summary

A consolidated summary of observations for all the point sizes and all the corresponding instance variations is shown in the following table.

##### cint16 Design

| FFT Configuration - Number of Instances    | Aggregate Throughput<br/>(in MSPS) | Average Latency<br/>(in μs) | Number of Compute Cores | Vector Load | Number of Active Memory Banks | Mem R/W Rate | FF (Regs)| CLB LUTs | Dynamic Power<br/>(in mW) | Performance per Watt<br/>(in MSPS/Watt) |
|:---------------------------------------:|:----------------------------------:|:---------------------------:|:--------------------:|:-----------:|:--------------------------:|:------------:|:--------:|:--------:|:-------------------------:|:---------------------------------------:|
| 64 point<br/>(32 x 64)<br/> - x1        |  250.829                           | 7.868                        | 2                    | 11%       |    28                      |    5%        | 1136     | 369      | 840                     |  298.606                                  |
| 128 point<br/>(64 x 128)<br/> - x1      |  356.855                           | 20.4832                      | 2                    | 29%       |    28                      |    11%       | 1136     | 380      | 925                     |  385.7892                                 |
| 256 point<br/>(128 x 256)<br/> - x1     |  486.6468                          | 63.2080                      | 2                    | 45%       |    28                      |    17%       | 1136     | 369      | 999                     |  487.133                                  |
| 512 point<br/>(256 x 512)<br/> - x1     |  572.2053                          | 227.593                      | 2                    | 59%       |    30                      |    20%       | 1136     | 369      | 1066                    |  536.778                                  |
| 2048 point<br/>(1024 x 2048)<br/> - x1  |  621.5528                          | 3546.4672                    | 2                    | 78%       |    42                      |    19%       | 1136     | 369      |  1180                   |  526.778                                  |
| 64 point<br/>(32 x 64)<br/> - x5        |  1254.1365                         | 7.868                        | 10                   | 11%       |    140                     |    5%        | 5680     | 1829     | 1833                    |  684.1988                                 |
| 128 point<br/>(64 x 128)<br/> - x5      |  1784.2756                         | 20.4832                      | 10                   | 29%       |    140                     |    9%        | 5680     | 1829     | 2257                    |  790.5519                                 |
| 256 point<br/>(128 x 256)<br/> - x5     |  2434.9722                         | 63.20800                     | 10                   | 45%       |    140                     |    17%       | 5680     | 1829     | 2623                    |  928.3157                                 |
| 512 point<br/>(256 x 512)<br/> - x5     |  2861.0269                         | 227.5936                     | 10                   | 59%       |    150                     |    20%       | 5680     | 1829     | 2954                    |  968.315                                  |
| 2048 point<br/>(1024 x 2048)<br/> - x5  |  3106.654                          | 3546.4672                    | 10                   | 78%       |    210                     |    19%       | 5680     | 1829     | 3594                    |  864.4003                                 |
| 64 point<br/>(32 x 64)<br/> - x10       |  2508.2792                         | 7.868                        | 20                   | 11%       |    280                     |    5%        | 11360    | 3654     | 2926                    |  857.2383                                 |
| 128 point<br/>(64 x 128)<br/> - x10     |  3568.5513                         | 20.4832                      | 20                   | 29%       |    280                     |    11%       | 11360    | 3800     | 3773                    |  945.8127                                 |
| 256 point<br/>(128 x 256)<br/> - x10    |  4866.9111                         | 63.20800                     | 20                   | 44%       |    280                     |    17%       | 11360    | 3654     | 4504                    |  1080.5752                                |
| 512 point<br/>(256 x 512)<br/> - x10    |  5721.85427                        | 227.5936                     | 20                   | 58%       |    300                     |    20%       | 11360    | 3654     | 5165                    |  1107.813025                              |
| 2048 point<br/>(1024 x 2048)<br/> - x10 |  6215.5280                         | 3546.4672                    | 20                   | 77%       |    420                     |    19%       | 11360    | 3654     | 6781                    |  916.60936                                |
 
##### cfloat Design
| FFT Configuration - Number of Instances    | Aggregate Throughput<br/>(in MSPS) | Average Latency<br/>(in μs) | Number of Compute Cores | Vector Load | Number of Active Memory Banks | Mem R/W Rate | FF (Regs)| CLB LUTs | Dynamic Power<br/>(in mW) | Performance per Watt<br/>(in MSPS/Watt) |
|:---------------------------------------:|:----------------------------------:|:---------------------------:|:--------------------:|:-----------:|:--------------------------:|:------------:|:--------:|:--------:|:-------------------------:|:---------------------------------------:|
| 64 point<br/>(32 x 64)<br/> - x1        |  159.560                            |   13.088                   | 2                    | 43%         | 24                         | 10%          | 1136     |  367     |    949                    |  168.135086                                  |
| 128 point<br/>(64 x 128)<br/> - x1      |  186.427                            |   45.244                   | 2                    | 60%         | 24                         | 14%          | 1136     |  367     |    1013                   |  184.0346884                                 |
| 256 point<br/>(128 x 256)<br/> - x1     |  197.008                            |   174.364                  | 2                    | 73%         | 24                         | 16%          | 1136     |  367     |    1060                   |  185.8569065                                 |
| 512 point<br/>(256 x 512)<br/> - x1     |  195.077                            |   713.74                   | 2                    | 82%         | 26                         | 17%          | 1136     |  367     |    1091                   |  178.8061921                                 |
| 2048 point<br/>(1024 x 2048)<br/> - x1  |  174.650                            |   12861.968                | 2                    | 89%         | 52                         | 9%           | 1136     |  367     |    1164                   |  150.0429553                                 |
| 64 point<br/>(32 x 64)<br/> - x5        |  797.800                            |   13.088                   | 10                   | 43%         | 120                        | 10%          | 5680     |  1828    |    2372                   |  336.3407605                                 |
| 128 point<br/>(64 x 128)<br/> - x5      |  932.135                            |   45.244                   | 10                   | 60%         | 120                        | 14%          | 5680     |  1828    |    2686                   |  347.0349327                                 |
| 256 point<br/>(128 x 256)<br/> - x5     |  985.041                            |   174.364                  | 10                   | 73%         | 120                        | 16%          | 5680     |  1828    |    2925                   |  336.7663605                                 |
| 512 point<br/>(256 x 512)<br/> - x5     |  975.3879                           |   713.74                   | 10                   | 81%         | 130                        | 17%          | 5680     |  1828    |    3080                   |  316.6843877                                 |
| 2048 point<br/>(1024 x 2048)<br/> - x5  |  873.2685                           |   12861.968                | 10                   | 88%         | 260                        | 9%           | 5680     |  1828    |    3644                   |  239.6455821                                 |
| 64 point<br/>(32 x 64)<br/> - x10       |  1595.601                           |   13.088                   | 20                   | 43%         | 240                        | 10%          | 11360    |  3654    |    4007                   |  398.203577                                  |
| 128 point<br/>(64 x 128)<br/> - x10     |  1864.271                           |   45.244                   | 20                   | 60%         | 240                        | 14%          | 11360    |  3654    |    4631                   |  402.5635195                                 |
| 256 point<br/>(128 x 256)<br/> - x10    |  1970.083                           |   174.364                  | 20                   | 72%         | 240                        | 16%          | 11360    |  3654    |    5084                   |  387.5065321                                 |
| 512 point<br/>(256 x 512)<br/> - x10    |  1950.775                           |   713.74                   | 20                   | 81%         | 260                        | 17%          | 11360    |  3654    |    5411                   |  360.5203259                                 |
| 2048 point<br/>(1024 x 2048)<br/> - x10 |  1746.537                           |   12861.968                | 20                   | 88%         | 520                        | 9%           | 11360    |  3654    |    6606                   |  264.3864672                                 |

These observations show that with the increase in the FFT point size, the window buffer size used in the AI Engines increases, and with that, the throughput increases. Increasing the `TP_WINDOW_VSIZE` parameter in the FFT AI Engine graph can further increase the throughput, especially for the lower point sizes. Still, the AI Engine mapper/router could encounter issues due to the higher memory requirement.

Furthermore, the FFT point sizes increase and the power does _not_ increase proportionately, so the performance per Watt maintains an increasing trend initially and saturates towards the end.

</details>

#### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
