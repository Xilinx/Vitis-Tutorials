<table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Application Acceleration Development Flow Tutorials</h1>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Vitis Getting Started Tutorial

## Part 4: Build and Run the Embedded Processor Application

 In this fourth part of the Introduction to Vitis tutorial, you will compile and run the vector-add example using each of three build targets supported in the Vitis flow as described below. The overall flow is described in [Introduction to Vitis Tools for Embedded System Designers](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Introduction-to-Vitis-Tools-for-Embedded-System-Designers), and includes the image flow diagram shown below. From the image you can see the G++ cross-compiler  for building the host application for the Arm processor, and the use of the Vitis compiler (`v++`) for building the AMD device binary (`xclbin`). These are the steps you will be working through in this lab. 

![img](./images/embedded_processor_flow.png)

* Software Emulation: The kernel code is compiled to run on an emulation environment (QEMU) version of the embedded processor. The software emulation target allows quick iterative algorithm refinement through fast build-and-run loops. This target is useful for identifying syntax errors, performing source-level debugging of the kernel code running together with application, and verifying the behavior of the system. Refer to [Software Emulation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Software-Emulation) for more information.

* Hardware Emulation: The kernel code is compiled into a hardware model (RTL), which is run in the Vivado logic simulator. The host application runs on an emulation environment (QEMU) version of the embedded processor. This build-and-run loop takes longer but provides a detailed, cycle-accurate view of kernel activity. This target is useful for testing the functionality of the logic that will go in the FPGA and getting initial performance estimates. Refer to [Hardware Emulation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Hardware-Emulation) for more information.

* Hardware: The kernel code is compiled into a hardware model (RTL) and then implemented on the FPGA, resulting in a binary that will run on the actual FPGA. In the Embedded Processor platform, the host application, xclbin, and required support files are written to an SD card (for instance) and then used to boot and configure the system. Refer to [Running the Application Hardware Build](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Running-the-Application-Hardware-Build) for more information.

## Building and Running on ZCU102 Zynq MPSoC Embedded Processor Platform

### Setting up the environment

> **IMPORTANT:** This tutorial requires Vitis 2023.1 or later to run.

>**NOTE:** The instructions provided below assume that you are running in a bash shell.

* To configure the environment to run Vitis, source the following scripts:

  ```bash
  source <VITIS_install_path>/settings64.sh
  ```

* Then make sure the following environment variables are correctly set to point to your installed ZCU102 platform, rootfs and sysroot directories respectively.

  ```bash
  export PLATFORM_REPO_PATHS=<path to the ZCU102 platform install dir>
  export ROOTFS=<path to the ZYNQMP common image directory, containing rootfs>
  export SYSROOT=$ROOTFS/sysroots/cortexa72-cortexa53-xilinx-linux
  export SDKTARGETSYSROOT=$SYSROOT
  ```

>**NOTE:** The ZYNQMP common image file can be downloaded from the [Vitis Embedded Platforms](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html) page, and contains the Sysroot, Rootfs, and boot Image for AMD Zynq™ MPSoC devices.


### Targeting Software Emulation

To build for software emulation, enter the following commands to setup the target build directory:

```bash
cd <Path to the cloned repo>/Getting_Started/Vitis/example/zcu102
mkdir sw_emu
cp xrt.ini sw_emu
cp run_sw_emu.sh sw_emu
cd sw_emu
```

Then, after changing into the target build directory, enter the following commands to build the host application and device binary:

```bash
$CXX -g -std=c++17 -Wall -O0 -fmessage-length=0 ../../src/host.cpp -o ./app.exe -I$SYSROOT/usr/include/xrt -LSYSROOT/usr/lib -lxrt_coreutil -pthread --sysroot=$SYSROOT
v++ -c -t sw_emu --platform xilinx_zcu102_base_202310_1 --config ../../src/zcu102.cfg -k vadd -I../../src ../../src/vadd.cpp -o ./vadd.xo 
v++ -l -t sw_emu --platform xilinx_zcu102_base_202310_1 --config ../../src/zcu102.cfg ./vadd.xo -o ./vadd.xclbin
v++ -p -t sw_emu --platform xilinx_zcu102_base_202310_1 --config ../../src/zcu102.cfg ./vadd.xclbin --package.out_dir ./package --package.rootfs ${ROOTFS}/rootfs.ext4 --package.sd_file ${ROOTFS}/Image --package.sd_file ./xrt.ini --package.sd_file ./app.exe --package.sd_file ./vadd.xclbin --package.sd_file ./run_sw_emu.sh
```

Here is a brief explanation of each of these four commands:

