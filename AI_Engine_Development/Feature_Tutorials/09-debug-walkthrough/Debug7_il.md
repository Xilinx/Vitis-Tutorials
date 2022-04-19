<table>
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Debug Walkthrough Tutorial - From Simulation to Hardware</h1>
   </td>
 </tr>
 <tr>
 <td align="center"><h1>AI Engine Debug with Integrated Logic Analyzer (ILA)</h1>
 </td>
 </tr>
</table>


[AI Engine Debug with Integrated Logic Analyzer](#AI_Engine_Debug_with_Integrated_Logic_Analyzer)


# AI Engine Debug with Integrated Logic Analyzer (ILA)

Adding ChipScope helps debugging AIE PL interfaces in the design running on hardware including checking for AXI protocol violations, hardware kernel issues, data integrity, and performance issues of the design. 
To debug using ILAs, the design is required to enable ILA in hardware build.

## Enable ILA in design

This tutorial uses v++ --debug option to enable ILA IP core insertion to the design.
Syntax:
```
--debug.chipscope <cu_name>[:<interface_name>]
```
Example:
```
v++ -g -l --platform ${PLATFORM} ${XOS} ${LIBADF} \
       --debug.chipscope s2mm_v4_1:s0 \
       --debug.chipscope mm2s_v4_1:s0 \
       -t ${TARGET} ${VPP_FLAGS} -o $@
```
**Note:** v++ allows multiple --debug.chipscope lines to meet design debug needs.


### Step 1. Prepare the ILA enabled Makefile.
Use the provided Makefile.ILA from this tutorial's root directory. Rename Makefile.ILA to Makefile
```bash
cd ${DOWNLOAD_PATH}/AI_Engine_Development/Feature_Tutorials/09-debug-walkthrough
cp Makefile.ILA Makefile
```

### Step 2. Build the ILA enabled design
```bash
make clean; make
```
After the design is built. Inspect the ILA insertion with mm2s_v4_1:s0 and s2mm_v4_1:s0 interfaces.
<img src="images/il_vivado0.png" width="900">
<img src="images/il_vivado1.png" width="900">

### Step 3. Boot up the design
Flash the SD card with the ILA enabled design, plug in the flashed SD card to VCK190 sd card slot and boot up the board.


## Setup the connection with Vivado
### Step 4. Setup Connection to the Hardware Board
#### Step 4.1. Target Connection Setup
##### Run hardware server from computer that connects to target board
Launch hw_server from the computer that has JTAG connection to the VCK190 board.
<img src="images/hw_ta_hw.png" width="600">

#### Step 4.2. Launch Vivado
Launch Vivado™ from where the to be debugged project is by issue command, "vivado"  

#### Step 4.3. Setup Vivado Hardware Manager
After Vivado™ is up, click on highlighted icon.
<img src="images/il_open.png" width="900">

#### Step 4.4. Setup Hardware Target
<img src="images/il_open_2.png" width="900">

#### Step 4.5. Input Hardware Target
Enter hardware server's IP address or hostname to highlighted area and click on "Next" icon.
<img src="images/il_open_3.png" width="900">

#### Step 4.6. Check Hardware Target
Confirm target's information from JTAG chain then click on "Next" icon. 
<img src="images/il_open_4.png" width="900">

#### Step 4.7. Select probe file
Click on `...` to select the generated probe file. It is named `bf_hw.ltx` from this tutorial.
<img src="images/il_pb_file.png" width="900">

#### Step 4.8. Select ILA to be examined.
Click on `hw_ila_1' to examine the design.
<img src="images/il_pb_menu.png" width="900">

#### Step 4.9. Setup design's signals to be debugged
Click on `+` to configure the desired signals and their values during run time.
<img src="images/il_pb_trig.png" width="900">
<img src="images/il_pb_trig1.png" width="900">

#### Step 4.10. Enable ILA to capture the configured signals to debug
Click on one of highlighted icons to capture configured signals.
<img src="images/il_trig_run.png" width="900">

Wait for ILA is ready to capture signals, run the application from vck190 board.

#### Step 4.11. Expected result
<img src="images/il_run_done.png" width="900">

After completing above steps, we are able to see the captured, related AXI signals and data are shown in waveform with timing information from Vivado™.

#### Step 5. Examine captured results
<img src="images/il_run_result.png" width="900">
Review above captured result, mm2s_v4_1_s0:TVALID shows '1' indicates a valid data is available at the time marker pointed.
Moving time marker across time line, changes of AIX protocol valuses indicate values changed at what time shown below.
<img src="images/il_run_result1.png" width="900">


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
