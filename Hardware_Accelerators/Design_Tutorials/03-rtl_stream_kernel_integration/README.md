# Mixed Kernels Design Tutorial with AXI Stream and Vitis

This tutorial demostrate the design flow for an example mixed kernels hardware design, which includes both RTL kernel and HLS C kernel, as well as Vitis Vision Library. The design generates a real-time clock image, resize it, then alpha-mix it with an input image in global memory, finally output the result image to global memory. AXI stream interface is used for the kernel-to-kernel connection.

<div align="center">
<img src="./doc/images/alpha_mixing.jpg" alt="Alpha Mixing" >
</div>

<br/><br/>

The hardware design includes three kernels: *rtc_gen*, *alpha_mix*, and *strm_dump*. These kernels are directly connected together using AXI stream link. The topology of the design is shown in the figure below. 

<div align="center">
<img src="./doc/images/topo.png" alt="Topology" >
</div>

The designs can run with following software/hardware environment and tool chain version:
* Operating System
  * Redhat/CentOS 7.4 - 7.8
  * Ubuntu 16.04/18.04
  * OpenCV library need to be installed
* Vitis: 2020.1
* XRT: 2.6.655
* Hardware and Platform (need both the deployment and development platforms)
  * Alveo U200, xilinx_u200_xdma_201830_2
  * Alveo U250, xilinx_u250_xdma_201830_2
  * Alveo U50, xilinx_u50_gen3x16_xdma_201920_3
  * Alveo U280, xilinx_u280_xdma_201920_3

The directory struction and brief explainations are as below.
~~~
├── doc/                            # documents
├── hw/                             # Hardware build working directory
│   ├── alpha_mix.cpp               # HLS C source code for alpha_mix kernel
│   ├── build_rtc_gen_xo.sh         # shell script to call Vivado to package rtc_gen kernel IP to xo file
│   ├── include/                    # Vision Vision library include file for HLS C
│   ├── Makefile                    # Makefile for hardware building
│   ├── package_rtc_gen.tcl         # Vivado tcl script to package rtc_gen kernel IP to xo file
│   ├── rtc_gen_ip/                 # IP directory for rtc_gen kernel, including all the RTL source codes
│   ├── rtc_gen_kernel.xml          # Kernel desctiption file for rtc_gen kernel
│   ├── strm_dump.cpp               # HLS C source for strm_dump kernel
│   ├── xclbin_rtc_alpha.ini        # v++ configuration file for rtc_alpha binary build
│   └── xclbin_rtc_gen_test.ini     # v++ configuration file for rtc_gen_test binary build
├── README.md
├── rtc_gen                         # working directory for rtc_gen kernel development
│   ├── font_sim_data.txt           # text format font library file for RTL simulation
│   └── src/                        # RTL source code for rtc_gen kernel
├── sw/                             # Test program directory
│   ├── build/                      # Software build working directory
│   │   ├── font.dat                # Font library file including 11 characters
│   │   ├── setup_emu.sh            # setup script for emulation mode
│   │   └── xrt.ini                 # XRT configuration for emulation and debug
│   ├── CMakeLists.txt              # cmake configuration file
│   ├── media/                      # Media files for test program
│   └── src/                        # Test program source codes
│       ├── rtc_alpha_tb.cpp        # Test program for the whole design
│       ├── rtc_gen_test.cpp        # Test program for rtc_gen kernel
│       └── xcl2/                   # Xilinx OpenCL include files
└── xclbin/                         # Pre-built XCLBIN files
    ├── u200/                       # Pre-built XCLBIN files for Alveo U200 card
    ├── u250/                       # Pre-built XCLBIN files for Alveo U250 card
    ├── u280/                       # Pre-built XCLBIN files for Alveo U280 card
    └── u50/                        # Pre-built XCLBIN files for Alveo U50 card
~~~

## RTL Kernel: rtc_gen (XO)

*rtc_gen* is the real-time clock digit image generation kernel written in Verilog HDL. *rtc_gen* has an internel always-run real-time-clock driven by AXI bus clock with a clock divider. The time value can be set by host via kernel arguments. The kernel will firstly load the font image library for digits 0-9 from global memory to on-chip buffer, then output the real-time-clock digit image through AXI stream port. 

The character size in the font library is 240 (height) by 160 (width) pixels, and the font library includes 11 characters, i.e. digits 0-9 and colon. Refer to the image below for the font library contents.  

