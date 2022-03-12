<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>

# Versal Custom Thin Platform Extensible System
This tutorial describes a Versal VCK190 System Example Design based on a thin custom platform (minimal clocks and AXI exposed to PL) including HLS/RTL kernels and an AI Engine kernel using a full Makefile build-flow for Vivado™/Petalinux/Vitis 2021.2.

## Getting Started
### Build-flow
The Versal VCK190 System Example Design full Makefile build-flow builds the whole project in the following order:
```
  1. version_check: Checks if the Vivado, Petalinux and Vitis tools are setup and if the versions are 2021.2
  2. board_repo:    Downloads the board files for pre-production and es1 from the Xilinx GitHub 
  3. xsa:           Building the thin platform xsa (only pre-synth)
  4. petalinux:     Building Petalinux and sysroot
  5. xpfm:          Building the Vitis Platform
  6. bif:           Copy over necessary petalinux files to the generated software platform (required by the Vitis packager)
  7. ip:            Building Ai Engine graph(s) towards libadf.a and compiling hls/rtl kernels to *.xo
  8. ps_apps:       Building all XRT-based PS applications
  9. vitis:         Linking all kernels in the thin platform and packaging all necessary boot/run files
```
### Build-flow Dependencies
The following diagram explains the build-flow dependencies.

**Notes:**
 - The diagram should be read from right to left.
 - The diagram is for illustration only. The actual build-flow is more sequential.
<img src="./documentation/readme_files/Design_dependencies.svg">
 
### Build & Prerequisites
In the `[project-root]` you can start the full build with `make all` **after** taking following prerequisites into account:
  - **Before starting the build, please correctly setup the 2021.2 version of Vivado, Petalinux and Vitis**
    - If the tools are not setup correctly the build will stop with an ERROR message telling you what is not correctly setup!
    - Be sure that the 2021.2 Y2K22 patch is installed (This is not verified)! 
      - More info can be found here: https://support.xilinx.com/s/article/76960?language=en_US
  - Everything is in the GitHub repository; no extra files are needed. Although some are downloaded from GitHub.
  - `[project-root]/Makefile`: `export DEVICE_NAME := xcvc1902-vsva2197-2MP-e-S` for pre-production board version (default); **change it to `export DEVICE_NAME := xcvc1902-vsva2197-2MP-e-S-es1` for es1 board version**.
  - `[project-root]/Makefile`: `export TARGET := hw` for targetting a VCK190 (default); **change it to `export TARGET := hw_emu` to target hardware emulation**.
    - The build flow supports both TARGET's in the same `[project-root]`; but you need to execute them yourself the one after the other if you need both results!
    - Some generated directories are depending on the TARGET and are further shown as `[dir]_${TARGET}`.
  - `[project-root]/Makefile`: `export ILA_EN := 0` for disabling the ILA (default); **change it to `export ILA_EN := 1` to enable the ILA**.
    - Remark: When building **vitis** with `export TARGET := hw_emu` ILA_EN will be forced to `ILA_EN = 0` (ILA Disabled) in the `[project-root]/vitis/Makefile`. There is **NO** issue to first build everything with `export TARGET := hw` and `export ILA_EN := 1` and afterwards ONLY (re-)build vitis with `export TARGET := hw_emu`! **NO** need for a full (re-)build in that respect! 
  - `[project-root]/Makefile`: `export ETH_STATIC := 0` for Ethernet DHCP Configuration (default); **change it to `export ETH_STATIC := 1` for Ethernet Static Configuration**.
    - `export ETH_STATIC := 0`; **NO** extra configuration needed.
    - `export ETH_STATIC := 1`; please setup your required Ethernet Static Configuration in `[project-root]/petalinux/src/init-ifupdown/interfaces`.
  - `[project-root]/petalinux/Makefile`: the generated tmp-dir ends up in `/tmp`. 
    - If you want to place it somewhere else you need to add `--tmpdir [your_tmp_dir]` at the end of the `petalinux-create` line in the `[project-root]/petalinux/Makefile`. 
      - Be aware that `[your_tmp_dir]` may **NOT** be located on an NFS-drive!
  - End result: 
    - `export TARGET := hw`: `[project-root]/package_output_hw/sd_card/*` can be used for FAT-32 SD-card (partition); or `[project-root]/package_output_hw/sd_card.img` can be used.
    - `export TARGET := hw_emu`: `[project-root]/package_output_hw_emu/launch_hw_emu.sh` can be used to launch the hardware emulation.

