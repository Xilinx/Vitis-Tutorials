<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2021.1 Vitis™ Getting Started Tutorial</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.2">See 2020.2 Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Vitis Flow 101 – Part 4 : Build and Run the Example

 In this fourth part of the Introduction to Vitis tutorial, you will compile and run the vector-add example using each of three build targets supported in the Vitis flow (software emulation, hardware emulation and hardware).

* Software Emulation - The kernel code is compiled to run on the host processor. This allows iterative algorithm refinement through fast build-and-run loops. This target is useful for identifying syntax errors, performing source-level debugging of the kernel code running together with application, and verifying the behavior of the system.

* Hardware Emulation - The kernel code is compiled into a hardware model (RTL), which is run in a dedicated simulator. This build-and-run loop takes longer but provides a detailed, cycle-accurate view of kernel activity. This target is useful for testing the functionality of the logic that will go in the FPGA and getting initial performance estimates.

* Hardware - The kernel code is compiled into a hardware model (RTL) and then implemented on the FPGA, resulting in a binary that will run on the actual FPGA.


There are slight differences when targeting data-center and embedded platforms. Instructions for both the ZCU102 and Alveo U200 cards are provided below. These instructions can be easily adapted to other cards. 

> IMPORTANT: This tutorial requires Vitis 2021.1 or later to run.

<details>
<summary><b>Instructions for the ZCU102 platform, click here</b></summary>


## Building and Running on an Embedded Platform (ZCU102)

### Setting up the environment

*NOTE: The instructions provided below assume that you are running in a bash shell.*

* To configure the environment to run Vitis, source the following scripts:


```bash
source <VITIS_install_path>/settings64.sh
source <XRT_install_path>/setup.sh
unset LD_LIBRARY_PATH
```

* Then make sure the following environment variables are correctly set to point to the your ZCU102 platform, rootfs and sysroot directories respectively.

```bash
export PLATFORM_REPO_PATHS=<path to the ZCU102 platform install dir>
export ROOTFS=<path to the ZYNQMP common image directory, containing rootfs>
```

To properly source the cross-compilation SDK, run the `environment-setup-aarch64-xilinx-linux` script in the directory
where you extracted the SDK source.

```bash
source <path to the SDK>/environment-setup-aarch64-xilinx-linux
```

*NOTE: The ZYNQMP common image file can be downloaded from the [Vitis Embedded Platforms](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html) page, and contains the Sysroot, Rootfs, and boot Image for Xilinx Zynq MPSoC devices.*


### Targeting Software Emulation

* To build for software emulation, enter the following commands:


```bash
cd <Path to the cloned repo>/Getting_Started/Vitis/example/zcu102/sw_emu

$(CXX) -Wall -g -std=c++11 ../../src/host.cpp -o app.exe -I/usr/include/xrt -lOpenCL -lpthread -lrt -lstdc++
v++ -c -t sw_emu --config ../../src/zcu102.cfg -k vadd -I../../src ../../src/vadd.cpp -o vadd.xo 
v++ -l -t sw_emu --config ../../src/zcu102.cfg ./vadd.xo -o vadd.xclbin
v++ -p -t sw_emu --config ../../src/zcu102.cfg ./vadd.xclbin --package.out_dir package --package.rootfs ${ROOTFS}/rootfs.ext4 --package.sd_file ${ROOTFS}/Image --package.sd_file xrt.ini --package.sd_file app.exe --package.sd_file vadd.xclbin --package.sd_file run_app.sh
```


Here is a brief explanation of each of these five commands:
1. `$(CXX)` compiles the host application using the ARM cross-compiler. This variable contains the full compiler executable plus flags relevant to cross-compilation, and is set when you source the SDK environment setup script.
2. `v++ -c` compiles the source code for the vector-add accelerator into a compiled kernel object (.xo file). 
3. `v++ -l` links the compiled kernel with the target platform and generates the FPGA binary (.xclbin file). 
4. `v++ -p` packages the host executable, the rootfs, the FPGA binary and a few other files and generates a bootable image.

The -t option of the v++ tool specifies the build target. Here it is set to sw_emu as we are building for software emulation.

Notice also the --config option which is used to specify the name of a configuration file containing additional options. Here we are using this configuration file to specify, amongst others, the name of the targeted platform:

```
platform=xilinx_zcu102_base_202110_1
save-temps=1
debug=1

# Enable profiling of data ports
[profile]
data=all:all:all
```

* Building for software emulation is quick and shouldn’t take more than a minute or two. After the build process completes, you can launch the software emulation run by using the launch script generated during the packaging step (v++ -p):


