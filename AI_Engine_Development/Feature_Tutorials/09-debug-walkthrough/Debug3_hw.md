<table>
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Debug Walkthrough Tutorial - From Simulation to Hardware</h1>
   </td>
 </tr>
 <tr>
 <td align="center"><h2>AI Engine Source Code Debug on Hardware</h2>
 </td>
 </tr>
</table>

The following showcase the features:

[Source Code Debug on Hardware](#Source-code-debug-on-hardware)
<!--
[Command-line project debug with Vitis IDE](#Command-line-project-debug-with-Vitis-IDE)
-->
[Limitations of Source Code Debug on Hardware](#Limitations-of-source-code-debug-on-hardware)


## Source Code Debug on Hardware

### Step 1. Setup Connection to the Hardware Board
#### Step 1.1. Target Connection Setup
##### Run hardware server from computer that connects to target board
Launch hw_server from the computer that has JTAG connection to the VCK190 board.
<img src="images/hw_ta_hw.png" width="600">

#### Step 1.2. Vitis IDE Setup
Create debug target connection from Vitis™ IDE.

<img src="images/hw_ta_con.png" width="450">
<img src="images/hw_ta_con1.png" width="450">

#### Step 1.3. Target Connection Verification
<img src="images/hw_ta_test.png" width="450">
<img src="images/hw_ta_test1.png" width="450">
<img src="images/hw_ta_test2.png" width="450">

### Step 2. Debug Project in Vitis IDE
#### Step 2.1. Target Preparation
* Make sure that sd_card is burn with expected binaries.
* Plug-in sd_card into SD card slot and power up the board.
* After board boots up, click on **Debug** to launch debugger.

<img src="images/hw_ta_sd.png">

**Note:** The `sd_card.img` at `Hardware/package/sd_card.img` is the file for regular use. `sd_card.img` at `Hardware/package_aie_debug` is for debug run on board.

#### Step 2.2. Launch debugger
Launch PS application with xclbin (or PS application expected variables), so it is ready to be debugged.
<img src="images/hw_ta_run2.png" width="450">

After PS application is running and waiting to be debugged, launch Vitis IDE debugger.
<img src="images/hw_ta_run.png">

Debugger launched, expected to see AI Engine cores, source code window that allows debug (step-into, step-through, step-return), variables view, breakpoint view, register view. Application is ready to be debugged.
<img src="images/hw_ta_run1.png">

From Vitis IDE, the application is being debugged during run time.

<img src="images/hw_ta_run3.png" width="450">

From Vitis IDE, click on **resume** for all AI Engine cores, and final “TEST PASSED” is displayed.
<img src="images/hw_ta_run4.png" width="450">

<!--
## Command-line project debug with Vitis IDE
Vitis IDE allows project-less debug that applies to command line projects. This method does not require 'project to be debugged'  migrated to Vitis IDE. It instead issues command(s) to launch Vitis IDE to help debug the intended command line projects.

### Step 1. Prepare built binary to sd card.

#### Step 1.1. Build binaries with debug capabilities.

Use this tutorial's `Makefile.hw` that adds `--package.enable_aie_debug` option in packaging step. This option inserts CDO that generates stop requests for the AI Engine cores, so that they stop at the reset vector. This option is required to add debug capabilities. Issue command `make package_dbg` with this tutorial's Makefile to package binaries that can be debugged.
Note: The packaged binaries that have debug capabilities require to run with debugger. Run without debugger will see execution hang due to wait for debugger invocation. Issue command `make package` to package binaries back without debugging capabilities.
Issue commands to build the project to be debugged.
```bash
cp Makefile.hw Makefile
make
make package_dbg
```
Note: It takes about 90 minutes or more to complete build depends on build computer's spec.

#### Step 1.2. Burn/copy `sd_card.img` to SD card.

#### Step 1.3. Plug in the SD card into the vck190 SD card slot.

#### Step 1.4. Launch tera terminal to connect vck190 board serial port.

#### Step 1.5. Power up the vck190 board.

### Step 2. Launch Vitis IDE for AI Engine only Debug
Issue this command to launch Vitis IDE.
```bash
vitis -debug -flow embedded -os baremetal -host ${HOST_IP} -launch-script ${PROJECT_PATH}/aie_app_debug_hw.tcl
```

Command option explanation:
1. `vitis -debug`: Launches the Vitis IDE in standalone debug mode.
2. `-flow embedded`: Specifies the embedded processor flow for the AI Engine processor.
3. `-os baremetal`: For baremetal OS of the AI Engine domain.
4. `-host`: Specifies which host runs hw_server.
5. `-launch_script ${aie_project}/aie_app_debug_hw.tcl`: Specifies the Tcl script which sets up the AI Engine debug environment.

Note:
1. The `aie_app_debug_hw.tcl` is provided from this tutorial, however, additional updates are required to match project specific configurations such as ${PROJECT_PATH} and ${HOST_IP}.
2. A Vitis workspace is created at the directory where this command is issued.

For more info about project-less debug command.
```bash
vitis -debug -h
```

### Step 3. Expected result
<img src="images/aie_cl_run.png">
-->

## Limitations of Source Code Debug on Hardware
1. There are maximum four breakpoints available for each tile. One of these four breakpoints is assigned to first line of main() by default and can be cleared and then assigned to other lines during debug.
2. Due to compiler, `-O0` option is not supported, non-sequential execution when stepping through source code is expected.
3. If an individual kernel is highlighted, select resume button to continue execution until next breakpoint or blocked to wait for I/O. If the beamformer design is highlighted, select resume button to resume all kernels execution until meet each kernel's breakpoint or blocked  waiting for each kernel's I/O operation.
4. Due to compiler optimization, some variables' values are stored in registers. "N/A" is shown in variables view for those optimized variables' values. Limitations from <a href="Debug2_ai.md">AI Engine debug with AIE emulator</a> applies.
5. `printf()` is not supported in hardware debug.

# Support

GitHub issues will be used for tracking requests and bugs. For questions go to [support.xilinx.com](https://support.xilinx.com/).

# License

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

<p align="center"><sup>XD005 | &copy; Copyright 2021 Xilinx, Inc.</sup></p>