<div align="center">
<img src="./doc/images/font_lib.png" alt="Font Library" >
</div>
<br/><br/>

Each pixel in the font library is represented with 4-bit, which is actually the opacity value for each pixel. When output through AXI stream port, the 4-bit opacity value will be expanded to 8-bit by left shifting 4-bit then add 15 (i.e., expand 0xB to 0xBF). The opacity value will be used by the downstream alpha-mixing kernel to generate time digit with color setting. The font image data size for single chracter is:
~~~
  240 x 160 x 4 = 153,600 bits = 19,200 bytes
~~~

The total font image library size is:

~~~
  19,200 x 11 = 211,200 bytes
~~~

*rtc_gen* support two time format: one is with centi-second, namely HOUR:MIN:SEC:CENTISEC, including 11 characters; the other is without centi-second, namely HOUR:MIN:SEC, including 8 chracters. The time format is set in kernel arguments. Refer to the figures below for examples of the two output time format.

<div align="center">
<img src="./doc/images/time_format.png" alt="Time Format" >
</div>

The *rtc_gen* kernel has three bus interfaces:
+ AXI-Lite slave interface for kernel argument and control
+ AXI-Lite master interface for font library data loading
+ AXI stream master interface for clock digit image output

The kernel is composed of three blocks: *rtc_gen_axi_read_master* for AXI master based font library reading, *rtc_gen_control_s_axi* for AXI slave based kernel arguments and control, and *rtc_gen_core* for core kernel function and AXI stream output. *rtc_gen_axi_read_master* and *rtc_gen_control_s_axi* are standard blocks generated by Vitis RTL Kernel Wizard.

<div align="center">
<img src="./doc/images/rtc_gen_block.png" alt="rtc_gen Block" >
</div>

When trigger by the host, the kernel will read time value from internel real-time-clock, and output a frame of time image corresponding to the time value.

Following table summarizes the arguments used by *rtc_gen* kernel.

|No.   | Arguments   | Width  | Description |
| ---- | ----        | ----   | ---- |
|0     | work_mode   | 1      |[0]: determine the kernel working mode <br> 0 – load font from global memory to on-chip SRAM via AXI read master <br> 1 – output RTC digit figure via AXI steam master |
|1     | time_format | 1      |[0]: determine whether centisecond is included in the output digit images <br> 0 – disable centiseconds output <br> 1 – enable centiseconds output |
|2     | time_set_val| 32     |Set time value for internel free-running clock: <br> [31:24] – hours <br> [23:16] – minutes <br> [15:8] – seconds <br> [7:0] – centi-seconds |
|3     | time_set_en | 1      |[0]: write 1 to this bit will load the time_set_value to internel free-running clock. |
|4     | read_addr   | 64     |AXI master pointer, this is the FPGA device buffer address for font library |
|5     | reserved    | -      | - |

<br/><br/>

Please read [RTC_GEN RTL Kernel Creation](./doc/rtc_gen_tutorial.md) for more detailes of the RTL kernel *rtc_gen* and the step-by-step guideline to create this RTL kernel.

## HLS C Kernel: alpha_mix (XO)

The kernel *alpha_mix* finishes follow tasks in order:
* Receive the clock digit image from *rtc_gen* kernel via AXI stream port
* Resize the clock digit image with Vitis Vison Library resize function
* Load the background image from global memory, then execute alpha mixing with the clock digit image
* Send out the mixed image via AXI stream port

<div align="center">
<img src="./doc/images/alpha_flow.png" alt="alpha_mix flow" >
</div>

The *alpha_mix* kernel has four bus interfaces:

* AXI-Lite slave interface for control
* AXI-Lite master interface for background image loading
* AXI stream slave interface for clock digit image receiving
* AXI stream master interface for mixed image output

Following table summarized the arguments used by *alpha_mix* kernel. Please note the kernel use *XF_NPPC8* mode, namely eight pixels will be processed at each clock cycle, so please ensure the *background image width* and *resized time image width* are integer multiples of 8, otherwise the kernel might hang.