```bash
./package/launch_sw_emu.sh 
```

* This command with launch software emulation, start the Xilinx Quick Emulation (QEMU) and initiate the boot sequence. Once Linux has finished booting, enter the following commands to run the example program:

```bash
cd /media/sd-mmcblk0p1
export XILINX_XRT=/usr
export XCL_EMULATION_MODE=sw_emu
./app.exe vadd.xclbin
```

* You should see the following messages, indicating that the run completed successfully:

```bash
INFO: Found Xilinx Platform
INFO: Loading 'vadd.xclbin'
TEST PASSED
```

* Press Ctrl+a x to exit QEMU and return to your bash shell.




### Targeting Hardware Emulation

* To build for hardware emulation, enter the following commands:

```bash
cd ../hw_emu

$(CXX) -Wall -g -std=c++11 ../../src/host.cpp -o app.exe -I/usr/include/xrt -lOpenCL -lpthread -lrt -lstdc++
v++ -c -t hw_emu --config ../../src/zcu102.cfg -k vadd -I../../src ../../src/vadd.cpp -o vadd.xo 
v++ -l -t hw_emu --config ../../src/zcu102.cfg ./vadd.xo -o vadd.xclbin
v++ -p -t hw_emu --config ../../src/zcu102.cfg ./vadd.xclbin --package.out_dir package --package.rootfs ${ROOTFS}/rootfs.ext4 --package.sd_file ${ROOTFS}/Image --package.sd_file xrt.ini --package.sd_file app.exe --package.sd_file vadd.xclbin --package.sd_file run_app.sh
```

* The only difference with the previous step is the v++ target (-t) option which is changed from sw_emu to hw_emu. All other options remain identical.

* Building for hardware emulation takes about 5 minutes. After the build process completes, you can launch the hardware emulation run by using the launch script generated during the packaging step.

```bash
./package/launch_hw_emu.sh 
```

* Once Linux has finished booting, enter the following commands on the QEMU command line to run the example program:

```bash
cd /media/sd-mmcblk0p1
export XILINX_XRT=/usr
export XCL_EMULATION_MODE=hw_emu
./app.exe vadd.xclbin
```

* You should see messages that say TEST PASSED indicating that the run completed successfully

* Press Ctrl+a x to exit QEMU and return to your bash shell.

 

### Targeting Hardware

* To build for hardware, enter the following commands:

```bash
cd ../hw

$(CXX) -Wall -g -std=c++11 ../../src/host.cpp -o app.exe -I/usr/include/xrt -llOpenCL -lpthread -lrt -lstdc++
v++ -c -t hw --config ../../src/zcu102.cfg -k vadd -I../../src ../../src/vadd.cpp -o vadd.xo 
v++ -l -t hw --config ../../src/zcu102.cfg ./vadd.xo -o vadd.xclbin
v++ -p -t hw --config ../../src/zcu102.cfg ./vadd.xclbin --package.out_dir package --package.rootfs ${ROOTFS}/rootfs.ext4 --package.sd_file ${ROOTFS}/Image --package.sd_file xrt.ini --package.sd_file app.exe --package.sd_file vadd.xclbin --package.sd_file run_app.sh
```

* To target Hardware the v++ -t option is set to hw. All other options remain identical.
* Building for hardware takes about 30 minutes, but the exact duration will depend on the machine you are building on and its load.
* After the build process completes, copy the sd_card directory to an SD card and plug it into the platform and boot until you see the Linux prompt. At that point, enter the following commands to execute the accelerated application:

```bash
cd /media/sd-mmcblk0p1
export XILINX_XRT=/usr
./app.exe vadd.xclbin
```

* You will see the same TEST PASSED message indicating that the run completed successfully.
* Congratulations, you just completed your first run of a Vitis accelerated application on the ZCU102 card! 

</details>



<details>
<summary><b>Instructions for the Alveo U200 platform, click here</b></summary>



## Building and Running on a Data-Center Platform (U200)

### Setting up the environment

*NOTE: The instructions provided below assume that you are running in a bash shell.*

* To configure the environment to run Vitis, source the following scripts:

```bash
source <VITIS_install_path>/settings64.sh
source <XRT_install_path>/setup.sh
```

* Then make sure the following environment variable is correctly set to point to the your U200 platform installation directory.

```bash
export PLATFORM_REPO_PATHS=<path to the U200 platform install dir>
```

### Targeting Software Emulation

* To build for software emulation, enter the following commands:


```bash
cd <Path to the cloned repo>/Getting_Started/Vitis/example/u200/sw_emu

g++ -Wall -g -std=c++11 ../../src/host.cpp -o app.exe -I${XILINX_XRT}/include/ -L${XILINX_XRT}/lib/ -lOpenCL -lpthread -lrt -lstdc++
emconfigutil --platform xilinx_u200_gen3x16_xdma_1_202110_1 --nd 1
v++ -c -t sw_emu --config ../../src/u200.cfg -k vadd -I../../src ../../src/vadd.cpp -o vadd.xo 
v++ -l -t sw_emu --config ../../src/u200.cfg ./vadd.xo -o vadd.xclbin
```


Here is a brief explanation of each of these four commands:
1. `g++` compiles the host application using the standard GNU C compiler.
2. `emconfigutil` generates an emulation configuration file which defines the device type and quantity of devices to emulate for the specified platform. 
3. `v++ -c` compiles the source code for the vector-add accelerator into a compiled kernel object (.xo file). 
4. `v++ -l` links the compiled kernel with the target platform and generates the FPGA binary (.xclbin file). 

The -t option of the v++ tool specifies the build target. Here it is set to sw_emu as we are building for software emulation.

Notice also the --config option which is used to specify the name of a configuration file containing additional options. Here we are using this configuration file to specify, amongst others, the name of the targeted platform and the mapping of kernel arguments to DDR banks.

```
platform=xilinx_u200_gen3x16_xdma_1_202110_1
debug=1
save-temps=1

[connectivity]
nk=vadd:1:vadd_1
sp=vadd_1.in1:DDR[1]
sp=vadd_1.in2:DDR[2]
sp=vadd_1.out:DDR[1]

[profile]
data=all:all:all
```

* Building for software emulation is quick and shouldn’t take more than a minute or two. After the build process completes, you can launch the software emulation run as follows:

```bash
export XCL_EMULATION_MODE=sw_emu
./app.exe
```

* You should see the following messages, indicating that the run completed successfully:

```bash
INFO: Found Xilinx Platform
INFO: Loading 'vadd.xclbin'
TEST PASSED
```

 

### Targeting Hardware Emulation

* To build for hardware emulation, enter the following commands:

```bash
cd ../hw_emu

g++ -Wall -g -std=c++11 ../../src/host.cpp -o app.exe -I${XILINX_XRT}/include/ -L${XILINX_XRT}/lib/ -lOpenCL -lpthread -lrt -lstdc++
emconfigutil --platform xilinx_u200_gen3x16_xdma_1_202110_1 --nd 1
v++ -c -t hw_emu --config ../../src/u200.cfg -k vadd -I../../src ../../src/vadd.cpp -o vadd.xo 
v++ -l -t hw_emu --config ../../src/u200.cfg ./vadd.xo -o vadd.xclbin
```


* The only difference with the previous step is the v++ target (-t) option which is changed from sw_emu to hw_emu. All other options remain identical.

* Building for hardware emulation takes about 5 or 6 minutes. After the build process completes, you can launch the hardware emulation run as follows.

```bash
export XCL_EMULATION_MODE=hw_emu
./app.exe
```

* When the run completes, you should see the TEST PASSED message indicating that the run completed successfully

 

### Targeting Hardware

* To build for the hardware, enter the following commands:

```bash
cd ../hw

g++ -Wall -g -std=c++11 ../../src/host.cpp -o app.exe -I${XILINX_XRT}/include/ -L${XILINX_XRT}/lib/ -lOpenCL -lpthread -lrt -lstdc++
v++ -c -t hw --config ../../src/u200.cfg -k vadd -I../../src ../../src/vadd.cpp -o vadd.xo 
v++ -l -t hw --config ../../src/u200.cfg ./vadd.xo -o vadd.xclbin
```

* To target Hardware, the v++ -t option is set to hw and the emconfigutil step is skipped as it only applies to emulation. All other options remain identical.
* Building for hardware may take between 1.5 and 2 hours depending on the machine you are building on and its load.
* After the build completes you can run the application accelerated with the U200 card:

```bash
./app.exe
```

*NOTE: Make sure to run the program on the server where the Alveo card is installed. If you built the application on a different machine, you will need to source the /opt/xilinx/xrt/setup.sh script after connecting to the desired server and before running the above command.*

* You should see the same TEST PASSED message indicating that the run completed successfully.
* Congratulations, you just completed your first run of a Vitis accelerated application on the Alveo U200 card! 


</details>

 

## Next Step

Now that you ran your first example, proceed to [**Part 5**](./Part5.md) of this tutorial to learn how to visualize and profile your application with Vitis Analyzer.

 

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