### Custom Thin Base Platform
The GENERATED Custom Thin Base Platform BD:
<img src="./documentation/readme_files/base_platform_bd.png">

It exposes 5 clocks that can be used in the `[project-root]/vitis/src/system.cfg`:

  ```
  [clock]
  #id=0 -> clk_out1_o1 -> 500.00MHz
  id=0:subtractor_0,counter_0

  #id=1 -> clk_out1_o2 -> 250.00MHz
  id=1:vadd_mm_1

  #id=2 -> clk_out1_o3 -> 125.00MHz

  #id=3 -> clk_out1_o4 -> 62.50MHz

  #id=4 -> clk_out2 -> 333.33MHz
  id=4:vadd_s_1,mm2s_vadd_s_1,mm2s_vadd_s_2,s2mm_vadd_s_1
  ``` 

Vitis will nicely add all required ip and connections depending on the added kernels and their needed clocks/resets onto this GENERATED Custom Thin Base Platform.
 
## More In-Depth
The following explains the different sub-build steps. Click on each item for more detailed information.  
Each step is sequential (in the order listed - by the `[project-root]/Makefile`): 

<details>
 <summary> make version_check </summary>
 
 -  Checks if the Vivado, Petalinux and Vitis tools are setup and if the versions are 2021.2.
 
</details>
<details>
 <summary> make platform/hw/board_repo </summary>
 
 - Downloads all pre-production and es1 board files from the Xilinx GitHub.
 
</details>
<details>
 <summary> make xsa -C platform </summary>
 