|No.   | Arguments          | Width | Description |
| ---- | ----               | ----  | ----  |
|0     | reserved           | -     | -     |
|1     | bgr_img_input      | 64    | AXI master pointer, FPGA device buffer for input background image     |
|2     | reserved           | -     | -     |
|3     | time_img_rows_in   | 32    | Input time image height           |
|4     | time_img_cols_in   | 32    | Input time image width            |
|5     | time_img_rows_rsz  | 32    | Resized time image height         |
|6     | time_img_cols_rsz  | 32    | Resized time image width          |
|7     | time_img_pos_row   | 32    | Time image vertical coordinate, start from 0    |
|8     | time_img_pos_col   | 32    | Time image horizontal coordinate, start from 0  |
|9     | time_char_color    | 32    | Time figure color, bit range [23:0] used for [RGB]|
|10    | time_bgr_color     | 32    | Time background color, bit range [23:0] used for [RGB]|       |
|11    | time_bgr_opacity   | 32    | Time background opacity, [7:0] used, value range from 0 - 255 |
|12    | bgr_img_rows       | 32    | Background image height           |
|13    | bgr_img_cols       | 32    | Background image width            |

You could refer to below figure for the meaning of some kernel arguments.

<div align="center">
<img src="./doc/images/alpha_arg.png" alt="alpha_mix kernel arguments" >
</div>

Please read [ALPHA_MIX HLS C Kernel Creation](./doc/alpha_mix_tutorial.md) for more detailes of the HLS C kernel *alpha_mix*.

## HLS C Kernel: strm_dump (XO)

*strm_dump* is a simple HLS kernel to dump the input AXI stream to global memory via AXI Lite master.

Following table summarizes the arguments used by *strm_dump* kernel.

|No.   | Arguments   | Width  | Description |
| ---- | ----        | ----   | ---- |
|0     | reserved    | -      |- |
|1     | output_addr | 64     |AXI master pointer, this is the FPGA device buffer address for output image |
|2     | byte_size   | 32     |Data quantity to be output in bytes. This can be calculated based on time format and color depth |

## Bitstream Implementation (XCLBIN)

### rtc_gen_test_hw.xclbin / rtc_gen_test_hw_emu.xclbin

This is a simple test system for *rtc_gen* kernel, which integrates two kernels: *rtc_gen* and *strm_dump*, which are connected together using AXI stream bus. Refer to the following connection diagram on U50 platform. According to the different building target (hw or hw_emu), two XCLBIN files will be generated.

<div align="center">
<img src="./doc/images/rtc_gen_test_diagram.png" alt="rtc_gen_test Diagram" >
</div>

### rtc_alpha_hw.xclbin / rtc_alpha_hw_emu.xclbin

This is the fully implemented system, which integrated all the three kernels: *rtc_gen*, *alpha_mix* and *strm_dump*, which are connected together via AXI stream bus. Please note the function of the kernel *strm_dump* is very easy to be merged into *alpha_mix* kernel. We seperated the *strm_dump* kernel here just to demostrate the kernel-to-kernel AXI stream connection functionality. Refer to the following connection diagram on U50 platform. According to the different building target (hw or hw_emu), two XCLBIN files will be generated.

<div align="center">
<img src="./doc/images/rtc_alpha_diagram.png" alt="rtc_alpha Diagram" >
</div>

## Test Program

### rtc_gen_test.cpp

This program first judge the running mode according to the environment variable *XCL_EMULATION_MODE*, then choose to  use binary file *rtc_gen_test_hw.xclbin* or *rtc_gen_test_hw_emu.xclbin* to finish the testing of RTL kernel *rtc_gen*. It will test both the 8-digit and 11-digit clock format, and the generated clock image will be displayed directly.

### rtc_alpha_tb.cpp

This program first judge the running mode according to the environment variable *XCL_EMULATION_MODE*, then choose to use binary file *rtc_alpha_hw.xclbin* or *rtc_alpha_hw_emu.xclbin* to mix the generated real time clock images to a background image. The user can select background image, set time format, set clock time by command parameters. The user can also change the color, size, and position of the clock image by modified the program source code.

<br/>

## How to Use This Repository

Before go through the following steps, don't forget to source XRT and Vitis setup files, for example:

~~~
source /opt/xilinx/xrt/setup.sh
source /opt/xilinx/Vitis/2020.1/settings64.sh
~~~

The two test programs need to display images. So if you are using remote server, please use VNC desktop, or ssh connection with X11 forwarding along with local X11 server.

### Build the hardware

Change to *./hw* directory, then use **make** command to finish the building of three XO files and to XCLBIN files. All available make command option includes:

~~~
make
      Display help information

make all TARGET=<hw/hw_emu> PLATFORM=<FPGA PLATFORM>
      Command to build all the rtc_gen_test and rtc_alpha xclbin and necessary kernel files (xo) for  
      specified target and platform. By default, TARGET=hw, PLATFORM=xilinx_u200_xdma_201830_2

make all_xo TARGET=<hw/hw_emu> PLATFORM=<FPGA PLATFORM>
      Command to build all the kernel files (xo), including rtc_gen.xo, alpha_mix.xo and strm_dump.xo
      By default, TARGET=hw, PLATFORM=xilinx_u200_xdma_201830_2

make clean
      Command to remove all the generated files.
~~~

In the make commond options, the TARGET can be *hw* or *hw_emu*. Because the *rtc_gen* kernel doesn't provide software emulation model, so *sw_emu* mode cannot be used. When the TARGET is *hw*, the XCLBIN and XO files will be with *_hw* postfix; when the TARGET is *hw_emu*, the XCLBIN and XO files will be with *_hw_emu* postfix. Please note the RTL kernel *rtc_gen* will not be affected by the *hw* or *hw_emu* option, and there will only be a XO file *rtc_gen.xo*.

The PLATFORM could be one of the four choices: xilinx_u200_xdma_201830_2, xilinx_u250_xdma_201830_2, xilinx_u50_gen3x16_xdma_201920_3 and xilinx_u280_xdma_201920_3. No matter whether you have these Alveo cards installed, you can use the platform as the build PLATFORM if you have installed the deployment and development platform package (deb or rpm packages) on your system. You can look into */opt/xilinx/platform* directory or use command *platforminfo -l* to check which platforms have been installed. The finally generated xclbin and xo files will be in *./hw* directory after the successful execution of the make command. 

For example, if you would like to build all XO and XCLBIN files in hardware emulation mode with U50 card, just input:

~~~
make all TARGET=hw_emu PLATFORM=xilinx_u50_gen3x16_xdma_201920_3
~~~

Because the XCLBIN file building for hardware target needs a long time, to save your time, we also provide the pre-built XCLBIN files (*rtc_gen_test_hw.xclbin* and *rtc_alpha_hw.xclbin*) in *./xclbin* directory for each kind of Alveo card. Please note they are built with *TARGET=hw* option and cannot be used in *hw_emu* mode. For *hw_emu* target XCLBIN files, it's much faster to build and system dependent, so please build them by yourself.

**To use the pre-built xclbin files, please copy them into *./hw* directory, which will be used directly in downstream steps.**

### Build and run the software

* Step 1: generate Makefile

Change to *./sw/build* directory, then enter **cmake ..** or **cmake3 ..** command. This will generate the *Makefile* for software builds, as well as link the two XCLBIN files in *./hw* directory to *./sw/build* directory.

~~~
cd ./sw/build
cmake ..
~~~

* Step 2: compile the programs

Enter **make** command, then the two C++ program will be compiled. This will generate two executables: *rtc_gen_test* and *rtc_alpha_tb*.

~~~
make
~~~

* Step 3: configure running mode and emulation

You can run the *rtc_gen_test* and *rtc_alpha_tb* in both hardware emulation mode and real hardware mode, the default mode is real hardware mode. You can refer to the script *setup_emu.sh* to switch between hardware emulation mode and real hardware mode. The usage of this script is as below.

~~~
source setup_emu.sh
      Display help information

source setup_emu.sh -m EMU_MODE -p PLATFORM        
      Turn on and set emulation mode
      EMU_MODE: hw - hw_emu
                sw - sw_emu
      PLATFORM: u200/u250/u50/u280

source setup_emu.sh off                            
      Turn off emulation mode
~~~

For example, if you want to run the executable in hardware emulation mode with U50 platform, just input:

~~~
source setup_emu.sh -m hw -p u50
~~~

*setup_emu.sh* will generate necessary configuration file and setting up the environment. 

**Please note:**
+ **The *PLATFORM* you input here should be consistant with the XCLBIN files in *./sw/build* directory.**
+ ***sw* option for *EMU_MODE* will not work since the RTL kernel doesn't include software emulation model. So please don't use *sw* as option in this example design.**



To turn off emuluation mode and return to normal hardware running mode, just enter:

~~~
source setup_emu.sh off
~~~