1. `$CXX` compiles the host application using the Arm cross-compiler. This variable contains the full compiler executable plus flags relevant to cross-compilation, and is set when you source the software development kit (SDK) environment setup script. Refer to [Building the Software Application](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Building-the-Software-Application) for more information. This should resolve to `$XILINX_VITIS/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++`.
2. `v++ -c` compiles the source code for the vector-add accelerator into a compiled kernel object (.xo file). Refer to [Compiling C/C++ PL Kernels](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Compiling-C/C-PL-Kernels) for more information.
3. `v++ -l` links the compiled kernel with the target platform and generates the FPGA binary (.xclbin file). Refer to [Linking the Kernels](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Linking-the-Kernels) for more information.
4. `v++ -p` packages the host executable, the rootfs, the FPGA binary and a few other files and generates a bootable image. Refer to [Packaging for Embedded Platforms](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Packaging-for-Embedded-Platforms) for more information.

There are two important differences to take note of here between building for Data Center accelerator cards and building for Embedded Platforms.

* The first is the use of the `v++ --package` command. This is a required step in the Embedded Processor platform flow and is largely optional in the Data Center flow. The Data Center application runs in the X86 environment, and loads the xclbin from disk. However, in the Embedded Platform flow, generally, the processor must be booted from an SD card, and the `--package` option gathers the files and generates the SD card.
* The second is the `emconfigutil` command. This command is used in the Data Center flow to build an emulation version of the hardware platform. However, in the Embedded Platform flow, the embedded processor requires the use of an emulation environment (QEMU) as described in [Simulating the Application with the Emulation Flow](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Simulating-the-Application-with-the-Emulation-Flow). In this flow, you will use a `launch_emulation` script rather than the emulation platform.

The `-t` option of the `v++` command specifies the build target. Here it is set to `sw_emu` so you are building for software emulation.

Notice also the `--config` option which is used to specify the name of a configuration file containing additional options.

```bash
save-temps=1
debug=1

# Enable profiling of data ports
[profile]
data=all:all:all
```

Building for software emulation is quick and should not take more than a few minutes. After the build process completes, you can launch the software emulation run as follows:

```bash
./package/launch_sw_emu.sh -forward-port 1440 22
```

This command will launch software emulation, start the Quick Emulation (QEMU) environment and initiate the boot sequence. Refer to [launch_emulator Utility](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/launch_emulator-Utility) for more information. 

>**IMPORTANT**: You must use the `-forward-port` option as shown above in order to later retrieve files from the QEMU environment as described in [Running Emulation on an Embedded Processor Platform](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Running-Emulation-on-an-Embedded-Processor-Platform).

When Linux has finished booting, enter the following commands from within the QEMU environment to run the example program:

```bash
cd /run/media/mmcblk0p1
export XCL_EMULATION_MODE=sw_emu
./app.exe
```

* You should see the following messages, indicating that the run completed successfully:

  ```bash
  INFO: Found Xilinx Platform
  INFO: Loading 'vadd.xclbin'
  TEST PASSED
  ```

If you look at the directory contents for the `zcu104/sw_emu` directory, you should see some of the following files that were created during this exercise:

* **app.exe**: The compiled and linked host application.
* **vadd.xclbin**: The device binary linking the kernel and target platform.
* **native_trace.csv**: A report of events occurring during the application runtime.
* **summary.csv**: A report of the application profile.
* **xrt.ini**: The runtime initilization file.
* **xrt.run_summary**: A summary report of the events of the application runtime.

These files and reports are the results of the build and run process targeting the software emulation build. You will be taking a closer look at some of these files in Part 5 of this tutorial. To examine these files later, you must retrieve them from the QEMU environment and copy them into your local system. You can do this using the `scp` command as described in [Running Emulation on an Embedded Processor Platform](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Running-Emulation-on-an-Embedded-Processor-Platform).

This command must be run from a Linux shell, outside of the QEMU environment. For example:

```bash
scp -P 1440 root@127.0.0.1:/run/media/mmcblk0p1/xrt.run_summary ./xrt.run_summary
```

Press **Ctrl+a+x** to exit QEMU and return to your bash shell.

>**TIP:** If you have trouble exiting the QEMU environment, you can use `kill -9 <qemu_pid>` to kill the process from another terminal window.

### Targeting Hardware Emulation

To build for hardware emulation, enter the following commands to setup the target build directory:

```bash
cd <Path to the cloned repo>/Getting_Started/Vitis/example/zcu102
mkdir hw_emu
cp xrt.ini hw_emu
cp run_hw_emu.sh hw_emu
cd hw_emu
```

Then, after changing into the target build directory, enter the following commands to build the host application and device binary:

```bash
$CXX -g -std=c++17 -Wall -O0 -fmessage-length=0 ../../src/host.cpp -o ./app.exe -I$SYSROOT/usr/include/xrt -LSYSROOT/usr/lib -lxrt_coreutil -pthread --sysroot=$SYSROOT
v++ -c -t hw_emu --platform xilinx_zcu102_base_202310_1 --config ../../src/zcu102.cfg -k vadd -I../../src ../../src/vadd.cpp -o ./vadd.xo 
v++ -l -t hw_emu --platform xilinx_zcu102_base_202310_1 --config ../../src/zcu102.cfg ./vadd.xo -o ./vadd.xclbin
v++ -p -t hw_emu --platform xilinx_zcu102_base_202310_1 --config ../../src/zcu102.cfg ./vadd.xclbin --package.out_dir ./package --package.rootfs ${ROOTFS}/rootfs.ext4 \
--package.sd_file ${ROOTFS}/Image --package.sd_file ./xrt.ini --package.sd_file ./app.exe --package.sd_file ./vadd.xclbin --package.sd_file ./run_hw_emu.sh
```

Refer to *Targeting Software Emulation* for a brief explanation of these different commands. The only difference with the previous step is the `v++` target (`-t`) option which is changed from `sw_emu` to `hw_emu`. All other options remain the same.

Building for hardware emulation takes more time than for software emulation, but still much less than when targeting the hardware accelerator card. After the build process completes, you can launch the hardware emulation run by using the launch script generated during the packaging step.

```bash
./package/launch_hw_emu.sh 
```

When Linux has finished booting, enter the following commands at the QEMU command prompt to run the example program:

```bash
cd /run/media/mmcblk0p1
export XCL_EMULATION_MODE=hw_emu
./app.exe
```

You should see messages that say `TEST PASSED` indicating that the run completed successfully.

Running the application in the QEMU generates some report files during the run. These files and reports are the results of the run process targeting the software emulation build. You will be taking a closer look at some of these files in Part 5 of this tutorial. To examine these files later, you must retrieve them from the QEMU environment and copy them into your local system. You can do this using the `scp` command as described in [Running Emulation on an Embedded Processor Platform](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Running-Emulation-on-an-Embedded-Processor-Platform).

This command must be run from a Linux shell, outside of the QEMU environment. For example:

```bash
scp -P 1440 root@127.0.0.1:/run/media/mmcblk0p1/xrt.run_summary ./xrt.run_summary
```

Press **Ctrl+a x** to exit QEMU and return to your bash shell.

>**TIP:** If you have trouble exiting the QEMU environment, you can use `kill -9 <qemu_pid>` to kill the process from another terminal window.

### Targeting Hardware

To build for the hardware target, enter the following commands to set up the target build directory:

```bash
cd <Path to the cloned repo>/Getting_Started/Vitis/example/zcu102
mkdir hw
cp xrt.ini hw
cp run_hw.sh hw
cd hw
```

Then, after changing into the target build directory, enter the following commands to build the host application and device binary:

```bash
$CXX -g -std=c++17 -Wall -O0 -fmessage-length=0 ../../src/host.cpp -o ./app.exe -I$SYSROOT/usr/include/xrt -LSYSROOT/usr/lib -lxrt_coreutil -pthread --sysroot=$SYSROOT
v++ -c -t hw --platform xilinx_zcu102_base_202310_1 --config ../../src/zcu102.cfg -k vadd -I../../src ../../src/vadd.cpp -o ./vadd.xo 
v++ -l -t hw --platform xilinx_zcu102_base_202310_1 --config ../../src/zcu102.cfg ./vadd.xo -o ./vadd.xclbin
v++ -p -t hw --platform xilinx_zcu102_base_202310_1 --config ../../src/zcu102.cfg ./vadd.xclbin --package.out_dir ./package --package.rootfs ${ROOTFS}/rootfs.ext4 \
--package.sd_file ${ROOTFS}/Image --package.sd_file ./xrt.ini --package.sd_file ./app.exe --package.sd_file ./vadd.xclbin --package.sd_file ./run_hw.sh
```

To target Hardware, the `v++ -t` option is set to `hw` and the `emconfigutil` command is not needed, as you will be running on an actual hardware platform rather than an emulated platform. All other options remain identical.

Building for hardware targets can take a couple of hours, or more significant time depending on the machine you are building on and its current workload.

After the build process completes, copy the `sd_card` directory to an SD card, and plug it into the platform and boot until you see the Linux prompt. At that point, enter the following commands to run the accelerated application on the Embedded Processor platform:

```bash
cd /run/media/mmcblk0p1
./app.exe
```

You should see the same TEST PASSED message indicating that the run completed successfully. If you look in the `zcu102/hw` folder you will see some of the files that were created during this build and run process. Refer to *Targeting Software Emulation* for a brief explanation of the different files.

Congratulations!! You have just completed your first run of a Vitis accelerated application on the ZCU102 card! There are additional [Vitis-Tutorials](https://github.com/Xilinx/Vitis-Tutorials) to work through to learn additional details of the Vitis tools, and [Vitis_Accel_Examples](https://github.com/Xilinx/Vitis_Accel_Examples/tree/2022.2) to use for examples of host application and kernel coding.

## Next Step

  **Click here to [Visualizing Results and Viewing Reports](./Part5.md)**

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