`[project-root]/platform` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|--------------------------------------------------------------
| Makefile            | The platform xsa/xpfm Makefile                                  
| hw/*                | The hardware platform Makefile and sources

 - Builds the output file needed for Petalinux and Vitis software platform creation -> `[project-root]/platform/hw/build/vck190_thin.xsa`.
 - After this step you could open the platform blockdesign in Vivado for review:
   - cd `[project-root]/platform/hw/build/vck190_thin_vivado`
   - vivado `vck190_thin.xpr`
 
</details>
<details>
  <summary> make all -C petalinux </summary>

`[project-root]/petalinux` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|--------------------------------------------------------------
| Makefile            | The Petalinux Makefile                                  
| src/config          | A script used to exchange/add petalinux configuration items
| src/device-tree/*   | Some device-tree changes needed for VCK190              
| src/boot_custom.bif | bif file needed to have a correct BOOT.BIN in the Vitis packager   

 - Builds all required Petalinux images which end up in `[project-root]/petalinux/linux/images/linux`.
 - It also builds a `sysroot` which ends up in `[project-root]/petalinux/sysroot` (needed for `[project-root]/ps_apps` build).
 
</details>
<details>
  <summary> make xpfm -C platform </summary>

`[project-root]/platform` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|--------------------------------------------------------------
| Makefile            | The platform xsa/xpfm Makefile                                  
| sw/*                | The Vitis platform Makefile and sources

 - Builds the output files needed for ip and Vitis -> `[project-root]/platform/sw/build/vck190_thin/export/vck190_thin/vck190_thin.xpfm` and some generated subfolders.
 
</details>
<details>
  <summary> make bif </summary>

 - Copies over some Petalinux-generated files to the software platform. These are necessary for a correct Vitis-build.
 
</details>
<details>
  <summary> make all -C ip </summary>
 
`[project-root]/ip` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|--------------------------------------------------------------
| Makefile            | The ip generic Makefile; it automatically searches for sub-projects to build                             
| aie/*               | aie "datamover" kernel Makefile and sources
| counter/*           | free-running RTL "counter" kernel Makefile and sources that feeds the aie "datamover" kernel
| subtractor/*        | Managed RTL "subtractor" kernel Makefile and sources that measures the delay between the counter-input and the aie-output
| vadd_mm/*           | XRT-controlled HLS vadd memory mapped kernel Makefile and sources
| vadd_s/*            | XRT-controlled HLS vadd streaming kernel Makefile and sources
| mm2s_vadd_s/*       | XRT-controlled HLS memory-to-streaming kernel used with vadd_s Makefile and sources
| s2mm_vadd_s/*       | XRT-controlled HLS streaming-to-memory kernel used with vadd_s Makefile and sources

 - Builds the output files needed for Vitis linker -> `[project-root]/ip/aie/libadf.a` and `[project-root]/ip/xo_${TARGET}/*.xo`
 - Kernel structure/flow:
    - vadd_mm is a memory mapped kernel
    - mm2s_vadd_s -> vadd_s -> s2mm_vadd_s
    - counter -> aie "datamover" -> subtractor
    
</details>
<details>
  <summary> make all -C ps_apps </summary>

`[project-root]/ps_apps` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|---------------------------------------------------------
| Makefile            | The ps_apps generic Makefile; it automatically searches for sub-projects to build
| aie_dly_test/*      | PS XRT Application - using the native XRT API - Makefile and sources that measures the delay between counter-input and aie-output
| vadd_mm_cpp/*       | PS XRT Application - using the native XRT API - Makefile and sources that checks out the vadd_mm kernel
| vadd_mm_ocl/*       | PS XRT Application - using the opencl XRT API - Makefile and sources that checks out the vadd_mm kernel
| vadd_s/*            | PS XRT Application - using the native XRT API - Makefile and sources that checks out the mm2s_vadd_s -> vadd_s -> s2mm_vadd_s kernels

 - Builds the output files needed for vitis packager -> `[project-root]/ps_apps/exe/*.exe`
 
</details>
<details>
  <summary> make all -C vitis </summary>

`[project-root]/vitis` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|---------------------------------------------------------
| Makefile            | The Vitis generic Makefile for linker and packager
| src/system.cfg      | Vitis connection and clock configuration needed for Vitis linker
| src/ila_0_bd.cfg    | ILA Vitis connection needed for Vitis linker when `ILA_EN = 1`
| src/ila_0_def.tcl   | ILA Vitis tcl needed for Vitis linker when `ILA_EN = 1`

 - Runs the Vitis linker and packager
 - The output of the Vitis packager ends up in `[project-root]/package_output_${TARGET}`
 - After this step you could open the full blockdesign (platform extended with all kernels) in Vivado for review:
   - cd `[project-root]/vitis/build_${TARGET}/_x/link/vivado/vpl/prj`
   - vivado `prj.xpr`
 
</details>

## Testing
### Running on a VCK190
  1. Prerequisite: Build was executed with `export TARGET := hw` and for your correct VCK190 version (pre-production or es1)
  2. Copy over the `[project-root]/package_output_hw/sd_card/*` to an SD-card or put the `[project-root]/package_output_hw/sd_card.img` on an SD-card.
  3. Put the SD-card in the VCK190 Versal SD-card slot (VCK190 top SD-card slot closest to the bracket).
  4. Connect the included USB-cable between the VCK190 (Middle bottom of the bracket) and a computer:
     - Usually you will see 3 serial ports in your device manager:
       - One for the ZU04 system controller device.
       - Two for Versal; however only one of the Versal serial ports are in use.
       - To see the serial ports, the VCK190 does not need to be powered-ON, the physical USB connection should be enough!
     - Connect to the serial port(s) by using a terminal emulator like Putty (Windows) with the following settings:
       - 115200 baud
       - 8 data bits
       - 1 stop bit
       - Parity none
       - Flow control XON/XOFF
     - Maybe for the first time open all 3 serial ports to see which one is the correct Versal serial port where you can follow the Versal-boot and interact later on.
  5. Power-UP:
     - It will first boot-up up the ZU04, next it will start the Versal boot. 
     - Only one of the Versal serial ports will give you the Linux command line prompt after booting.
     - No Password is needed for linux login.
  6. Continue to "Execution & Results"

### Running Hardware Emulation
  1. Prerequisite: Build was executed with `export TARGET := hw_emu`
  2. Execute `source ./launch_hw_emu.sh` in `[project-root]/package_output_hw_emu/`.
  3. The hardware emulation will start and boot-up the whole system and should give you the Linux command line prompt.
     - This can take some time!
     - No password is needed for linux login.
  4. Continue to "Execution & Results"

### Execution & Results
  - Execute the following after boot-up when you reached the Linux command line prompt:
    - In the logging below you find all results/responses that you should get after every Linux command line input you should give.
  
 ```
root@linux:~# cd /media/sd-mmcblk0p1/
root@linux:/media/sd-mmcblk0p1# ./vadd_s.exe a.xclbin
INFO:    samples = 256
INFO:    bsize   = 512
PASSED:  auto my_device = xrt::device(0)
XAIEFAL: INFO: Resource group Avail is created.
XAIEFAL: INFO: Resource group Static is created.
XAIEFAL: INFO: Resource group Generic is created.
PASSED:  auto xclbin_uuid = my_device.load_xclbin(a.xclbin)
PASSED:  auto in_0 = xrt::kernel(my_device, xclbin_uuid, "mm2s_vadd_s:{mm2s_vadd_s_1}")
PASSED:  auto in_1 = xrt::kernel(my_device, xclbin_uuid, "mm2s_vadd_s:{mm2s_vadd_s_2}")
PASSED:  auto in_01_bo = xrt::bo(my_device, bsize, XCL_BO_FLAGS_NONE, in_01.group_id(0))
PASSED:  auto in_01_bo_mapped = = in_01_bo.map<TYPE_DATA*>()
PASSED:  in_01_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE)
PASSED:  auto in_01_run = in_01(in_01_bo, nullptr, 256)
PASSED:  auto out = xrt::kernel(my_device, xclbin_uuid, "s2mm_vadd_s:{s2mm_vadd_s_1}")
PASSED:  auto out_bo = xrt::bo(my_device, bsize, XCL_BO_FLAGS_NONE, out.group_id(0))
PASSED:  auto out_bo_mapped = out_bo.map<TYPE_DATA*>()
PASSED:  auto out_run = out(out_bo, nullptr, 256)
PASSED:  dut = xrt::kernel(my_device, xclbin_uuid, "vadd_s:{vadd_s_1}")
PASSED:  dut_run = dut(256, nullptr, nullptr, nullptr)

INFO:    Waiting for kernels to end...

PASSED:  in_0_run.wait()
PASSED:  in_1_run.wait()
PASSED:  dut_run.wait()
PASSED:  out_run.wait()
PASSED:  out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE)

PASSED:  ./vadd_s.exe

root@linux:/media/sd-mmcblk0p1# ./vadd_mm_cpp.exe a.xclbin
PASSED:  auto my_device = xrt::device(0)
XAIEFAL: INFO: Resource group Avail is created.
XAIEFAL: INFO: Resource group Static is created.
XAIEFAL: INFO: Resource group Generic is created.
PASSED:  auto xclbin_uuid = my_device.load_xclbin(a.xclbin)
PASSED:  auto my_vadd = xrt::kernel(my_device, xclbin_uuid, "vadd_mm:{vadd_mm_1}")

PASSED:  ./vadd_mm_cpp.exe

root@linux:/media/sd-mmcblk0p1# ./vadd_mm_ocl.exe a.xclbin
Loading: 'a.xclbin'
XAIEFAL: INFO: Resource group Avail is created.
XAIEFAL: INFO: Resource group Static is created.
XAIEFAL: INFO: Resource group Generic is created.

PASSED:  ./vadd_mm_ocl.exe

root@linux:/media/sd-mmcblk0p1# ./aie_dly_test.exe a.xclbin
Initializing ADF API...
PASSED:  auto my_device = xrt::device(0)
XAIEFAL: INFO: Resource group Avail is created.
XAIEFAL: INFO: Resource group Static is created.
XAIEFAL: INFO: Resource group Generic is created.
PASSED:  auto xclbin_uuid = my_device.load_xclbin(a.xclbin)
PASSED:  auto my_rtl_ip = xrt::ip(my_device, xclbin_uuid, "subtractor:{subtractor_0}")
PASSED:  auto my_graph  = xrt::graph(my_device, xclbin_uuid, "mygraph_top")
PASSED:  my_graph.reset()
PASSED:  my_graph.run()
Poll subtractor register
  Value Reg0:  220
  Value Reg1:  11c
  Value Reg2:  175
  Value Reg3:  36
Poll subtractor register
  Value Reg0:  1ee
  Value Reg1:  11c
  Value Reg2:  173
  Value Reg3:  38
Poll subtractor register
  Value Reg0:  1f0
  Value Reg1:  118
  Value Reg2:  174
  Value Reg3:  3a
Poll subtractor register
  Value Reg0:  22c
  Value Reg1:  11a
  Value Reg2:  173
  Value Reg3:  38
Poll subtractor register
  Value Reg0:  1ee
  Value Reg1:  11a
  Value Reg2:  174
  Value Reg3:  38
Poll subtractor register
  Value Reg0:  23e
  Value Reg1:  118
  Value Reg2:  172
  Value Reg3:  3a
Poll subtractor register
  Value Reg0:  212
  Value Reg1:  118
  Value Reg2:  185
  Value Reg3:  3a
Poll subtractor register
  Value Reg0:  240
  Value Reg1:  118
  Value Reg2:  170
  Value Reg3:  3a
Poll subtractor register
  Value Reg0:  23e
  Value Reg1:  116
  Value Reg2:  175
  Value Reg3:  36
Poll subtractor register
  Value Reg0:  1ec
  Value Reg1:  11a
  Value Reg2:  175
  Value Reg3:  3a
Poll subtractor register
  Value Reg0:  24a
  Value Reg1:  118
  Value Reg2:  170
  Value Reg3:  38
Poll subtractor register
  Value Reg0:  21a
  Value Reg1:  11c
  Value Reg2:  171
  Value Reg3:  36
Poll subtractor register
  Value Reg0:  1ee
  Value Reg1:  116
  Value Reg2:  173
  Value Reg3:  3a
Poll subtractor register
  Value Reg0:  1ea
  Value Reg1:  11a
  Value Reg2:  172
  Value Reg3:  38
Poll subtractor register
  Value Reg0:  23e
  Value Reg1:  116
  Value Reg2:  173
  Value Reg3:  38
Poll subtractor register
  Value Reg0:  1ee
  Value Reg1:  11a
  Value Reg2:  170
  Value Reg3:  38
Poll subtractor register
  Value Reg0:  214
  Value Reg1:  11c
  Value Reg2:  170
  Value Reg3:  38
Poll subtractor register
  Value Reg0:  204
  Value Reg1:  11c
  Value Reg2:  177
  Value Reg3:  38
Poll subtractor register
  Value Reg0:  214
  Value Reg1:  118
  Value Reg2:  172
  Value Reg3:  3a
Poll subtractor register
  Value Reg0:  24c
  Value Reg1:  118
  Value Reg2:  173
  Value Reg3:  34
PASSED:  my_graph.end()

PASSED:  ./aie_dly_test.exe

root@linux:/media/sd-mmcblk0p1#
  ```

## Notes
  - xsa: CIPS settings are added manually; configured in the bd-files.
  - The example design is fully FAT-32 
    - if you like to use ext4 rootfs instead: 
      - petalinux already generates it.
      - You will need to copy it to the Vitis platform in the `[project-root]/Makefile` in the `bif` section.
      - The `v++ -p` command line in `[project-root]\vitis\Makefile` will need adaptations to be able to use ext4 rootfs instead of FAT-32.
  - `export ILA_EN := 1`
    - The ILA core connectivity is set up during v++ linking process loading the cfg file `[project-root]/vitis/src/ila_0_bd.cfg` and further configuration of ILA properties is managed in tcl file `[project-root]/vitis/src/ila_0_def.tcl`.
    - Using the configuration file `[project-root]/vitis/src/ila_0_bd.cfg` allows the designer to mark AXI port for debug nets to and from the AIE engine for analysis. 
    - After completing the linking process, the designer can verify conectivity and configuration of the ILA core in the generated block design in project `[project-root]/vitis/build_${TARGET}/_x/link/vivado/vpl/prj/prj.xpr`.
    - Once the build process is completed and petalinux boots on your board, it is required to manually set the path for probe file `[project-root]/package_output/probe_0.ltx` in the Vivado Hardware Manager to load the ILA core if this was enabled. 
    - A quick use case would be to validate the values of subtractor registers. After the probing file is loaded and the ILA is armed, re-running `./aie_dly_test.exe a.xclbin` will trigger the ILA capturing the signal values that should match those in the console.
  - Simulation is **NOT** part and **NOT** demonstrated in this Tutorial!
  
## Design Considerations
  Note: The **MUST**'s in below explanations are due to how the generic Makefiles are setup, and is **NOT** a Xilinx tools requirement!
  - `[project-root]/ps_apps`: PS applications can easily be added by adding a sub-project for each in `[project-root]/ps_apps/`.
    - Vitis will automatically package them and they will end up in `[project-root]/package_output_${TARGET}`.
    - The `[PS Application].exe` (extension **MUST** be .exe) **MUST** end up in the `[project-root]/ps_apps/exe` dir.
  - `[project-root]/ip`: Kernels can be added by just adding a sub-project in `[project-root]/ip`.
    - **You will need to update the `[project-root]/vitis/src/system.cfg` file to setup the correct connections/clocks.**
    - A `[kernel].xo` file **MUST** end up in the `[project-root]/ip/xo_${TARGET}` dir
    - An extra aie graph **MUST** be added in the `[project-root]/ip/aie` dir, the `[project-root]/ip/aie/Makefile` will need adaptations.

## References
The following documents provide supplemental information for this tutorial.

### [AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2021_2/vitis_doc/yii1603912637443.html)
Contains sections on how to develop AI Engine graphs, how to use the AI Engine compiler, AI Engine simulation, and performance analysis.

### [Xilinx Runtime (XRT) Architecture](https://xilinx.github.io/XRT/master/html/index.html)
The following are links to the XRT information used by this tutorial: 

* [XRT Documentation](https://xilinx.github.io/XRT/master/html/index.html): Explains general XRT API calls used in the PS Host Application. 

* [XRT Github Repo](https://github.com/Xilinx/XRT): Contains the XRT source code. 

* [XRT AIE API](https://github.com/Xilinx/XRT/blob/master/src/runtime_src/core/include/experimental/xrt_aie.h): Documents the AI Engine XRT API calls

### [Vitis Unified Software Development Platform 2021.2 Documentation](https://www.xilinx.com/html_docs/xilinx2021_2/vitis_doc/index.html)
The following are links to Vitis related information referenced in this tutorial:

* [Vitis Application Acceleration Development Flow Documentation](https://www.xilinx.com/html_docs/xilinx2021_2/vitis_doc/kme1569523964461.html)

* [Vitis Application Acceleration Development Flow Tutorials](https://github.com/Xilinx/Vitis-Tutorials)

* [Vitis HLS](https://www.xilinx.com/html_docs/xilinx2021_2/vitis_doc/irn1582730075765.html)

## Revision History
Click on each item below to see the detailed Revision History:

<details>
  <summary> March 2022 </summary>
 
  - platform:
    - Removed some ip and connections to end-up with the bare minimum base platform needed
      - Vitis nicely adds and changes IP and connections as required
    - Exposed 1 extra clock via BUFG (clk_out2) next to the 4 exisiting clocks via MBUFGCE (clk_out1_o1-4)
      - id=0 -> clk_out1_o1 -> 500MHz
      - id=1 -> clk_out1_o2 -> 250MHz
      - id=2 -> clk_out1_o3 -> 125MHz
      - id=3 -> clk_out1_o4 -> 62.5MHz
      - id=4 -> clk_out2    -> 333MHz
  - ip:
    - Added vadd streaming kernels: mm2s_vadd_s -> vadd_s -> s2mm_vadd_s
    - Renamed vadd to vadd_mm (vadd memory mapped kernel)
  - ps_apps:
    - Added vadd_s to check the vadd_s streaming kernels
    - Renamed vadd_cpp and vadd_ocl to vadd_mm_cpp and vadd_mm_ocl to check the vadd memory mapped kernel
    - Using explicit "kernel:{kernel_id}" for kernel selection
    - Streamline print out messages
  - vitis:
    - Added vitis/src/system.cfg as dependency for the Vitis Linker
    - Added vitis/src/ila_0_bd.cfg as dependency for the Vitis Linker when build with 'ILA_EN=1'
    - Disable ILA when building vitis with 'TARGET=hw_emu'
    - counter and subtractor kernels running @ 500MHz (id=0 -> clk_out1_o1) 
    - vadd_mm (vadd memory mapped kernel) running @ 250MHz (id=1 -> clk_out1_o2)
    - mm2s_vadd_s -> vadd_s -> s2mm_vadd_s (vadd_s streaming kernels) running @ 333MHz (id=4 -> clk_out2)
  - general:
    - Added "make clean_vitis" to be able to clean everything (ip, ps_apps, vitis) after the (fixed) platform
    - Makefiles: Streamlining and finetuning
 
</details>

<details>
  <summary> Februari 2022 </summary>

 - petalinux:
    - Added the option ETH_STATIC to setup static Ethernet Configuration
  - ip:
    - Bugfix for the v++ linker to fail in TARGET=hw_emu when other HLS-kernels (like mm2s) are added later on to the design
  - general:
    - Adding the option to build for TARGET=hw_emu
 
</details>

<details>
  <summary> October 2021 </summary>

 - petalinux:
    - Improved petalinux version check
  - ip/aie: 
    - Optimized AI Engine Datamovers
  - general: 
    - Added more clarifications in the README.md
 
</details>

<details>
  <summary> September 2021 - Initial Release </summary>
 
 </details>

 
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

<p align="center"><sup>XD106 | © Copyright 2021 Xilinx, Inc.</sup></p>
