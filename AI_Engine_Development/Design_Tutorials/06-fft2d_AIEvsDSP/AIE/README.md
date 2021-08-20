<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2021.1 Versal™ 2D-FFT Tutorial (AI Engine Implementation)</h1>
   </td>
 </tr>
</table>

# Table of Contents
[Building the Design](#Building-the-Design)

[Hardware Design Details](#Hardware-Design-Details)

[Software Design Details](#Software-Design-Details)

[Resource Utilization & Power, Throughput & Latency and Performance per Watt](#Resource-Utilization--Power-Throughput--Latency-and-Performance-per-Watt)

# Building the Design

<details>
<summary>Design Build</summary> 

## Design Build
In this section, you will build and run the 2D-FFT design using the AI Engine implementation. You will compile the AI Engine design and integrate it into a larger system design (including the Programmable Logic (PL) kernels and Processing System (PS) host application). You can review [Integrating the Application Section in the AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/yii1603912637443.html) for the general flow. 

At the end of this section, the design flow will generate a new directory (called `build/`). Underneath are sub-directories named `fft2d_$(MAT_ROWS)x$(MAT_COLS)/x$(FFT_2D_INSTS)/` (for example, fft2d_1024x2048/x1/) depending on value of matrix dimensions `${MAT_ROWS}`, `${MAT_COLS}` and number of instances `$(FFT_2D_INSTS)` chosen in the build. Each sub-directory contains the `hw_emu/`, and/or `hw/` subfolders. The respective subfolders contain `Work/` and libadf.a, outputs from the AI Engine compiler, host app executable and the builds, targeted to `hw` or `hw_emu` respectively. The `hw_emu/` subfolder contains the build for hardware emulation. The `hw/` subfolder contains the build for hardware run on a VCK190 board.

</details>

<details>
<summary>Make Steps</summary> 

## Make Steps
To run the following `make` steps (e.g. `make kernels`, `make graph`, etc), you must be in the `AIE/` folder.\
Below are the `options` that can be specified in the make steps, see the make steps below for instructions how to apply them

`TARGET:` It can be set to hw or hw_emu to build the design in hardware or hardware emulation flow. Default is `hw_emu`

`FFT_2D_INSTS:` It can be set to 1, 5, or 10 to build the design with the number of kernel instances. Default is `1`

`ITER_CNT:` Number of iterations the design is run. Default is `8`

`FFT_2D_PT`: FFT 2D Point. Permissible values `64, 128, 256, 512 and 2048`

Dimensions of the Matrix:\
`MAT_ROWS x MAT_COLS:` Automatically configured as `FFT_2D_PT/2, FFT_2D_PT` -\
Number of Rows in the Input Matrix x Number of Cols in the Input Matrix,\
permissible values are `32x64, 64x128, 128x256, 256x512 and 1024x2048`. Default is `1024x2048`

`EN_TRACE:` Flag to enable trace-profiling. 0 is disabled and 1 is enabled. Default is `0 (disabled)`

The Makefile uses the below directory references:
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

This command will run the `make kernels`,`make graph`,`make xclbin`,`make application`,`make package` and `make run_emu` for hardware emulation or to run on hardware (VCK190 board) depending on the `TARGET` you specify. The settings also apply to individual make steps listed below.

**Note**

1) The generated files for each `FFT_2D_INSTS` are placed under invididual directory: `$(BUILD_TARGET_DIR)/`
2) See the specification in each make step below for options used and location of input and output files.

</details>

### The individual make steps to build the design with the options that applied to them are specified below

<details>
<summary>make kernels: Compile PL Kernels</summary> 

## make kernels: Compile PL Kernels
In this step, the Vitis compiler takes any V++ kernels (RTL or HLS C) in the PL region of the target platform (`xilinx_vck190_base_202110_1`) and the AI Engine kernels and graph and compiles them into their respective XO files. 

The following commands compiles the kernels (default TARGET=hw_emu, FFT_2D_INSTS=1, ITER_CNT=8 and FFT_2D_PT=2048). 

```
make kernels
```

The expanded command is as follows:

for dma_hls
```
mkdir -p $(BUILD_TARGET_DIR); \

cd $(BUILD_TARGET_DIR); \

v++ --target hw_emu --hls.clock 250000000:dma_hls --platform xilinx_vck190_base_202110_1 \
   --save-temps --temp_dir $(BUILD_TARGET_DIR)/_x --verbose -g -c -k dma_hls \
   $(DESIGN_REPO)/pl_src/dma_hls.cpp -o $(BUILD_TARGET_DIR)/dma_hls.hw_emu.xo
```
[Detailed Desicription of All Vitis Compiler Switches](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitiscommandcompiler.html#wrj1504034328013)\
Summary of the Switches used:
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
|$(PL_SRC_REPO)/dma_hls.cpp|Defines the data-mover PL kernel.|

|Output|Description|
|  ---  |  ---  |
|$(BUILD_TARGET_DIR)/dma_hls.hw_emu.xo|The data-mover kernel object file.|

</details>

<details>
<summary>make graph: Creating the AI Engine ADF Graph for Vitis Compiler Flow</summary> 

## make graph: Creating the AI Engine ADF Graph for Vitis Compiler Flow

An ADF graph can be connected to an extensible Vitis platform (the graph I/Os can be connected either to platform ports or to ports on Vitis kernels through Vitis compiler connectivity directives). 
* The AI Engine ADF C++ graph of the design contains AI Engine kernels and PL kernels. 
* All interconnects between kernels are defined in the C++ graph
* All interconnections to external I/O are fully specified in the C++ simulation testbench (`graph.cpp`) that instantiates the C++ ADF graph object. 

To compile the graph using the Makefile flow type (default FFT_2D_INSTS=1, ITER_CNT=8, FFT_2D_PT=2048) :
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
[AIE Programming Environment Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vjv1611769393894.html)
|Switch|Description|
|  ---  |  ---  |
|--include=\<string\>|Specify compile-time include directory (zero or more).|
|--verbose\|-v|Verbose output of the AI Engine compiler emits compiler messages at various stages of compilation. These debug and tracing logs provide useful messages on the compilation process.|
|--Xpreproc="-D\<Pre-processor Macro String\>"|Specify compile time Macro.|
|--Xchess="\<Chess Make Options\>"|Specify compile time chess Make Options, "main:bridge.llibs=softfloat m" enables floating point operations.|
|--heapsize=\<int\>|Heapsize in bytes.|
|--log-level=\<int\>|Log level for verbose logging (default=1).|
|--workdir=\<string\>|By default, the compiler writes all outputs to a sub-directory of the current directory, called Work. Use this option to specify a different output directory.|

The following is a description of the output objects that results from executing the AI Engine compiler (`aiecompiler`) command

|Inputs Sources|Description|
|  ---  |  ---  |
|$(AIE_SRC_REPO)/graph.cpp|Defines the rowise and colwise fft graph objects.|

|Output Objects|Description|
|  ---  |  ---  |
|$(BUILD_TARGET_DIR)/libadf.a|Compiled AI Engine design graph|
|$(BUILD_TARGET_DIR)/Work/|Directory that contains all outputs of the AI Engine compiler.|
</details>

<details>
<summary>make xclbin: Use Vitis Tools to Link AI Engine and HLS Kernels with the Platform</summary> 

## make xclbin: Use Vitis Tools to Link AI Engine and HLS Kernels with the Platform
After the AI Engine kernels and graph and PL HLS kernels have been compiled, you can use the Vitis compiler to link them with the platform to generate a XCLBIN file. 

The Vitis tools allow you to integrate the AI Engine, HLS, and RTL kernels into an existing extensible platform. This is an automated step from a software developer perspective where the platform chosen is provided by the hardware designer (or you can opt to use one of the many extensible base platforms provided by Xilinx and the Vitis tools build the hardware design and integrate the AI Engine and PL kernels into the design).
 
To test this feature in this tutorial, use the base VCK190 platform to build the design.
 
The command to run this step is shown as follows (default TARGET=hw_emu, FFT_2D_INSTS=1, ITER_CNT=8, EN_TRACE=0, FFT_2D_PT=2048 ):
```
make xclbin
``` 

The expanded command is as follow: 
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

If EN_TRACE is enabled, the following v++ flags are also set
```
   --profile.data dma_hls:all:all or profile.data dma_hls:all:strmInp_from_colwiseFFT (for higher instances) \
   --profile.trace_memory DDR

```
For higher values of FFT_2D_INSTS, only the `strmInp_from_colwiseFFT` port is profiled to avoid too much data.

[Detailed Description of Vitis Linking Options](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/buildingdevicebinary.html#mjs1528399150499)\
|Switch|Description|
|  ---  |  ---  |
|--platform \| -f|Specifies the name of a supported acceleration platform as specified by the $PLATFORM_REPO_PATHS environment variable or the full path to the platform XPFM file.|
|--save-temps \| -s|Directs the V++ command to save intermediate files/directories created during the compilation and link process. Use the `--temp_dir` option to specify a location to write the intermediate files to.|
|--temp_dir <string>|This allows you to manage the location where the tool writes temporary files created during the build process. The temporary results are written by the Vitis compiler, and then removed, unless the `--save-temps` option is also specified.|
|--verbose|Display verbose/debug information.|
|--config <config_file>|Specifies a configuration file containing V++ switches.|
|--output \| -o|Specifies the name of the output file generated by the V++ command. In this design the outputs of the DMA HLS kernels and the PL kernels interfacing with the AI Engine are in XO files.|
|--profile.data [<kernel_name>\|all]:[<cu_name>\|all]:[<interface_name>\|all]\(:[counters\|all]\)|Enables monitoring of data ports through the monitor IPs. This option needs to be specified during linking. [Detailed Profiling Options](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitiscommandcompiler.html#lpy1600804966354)|
|--profile.trace_memory \<FIFO\>:\<size\>\|\<MEMORY\>[\<n\>]|When building the hardware target \(-t=hw\), use this option to specify the type and amount of memory to use for capturing trace data. [Detailed Profiling Options](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitiscommandcompiler.html#lpy1600804966354)|

The information to tell the linker how to connect the AI Engine and PL kernels together is described in a configuration file `system_configs/x$(FFT_2D_INSTS).cfg`. The file describes the overall connection scheme of the system.

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

[Detailed Description of Vitis Compiler Configuration File](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitiscommandcompiler.html?hl=--config#pni1524163195211)\
|Switch|Comment|
|  ---  |  ---  |
|--connectivity.nk|Number of kernels. `dma_hls:1:dma_hls_0` means that the Vitis compiler should instantiate one dma_hls kernel and name the instance `dma_hls_0`.|
|--connectivity.stream_connect|How the kernels will connect to IPs, platforms, or other kernels. The output of the AI Engine compiler tell you the interfaces that need to be connected. `dma_hls_0.strmOut_to_rowiseFFT:ai_engine_0.DataIn0` means that the Vitis compiler should connect the port `strmOut_to_rowiseFFT` of `dma_hls` pl-kernel to the shim channel of AI Engine with logical name `DataIn0`, defined in `$(AIE_SRC_REPO)/graph.cpp` as part of PLIO instantiation.|
|param=compiler.addOutputTypes=hw_export| This option tells the Vitis compiler that besides creating an XCLBIN file, it also outputs an XSA file which is needed to create a post-Vivado fixed platform for Vitis software developement.|

Note that the Vitis compiler calls Vivado® IP integrator under the hood to build the design. The platform and kernels are input to the Vivado Design Suite, which produces a simulation XSA or an XSA after running place and route on the design. The point at which the XSA is produced from Vivado is dependent on what `-target` option is set on the the Vitis compiler command line. 

Note that you can now view the Vivado project, which is located in the `$(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj` directory.

Now you have generated the XCLBIN file that will be used to execute your design on the platform.

</details>

<details>
<summary>make application: Compile the Host Application</summary> 

## make application: Compile the Host Application
You can compile the host application by following the typical cross-compilation flow for the Cortex-A72. To build the application run the following command (default FFT_2D_INSTS=1, ITER_CNT=8, FFT_2D_PT=2048 ):
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
[XRT Documentation](https://xilinx.github.io/XRT/2021.1/html/index.html)\
[Details of Host Application Programming](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/devhostapp.html#vpy1519742402284)
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
<summary>make package: Package the Design</summary> 

## make package: Package the Design
With the AI Engine outputs created, as well as the new platform, you can now generate the Programmable Device Image (PDI) and a package to be used on an SD card. The PDI contains all executables, bitstreams, configurations of the device. The packaged SD card directory contains everything to boot Linux, the generated applications and `.xclbin`.

The command to run this step is as follows (default TARGET=hw_emu, EN_TRACE=0, FFT_2D_INSTS=1, FFT_2D_PT=2048 ):
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

If EN_TRACE is enabled, the following v++ flags are also set
```
   --package.sd_file $(PROFILING_CONFIGS_REPO)/xrt.ini
```

If XRT_ROOT is set, the following v++ flags are also set
```
   --package.sd_dir $(XRT_ROOT)
```
[Details of Packaging the System](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/packagesystem1.html#cwq1586366344968)
|Switch|Description|
|  ---  |  ---  |
|--target \| -t [hw\|hw_emu]|Specifies the build target.|
|--package \| -p|Packages the final product at the end of the Vitis compile and link build process.|
|--package.rootfs \<arg\>|Where \<arg\> specifies the absolute or relative path to a processed Linux root file system file. The platform RootFS file is available for download from xilinx.com. Refer to the Vitis Software Platform Installation for more information.|
|--package.kernel_image \<arg\>|Where \<arg\> specifies the absolute or relative path to a Linux kernel image file. Overrides the existing image available in the platform. The platform image file is available for download from xilinx.com. Refer to the Vitis Software Platform Installation for more information.|
|--package.boot_mode \<arg\>|Where \<arg\> specifies <ospi\|qspi\|sd> Boot mode used for running the application in emulation or on hardware.|
|--package.image_format|Where \<arg\> specifies \<ext4\|fat32\> output image file format. `ext4`: Linux file system and `fat32`: Windows file system|
|--package.sd_file|Where \<arg\> specifies an ELF or other data file to package into the `sd_card` directory/image. This option can be used repeatedly to specify multiple files to add to the `sd_card`.|
|--package.defer_aie_run| Load the AI Engine application with the ELF file, but wait to run it until graph run directs it. Required in PS based AI Engine flow.|

|Inputs Sources|Description|
|  ---  |  ---  |
|$(PLATFORM_REPO_PATHS)/sw/versal/xrt|The PS Host Application needs the XRT headers in this folder to execute.|
|$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/rootfs.ext4|The Root Filesystem file for Petalinux.|
|$(PLATFORM_REPO_PATHS)/sw/versal/xilinx-versal/Image|The pre-built Petalinux Image the processor boots from.|
|$(BUILD_TARGET_DIR)/fft_2d_aie_xrt.elf|The PS Host Application executabled created in the `make application` step.|
|$(BUILD_TARGET_DIR)/vck190_aie_fft_2d.hw_emu.xclbin|The XCLBIN file created in the `make xclbin` step.|
|$(BUILD_TARGET_DIR)/libadf.a|The compiled AI Engine design graph created in the `make graph` step.|

The output of the V++ Package step is the package directory that contains the contents to run hardware emulation. 

|Output Objects|Description|
|  ---  |  ---  |
|$(BUILD_TARGET_DIR)/package|The hardware emulation package that contains the boot file, hardware emulation launch script, the PLM and PMC boot files, the PMC and QEMU command argument specification files, and the Vivado simulation folder.|

</details>

<details>
<summary>make run_emu: Run Hardware Emulation</summary>

## make run_emu: Run Hardware Emulation
After packaging, everything is set to run emulation or hardware. 
To run emulation use the following command ( default TARGET=hw_emu ):
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
When launched, you will see the QEMU simulator load. Wait for the autoboot countdown to go to zero, and after a few minutes, you will see the root Linux prompt come up: 
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
The `fft_2d_aie_xrt.elf` should execute, and after a few minutes, you should see the output with *TEST PASSED* on the console. When this is shown, run the following keyboard command to exit the QEMU instance: 

```
#To exit QEMU Simulation
Press CtrlA, let go of the keyboard, and then press x 
```

To run with waveform do the following:
```
cd $(BUILD_TARGET_DIR)/package
./launch_hw_emu.sh -g
```
The XSIM Waveform Viewer is launched. Drag and drop the signals into the Viewer and click Play to start the emulation. Go back to the terminal and wait for the Linux prompt to show up. 

In the XSIM Waveform Viewer, you will see the signals you added to the waveform adjusting over the execution of the design. Once done, hit the pause button and close the window to end the emulation.\
\
Waveform View of the 32x64 - 1x design:\
![Image of 2D-FFT AIE HW_EMU run Waveform View For 32x64-1x Design](images/fft_2d_aie_hw_emu_waveform_view_32x64_x1.PNG)

</details>

<details>
<summary>TARGET=hw: Run on Hardware</summary>

## Run on Hardware

To run the design in hardware, re-run the following "make" steps with TARGET=hw and other applicable options (see the make steps above)
```
make kernels TARGET=hw
make xclbin TARGET=hw 
make package TARGET=hw 
```
These command create a `$(BUILD_TARGET_DIR)` folder with the kernels, `xclbin`, and `package` for a hardware run. 

Then run the following step to setup the execution file; the generated images and base images (`$(BUILD_TARGET_DIR)/package/sd_card` and `$(BUILD_TARGET_DIR)/package/sd_card.img`)
```
make run_emu TARGET=hw 
```
These command create a `build/hw` folder with the kernels, `xclbin`, and `package` for a hardware run. 

Now follow **Steps 1-9** to run the `fft_2d_aie_xrt.elf` excutable on your VCK190 board. 

**Step 1.** Ensure your board is powered off. 

**Step 2.** Use an SD card writer (such as balenaEtcher) to flash the `sd_card.img` file an SD card. 

**Step 3.** Plug the flashed SD card into the top slot of the VCK190 board. 

**Step 4.** Set the switch SW1 Mode\[3:0\]=1110 = OFF OFF OFF ON

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

**Step 7.** Power on the board.

**Step 8.** Wait until you see the `root@versal-rootfs-common-2021_1` Linux command prompt. Press enter a few times to get past any `xinit` errors. 

**Step 9.** Run the following commands into the TeraTerm terminal: 
```
cd /mnt/sd-mmcblk0p1
export XILINX_XRT=/usr
./init.sh

./fft_2d_aie_xrt.elf a.xclbin
```

</details>

# Hardware Design Details
<details>
<summary>2D-FFT AI Engine Implementation architecture and  AI Engine/PL Function Partitioning</summary>

## 2D-FFT AI Engine Implementation architecture and  AI Engine/PL Function Partitioning
The following figure shows a high level block diagram of the design. The test harness consists of the AI Engine and data-mover-HLS kernels(dma_hls). In this setup, the interface between the data-mover kernels and AI Engine cores is AXI4-stream, with the data width of 128bit and the data-mover kernels and the AI Engine array interface are running at 250MHz.
Datamover is pl-based data generator and checker, it generates impulse input and checks the output of rowise-fft core for response of the same, and subsequently generates the transposed pattern of the rowise fft output and feeds that to the colwise fft core and checks the output of the same.

![Image of 2D-FFT AIE Implementation Architecture](images/fft_2d_aie_block_diagram.PNG)

</details>

<details>
<summary>Design Details</summary>

## Design Details
The design in this tutorial starts with a base platform containing the Control Interface and Processing System (CIPS), NoC, and AI Engine and the interfaces among them. The v++ linker step builds on top of the base platform by adding the AI Engine graphs and PL kernels. To add the various functions in a system level design, PL kernels are added to the base platform depending on the application, that is, the PL kernels present in each design may vary. An ADF graph is connected to an extensible Vitis platform where the graph I/Os are connected either to the platform ports or to ports on Vitis kernels through the the Vitis compiler connectivity directives. In the design, the components are added by v++ -l step (make XCLBIN in the tool flow section above) and include the following:
* `libadf.a`
* data-mover kernel (`dma_hls.[hw|hw_emu].xo`)
* connections interfaces defined in system configuration file

To see a schematic view of the design with the extended platform as shown in the following figure, open in Vivado 

`build/fft2d_$(MAT_ROWS)x$(MAT_COLS)/x$(FFT_2D_INSTS)/[hw|hw_emu]/_x/link/vivado/vpl/prj/prj.xpr`

![Image of 2D-FFT AIE 1x Vivado BD](images/fft_2d_aie_1x_vivado_bd.PNG)

In this design, the 2D FFT computation happens in two stages: the first compute is across the row vectors and the second stage is performed across the column vectors. The input data is accessed linearly and streamed to the AI Engines which perform MAT_COLS( default 2048 ) point FFT. The data coming out of the AI Engines is streamed to a PL Kernels where it is checked against the expected pattern(First row should be 1 remaining should be 0), mismatch if any is recorded in variable stage0_errCnt. Transposed pattern of the output of the row vectors is then linearly streamed into another AI Engine which performs MAT_ROWS( default 1024 ) point FFT. The output is streamed into data-mover kernel again and is checked against expected pattern(All values should be 1), mismatch if any is stored in variable stage1_errCnt. Finally the sum of stage0_errCnt and stage1_errCnt is returned from the kernel, which is read in the host app to determine whether the test has passed or failed.

Notice the system debugging and profiling IP (DPA) is added to the PL region of the device to capture AI Engine run-time trace data if the EN_TRACE option is enabled in the design. The dma_hls kernel and the AI Engine Array Interface are both operating at 250 MHz and unlike the DSP implementation there is no clock domain crossing in the PL region in this design.

</details>

<details>
<summary>AI Engine and PL Kernels</summary>

## AI Engine and PL Kernels
The top level AI Engine graph graph.cpp contains 2 subgraphs, `FFTrows_graph` and `FFTcols_graph`. Each subgraph contains the individual AI Engine kernel, `*FFTrow_gr.getKernels()` and `*FFTcol_gr.getKernels()` which performs `MAT_COLS` and `MAT_ROWS` point FFT respectively.

The PL-based data-movers consist of dma_hls kernel, which generates impulse input and check the output of each fft stage for the expected pattern.

**DMA_HLS**
* Internally comprises of 4 loops mm2s0, s2mm0 , mm2s1 and s2mm1. With s2mm0 - mm2s1 sequenced one after the other and wrapped into dmaHls_rowsToCols function and, mm2s0, dmaHls_rowsToCols and s2mm1 concurrently scheduled.
* The data width is 128 bits at both the AXI4-stream IO side.
* Working at 250 Mhz.

</details>

# Software Design Details
The software design in the AIE 2D-FFT tutorial consists of the following sections:

<details>
<summary>Methodology</summary>

## Methodology
The following figure elaborates on the AIE Implementation Methodology.

![Image of 2D-FFT AIE Implementation Methodology](images/fft_2d_aie_block_diagram_methodology.PNG)

### AIE
* **Independent Cores** \
Both AIE graphs for `FFTrows_graph` and `FFTcols_graph` are to be configured to be independent, with runtime ratios set to >= 0.6. So that each can be run independently of each other.
```
...
runtime<ratio>(*FFTrow_gr.getKernels()) = 0.6;
...
runtime<ratio>(*FFTcol_gr.getKernels()) = 0.6;
...
```

* **Window Streaming Buffer Config** \
Graph `FFTrows_graph` does `MAT_COLS` point FFT and runs for `MAT_ROWS` number of Iterations. But for graph `FFTcols_graph` increase the `TP_WINDOW_VSIZE` to `MAT_COLS` instead of `MAT_ROWS` and it does `MAT_ROWS` point FFT, but runs for `MAT_ROWS` number of Iterations instead of `MAT_COLS`. This reduces the ping-pong overhead which improves the overall throughput. \
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

### Data-Mover
* **Data Generation/Checking and Sequencing** \
The data-mover comprises of 4 loops `mm2s0, s2mm0, mm2s1 and s2mm1`.\
`s2mm0 and mm2s1` are wrapped into a single function `dmaHls_rowsToCols`. Within that the execution sequence is `s2mm0` followed by `mm2s1`. `s2mm0 and s2mm1` functions check the output of `rowise and colwise fft` respectively, against expected golden.

* **Concurrent Scheduling** \
The concurrent scheduling is required so that each function is independently running and the execution of one function is not blocking the other.\
The concurrent scheduling of the 3 functions `mm2s0, dmaHls_rowsToCols and s2mm1`, done as below using `#pragma HLS DATAFLOW`.
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

* **Vitis HLS Scheduling and Dataflow View**
![Image of Datamover Scheduler View](images/dma_hls_scheduler_view.PNG)
![Image of Datamover Dataflow View](images/dma_hls_dataflow_view.PNG)

### Streaming Interface Data-width
Streaming Interface Data-width is kept as `128Bit` to reduce Read/Write overhead while processing data.

### Frequency Selection
As In the AI Engine Implementation AIE Kernels are configured for `cint16 / 4bytes` and the streaming interface is at `128bit / 16bytes`, frequency of the AIE array is at 1000Mhz and the data-mover is kept at 250Mhz, maintaining a 1:4 ratio.

</details>

<details>
<summary>AI Engine Kernels and Graph Representation</summary>

## AI Engine Kernels and Graph Representation
An AI Engine kernel is a C/C++ program written using specialized intrinsic calls that target the VLIW vector processor. The AI Engine compiler compiles the kernel code to produce an executable ELF file for each of the AI Engines being used in the design. Review [AI Engine Kernel Programming Section in the AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/yii1603912637443.html) for a high-level overview of kernel programming. These kernels can be stitched together to function as AI Engine graphs written in C++. In this design, the AI Engine compiler writes a summary of compilation results $(BUILD_TARGET_DIR)/Work/graph.aiecompile_summary. You can view the graph by running the following command:

`vitis_analyzer $(BUILD_TARGET_DIR)/Work/graph.aiecompile_summary`

The following figures show the graph representation of the AI Engine kernels (default fft 2048 point and fft 1024 point, FFT_2D_INSTS=1). Note in addition to the compute units there are also the twiddle factor LUTs (fft_lut_tw*) and temporary buffers for FFT stages (fft_2048/1024_tmp*)

![Image of 2D-FFT AI Engine 2K point Graph](images/fft_2d_aie_fft_rows_graph_for_2kpt_1x.PNG)
![Image of 2D-FFT AI Engine 1K point Graph](images/fft_2d_aie_fft_cols_graph_for_1kpt_1x.PNG)

</details>

<details>
<summary>Adaptive Data Flow Graph (ADF Graph)</summary>

## Adaptive Data Flow Graph (ADF Graph)

This section describes the overall data-flow graph specification of the 2D-FFT design using AI Engine which is compiled by the AI Engine compiler. Refer to [AI Engine Programming Section in the AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/yii1603912637443.html) for information on ADF graphs.

The overall graph definition of the design is contained in the `graph.cpp` file. The top level graph in contains of 2 subgraphs, `FFTrows_graph` and `FFTcols_graph`, each with `FFT_2D_INSTS` number of objects. The following describes the definition of the subgraphs (the FFTrows_graph is used as illustration).

### Define the graph class
Define the graph classes by using the objects defined in the appropriate name space. It must include the Adaptive Data Flow (ADF) library and [Vitis DSP Library](https://xilinx.github.io/Vitis_Libraries/dsp/2021.1/user_guide/L2/2-dsp-lib-func.html#fft-ifft) for fft. A general specification is put in for adf namespace:
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
All user graphs are defined from the class graph, for example in the FFTrows_graph design:

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

### Top level Application
Define a top level application file (`graph.cpp` in this design) that contains an instance of the graph class and connect the graph to a simulation platform to provide file input and output (in the case of FFT2D_INSTS = 1 to the two subgraphs):

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

The main function is called under the guard bounds of \_\_AIESIM\_\_ like below, to avoid conflict with "main" function in the host application:

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
<summary>PL Data-Mover Kernel</summary>

## PL Data-Mover Kernel

In addition to the kernels operating in the AI Engine array, this design specifies a data-mover kernel to run in the PL region of the device (written in HLS C++). The data-mover kernel is brought into the design during the Vitis kernel compilation. Which is further replicated based on `FFT_2D_INSTS` value. The software design of the data-mover kernel is described below:

### dma_hls (dma_hls.cpp)
The `dma_hls` kernel reads data from a Memory Mapped AXI4 (MM-AXI4) interface and writes it to an AXI4-Stream Interface

#### Top Function Declaration
The `dma_hls` kernel takes the following arguments, as shown in the :
```
int dma_hls(
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_rowiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_rowiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmOut_to_colwiseFFT,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmInp_from_colwiseFFT,
      int matSz, int rows, int cols, int iterCnt
     );
```
* `ap_int<N>` is an arbitrary precision integer data type defined in `ap_int.h` where `N` is a bit-size from 1-1024. In this design, the bit-size is set to 128.
* `hls::stream<qdma_axis<D,0,0,0>>` is a data type defined in `ap_axi_sdata.h`. It is a special data class used for data transfer when using a streaming platform. The parameter `<D>` is the data width of the streaming interface which is set to 128. The remaining three parameters should be set to 0.

#### Top Function Definition
Use `dataflow` pragma for concurrently scheduling the three functions `mm2s0, dmaHls_rowsToCols and s2mm1`
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

   ap_uint<64> golden64 = 0x0000000100000001;
   goldenVal.range(127, 64) = golden64.range();
   goldenVal.range( 63,  0) = golden64.range();
   

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
The dma_hls kernel also specifies hls pragmas to help optimize the kernel code and adhere to interface protocols.

[Detailed Documentation of All HLS
Pragmas](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/hls_pragmas.html?hl=hls%2Cpragmas)\
Summary of hls pragmas used in the kernel:
|Switch|Description|
|  ---  |  ---  |
|#pragma HLS INTERFACE|In C/C++ code, all input and output operations are performed, in zero time, through formal function arguments. In a RTL design, these same input and output operations must be performed through a port in the design interface and typically operate using a specific input/output (I/O) protocol. For more information, see Defining Interfaces. [Detailed Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/hls_pragmas.html#jit1504034365862)|
|#pragma HLS PIPELINE II=1|Reduces the initiation interval (II) for a function or loop by allowing the concurrent execution of operations. The default type of pipeline is defined by the config_compile -pipeline_style command, but can be overridden in the PIPELINE pragma or directive. [Detailed Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/hls_pragmas.html#fde1504034360078)|
|#pragma HLS dataflow|The DATAFLOW pragma enables task-level pipelining as described in Exploiting Task Level Parallelism: Dataflow Optimization, allowing functions and loops to overlap in their operation, increasing the concurrency of the RTL implementation and increasing the overall throughput of the design. [Detailed Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/hls_pragmas.html#sxx1504034358866)|
|#pragma HLS loop_tripcount|When manually applied to a loop, specifies the total number of iterations performed by a loop. The `LOOP_TRIPCOUNT` pragma or directive is for analysis only, and does not impact the results of synthesis. [Detailed Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/hls_pragmas.html#sty1504034367099)|
 
</details>

<details>
<summary>PS Host Application</summary>

## PS Host Application
The 2D-FFT AIE tutorial uses the Embedded processing system (PS) as an external controller to control the AI Engine graph and data-mover PL kernels. Review [Programming the PS Host Application Section in the AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/yii1603912637443.html) to understand the process to create a host application.

In addition to the PS host application (`fft_2d_aie_app.cpp`), the AI Engine control code must also be compiled. This control code (`aie_control_xrt.cpp`) is generated by the AI Engine compiler when compiling the AI Engine design graph and kernel code.

The AI Engine control code is used by the PS host application for the following reasons:
* Control the initial loading of the AI Engine kernels
* Run the graph for several iterations, update the run time parameters associated with the graph, exit, and reset the AI Engine tiles.

The steps in the tutorial to run the A72 application are described as follows:

### 1. Include graph.cpp, other required headers and define required Macros
Include the `graph.cpp` AI Engine application file. This file contains the instantiation of the AI Engine 2D-FFT data flow graph object
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
### 2. Check Command Line Argument
The beginning of the A72 application is represented by the main function. It takes in one command line argument: an XCLBIN file.
`int main(int argc, char** argv)`

### 3. Open XCLBIN and Create Data Mover Kernel Handles
The A72 application loads the XCLBIN binary file and creates the data-mover kernels to be executed on the device. The steps are:

* Open device and load xclbin
```
auto dhdl = xrtDeviceOpen(0);
auto xclbin = load_xclbin(dhdl, xclbinFilename);
auto top = reinterpret_cast<const axlf*>(xclbin.data());
```
* Open data-mover kernel and obtain handles to start HLS PL kernels

for dma_hls PL kernel
```
xrtKernelHandle dma_hls_khdl;
xrtRunHandle dma_hls_rhdl;

// Open kernel handle exclusively to read the ap_return register later for reporting error...
dma_hls_khdl = xrtPLKernelOpenExclusive(dhdl, top->m_header.uuid, dma_hls_obj);
dma_hls_rhdl = xrtRunOpen(dma_hls_khdl);
```
### 4. Open Graph, Obtain Handle and Execute Graph
* The A72 processor opens and obtains its handle using the ` xrtGraphOpen` function.
* The A72 processor resets the graph using the `xrtGraphReset` function and runs the graph execution using the `xrtGraphRun` function for both the 2K point and 1K point subgraphs.

### 5. Execute the Data Mover Kernels and Generate the Output Results
* Set the `dma_hls` kernel arguments using `xrtRunSetArg` function.
* Start the `dma_hls` kernels using `xrtRunStart` function.
* Wait for `dma_hls` execution to finish using `xrtRunWait` runction.

### 6. Verify Output Results
By reading the `ap_return` in `$(BUILD_TARGET_DIR)/_x/dma_hls.$(TARGET)/dma_hls/dma_hls/ip/drivers/dma_hls_v1_0/src/xdma_hls_hw.h` \
Using the `xrtKernelRegister` API, as shown below:
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

### 7. Release Allocated Resources
After post-processing the data, release the allocated objects and handles using `xrtRunClose`, `xrtKernelClose`, `xrtGraphClose` and `xrtDeviceClose` functions.

</details>

# Resource Utilization & Power, Throughput & Latency and Performance per Watt

For all applications, designers must work to predefined specifications and build a system for their specific deployment by meeting their system requirements with respect to their available resources, latency, throughput, performance and power. In this section, it is outlined how to measure those characteristics for the AI Engine implementation in this tutorial.

<details>
<summary>Resource Utilization & Power</summary> 

### Resource Utilization & Power
It is measured by using vivado, vcdanalyze and Xilinx Power Estimator(XPE) for versal(2020.3 Version) tools.\
The `Registers` and `CLB LUT` Utilization information can be found in Vivado project. Steps are below:
* Open the vivado proj $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr
* Open Implemented Design -> Click On Report Utilization
* In the below shown Utilization tab select `ai_engine_0` and view the `Registers and CLB LUTs` for  `1024 x 2048 point - 1 instance` design:

![Image of 2D-FFT AIE Utilization](images/fft_2d_aie_vivado_resources.PNG)

The vcdanalyze tool is used to generate `graph.xpe` file which can be input to XPE for viewing the AIE Resource Utilization and Power. Steps:
* Do `make vcd`(recipe expanded below), this will create the `graph.xpe` file under `$(BUILD_TARGET_DIR)/aiesim_xpe/`:
```
cd $(BUILD_TARGET_DIR); \
aiesimulator --pkg-dir $(WORK_DIR)/ --dump-vcd x$(FFT_2D_INSTS) 2>&1 | tee -a vcd.log
cd $(BUILD_TARGET_DIR); \
vcdanalyze --vcd x$(FFT_2D_INSTS).vcd --xpe
```
* Follow the steps below to load the `graph.xpe` into [Xilinx Power Estimator (XPE)](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2021_1/ug1275-xilinx-power-estimator-versal.pdf), to see the AI Engine Power Comsumption(Step 5 below) and Resource Utilization(Step 6) for  `1024 x 2048 point - 1 instance` design:\
[Download XPE for Versal Version 2020.3](https://www.xilinx.com/products/technology/power/xpe.html)

![Image of 2D-FFT AIE XPE Intro](images/fft_2d_aie_xpe_intro_step1_2and3.PNG)
![Image of 2D-FFT AIE XPE Util and Power Measurement](images/fft_2d_aie_xpe_Pow_nUtil_step4_5and6.PNG)

Summary of Resource Utilization & Power for all Variations:
| No. of Instances | FFT Configuration            | Number of Compute Cores | Vector load | Number of Active Memory Banks | Mem R/W Rate | Active AIE Tiles | Interconnect load | FF (Regs) | CLB LUTS  | Dynamic power<br/>(in mW) | 
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
<summary>Throughput & Latency</summary> 

### Throughput & Latency
Throughput is measured in Mega Samples Trasferred per Second (MSPS) and Latency is defined as the time between, when the first sample is sent by data-mover into `rowise fft` kernel and when the first sample from the `colwise fft` kernel is received by the data-mover. It is measured by viewing the runtime generated trace texts via vitis_analzer. Throughput and Latency measurement steps are below:

* Compile the design using `EN_TRACE=1` it automatically includes a xrt.ini file while packaging, which comprises of:
```
[Debug]
xrt_trace=true
data_transfer_trace=fine
trace_buffer_size=500M
```
[Refer to xrt.ini Documentation for more Information](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/xrtini.html#tpi1504034339424).

* After execution on the board transfer the generated `device_trace_0.csv, hal_host_trace.csv and xclbin.run_summary` files back to your system.

* Open `xclbin.ex.run_summary` using vitis_analyzer -> `vitis_analyzer xclbin.ex.run_summary`.

* The snapshot of the timeline trace for the AI Engine 1024x2048 point 1 instance design run with ITER_CNT=8 is shown in the following figure
![Image of 2D-FFT AI Engine implementation 1x Timeline Trace](images/fft_2d_aie_trace_1kx2k_1x_iter8.PNG)

* The profiling setup in the Makefile measures the execution time and all the interfaces and for higher instances designs only strmInp_from_colwiseFFT is profiled,\
the calculation of throughput and latency for `1024 x 2048 point - 1 instance` design, are as follows:
```
Execution Time:
   = Difference in execution timeline trace
   = (End Timestamp of s2mm1 - Start Timestamp of mm2s0) x (150 / 250)
   = (582657.91us - 534736.07us) x (150 / 250)
   = 28753.0us

Latency:
   = Difference between strmInp_from_colwiseFFT beginning and execution beginning
   = (Start Timestamp of s2mm1 - Start Timestamp of mm2s0) x (150 / 250)
   = (537687.61us - 534736.07us) x (150 / 250)
   = 1770.8us

Throughput = (Samples transferred x Iterations) / execution time
           = (MAT_ROWS x MAT_COLS x ITER_CNT) / execution time
           = (1024 x 2048 x 8) / 28753.0us
           = 583.495 MSamples/s
           = 583.495 x 4 MB/s (As each sample is 4bytes)
           = 2333.98 MB/s
```
**The time stamps are scaled by (150 / 250) due to a known issue, wherein the timeline trace doesn't scale the timestamps based on the changed frequency, but reports it as thought the frequency is the default frequency (150Mhz). As 250Mhz is used for the data-mover and profiling, timestamps have to be scaled by (150 / 250).**

Summary of Throughput & Latency for all Variations:
| No. of Instances | FFT Configuration            | Data Transfer size | Aggregate Throughput<br/>(in MSPS) | Average Latency<br/>(in us) | Average Latency<br/>(in us) | Minimum Latency<br/>(in us) |
|:----------------:|:----------------------------:|:------------------:|:----------------------------------:|:---------------------------:|:---------------------------:|:---------------------------:|
| 1                | 64 point<br/>(32 x 64)       | 16384              | 198.330                            | 198.330                     | 4.702                       |  4.702                      |
| 1                | 128 point<br/>(64 x 128)     | 65536              | 299.182                            | 299.182                     | 11.662                      |  11.662                     |
| 1                | 256 point<br/>(128 x 256)    | 262144             | 422.250                            | 422.250                     | 33.601                      |  33.601                     |
| 1                | 512 point<br/>(256 x 512)    | 1048576            | 518.375                            | 518.375                     | 119.386                     |  119.386                    |
| 1                | 2048 point<br/>(1024 x 2048) | 16777216           | 583.495                            | 583.495                     | 1770.834                    |  1770.834                   |
| 5                | 64 point<br/>(32 x 64)       | 16384              | 202.816                            | 1014.082                    | 4.709                       |  4.702                      |
| 5                | 128 point<br/>(64 x 128)     | 65536              | 300.489                            | 1502.444                    | 11.812                      |  11.664                     |
| 5                | 256 point<br/>(128 x 256)    | 262144             | 410.064                            | 2050.321                    | 35.527                      |  33.600                     |
| 5                | 512 point<br/>(256 x 512)    | 1048576            | 517.910                            | 2589.550                    | 119.606                     |  119.378                    |
| 5                | 2048 point<br/>(1024 x 2048) | 16777216           | 583.518                            | 2917.592                    | 1770.915                    |  1770.836                   |
| 10               | 64 point<br/>(32 x 64)       | 16384              | 202.891                            | 2028.913                    | 4.715                       |  4.700                      |
| 10               | 128 point<br/>(64 x 128)     | 65536              | 300.171                            | 3001.711                    | 11.827                      |  11.660                     |
| 10               | 256 point<br/>(128 x 256)    | 262144             | 418.268                            | 4182.683                    | 34.103                      |  33.600                     |
| 10               | 512 point<br/>(256 x 512)    | 1048576            | 517.668                            | 5176.675                    | 119.720                     |  119.378                    |
| 10               | 2048 point<br/>(1024 x 2048) | 16777216           | 583.531                            | 5835.315                    | 1770.840                    |  1770.834                   |


</details>

<details>
<summary>Performance per Watt</summary> 

### Performance per Watt
Performance per Watt is represented as Throughput in MSPS / Power in Watts,\
below is the `Performance per Watt` calculation for `1024 x 2048 point - 1 instance` design:
```
Performance per Watt = Throughput(MSPS) / Power(Watt)
                     = (583.495 / 0.950) MSPS/Watt
                     = 614.205 MSPS/Watt
```


Summary of Performance Per Watt for all Variations:
| No. of Instances | FFT Configuration        | Performance per Watt (in MSPS/Watt) |
|:----------------:|:------------------------:|:-----------------------------------:|
| 1                | 64 point (32 x 64)       | 285.37                              |
| 1                | 128 point (64 x 128)     | 409.84                              |
| 1                | 256 point (128 x 256)    | 537.21                              |
| 1                | 512 point (256 x 512)    | 606.29                              |
| 1                | 2048 point (1024 x 2048) | 614.21                              |
| 5                | 64 point (32 x 64)       | 696.49                              |
| 5                | 128 point (64 x 128)     | 894.31                              |
| 5                | 256 point (128 x 256)    | 1083.11                             |
| 5                | 512 point (256 x 512)    | 1140.27                             |
| 5                | 2048 point (1024 x 2048) | 1049.12                             |
| 10               | 64 point (32 x 64)       | 856.44                              |
| 10               | 128 point (64 x 128)     | 1064.44                             |
| 10               | 256 point (128 x 256)    | 1266.71                             |
| 10               | 512 point (256 x 512)    | 1287.41                             |
| 10               | 2048 point (1024 x 2048) | 1148.46                             |

</details>

<details>
<summary>Consolidated Summary</summary> 

### Consolidated Summary
Consolidated Summary of Observations for all the point sizes and the all the corresponding instance variations:
| FFT Configuration - No. of Instances    | Aggregate Throughput<br/>(in MSPS) | Average Latency<br/>(in us) | No. of Compute Cores | Vector load | No. of Active Memory Banks | Mem R/W Rate | FF (Regs)| CLB LUTs | Dynamic Power<br/>(in mW) | Performance per Watt<br/>(in MSPS/Watt) |
|:---------------------------------------:|:----------------------------------:|:---------------------------:|:--------------------:|:-----------:|:--------------------------:|:------------:|:--------:|:--------:|:-------------------------:|:---------------------------------------:|
| 64 point<br/>(32 x 64)<br/> - x1        | 198.330                            | 4.702                       | 2                    | 8%          | 28                         | 4%           | 1172     | 413      | 695                       | 285.37                                  |
| 128 point<br/>(64 x 128)<br/> - x1      | 299.182                            | 11.662                      | 2                    | 17%         | 28                         | 7%           | 1172     | 413      | 730                       | 409.84                                  |
| 256 point<br/>(128 x 256)<br/> - x1     | 422.250                            | 33.601                      | 2                    | 32%         | 28                         | 13%          | 1172     | 413      | 786                       | 537.21                                  |
| 512 point<br/>(256 x 512)<br/> - x1     | 518.375                            | 119.386                     | 2                    | 50%         | 30                         | 18%          | 1172     | 415      | 855                       | 606.29                                  |
| 2048 point<br/>(1024 x 2048)<br/> - x1  | 583.495                            | 1770.834                    | 2                    | 76%         | 42                         | 19%          | 1172     | 413      | 950                       | 614.21                                  |
| 64 point<br/>(32 x 64)<br/> - x5        | 1014.082                           | 4.709                       | 10                   | 9%          | 132                        | 5%           | 5860     | 2079     | 1456                      | 696.49                                  |
| 128 point<br/>(64 x 128)<br/> - x5      | 1502.444                           | 11.812                      | 10                   | 18%         | 132                        | 8%           | 5860     | 2079     | 1680                      | 894.31                                  |
| 256 point<br/>(128 x 256)<br/> - x5     | 2050.321                           | 35.527                      | 10                   | 32%         | 132                        | 13%          | 5860     | 2079     | 1893                      | 1083.11                                 |
| 512 point<br/>(256 x 512)<br/> - x5     | 2589.550                           | 119.606                     | 10                   | 50%         | 142                        | 19%          | 5860     | 2079     | 2271                      | 1140.27                                 |
| 2048 point<br/>(1024 x 2048)<br/> - x5  | 2917.592                           | 1770.915                    | 10                   | 76%         | 202                        | 19%          | 5860     | 2091     | 2781                      | 1049.12                                 |
| 64 point<br/>(32 x 64)<br/> - x10       | 2028.913                           | 4.715                       | 20                   | 8%          | 264                        | 4%           | 11720    | 4154     | 2369                      | 856.44                                  |
| 128 point<br/>(64 x 128)<br/> - x10     | 3001.711                           | 11.827                      | 20                   | 18%         | 264                        | 8%           | 11720    | 4154     | 2820                      | 1064.44                                 |
| 256 point<br/>(128 x 256)<br/> - x10    | 4182.683                           | 34.103                      | 20                   | 32%         | 272                        | 13%          | 11720    | 4154     | 3302                      | 1266.71                                 |
| 512 point<br/>(256 x 512)<br/> - x10    | 5176.675                           | 119.720                     | 20                   | 50%         | 284                        | 19%          | 11720    | 4150     | 4021                      | 1287.41                                 |
| 2048 point<br/>(1024 x 2048)<br/> - x10 | 5835.315                           | 1770.840                    | 20                   | 76%         | 396                        | 20%          | 11720    | 4154     | 5081                      | 1148.46                                 |

From the observations above it can seen that, with the increase in the FFT Point size, the Window Buffer size used in the AIE Cores increases and with that the throughput increases as well. By increasing the TP_WINDOW_VSIZE parameter in the fft AIE graph, the throughput can be further increased, especially for the lower point sizes, but the AIE Mapper/Router may run into issues due to higher memory requirement.\
Also as the FFT Point sizes increase, the power does not increase proportionately, therefore the Performance per Watt maintains an increasing trend in the beginning and saturates towards the end.

</details>
