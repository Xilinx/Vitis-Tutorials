<table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Debug Walkthrough Tutorial - From Simulation to Hardware</h1>
   </td>
 </tr>
 <tr>
 <td align="center"><h2>AI Engine Source Code Debug with Hardware Emulator</h2>
 </td>
 </tr>
</table>

A system project includes PS, PL, and AI Engine development and additionally a system wide configuration that requires to be configured correctly. The hardware emulator supports the system project debug needs and is capable of debug the PS and AI Engine domains. Currently, PL domain debug is not supported.

The following showcase features of the hardware emulator:

[Hardware Emulation Source Code Debug](#hardware-emulation-source-code-debug)

* [Hardware emulation source code debug limitations](#hardware-emulation-source-code-debug-limitations)

[Command-Line Project Source Code Debug with Hardware Emulator](#command-line-project-source-code-debug-with-hardware-emulator)

## Hardware Emulation Source Code Debug

### Step 1: Build

Select the build configuration to be **Emulation-HW**.

![alt text](images/he_run_cfg.png)

Highlight the beamformer system project and right-click to select **build project**. It takes over 60 minutes to complete the build.

### Step 2: Launch Hardware Emulator

![alt text](images/he_run_init.png)

>**NOTE:** It takes a couple of minutes to launch the emulator and boot up Petalinux.

### Step 3: During Debug

#### Step 3.1: Debug - PS

![alt text](images/he_run_ps.png)

#### Step 3.2: Debug - AI Engine

![alt text](images/he_run_aie.png)

AI Engine only debugs within system project via hardware emulator, AI Engine emulator debug still applies. [AI Engine debug with AIE emulator](./Debug2_ai.md) are applicable at this condition.

## Hardware Emulation Source Code Debug Limitations

1. Limitations from [AI Engine debug with AIE emulator](./Debug2_ai.md) applies.
2. PL debug is not supported from the Vitis IDE.

## Command Line Project Source Code Debug with Hardware Emulator

Use cases that involve using the Vitis IDE functionalities without migrating to Vitis IDE are permitted.

### Step 1: Build the Project to be Debugged

If the project is not built already, use this tutorial's `Makefile.emu` that has the `--package.enable_aie_debug` option in the packaging step. This option insert a configuration data object (CDO) that generates stop requests for the AI Engine cores, so that they stop at the reset vector. This option is required to add debug capabilities. Issue the command `make package_dbg` with this tutorial's `Makefile.emu` to package binaries that can be debugged.
>**NOTE:** The packaged binaries that have debug capabilities require to run with debugger. Running without dthe ebugger will see execution a hang due to the wait for the debugger invocation. Run the command `make package` to package back regular binaries.

```bash
cp Makefile.emu Makefile
make
make package_dbg
```

### Step 2: Launch the Hardware Emulator and Boot Petalinux

From terminal 1, set up the tool path properly, and issue this command to launch the hardware emulator and boot up Petalinux.

```bash
./launch_hw_emu.sh -add-env ENABLE_RDWR_DEBUG=true -add-env RDWR_DEBUG_PORT=10100 -pid-file emulation.pid -no-reboot -forward-port 1440 1534
```

Command option explanation:

1. `-add-env RDWR_DEBUG_PORT=${aie_mem_sock_port}`: Defines the port for communicating with the AI Engine domain. In the previous example, it is 10100.
2. `-forward-port ${linux_tcf_agent_port} 1534`: Defines the port for the Linux TCF agent. In the previous example, it is 1440, which is the default.

>**NOTE**:
>
>1. `launch_hw_emu.sh` is generated properly when the project under debug is built and packaged with the hardware emulator correctly. Update the repository's `Makefile` line 3 from `TARGET = hw` to `TARGET = hw_emu`.
>2. The previous command takes a few minutes to complete because both the hardware emulator and Petalinux are required to boot up properly.
>3. Wait until both the hardware server and Petalinux boot up BEFORE moving to the next steps.

### Step 3: Launch the Vitis IDE and XRT server
From terminal 2, setup tool path properly and issue this command to launch the Xilinx Runtime (XRT) server and Vitis IDE.

```bash
vitis -debug -flow embedded_accel -target hw_emu -exe ./host.exe -program-args ./a.xclbin -port 1440
xrt_server -I30000 -S -s tcp::4352
```

Command option explanation:

1. `vitis -debug`: Launches the Vitis IDE in stand-alone debug mode.
2. `-flow embedded_accel`: Specifies the embedded processor application acceleration flow.
3. `-target hw_emu`: Indicates the target build being debugged.
4. `-exe ./ps_app`: Indicates the PS application to run and debug.
5. `-program-args ${xcl_bin_dir}/binary_container_1.xclbin`: Refers to the location of the `xclbin` file to be loaded as an argument to the executable.
6. `-port 1440`: Specifies the `${linux_tcf_agent_port}` as discussed previously.
7. `-I30000`: Defines an idle timeout in seconds, in which the server will quit if there is no response.
8. `-S`: Specifies the print server properties in JSON format to stdout.
9. `-s tcp::${xrt_server_port}`: Defines the agent listening protocol and port. It is 4352 in the example but can be any free port.

Expected result:
![alt text](images/he_temp_run.png)

>**NOTE:** The previously listed step takes a couple of minutes to complete.

### Step 4: Set Up the Vitis AI Engine Debug Configuration

#### Step 4.1: Configure Debug Using the Click-on Debug Icon

![alt text](images/he_cl_config0.png)

#### Step 4.2: Under "Single Application Debug" to Create a New Debug Configuration

![alt text](images/he_cl_config1.png)

#### Step 4.3: Set Up the New Debug Type Name and Connection

![alt text](images/he_cl_config2.png)

#### Step 4.4: Set Up the New Connection Name, Host, and Port

![alt text](images/he_cl_config3.png)

#### Step 4.5: Set Up to Execute the Script of the Newly Created Debug Type

![alt text](images/he_cl_config4.png)

>**NOTE:** The script, `aie_app_debug_em.tcl` is provided in this tutorial and needs to be updated to match your environment settings. The `aie_work_dir` variable should be in the Work folder inside this lab – e.g., `set aie_work_dir "${PROJECT_PATH}/beamformer/Work/"` and `set vitis_install "${XILINX_VITIS_PATH}/Xilinx/Vitis/2022.1"`.

#### Step 4.6: Close the Debug Configuration

![alt text](images/he_cl_config5.png)

#### Step 4.7: Run the PS (by resume) Application so AI Engine Can be Initialized and Debugged

![alt text](images/he_cl_config6.png)

#### Step 4.8: Wait until the PS Has Initialized AI Engine and Observe "Starting graph run" From the Console

![alt text](images/he_cl_config6_1.png)

#### Step 4.9: Launch the AI Engine Debugger to Debug the AI Engine Sub-project

![alt text](images/he_cl_config0.png)
![alt text](images/he_cl_config7.png)

>**NOTE:**
>
>1. The script, `aie_app_debug_em.tcl`, is provided in this tutorial and needs to be updated to match your environment settings. The `aie_work_dir` variable should be the Work folder inside this lab. For example, `set aie_work_dir ${PROJECT_PATH}/beamformer/Work/` and `set vitis_install ${XILINX_VITIS_PATH}/Xilinx/Vitis/2022.1`.
>2. Step 4.7 requires to start the PS application BEFORE launching the debugger to debug the AI Engine sub-project. You will see issues if the above step sequence is not followed.
>3. It takes some time depending on number of times in design to launch AI Engine debugger completely. The beamformer design contains 64 tiles that needs several minutes to complete.

### Step 5: Expected Vitis IDE Result

![alt text](images/he_cl_run.png)

### Step 6: Clean Up the Launched Processes

```bash
killall -9 pllauncher; killall -9 qemu-system-aarch64; killall -9 xrt_server
```

These commands kill leftover processes for this debug run.

# Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](https://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
