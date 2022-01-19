<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2021.1 Versal 2D-FFT Implementation Using Vitis Acceleration Library Tutorial (XD073)</h1>
   </td>
 </tr>
</table>

# AI Engine Implementation 

## Table of Contents

[Building the Design](#Building-the-Design)

[Hardware Design Details](#Hardware-Design-Details)

[Software Design Details](#Software-Design-Details)

[Performance Details](#Performance-Details)

# Building the Design

<details>
<summary>Design Build</summary> 

## Design Build

In this section, you will build and run the 2D-FFT design using the AI Engine implementation. You will compile the AI Engine design and integrate it into a larger system design (including the PL kernels and PS host application). Review [Integrating the Application Section in the AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/yii1603912637443.html) for the general flow. 

At the end of this section, the design flow will generate a new directory (called `build/`). Underneath are sub-directories named `fft2d_$(MAT_ROWS)x$(MAT_COLS)/x$(FFT_2D_INSTS)/` (for example, `fft2d_1024x2048/x1/`) depending on value of matrix dimensions `${MAT_ROWS}`, `${MAT_COLS}` and the number of instances `$(FFT_2D_INSTS)` chosen in the build. Each sub-directory contains the `hw_emu/` and/or `hw/` subfolders. The respective subfolders contain `Work/` and `libadf.a`, outputs from the AI Engine compiler, the host app executable and the builds, targeted to `hw` or `hw_emu` respectively. The `hw_emu/` subfolder contains the build for hardware emulation. The `hw/` subfolder contains the build for hardware run on a VCK190 board.

</details>

<details>
<summary>Make Steps</summary> 

## Make Steps

To run the following `make` steps (that is, `make kernels`, `make graph`, and so on), you must be in the `AIE/` folder. The options that can be specified in the `make` steps are as follows.

`TARGET:` This can be set to `hw` or `hw_emu` to build the design in the hardware or hardware emulation flow respectively. The default option is `hw_emu`.

`FFT_2D_INSTS:` This can be set to 1, 5, or 10 to build the design with the number of kernel instances. The default is `1`.

`ITER_CNT:` The number of iterations the design is run. The default is `8`.

`FFT_2D_PT`: FFT 2D point. Permissible values are `64`, `128`, `256`, `512`, and `2048`.

`MAT_ROWS x MAT_COLS:` Dimensions of the matrix (number of rows in the input matrix x number of cols in the input matrix). Automatically configured as `FFT_2D_PT/2, FFT_2D_PT`. Permissible values are `32x64`, `64x128`, `128x256`, `256x512`, and `1024x2048`. The default is `1024x2048`.

`EN_TRACE:` Flag to enable trace profiling. `0` is disabled and `1` is enabled. The default is `0` (disabled).

The Makefile uses the following directory references:

```
# Relative fft_2d directory
RELATIVE_PROJECT_DIR := ./

# Absolute fft_2d directory = <user path>/Tutorials/AI_Engine/fft_2d
PROJECT_REPO := $(shell readlink -f $(RELATIVE_PROJECT_DIR))

DESIGN_REPO  := $(PROJECT_REPO)/design
AIE_SRC_REPO := $(DESIGN_REPO)/aie_src
HOST_APP_SRC := $(DESIGN_REPO)/host_app_src
PL_SRC_REPO  := $(DESIGN_REPO)/pl_src
SYSTEM_CONFIGS_REPO    := $(DESIGN_REPO)/system_configs
PROFILING_CONFIGS_REPO := $(DESIGN_REPO)/profiling_configs
BASE_BLD_DIR     := $(PROJECT_REPO)/build
FFTPT_BLD_DIR    := $(BASE_BLD_DIR)/fft2d_$(MAT_ROWS)x$(MAT_COLS)
INSTS_BLD_DIR    := $(FFTPT_BLD_DIR)/x$(FFT_2D_INSTS)
BUILD_TARGET_DIR := $(INSTS_BLD_DIR)/$(TARGET)
WORK_DIR         := Work
```

</details>

<details>
<summary>Build the Entire Design with a Single Command</summary>

## Build the Entire Design with a Single Command

If you are already familiar with the AI Engine and Vitis kernel compilation flows, you can build the entire design for each case of `FFT_2D_INSTS` with one command: 

```bash
make run (default hardware emulation, 1 instance, iterations=8, matrix dimentions rows=1024 and columns=2048, no trace-profiling )
```
or 

```bash
make run TARGET=hw FFT_2D_INSTS=5 ITER_CNT=16 EN_TRACE=1 FFT_2D_PT=64 (hardware, 5 instances, 16 iterations, enable trace profiling, matrix dimentions rows=32 and columns=64 )
```

This command runs the `make kernels`,`make graph`,`make xclbin`,`make application`,`make package`, and `make run_emu` for hardware emulation or to run on hardware (VCK190 board) depending on the `TARGET` you specify. The settings also apply to the individual make steps listed below.

The generated files for each `FFT_2D_INSTS` are placed under an individual directory: `$(BUILD_TARGET_DIR)/`. Each `make` step to build the design is specified in the following sections. These sections also detail the options used and the location of input and output files in each case.

</details>

<details>
<summary>make kernels: Compiling PL Kernels</summary> 

## make kernels: Compiling PL Kernels

In this step, the Vitis compiler takes any Vitis compiler kernels (RTL or HLS C) in the PL region of the target platform (`xilinx_vck190_base_202110_1`) and the AI Engine kernels and graph and compiles them into their respective XO files. The following commands compile the kernels (default `TARGET=hw_emu`, `FFT_2D_INSTS=1`, `ITER_CNT=8`, and `FFT_2D_PT=2048`). 

```
make kernels
```

The expanded command is as follows (for `dma_hls`):

```
mkdir -p $(BUILD_TARGET_DIR); \

cd $(BUILD_TARGET_DIR); \

v++ --target hw_emu --hls.clock 250000000:dma_hls --platform xilinx_vck190_base_202110_1 \
   --save-temps --temp_dir $(BUILD_TARGET_DIR)/_x --verbose -g -c -k dma_hls \
   $(DESIGN_REPO)/pl_src/dma_hls.cpp -o $(BUILD_TARGET_DIR)/dma_hls.hw_emu.xo
```

See [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitiscommandcompiler.html#wrj1504034328013) for a detailed description of all Vitis compiler switches. The following table provides a summary of the switches used. 

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

## make graph: Creating the AI Engine ADF Graph for Vitis Compiler Flow

An ADF graph can be connected to an extensible Vitis platform (the graph I/Os can be connected either to platform ports or to ports on Vitis kernels through Vitis compiler connectivity directives). 

* The AI Engine ADF C++ graph of the design contains AI Engine kernels and PL kernels. 
* All interconnects between kernels are defined in the C++ graph
* All interconnections to external I/O are fully specified in the C++ simulation testbench (`graph.cpp`) that instantiates the C++ ADF graph object. 

To compile the graph using the Makefile flow type (default `FFT_2D_INSTS=1`, `ITER_CNT=8`, `FFT_2D_PT=2048`):

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
   --verbose --Xpreproc="-DFFT2D_INSTS=1" --Xpreproc="-DMAT_ROWS=1024" --Xpreproc="-DMAT_COLS=2048" \
   --platform=<PLATFORM_REPO_PATHS/xilinx_vck190_base_202110_1>/xilinx_vck190_base_202110_1.xpfm \
   --log-level=5 --test-iterations=2 --dataflow --heapsize=7000 \
   --Xchess="main:bridge.llibs=softfloat m" --workdir=Work $(AIE_SRC_REPO)/graph.cpp 2>&1 | tee -a aiecompiler.log 

 ```

See [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vjv1611769393894.html) for full AI Engine programming environment documentation.

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
<summary>make xclbin: Using the Vitis Tools to Link AI Engine and HLS Kernels with the Platform</summary> 

## make xclbin: Using the Vitis Tools to Link AI Engine and HLS Kernels with the Platform

After the AI Engine kernels and graph and PL HLS kernels have been compiled, you can use the Vitis compiler to link them with the platform to generate a XCLBIN file. 

The Vitis tools allow you to integrate the AI Engine, HLS, and RTL kernels into an existing extensible platform. This is an automated step from a software developer perspective where the platform chosen is provided by the hardware designer. Alternatively, you can opt to use one of the many extensible base platforms provided by Xilinx, and use the Vitis tools to build the hardware design and integrate the AI Engine and PL kernels into it.
 
To test this feature in this tutorial, use the base VCK190 platform to build the design. The command to run this step is shown in the following example (default `TARGET=hw_emu`, `FFT_2D_INSTS=1`, `ITER_CNT=8`, `EN_TRACE=0`, `FFT_2D_PT=2048`):

```
make xclbin
``` 

The expanded command is as follows: 

```
cd $(BUILD_TARGET_DIR);	\

v++ -l --platform xilinx_vck190_base_202110_1 --save-temps \
   --temp_dir $(BUILD_TARGET_DIR)/_x --verbose -g --clock.freqHz 250000000:dma_hls_0 \
   --clock.defaultTolerance 0.001 --config $(SYSTEM_CONFIGS_REPO)/x1.cfg \
   --vivado.prop fileset.sim_1.xsim.simulate.log_all_signals=true \
   -t hw_emu -o $(BUILD_TARGET_DIR)/vck190_aie_fft_2d.hw_emu.xclbin \
   $(BUILD_TARGET_DIR)/dma_hls.hw_emu.xo \
   $(BUILD_TARGET_DIR)/libadf.a

```

If `EN_TRACE` is enabled, the following Vitis compiler flags are also set:

```
   --profile.data dma_hls:all:all or profile.data dma_hls:all:strmInp_from_colwiseFFT (for higher instances) \
   --profile.trace_memory DDR

```
For higher values of `FFT_2D_INSTS`, only the `strmInp_from_colwiseFFT` port is profiled to avoid too much data.

See [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/buildingdevicebinary.html#mjs1528399150499) for a detailed description of Vitis linking options.

|Switch|Description|
|  ---  |  ---  |
|--platform \| -f|Specifies the name of a supported acceleration platform as specified by the $PLATFORM_REPO_PATHS environment variable or the full path to the platform XPFM file.|
|--save-temps \| -s|Directs the V++ command to save intermediate files/directories created during the compilation and link process. Use the `--temp_dir` option to specify a location to write the intermediate files to.|
|--temp_dir <string>|This allows you to manage the location where the tool writes temporary files created during the build process. The temporary results are written by the Vitis compiler, and then removed, unless the `--save-temps` option is also specified.|
|--verbose|Display verbose/debug information.|
|--config <config_file>|Specifies a configuration file containing V++ switches.|
|--output \| -o|Specifies the name of the output file generated by the V++ command. In this design the outputs of the DMA HLS kernels and the PL kernels interfacing with the AI Engine are in XO files.|
|--profile.data [<kernel_name>\|all]:[<cu_name>\|all]:[<interface_name>\|all]\(:[counters\|all]\)|Enables monitoring of data ports through the monitor IPs. This option needs to be specified during linking. See [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitiscommandcompiler.html#lpy1600804966354) for detailed profiling options.|
|--profile.trace_memory \<FIFO\>:\<size\>\|\<MEMORY\>[\<n\>]|When building the hardware target \(-t=hw\), use this option to specify the type and amount of memory to use for capturing trace data. See [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitiscommandcompiler.html#lpy1600804966354) for detailed profiling options.|

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
# Disable Profiling in hw_emu so that it is faster...
param=hw_emu.enableProfiling=false

# Export the xsa of the design..
param=compiler.addOutputTypes=hw_export

```

See [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitiscommandcompiler.html?hl=--config#pni1524163195211) for a detailed description of the Vitis compiler configuration file.


|Switch|Comment|
|  ---  |  ---  |
|--connectivity.nk|Number of kernels. `dma_hls:1:dma_hls_0` means that the Vitis compiler should instantiate one dma_hls kernel and name the instance `dma_hls_0`.|
|--connectivity.stream_connect|How the kernels will connect to IPs, platforms, or other kernels. The output of the AI Engine compiler tells you the interfaces that need to be connected. `dma_hls_0.strmOut_to_rowiseFFT:ai_engine_0.DataIn0` means that the Vitis compiler should connect the port `strmOut_to_rowiseFFT` of the `dma_hls` PL kernel to the shim channel of the AI Engine with the logical name `DataIn0`, defined in `$(AIE_SRC_REPO)/graph.cpp` as part of the PLIO instantiation.|
|param=compiler.addOutputTypes=hw_export| This option tells the Vitis compiler that besides creating an XCLBIN file, it also outputs an XSA file which is needed to create a post-Vivado fixed platform for Vitis software development.|

The Vitis compiler calls the Vivado® IP integrator under the hood to build the design. The platform and kernels are input to the Vivado Design Suite, which produces a simulation XSA or an XSA after running place and route on the design. The point at which the XSA is produced from Vivado depends on the `-target` option set on the Vitis compiler command line. 

You can now view the Vivado project, which is located in the `$(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj` directory. You have now have generated the XCLBIN file that will be used to execute your design on the platform.

</details>

<details>
<summary>make application: Compiling the Host Application</summary> 

## make application: Compiling the Host Application

You can compile the host application by following the typical cross-compilation flow for the Cortex A72. To build the application, run the following command (default `FFT_2D_INSTS=1`, `ITER_CNT=8`, `FFT_2D_PT=2048`):

```
make application
```
or

```
cd $(BUILD_TARGET_DIR);	\

aarch64-xilinx-linux-g++ -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong\
   -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$(SDKTARGETSYSROOT) -O -c\
   -std=c++14 -D__linux__ -D__PS_ENABLE_AIE__ -DXAIE_DEBUG -DFFT2D_INSTS=1 -DITER_CNT=8\
   -DMAT_ROWS=1024 -DMAT_COLS=2048 -I$(SDKTARGETSYSROOT)/usr/include/xrt -I$(XILINX_VITIS)/aietools/include/\
   -I$(SDKTARGETSYSROOT)/usr/include -I$(SDKTARGETSYSROOT)/usr/lib -I$(AIE_SRC_REPO) -I$(HOST_APP_SRC)\
   -I$(DSPLIB_ROOT)/L1/include/aie -I$(DSPLIB_ROOT)/L1/src/aie -I$(DSPLIB_ROOT)/L1/tests/aie/inc\
   -I$(DSPLIB_ROOT)/L1/tests/aie/src -I$(DSPLIB_ROOT)/L2/include/aie -I$(DSPLIB_ROOT)/L2/tests/aie/common/inc\
   $(BUILD_TARGET_DIR)/$(WORK_DIR)/ps/c_rts/aie_control_xrt.cpp -o $(BUILD_TARGET_DIR)/app_control.o

aarch64-xilinx-linux-g++ -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong\
   -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$(SDKTARGETSYSROOT) -O -c\
   -std=c++14 -D__linux__ -D__PS_ENABLE_AIE__ -DXAIE_DEBUG -DFFT2D_INSTS=1 -DITER_CNT=8\
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

See [this page](https://xilinx.github.io/XRT/2021.1/html/index.html) for XRT documentation. See [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/devhostapp.html#vpy1519742402284) for details of host application programming.


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

## make package: Packaging the Design

With the AI Engine outputs created, as well as the new platform, you can now generate the programmable device image (PDI) and a package to be used on an SD card. The PDI contains all the executables, bitstreams, and configurations of the device. The packaged SD card directory contains everything to boot Linux, the generated applications, and the XCLBIN.

The command to run this step is as follows (default `TARGET=hw_emu`, `EN_TRACE=0`, `FFT_2D_INSTS=1`, and `FFT_2D_PT=2048`):

```
make package
``` 

or

```
cp $(PROJECT_REPO)/run_script.sh $(BUILD_TARGET_DIR)/
cd$(BUILD_TARGET_DIR);	\

v++ -p -t hw --save-temps --temp_dir $(BUILD_TARGET_DIR)/_x -f xilinx_vck190_base_202110_1\
   --package.rootfs $(XLNX_VERSAL)/rootfs.ext4\
   --package.kernel_image $(XLNX_VERSAL)/Image --package.boot_mode=sd\
   --package.out_dir $(BUILD_TARGET_DIR)/package --package.image_format=ext4\
   --package.sd_file $(BUILD_TARGET_DIR)/fft_2d_aie_xrt.elf \
   $(BUILD_TARGET_DIR)/vck190_aie_fft_2d.hw.xclbin $(BUILD_TARGET_DIR)/libadf.a \
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

See [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/packagesystem1.html#cwq1586366344968) for more details about packaging the system.

|Switch|Description|
|  ---  |  ---  |
|--target \| -t [hw\|hw_emu]|Specifies the build target.|
|--package \| -p|Packages the final product at the end of the Vitis compile and link build process.|
|--package.rootfs \<arg\>|Where \<arg\> specifies the absolute or relative path to a processed Linux root file system file. The platform RootFS file is available for download from xilinx.com. Refer to the [Vitis Software Platform Installation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/acceleration_installation.html) for more information.|
|--package.kernel_image \<arg\>|Where \<arg\> specifies the absolute or relative path to a Linux kernel image file. Overrides the existing image available in the platform. The platform image file is available for download from xilinx.com. Refer to the [Vitis Software Platform Installation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/acceleration_installation.html) for more information.|
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
|$(BUILD_TARGET_DIR)/vck190_aie_fft_2d.hw_emu.xclbin|The XCLBIN file created in the `make xclbin` step.|
|$(BUILD_TARGET_DIR)/libadf.a|The compiled AI Engine design graph created in the `make graph` step.|

The output of the Vitis compiler package step is the package directory that contains the contents to run hardware emulation. 

|Output Objects|Description|
|  ---  |  ---  |
|$(BUILD_TARGET_DIR)/package|The hardware emulation package that contains the boot file, hardware emulation launch script, PLM and PMC boot files, PMC and QEMU command argument specification files, and Vivado simulation folder.|

</details>

<details>
<summary>make run_emu: Running Hardware Emulation</summary>

## make run_emu: Running Hardware Emulation

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

When hardware emulation is launched, you will see the QEMU simulator load. Wait for the autoboot countdown to go to zero. After a few minutes, the root Linux prompt comes up: 

```bash
root@versal-rootfs-common-2021.1:~#
```

After the root prompt comes up, run the following commands to run the design:  

```
mount /dev/mmcblk0p1 /mnt
cd /mnt
export XILINX_XRT=/usr
./fft_2d_aie_xrt.elf a.xclbin
```

The `fft_2d_aie_xrt.elf` executes. After a few minutes, you should see the output with `TEST PASSED` on the console. When this is shown, run the following keyboard command to exit the QEMU instance: 

```
#To exit QEMU Simulation
Press CtrlA, let go of the keyboard, and then press x 
```

To run with waveform, do the following:

```
cd $(BUILD_TARGET_DIR)/package
./launch_hw_emu.sh -g
```

The XSIM Waveform Viewer is launched. Drag and drop the signals into the viewer and click **Play** to start the emulation. Go back to the terminal and wait for the Linux prompt to show up. In the XSIM Waveform Viewer, you will see the signals you added to the waveform adjusting over the execution of the design. When this is done, hit the pause button and close the window to end the emulation.

The following figure shows a waveform view of the 32x64 - 1x design.

![Image of 2D-FFT AIE HW_EMU Run Waveform View For 32x64-1x Design](images/fft_2d_aie_hw_emu_waveform_view_32x64_x1.PNG)

</details>

<details>
<summary>TARGET=hw: Running on Hardware</summary>

## Running on Hardware

To run the design in hardware, rerun the following `make` steps with `TARGET=hw` and other applicable options (see the preceding `make` steps specified above).

```
make kernels TARGET=hw
make xclbin TARGET=hw 
make package TARGET=hw 
```

These commands create a `$(BUILD_TARGET_DIR)` folder with the kernels, XCLBIN, and `package` for a hardware run. 

Run the following step to set up the execution file, generated images, and base images (`$(BUILD_TARGET_DIR)/package/sd_card` and `$(BUILD_TARGET_DIR)/package/sd_card.img`).

```
make run_emu TARGET=hw 
```

These commands create a `build/hw` folder with the kernels, XCLBIN, and `package` for a hardware run. Follow steps 1-9 to run the `fft_2d_aie_xrt.elf` executable on your VCK190 board. 

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

**Step 8.** Wait until you see the `root@versal-rootfs-common-2021_1` Linux command prompt. Press **Enter** a few times to get past any `xinit` errors. 

**Step 9.** Run the following commands in the TeraTerm terminal: 

```
cd /mnt/sd-mmcblk0p1
export XILINX_XRT=/usr
./init.sh

./fft_2d_aie_xrt.elf a.xclbin
```

</details>

# Hardware Design Details
<details>
<summary>2D-FFT AI Engine Implementation Architecture and AI Engine/PL Function Partitioning</summary>

## 2D-FFT AI Engine Implementation Architecture and AI Engine/PL Function Partitioning

The following figure shows a high-level block diagram of the design. The test harness consists of the AI Engine and data mover HLS kernels (`dma_hls`). In this setup, there is an AXI4-Stream interface between the data mover kernels and AI Engines, with a data width of 128 bits. The data mover kernels and the AI Engine array interface are running at 250 MHz.

The data mover is a PL-based data generator and checker. It generates impulse input and checks the output of the row-wise FFT core for its response. It then generates the transposed pattern of the row-wise FFT output and feeds that to the col-wise FFT core and checks its output.

![Image of 2D-FFT AIE Implementation Architecture](images/fft_2d_aie_block_diagram.PNG)

</details>

<details>
<summary>Design Details</summary>

## Design Details

The design in this tutorial starts with a base platform containing the control interface and processing system (CIPS), NoC, AI Engine, and the interfaces among them. The Vitis compiler linker step builds on top of the base platform by adding the AI Engine graphs and PL kernels. To add the various functions in a system-level design, PL kernels are added to the base platform depending on the application (that is, the PL kernels present in each design might vary). An ADF graph is connected to an extensible Vitis platform where the graph I/Os are connected either to the platform ports or to ports on Vitis kernels through the Vitis compiler connectivity directives. In the design, the components are added by the Vitis compiler `-l` step (see [make XCLBIN](#make-xclbin-using-the-vitis-tools-to-link-ai-engine-and-hls-kernels-with-the-platform)) and include the following:


* `libadf.a`
* Data mover kernel (`dma_hls.[hw|hw_emu].xo`)
* Connection interfaces defined in the system configuration file

To see a schematic view of the design with the extended platform as shown in the following figure, open the following in Vivado:

```
`build/fft2d_$(MAT_ROWS)x$(MAT_COLS)/x$(FFT_2D_INSTS)/[hw|hw_emu]/_x/link/vivado/vpl/prj/prj.xpr`
```

![Image of 2D-FFT AIE 1x Vivado BD](images/fft_2d_aie_1x_vivado_bd.PNG)

In this design, the 2D FFT computation happens in two stages: the first compute is across the row vectors and the second stage is performed across the column vectors.The input data is accessed linearly and streamed to the AI Engines which perform `MAT_COLS( default 2048 )` point FFT. The data coming out of the AI Engines is streamed to a PL kernel where it is checked against the expected pattern (the first row should be 1 and the remaining should be 0). If there is a mismatch, it is recorded in the variable `stage0_errCnt`. The transposed pattern of the output of the row vectors is then linearly streamed into another AI Engine which performs `MAT_ROWS( default 1024 )` point FFT. The output is streamed into a data mover kernel again and is checked against the expected pattern (all values should be 1). If there is a mismatch, it is stored in the variable `stage1_errCnt`. Finally, the sum of `stage0_errCnt` and `stage1_errCnt` is returned from the kernel, which is read in the host app to determine whether the test has passed or failed.

The system debugging and profiling IP (DPA) is added to the PL region of the device to capture AI Engine runtime trace data if the `EN_TRACE` option is enabled in the design. The `dma_hls` kernel and the AI Engine array interface are both operating at 250 MHz. Unlike the HLS/DSP implementation, there is no clock domain crossing in the PL region in this design.

</details>

<details>
<summary>AI Engine and PL Kernels</summary>

## AI Engine and PL Kernels

The top-level AI Engine graph, `graph.cpp`, contains two sub-graphs: `FFTrows_graph` and `FFTcols_graph`. Each sub-graph contains the individual AI Engine kernel, `*FFTrow_gr.getKernels()`, and `*FFTcol_gr.getKernels()`, which performs `MAT_COLS` and `MAT_ROWS` point FFT respectively.

### dma_hls

The PL-based data movers consist of the `dma_hls` kernel, which generates impulse input and checks the output of each FFT stage for the expected pattern.

* It internally comprises four loops (`mm2s0`, `s2mm0`, `mm2s1`, and `s2mm1`), with `s2mm0` - `mm2s1` sequenced one after the other and wrapped into the `dmaHls_rowsToCols` function. `mm2s0`, `dmaHls_rowsToCols`, and `s2mm1` are concurrently scheduled.
* The data width is 128 bits at both the AXI4-stream I/O sides, running at 250 MHz.

</details>

# Software Design Details

The software design in the AI Engine 2D-FFT tutorial consists of the following sections:

<details>
<summary>Methodology</summary>

## Methodology

The following figure elaborates on the AI Engine implementation methodology.

![Image of 2D-FFT AIE Implementation Methodology](images/fft_2d_aie_block_diagram_methodology.PNG)

### AI Engine

#### Independent Cores

Both AI Engine graphs for `FFTrows_graph` and `FFTcols_graph` are to be configured to be independent, with runtime ratios set to >= 0.6 so that each can be run independently of the other.

```
...
runtime<ratio>(*FFTrow_gr.getKernels()) = 0.6;
...
runtime<ratio>(*FFTcol_gr.getKernels()) = 0.6;
...
```

#### Window Streaming Buffer Config

The `FFTrows_graph` graph performs `MAT_COLS` point FFT and runs for `MAT_ROWS` number of iterations. For the `FFTcols_graph` graph, increase the `TP_WINDOW_VSIZE` to `MAT_COLS` instead of `MAT_ROWS` and it does `MAT_ROWS` point FFT, but runs for `MAT_ROWS` number of iterations instead of `MAT_COLS`. This reduces the ping-pong overhead which improves the overall throughput. 

Large windows may result in mapper errors due to excessive memory usage. The increased `TP_WINDOW_VSIZE` reduces ping-pong overhead, but increases the utilization of AIE cores and thereby the power consumption. In this design due to rows to cols ratio being 1:2 the `TP_WINDOW_VSIZE` of both graphs are also in the same ratio. Which gives an additional increase in throughput with minimal increase in utilization.
```
...
// TP_WINDOW_VSIZE for FFTrows_graph...
#define FFT_ROW_TP_WINDOW_VSIZE MAT_COLS

// TP_WINDOW_VSIZE for FFTcols_graph
// Increasing the "TP__WINDOW _VSIZE" so that the ping-pong overhead is less
// Assigning it as MAT_COLS instead of MAT_ROWS...
#define FFT_COL_TP_WINDOW_VSIZE MAT_COLS
...
// FFTrows_graph I/O WINDOW BUFF SIZE IN BYTES...
#define FFT_ROW_WINDOW_BUFF_SIZE (FFT_ROW_TP_WINDOW_VSIZE * 4)
// FFTcols_graph I/O WINDOW BUFF SIZE IN BYTES...
#define FFT_COL_WINDOW_BUFF_SIZE (FFT_COL_TP_WINDOW_VSIZE * 4)
...
```

### Data Mover

#### Data Generation/Checking and Sequencing

The data mover comprises four loops: `mm2s0`, `s2mm0`, `mm2s1`, and `s2mm1`. The `s2mm0` and `mm2s1` functions are wrapped into a single function, `dmaHls_rowsToCols`. Within that the execution sequence, `s2mm0` is followed by `mm2s1`. The `s2mm0` and `s2mm1` functions check the output of the row-wise and col-wise FFT respectively against the expected golden output.

#### Concurrent Scheduling

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

#### Vitis HLS Scheduling and Dataflow View

The following figure shows the data mover scheduler view.

![Image of Datamover Scheduler View](images/dma_hls_scheduler_view.PNG)

The following figure shows the data mover dataflow view.

![Image of Datamover Dataflow View](images/dma_hls_dataflow_view.PNG)

### Streaming Interface Data Width

The streaming interface data width is kept at 128 bits to reduce read/write overhead while processing data.

### Frequency Selection

AI Engine kernels are configured for `cint16 / 4bytes` and the streaming interface is at `128bit / 16bytes`. The frequency of the AI Engine array is 1000 MHz and the data mover is kept at 250 MHz, maintaining a 1:4 ratio.

</details>

<details>
<summary>AI Engine Kernels and Graph Representation</summary>

## AI Engine Kernels and Graph Representation

An AI Engine kernel is a C/C++ program written using specialized intrinsic calls that target the VLIW vector processor. The AI Engine compiler compiles the kernel code to produce an executable ELF file for each of the AI Engines being used in the design. Review the [AI Engine Kernel Programming](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/yii1603912637443.html) section in the AI Engine documentation for a high-level overview of kernel programming. These kernels can be stitched together to function as AI Engine graphs written in C++. In this design, the AI Engine compiler writes a summary of compilation results. You can view the graph by running the following command:

`vitis_analyzer $(BUILD_TARGET_DIR)/Work/graph.aiecompile_summary`

The following figures show the graph representation of the AI Engine kernels (default FFT 2048 point and FFT 1024 point, `FFT_2D_INSTS=1`). In addition to the compute units, there are also the twiddle factor LUTs (`fft_lut_tw*`) and temporary buffers for FFT stages (`fft_2048/1024_tmp*`).

![Image of 2D-FFT AI Engine 2K point Graph](images/fft_2d_aie_fft_rows_graph_for_2kpt_1x.PNG)
![Image of 2D-FFT AI Engine 1K point Graph](images/fft_2d_aie_fft_cols_graph_for_1kpt_1x.PNG)

</details>

<details>
<summary>Adaptive Data Flow (ADF) Graph</summary>

## Adaptive Data Flow (ADF) Graph

This section describes the overall data flow graph specification of the 2D-FFT design using AI Engine which is compiled by the AI Engine compiler. Refer to [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/yii1603912637443.html) in the AI Engine documentation for information on ADF graphs.

The overall graph definition of the design is contained in the `graph.cpp` file. The top-level graph contains two sub-graphs, `FFTrows_graph` and `FFTcols_graph`, each with a `FFT_2D_INSTS` number of objects. The following describes the definition of the sub-graphs (the `FFTrows_graph` is used as illustration).

### Defining the Graph Class

Define the graph classes by using the objects defined in the appropriate name space. It must include the ADF library and [Vitis DSP Library](https://xilinx.github.io/Vitis_Libraries/dsp/2021.1/user_guide/L2/2-dsp-lib-func.html#fft-ifft) for FFIT. A general specification is put in for the ADF namespace:

```
#pragma once

// Input data type...
#define FFT_2D_TT_DATA cint16
// Twiddle Factor data type...
#define FFT_2D_TT_TWIDDLE cint16

// FFTrows_graph FFT point size...
#define FFT_ROW_TP_POINT_SIZE MAT_COLS
// FFTcols_graph FFT point size...
#define FFT_COL_TP_POINT_SIZE MAT_ROWS

// 1 (FFT) or 0 (IFFT)...
#define FFT_2D_TP_FFT_NIFFT 1
// 0 Bit Shift before output, will have to change based on input...
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

// FFTrows_graph I/O WINDOW BUFF SIZE IN BYTES...
#define FFT_ROW_WINDOW_BUFF_SIZE (FFT_ROW_TP_WINDOW_VSIZE * 4)
// FFTcols_graph I/O WINDOW BUFF SIZE IN BYTES...
#define FFT_COL_WINDOW_BUFF_SIZE (FFT_COL_TP_WINDOW_VSIZE * 4)

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
         
         connect< window<FFT_ROW_WINDOW_BUFF_SIZE> > (in,   FFTrow_gr.in);
         connect< window<FFT_ROW_WINDOW_BUFF_SIZE> > (FFTrow_gr.out, out);
   	}
};
```

### Top-Level Application

Define a top-level application file (`graph.cpp` in this design) that contains an instance of the graph class and connect the graph to a simulation platform to provide file input and output (in the case of `FFT2D_INSTS = 1` to the two sub-graphs):

```
#include "graph.h"

// Subgraph objects...
FFTrows_graph fft_rows[FFT2D_INSTS];
FFTcols_graph fft_cols[FFT2D_INSTS];

// Instantiating PLIOs...
PLIO *attr_i0 = new PLIO("DataIn0",  plio_128_bits, "../../../../design/aie_src/aiesim_data/input0.txt");
PLIO *attr_i1 = new PLIO("DataIn1",  plio_128_bits, "../../../../design/aie_src/aiesim_data/input1.txt");

PLIO *attr_o0 = new PLIO("DataOut0", plio_128_bits, "data/output0.txt");
PLIO *attr_o1 = new PLIO("DataOut1", plio_128_bits, "data/output1.txt");

simulation::platform<2, 2> plat( attr_i0,
                                 attr_i1,
                                 
                                 attr_o0,
                                 attr_o1
                               );

connect<> neti0(plat.src[0], fft_rows[0].in);
connect<> neti1(plat.src[1], fft_cols[0].in);

connect<> neto0(fft_rows[0].out, plat.sink[0]);
connect<> neto1(fft_cols[0].out, plat.sink[1]);
```

The `main` function is called under the guard bounds of `\_\_AIESIM\_\_` as shown below, to avoid conflict with the `main` function in the host application:

```
#ifdef __AIESIM__

   int main(int argc, char ** argv)
   {
      for(int i = 0; i< FFT2D_INSTS; i++)
      {
         // Intialise the graphs...

         fft_rows[i].init();
         fft_cols[i].init();
      }

      for(int i = 0; i< FFT2D_INSTS; i++)
      {
         // Run the graphs...

         fft_rows[i].run(MAT_ROWS);
         fft_cols[i].run(MAT_ROWS);
      }

      for(int i = 0; i< FFT2D_INSTS; i++)
      {
         // End the graphs once done...

         fft_rows[i].end();
         fft_cols[i].end();
      }
      
      return 0;
   }

#endif
```

</details>

<details>
<summary>PL Data Mover Kernel</summary>

## PL Data Mover Kernel

In addition to the kernels operating in the AI Engine array, this design specifies a data mover kernel to run in the PL region of the device (written in HLS C++). The data mover kernel is brought into the design during the Vitis kernel compilation, which is further replicated based on the `FFT_2D_INSTS` value. The software design of the data mover kernel is described in the following sections. 

### dma_hls (dma_hls.cpp)

The `dma_hls` kernel reads data from a memory mapped AXI4 (MM-AXI4) interface and writes it to an AXI4-Stream interface.

#### Top Function Declaration

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

#### Top Function Definition

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

The `dma_hls` kernel also specifies HLS pragmas to help optimize the kernel code and adhere to interface protocols. See [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/hls_pragmas.html?hl=hls%2Cpragmas) for detailed documentation of all HLS pragmas. A summary of the HLS pragmas used in the kernel is provided in the following table.

|Switch|Description|
|  ---  |  ---  |
|#pragma HLS INTERFACE|In C/C++ code, all input and output operations are performed, in zero time, through formal function arguments. In a RTL design, these same input and output operations must be performed through a port in the design interface and typically operate using a specific input/output (I/O) protocol. For more information, see [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/hls_pragmas.html#jit1504034365862).|
|#pragma HLS PIPELINE II=1|Reduces the initiation interval (II) for a function or loop by allowing the concurrent execution of operations. The default type of pipeline is defined by the config_compile -pipeline_style command, but can be overridden in the PIPELINE pragma or directive. For more information, see [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/hls_pragmas.html#fde1504034360078).|
|#pragma HLS dataflow|The DATAFLOW pragma enables task-level pipelining, allowing functions and loops to overlap in their operation, increasing the concurrency of the RTL implementation and increasing the overall throughput of the design. See [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/hls_pragmas.html#sxx1504034358866) for more information.|
|#pragma HLS loop_tripcount|When manually applied to a loop, specifies the total number of iterations performed by a loop. The `LOOP_TRIPCOUNT` pragma or directive is for analysis only, and does not impact the results of synthesis. See [this page](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/hls_pragmas.html#sty1504034367099) for more information.|
 
</details>

<details>
<summary>PS Host Application</summary>

## PS Host Application

The 2D-FFT AI Engine tutorial uses the embedded processing system (PS) as an external controller to control the AI Engine graph and data mover PL kernels. Review the [Programming the PS Host Application](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/yii1603912637443.html) section in the AI Engine documentation to understand the process to create a host application.

In addition to the PS host application (`fft_2d_aie_app.cpp`), the AI Engine control code must also be compiled. This control code (`aie_control_xrt.cpp`) is generated by the AI Engine compiler when compiling the AI Engine design graph and kernel code. The AI Engine control code is used by the PS host application for the following purposes:

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
   // to be given as by 4.. 
   /////////////////////////////////////////////////
   #define MAT_SIZE_128b (MAT_SIZE / 4)
   #define MAT_ROWS_128b (MAT_ROWS / 4)
   #define MAT_COLS_128b (MAT_COLS / 4)
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
   * The A72 processor resets the graph using the `xrtGraphReset` function and runs the graph execution using the `xrtGraphRun` function for both the 2K point and 1K point sub-graphs.

5. Execute the data mover kernels and generate the output results:

   * Set the `dma_hls` kernel arguments using the `xrtRunSetArg` function.
   * Start the `dma_hls` kernels using the `xrtRunStart` function.
   * Wait for `dma_hls` execution to finish using the `xrtRunWait` runction.

6. Verify the output results by reading the `ap_return` in `$(BUILD_TARGET_DIR)/_x/dma_hls.$(TARGET)/dma_hls/dma_hls/ip/drivers/dma_hls_v1_0/src/xdma_hls_hw.h` using the `xrtKernelRegister` API, as shown below:

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

# Performance Details

For all applications, designers must work to predefined specifications and build a system for their specific deployment by meeting their system requirements with respect to their available resources, latency, throughput, performance, and power. In this section, it is outlined how to measure those characteristics for the AI Engine implementation in this tutorial.

<details>
<summary>Resource Utilization and Power</summary> 

### Resource Utilization and Power

Resource utilization and power are measured using Vivado, vcdanalyze, and Xilinx Power Estimator (XPE) for Versal (2020.3 version) tools.

The registers and CLB LUT utilization information can be found in the Vivado project if you perform the following steps:

1. Open the Vivado project: ``$(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr``.

2. Go to **Open Implemented Design** then click **Report Utilization**. In the Utilization tab shown in the following figure, select **ai_engine_0** and view the **Registers** and **CLB LUTs** for the 1024 x 2048 point 1-instance design:

![Image of 2D-FFT AIE Utilization](images/fft_2d_aie_vivado_resources.PNG)

The vcdanalyze tool is used to generate a `graph.xpe` file which can be input to XPE for viewing the AI Engine resource utilization and power. The steps are as follows:

1. Run `make vcd` (recipe expanded below) to create the `graph.xpe` file under `$(BUILD_TARGET_DIR)/aiesim_xpe/`:

```
cd $(BUILD_TARGET_DIR); \
aiesimulator --pkg-dir $(WORK_DIR)/ --dump-vcd x$(FFT_2D_INSTS) 2>&1 | tee -a vcd.log
cd $(BUILD_TARGET_DIR); \
vcdanalyze --vcd x$(FFT_2D_INSTS).vcd --xpe
```

2. If you do not already have it installed, download and install [XPE for Versal Version 2020.3](https://www.xilinx.com/products/technology/power/xpe.html). For full documentation of XPE, see [this page](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2021_1/ug1275-xilinx-power-estimator-versal.pdf).

3. Follow the steps below to load the `graph.xpe` into XPE to see the AI Engine power comsumption and resource utilization (step 5 and 6 in the below images) for the 1024 x 2048 point 1-instance design:

![Image of 2D-FFT AIE XPE Intro](images/fft_2d_aie_xpe_intro_step1_2and3.PNG)
![Image of 2D-FFT AIE XPE Util and Power Measurement](images/fft_2d_aie_xpe_Pow_nUtil_step4_5and6.PNG)

A summary of resource utilization and power for all variations is given in the following table.

| No. of Instances | FFT Configuration            | Number of Compute Cores | Vector Load | Number of Active Memory Banks | Mem R/W Rate | Active AI Engine Tiles | Interconnect Load | FF (Regs) | CLB LUTS  | Dynamic Power<br/>(in mW) | 
|:----------------:|:----------------------------:|:-----------------------:|:-----------:|:-----------------------------:|:------------:|:----------------:|:-----------------:|:---------:|:---------:|:-------------------------:|
| 1                | 64 point<br/>(32 x 64)       | 2                       | 8%          | 28                            | 4%           | 6                | 5%                | 1172      | 413       | 695                       |
| 1                | 128 point<br/>(64 x 128)     | 2                       | 17%         | 28                            | 7%           | 6                | 5%                | 1172      | 413       | 730                       |
| 1                | 256 point<br/>(128 x 256)    | 2                       | 32%         | 28                            | 13%          | 6                | 5%                | 1172      | 413       | 786                       |
| 1                | 512 point<br/>(256 x 512)    | 2                       | 50%         | 30                            | 18%          | 6                | 5%                | 1172      | 415       | 855                       |
| 1                | 2048 point<br/>(1024 x 2048) | 2                       | 76%         | 42                            | 19%          | 6                | 5%                | 1172      | 413       | 950                       |
| 5                | 64 point<br/>(32 x 64)       | 10                      | 9%          | 132                           | 5%           | 25               | 6%                | 5860      | 2079      | 1456                      |
| 5                | 128 point<br/>(64 x 128)     | 10                      | 18%         | 132                           | 8%           | 27               | 5%                | 5860      | 2079      | 1680                      |
| 5                | 256 point<br/>(128 x 256)    | 10                      | 32%         | 132                           | 13%          | 25               | 6%                | 5860      | 2079      | 1893                      |
| 5                | 512 point<br/>(256 x 512)    | 10                      | 50%         | 142                           | 19%          | 27               | 6%                | 5860      | 2079      | 2271                      |
| 5                | 2048 point<br/>(1024 x 2048) | 10                      | 76%         | 202                           | 19%          | 28               | 5%                | 5860      | 2091      | 2781                      |
| 10               | 64 point<br/>(32 x 64)       | 20                      | 8%          | 264                           | 4%           | 48               | 6%                | 11720     | 4154      | 2369                      |
| 10               | 128 point<br/>(64 x 128)     | 20                      | 18%         | 264                           | 8%           | 52               | 6%                | 11720     | 4154      | 2820                      |
| 10               | 256 point<br/>(128 x 256)    | 20                      | 32%         | 272                           | 13%          | 50               | 6%                | 11720     | 4154      | 3302                      |
| 10               | 512 point<br/>(256 x 512)    | 20                      | 50%         | 284                           | 19%          | 52               | 6%                | 11720     | 4150      | 4021                      |
| 10               | 2048 point<br/>(1024 x 2048) | 20                      | 76%         | 396                           | 20%          | 56               | 6%                | 11720     | 4154      | 5081                      |

</details>

<details>
<summary>Throughput and Latency</summary> 

### Throughput and Latency

Throughput is measured in mega-samples transferred per second (MSPS). Latency is defined as the time between the first sample being sent by the data mover into the row-wise FFT kernel and the first sample from the col-wise FFT kernel being received by the data mover. It is measured by viewing the runtime generated trace texts using Vitis analyzer. The steps to measure throughput and latency are listed below:

1. Compile the design using `EN_TRACE=1`. It automatically includes a `xrt.ini` file while packaging, which comprises the following:

   ```
   [Debug]
   xrt_trace=true
   data_transfer_trace=fine
   trace_buffer_size=500M
   ```

   Refer to the [xrt.ini](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/xrtini.html#tpi1504034339424) documentation for more information. 

2. After execution on the board, transfer the generated `device_trace_0.csv`, `hal_host_trace.csv`, and `xclbin.run_summary` files back to your system.

3. Open `xclbin.ex.run_summary` using `vitis_analyzer`: `vitis_analyzer xclbin.ex.run_summary`.

4. The snapshot of the timeline trace for the AI Engine 1024 x 2048 point 1-instance design run with `ITER_CNT=8` is shown in the following figure:

![Image of 2D-FFT AI Engine implementation 1x Timeline Trace](images/fft_2d_aie_trace_1kx2k_1x_iter8.PNG)

5. The profiling setup in the Makefile measures the execution time and all the interfaces. For higher instance designs only, `strmInp_from_colwiseFFT` is profiled.

The throughput and latency calculations for the 1024 x 2048 point 1-instance design based on the `hw_emu` run is as follows:

```
Execution Time:
   = Difference in execution timeline trace
   = (End of Execution Timestamp of Stream `strmInp_from_rowiseFFT`) -
     (Start of Execution Timestamp of Stream `strmOut_to_rowiseFFT`)
   = 4210.95us

Latency:
   = Difference between strmInp_from_colwiseFFT beginning and execution beginning
   = (Start of Execution Timestamp of Stream `strmOut_to_colwiseFFT`) -
     (Start of Execution Timestamp of Stream `strmOut_to_rowiseFFT`)
   = 4210.6us

Throughput = (Samples transferred) / execution time
           = (MAT_ROWS x MAT_COLS) / execution time
           = (1024 x 2048) / 4210.95us
           = 498 MSamples/s
           = 498 x 4 MB/s (As each sample is 4bytes)
           = 1992 MB/s
```

A summary of throughput and latency for all variations is shown in the following table.

| No. of Instances | FFT Configuration            | Data Transfer Size | Aggregate Throughput<br/>(in MSPS) | Average Throughput<br/>(in MSPS) | Average Latency<br/>(in μs) | Minimum Latency<br/>(in μs) |
|:----------------:|:----------------------------:|:------------------:|:----------------------------------:|:--------------------------------:|:---------------------------:|:---------------------------:|
| 1                | 64 point<br/>(32 x 64)       | 16384              | 235.402                            | 235.402                          | 8.7                         |  8.7                        |
| 1                | 128 point<br/>(64 x 128)     | 65536              | 362.478                            | 362.478                          | 22.6                        |  22.6                       |
| 1                | 256 point<br/>(128 x 256)    | 262144             | 468.114                            | 468.114                          | 70.0                        |  70.0                       |
| 1                | 512 point<br/>(256 x 512)    | 1048576            | 559.420                            | 559.420                          | 234.3                       |  234.3                      |
| 1                | 2048 point<br/>(1024 x 2048) | 16777216           | 498.065                            | 498.065                          | 4210.6                      |  4210.6                     |
| 5                | 64 point<br/>(32 x 64)       | 16384              | 1177.011                           | 235.402                          | 8.7                         |  8.7                        |
| 5                | 128 point<br/>(64 x 128)     | 65536              | 1812.389                           | 362.478                          | 22.6                        |  22.6                       |
| 5                | 256 point<br/>(128 x 256)    | 262144             | 2340.571                           | 468.114                          | 70.0                        |  70.0                       |
| 5                | 512 point<br/>(256 x 512)    | 1048576            | 2797.098                           | 559.420                          | 234.3                       |  234.3                      |
| 5                | 2048 point<br/>(1024 x 2048) | 16777216           | 2490.324                           | 498.065                          | 4210.6                      |  4210.6                     |
| 10               | 64 point<br/>(32 x 64)       | 16384              | 2327.273                           | 232.727                          | 8.8                         |  8.8                        |
| 10               | 128 point<br/>(64 x 128)     | 65536              | 3175.194                           | 317.519                          | 25.8                        |  25.8                       |
| 10               | 256 point<br/>(128 x 256)    | 262144             | 4920.120                           | 492.012                          | 66.6                        |  66.6                       |
| 10               | 512 point<br/>(256 x 512)    | 1048576            | 5594.195                           | 559.420                          | 234.3                       |  234.3                      |
| 10               | 2048 point<br/>(1024 x 2048) | 16777216           | 6012.822                           | 601.282                          | 3487.8                      |  3487.8                     |


</details>

<details>
<summary>Performance per Watt</summary> 

### Performance per Watt

Performance per Watt is represented as throughput in MSPS/power in Watts. The following example shows the calculation for the 1024 x 2048 point 1-instance design:

```
Performance per Watt = Throughput(MSPS) / Power(Watt)
                     = (498.065 / 0.950) MSPS/Watt
                     = 524.279 MSPS/Watt
```


A summary of performance per Watt for all variations is shown in the following table.


| No. of Instances | FFT Configuration        | Performance per Watt (in MSPS/Watt) |
|:----------------:|:------------------------:|:-----------------------------------:|
| 1                | 64 point (32 x 64)       | 338.708                             |
| 1                | 128 point (64 x 128)     | 496.545                             |
| 1                | 256 point (128 x 256)    | 595.565                             |
| 1                | 512 point (256 x 512)    | 654.292                             |
| 1                | 2048 point (1024 x 2048) | 524.279                             |
| 5                | 64 point (32 x 64)       | 808.387                             |
| 5                | 128 point (64 x 128)     | 1078.803                            |
| 5                | 256 point (128 x 256)    | 1236.435                            |
| 5                | 512 point (256 x 512)    | 1231.659                            |
| 5                | 2048 point (1024 x 2048) | 895.478                             |
| 10               | 64 point (32 x 64)       | 982.385                             |
| 10               | 128 point (64 x 128)     | 1125.955                            |
| 10               | 256 point (128 x 256)    | 1490.042                            |
| 10               | 512 point (256 x 512)    | 1391.245                            |
| 10               | 2048 point (1024 x 2048) | 1183.393                            |

</details>

<details>
<summary>Consolidated Summary</summary> 

### Consolidated Summary

A consolidated summary of observations for all the point sizes and all the corresponding instance variations is shown in the following table.

| FFT Configuration - No. of Instances    | Aggregate Throughput<br/>(in MSPS) | Average Latency<br/>(in μs) | No. of Compute Cores | Vector Load | No. of Active Memory Banks | Mem R/W Rate | FF (Regs)| CLB LUTs | Dynamic Power<br/>(in mW) | Performance per Watt<br/>(in MSPS/Watt) |
|:---------------------------------------:|:----------------------------------:|:---------------------------:|:--------------------:|:-----------:|:--------------------------:|:------------:|:--------:|:--------:|:-------------------------:|:---------------------------------------:|
| 64 point<br/>(32 x 64)<br/> - x1        | 235.402                            | 8.7                         | 2                    | 8%          | 28                         | 4%           | 1172     | 413      | 695                       | 338.708                                 |
| 128 point<br/>(64 x 128)<br/> - x1      | 362.478                            | 22.6                        | 2                    | 17%         | 28                         | 7%           | 1172     | 413      | 730                       | 496.545                                 |
| 256 point<br/>(128 x 256)<br/> - x1     | 468.114                            | 70.0                        | 2                    | 32%         | 28                         | 13%          | 1172     | 413      | 786                       | 595.565                                 |
| 512 point<br/>(256 x 512)<br/> - x1     | 559.420                            | 234.3                       | 2                    | 50%         | 30                         | 18%          | 1172     | 415      | 855                       | 654.292                                 |
| 2048 point<br/>(1024 x 2048)<br/> - x1  | 498.065                            | 4210.6                      | 2                    | 76%         | 42                         | 19%          | 1172     | 413      | 950                       | 524.279                                 |
| 64 point<br/>(32 x 64)<br/> - x5        | 1177.011                           | 8.7                         | 10                   | 9%          | 132                        | 5%           | 5860     | 2079     | 1456                      | 808.387                                 |
| 128 point<br/>(64 x 128)<br/> - x5      | 1812.389                           | 22.6                        | 10                   | 18%         | 132                        | 8%           | 5860     | 2079     | 1680                      | 1078.803                                |
| 256 point<br/>(128 x 256)<br/> - x5     | 2340.571                           | 70.0                        | 10                   | 32%         | 132                        | 13%          | 5860     | 2079     | 1893                      | 1236.435                                |
| 512 point<br/>(256 x 512)<br/> - x5     | 2797.098                           | 234.3                       | 10                   | 50%         | 142                        | 19%          | 5860     | 2079     | 2271                      | 1231.659                                |
| 2048 point<br/>(1024 x 2048)<br/> - x5  | 2490.324                           | 4210.6                      | 10                   | 76%         | 202                        | 19%          | 5860     | 2091     | 2781                      | 895.478                                 |
| 64 point<br/>(32 x 64)<br/> - x10       | 2327.273                           | 8.8                         | 20                   | 8%          | 264                        | 4%           | 11720    | 4154     | 2369                      | 982.385                                 |
| 128 point<br/>(64 x 128)<br/> - x10     | 3175.194                           | 25.8                        | 20                   | 18%         | 264                        | 8%           | 11720    | 4154     | 2820                      | 1125.955                                |
| 256 point<br/>(128 x 256)<br/> - x10    | 4920.120                           | 66.6                        | 20                   | 32%         | 272                        | 13%          | 11720    | 4154     | 3302                      | 1490.042                                |
| 512 point<br/>(256 x 512)<br/> - x10    | 5594.195                           | 234.3                       | 20                   | 50%         | 284                        | 19%          | 11720    | 4150     | 4021                      | 1391.245                                |
| 2048 point<br/>(1024 x 2048)<br/> - x10 | 6012.822                           | 3487.8                      | 20                   | 76%         | 396                        | 20%          | 11720    | 4154     | 5081                      | 1183.393                                |

From these observations it can be seen that with the increase in the FFT point size, the window buffer size used in the AI Engines increases, and with that the throughput increases as well. By increasing the `TP_WINDOW_VSIZE` parameter in the FFT AI Engine graph, the throughput can be further increased, especially for the lower point sizes, but the AI Engine mapper/router could encounter issues due to the higher memory requirement.

Furthermore, the FFT point sizes increase, the power does _not_ increase proportionately, so the performance per Watt maintains an increasing trend in the beginning and saturates towards the end.

</details>

# Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )


Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"> XD073 | &copy; Copyright 2021 Xilinx, Inc.</p>
