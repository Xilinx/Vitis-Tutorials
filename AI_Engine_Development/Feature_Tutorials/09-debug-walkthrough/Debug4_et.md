<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>AI Engine Debug Walkthrough Tutorial - From Simulation to Hardware</h1>
   </td>
 </tr>
 <tr>
 <td align="center"><h2>System Project Debug with Event Trace</h2>
 </td>
 </tr>
</table>

Event trace running on hardware is considered EA (Early Access) in 2020.2 release.

[Event Trace Considerations](#Event-trace-considerations)

[Event Trace Build and Generation](#Event-trace-build-and-generation)

[Event Trace Limitations](#Event-trace-limitations)


## Event Trace Considerations

### Event Trace Choice Considerations

|           | Baremetal | Petalinux | Bandwidth          | PL resources used | PS ARM used |
| :---| :---: | :---: | :---: | :--- | :--- |
| PLIO/XSDB | O         | O         | pl clock-rate * 64 | Yes               | No          |
| PLIO/XRT  |           | O         | pl clock-rate * 64 | Yes               | Yes         |

### Number of Event Trace Streams Methodology

| Number of cores | Recommended number of streams |
| :---| :---|
| Less than 10      | 1 |
| Between 10 and 20 | 2 |
| Between 20 and 40 | 4 |
| Between 40 and 80 | 8 |
| Larger than 80    | 16 |
| Intense debug     | 16 |
|                   | Not recommend using more than 16 streams due to resource constrains |

## Event Trace Build and Generation

### Step 1. Enable Event Trace Options in AI Engine Compiler
```bash
--event-trace=functions|functions_partial_stalls|functions_all_stalls|runtime
--num-trace-streams=[int], where int is number of trace streams to be used to collect generated event trace data.
--event-trace-advanced-mapping=[int], this option is considered hidden and int will be set to same as value from --num-trace-streams option.
--event-trace-bounding-box=[string], string to specify tiles are in scope to collect event trace data.
```

Issue command `aiecompiler -h` to show AI Engine Compiler options include event trace.

Makefile is currently configured to build event trace generation with `--event-trace=functions` option. This option generates events corresponding to entry into and exit out of kernel functions.

### Step 2. Event Trace Generation

#### Step 2.1. Event Trace with XSDB
```bash
xsdb
%xsdb connect -url TCP:${COMPUTER NAME/IP}:3121
%xsdb source /proj/xbuilds/2020.2_daily_latest/installs/lin64/Vitis/2020.2/scripts/vitis/util/aie_trace_profile.tcl
%xsdb aietrace::initialize ${PROJECT}/${DESIGN}.xclbin.link_summary 0x800000000 0x800000
```
Note
1. `0x800000000` is the address that needs to avoid collision with your design.
2. `0x800000` is the size of event trace file. Please adjust accordingly with your design size and amount of event trace data.

#### Step 2.2. Event Trace with XRT
Create an `xrt.ini` file on SD card using the following lines.
```bash
[Debug]
aie_trace=true
aie_trace_buffer_size=100M
```

#### Steps to Run Application after Petalinux Boots up on Board
```bash
cd /mnt/sd-mmcblk0p1
export XILINX_XRT=/usr
./ps_app.exe a.xclbin
```

### Step 3. Generate Event Trace Files

#### Step 3.1. XSDB Flow
```bash
%xsdb aietrace::offload
```
`aie_trace_N.txt` files are created and transferred to host computer that launched XSDB. Make sure those files are at same level as design's Work directory.

#### Step 3.2. XRT Flow
`aie_trace_N.txt` files are created by the SD card.
Transfer `aie_trace_N.txt` files from SD card back to where design is at same level as design's Work directory.

### Step 4. Launch Vitis Analyzer to Examine Event Trace Files
Launch Vitis Analyzer tool.
```bash
vitis_analyzer
```
Select **file->Open Summary**, then browse to the design's run_summary file.
<img src="images/et_va_init.png">
<img src="images/et_va_init1.png" width="600">

Select **Trace** from Vitis analyzer. Initially details of events are not shown.
<img src="images/et_va_init2.png">

Zoom in to see detailed information for each state of tiles.
<img src="images/et_va_run.png">

## Event Trace Limitations
1. Due to limited resources, overruns may be seen from event trace. Follow [Number of event trace streams methodology](#Number-of-event-trace-streams-methodology) to configure number of trace streams minimize overruns issue.
2. There is a time sync issue in 2020.2, where start time of each tile is increased by ~100ns or more.


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