For more detailes on the hardware emulation for this example, please read [Emulation Turotial](./doc/hw_emu_tutorial.md)

* Step 4: run program **rtc_gen_test**

No matter you are in real hardware mode or emulation mode, just run the executable *rtc_gen_test* to finish the test. Firstly an eight-digit clock will be displayed, keep the image window front and press *ESC* key, a second eleven-digit clock will be displayed. Keep the image window front then Press *ESC* key again to exit the program.

~~~
./rtc_gen_test
~~~

The program will firstly judge the running mode (hw or hw_emu), then look for *./sw/build/rtc_gen_test_hw.xclbin* or *./sw/build/rtc_gen_test_hw_emu.xclbin* file and analyze it to get the platform it is using, then compare it with the card you have installed. If mismatch is detected, error information will be reported and the program will exit.

**Note**: running under hardware emulation mode may take a long time since it is actually running RTL simulation.

* Step 5: run program **rtc_alpha_tb**

No matter you are in real hardware mode or emulation mode, just run the executable *rtc_alpha_tb* to run the program. There are a few command parameters for the executable, the usage is as below:

~~~
rtc_alpha_tb [-i BACK_IMAGE] [-f TIME_FORMAT] [-s SET_TIME]

  BACK_IMAGE     path to the background image, default is ../media/alveo.jpg
  TIME_FORMAT    0 - eight-digit clock, 1 - eleven-digit clock, default is 0
  SET_TIME       0 - don't use system time to set the clock, 1 - use system time to set the clock, default is 0
~~~

There are three images provided in *./sw/media* directory: alveo.jpg, vitis.jpg and victor.jpg, and you could also use other images. Please note the images should be in three-channel format (RGB without transparency). Also please use images big enough, otherwise please modify the program source code to adjust the clock image size or position. 

Following is some execution command line examples:

~~~
rtc_alpha_tb
      Mix the clock image with ../media/alveo.jpg and display, don't sync the kernel internal real-time-clock 
      with Linux system clock, and use 8-digit format.

rtc_alpha_tb -i ../media/vitis.jpg -f 1 -s 1
      Mix the clock image with ../media/vitis.jpg and display, sync the kernel internal real-time-clock with
      Linux system clock, and use 11-digit format.
~~~

To exit the program, just keep the image window front, then press **ESC** key.

The program will firstly judge the running mode (hw or hw_emu), then look for *./sw/build/rtc_alpha_hw.xclbin* or *./sw/build/rtc_alpha_hw_emu.xclbin* file and analyze it to get the platform it is using, then compare it with the card you have installed. If mismatch is detected, error information will be reported and the program will exit.

You could make modification to following *#define* section at the beginning of *./sw/src/rtc_alpha_tb.cpp* file to adjust the color, size, position and opacity of the clock image, then repeat **step 2** to re-compile the program and run to see the result. Don't forget to ensure that the width of background image and resized clock image are integer multiples of 8.

~~~c++
// position of clock image, top-left corner is (0,0)
#define RTC_POSITION_ROW 64
#define RTC_POSITION_COL 400

// resized clock image size for 8-digit font digit size
// ensure RTC_IMG_WIDTH is integer multiple of 8
#define RTC_IMG_WIDTH_8D 480
#define RTC_IMG_HEIGHT_8D 90

// resized clock image size for 11-digit font digit size
// ensure RTC_IMG_WIDTH is integer multiple of 8
#define RTC_IMG_WIDTH_11D 528
#define RTC_IMG_HEIGHT_11D 72

// clock image font color
#define FONT_COLOR_R 255
#define FONT_COLOR_G 255
#define FONT_COLOR_B 255

// clock image background color
#define BGR_COLOR_R 80
#define BGR_COLOR_G 80
#define BGR_COLOR_B 80

// clock image background opacity
#define BGR_OPA 100
~~~

**Note**: running under hardware emulation mode may take a long time since it is actually running RTL simulation. You could use some smaller background image to reduce the run time, and don't forget to modify those size and position parameters described above accordingly in this case.

* Step 6: try Vitis profiling function with **rtc_gen_test** and **rtc_alpha_tb** program.

Vitis provides powerful profiling features which enable you to get deeper view into the performance, bandwidth usage, design bottleneck, etc. Please read [Profiling the Application](./doc/profile_tutorial.md) for more details. 

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
