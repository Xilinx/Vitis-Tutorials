<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>

# Versal Custom Thin Platform Extensible System
This tutorial describes a Versal VCK190 System Example Design based on a thin custom platform (minimal clocks and AXI exposed to PL) including HLS/RTL kernels and an AI Engine kernel using a full Makefile build-flow for Vivado™/Petalinux/Vitis 2021.2.

## Getting Started
The Versal VCK190 System Example Design full Makefile build-flow builds the whole project in the following order:
```
  1. version_check: Checks if the Vivado, Petalinux, and Vitis version is 2021.2
  2. board_repo:    Downloads the board files for es1 and pre-production from the Xilinx GitHub 
  3. xsa:           Building the thin platform xsa (only pre-synth)
  4. petalinux:     Building Petalinux and sysroot
  5. xpfm:          Building the Vitis Platform
  6. bif:           Copy over necessary petalinux files to the generated software platform (required by the Vitis packager)
  7. ip:            Building Ai Engine graph(s) towards libadf.a and compiling hls/rtl kernels to *.xo
  8. ps_apps:       Building all XRT-based PS applications
  9. vitis:         Linking all kernels in the thin platform and packaging all necessary sd_card files
```
The following diagram explains the build-flow dependencies.

**Notes:**
 - The diagram should be read from right to left.
 - The diagram is for illustration only. The actual build-flow is more sequential.
<img src="./documentation/readme_files/Design_dependencies.svg">

In the `[project-root]` you can start the full build with `make all` after setting up the 2021.2 version of Vivado, Petalinux and Vitis:
  - Everything is in the GitHub repository; no extra files are needed.
  - In the `[project-root]/Makefile`: "export DEVICE_NAME := xcvc1902-vsva2197-2MP-e-S-es1" (for es1 version); **change it to xcvc1902-vsva2197-2MP-e-S (for pre-production version)**.
  - `[project-root]/petalinux/Makefile`: the generated tmp-dir ends up in `/tmp`. 
    - If you want to place it somewhere else you need to add `--tmpdir [your_tmp_dir]` at the end of the `petalinux-create` line in the `[project-root]/petalinux/Makefile`. 
      - Be aware that this may not be located on an NFS-drive! 
  - End result: `[project-root]/package_output/sd_card/*` can be used for FAT-32 sd_card (partition); or `[project-root]/package_output/sd_card.img` can be used.

## More In-Depth
The following explains the different sub-build steps. Click on each item for more detailed information.  
Each step is sequential (in the order listed - by the `[project-root]/Makefile`): 

<details>
 <summary> make version_check </summary>
 
 -  Checks if the Vivado, Petalinux, and Vitis versions are 2021.2.
 
</details>
<details>
 <summary> make platform/hw/board_repo </summary>
 
 - Retrieves all es1 and pre-production board files from the Xilinx GitHub.
 
</details>
<details>
 <summary> make xsa -C platform </summary>
 
`[project-root]/platform` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|--------------------------------------------------------------
| Makefile            | The platform xsa/xpfm Makefile                                  
| hw/*                | The hardware platform Makefile and sources

 - Builds the output file needed for Petalinux and Vitis software platform creation -> `[project-root]/platform/hw/build/vck190_thin.xsa`.
 - After this step you could open the platform blockdesign in Vivado for review
 
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
| vadd/*              | XRT-controlled HLS "vadd" kernel Makefile and sources

 - Builds the output files needed for Vitis linker -> `[project-root]/ip/aie/libadf.a` and `[project-root]/ip/*.xo`
 - Kernel structure/flow:
    - vadd is a separate kernel
    - counter -> aie "datamover" -> subtractor
    
</details>
<details>
  <summary> make all -C ps_apps </summary>

`[project-root]/ps_apps` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|---------------------------------------------------------
| Makefile            | The ps_apps generic Makefile; it automatically searches for sub-projects to build
| aie_dly_test/*      | PS XRT Application - using the native XRT API - Makefile and sources that measures the delay between counter-input and aie-output
| vadd_cpp/*          | PS XRT Application - using the native XRT API - Makefile and sources that checks out the vadd kernel
| vadd_ocl/*          | PS XRT Application - using the opencl XRT API - Makefile and sources that checks out the vadd kernel

 - Builds the output files needed for vitis packager -> `[project-root]/ps_apps/*.exe`
 
</details>
<details>
  <summary> make all -C vitis </summary>

`[project-root]/vitis` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|---------------------------------------------------------
| Makefile            | The Vitis generic Makefile for linker and packager
| src/system.cfg      | Vitis connection and clock configuration needed for Vitis linker

 - Runs the Vitis linker and packager
 - The output of the Vitis packager ends up in `[project-root]/package_output`
   - `[project-root]/package_output/sd_card.img` or you find all FAT-32 files in `[project-root]/package_output/sd_card/*`
 - All other files can be ignored; they are an output from the Vitis packager and are unused
 - After this step you could open the full blockdesign (platform extended with all kernels) in Vivado for review
 
</details>

## Testing
 1. Copy over the `[project-root]/package_output/sd_card/*` to an sd card or put the `[project-root]/package_output/sd_card.img` on an sd card.
 2. Start-up and boot-up (no password is needed for linux login).
 3. Execute the following after boot-up (it also includes the results):
 ```
    root@linux:~# cd /media/sd-mmcblk0p1/
    root@linux:/media/sd-mmcblk0p1# ./vadd_cpp.exe a.xclbin
    Passed: auto my_device = xrt::device(0)
    Passed: auto xclbin_uuid = my_device.load_xclbin(a.xclbin)
    Passed: auto my_vadd = xrt::kernel(my_device, xclbin_uuid, "vadd")
    VADD TEST PASSED
    root@linux:/media/sd-mmcblk0p1# ./vadd_ocl.exe a.xclbin
    Loading: 'a.xclbin'
    VADD TEST PASSED
    root@linux:/media/sd-mmcblk0p1# ./aie_dly_test.exe a.xclbin
    Initializing ADF API...
    Passed: auto my_device = xrt::device(0)
    Passed: auto xclbin_uuid = my_device.load_xclbin(a.xclbin)
    Passed: auto my_rtl_ip = xrt::ip(my_device, xclbin_uuid, "subtractor")
    Passed: auto my_graph  = xrt::graph(my_device, xclbin_uuid, "mygraph_top")
    Passed: my_graph.reset()
    Passed: my_graph.run()
    Poll subtractor register
      Value Reg0:  fcc4
      Value Reg1:  fd16
      Value Reg2:  fd54
      Value Reg3:  fd42
    Poll subtractor register
      Value Reg0:  fcb0
      Value Reg1:  fd4e
      Value Reg2:  fcae
      Value Reg3:  fcc6
    ...
    Poll subtractor register
      Value Reg0:  fd56
      Value Reg1:  fd4e
      Value Reg2:  fcec
      Value Reg3:  fd04
    Poll subtractor register
      Value Reg0:  fcd8
      Value Reg1:  fd4e
      Value Reg2:  fccc
      Value Reg3:  fce6
    Passed: my_graph.end()
    root@linux:/media/sd-mmcblk0p1#
  ```

## Notes
  - xsa: CIPS settings are added manually; configured in the bd-files.
  - The example design is fully FAT-32 
    - if you like to use ext4 rootfs instead: 
      - petalinux already generates it.
      - You will need to copy it to the Vitis platform in the `[project-root]/Makefile` in the `bif` section.
      - The `v++ -p` command line in `[project-root]\vitis\Makefile` will need adaptations to be able to use ext4 rootfs instead of FAT-32.
  - Simulations and Emulations are **NOT** included in this Tutorial!

## Design Considerations
  Note: The **MUST**'s in below explanations are due to how the generic Makefiles are setup, and is **NOT** a Xilinx tools requirement!
  - `[project-root]/ps_apps`: PS applications can easily be added by adding a sub-project for each in `[project-root]/ps_apps`.
    - Vitis will automatically package them and they will end up on the sd_card.
    - The `[PS Application].exe` (extension **MUST** be .exe) **MUST** end up in the `[project-root]/ps_apps` dir.
  - `[project-root]/ip`: Kernels can be added by just adding a sub-project in `[project-root]/ip`.
    - You will need to update the `[project-root]/vitis/src/system.cfg` file to setup the correct connections/clocks.
    - A `[kernel].xo` file **MUST** end up in the `[project-root]/ip` dir
    - An extra aie graph **MUST** be added in the `[project-root]/ip/aie` dir, the `[project-root]/ip/aie/Makefile` will need adaptations.

## References
The following documents provide supplemental information for this tutorial.

### [AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/yii1603912637443.html)
Contains sections on how to develop AI Engine graphs, how to use the AI Engine compiler, AI Engine simulation, and performance analysis.

### [Xilinx Runtime (XRT) Architecture](https://xilinx.github.io/XRT/master/html/index.html)
The following are links to the XRT information used by this tutorial: 

* [XRT Documentation](https://xilinx.github.io/XRT/master/html/index.html): Explains general XRT API calls used in the PS Host Application. 

* [XRT Github Repo](https://github.com/Xilinx/XRT): Contains the XRT source code. 

* [XRT AIE API](https://github.com/Xilinx/XRT/blob/master/src/runtime_src/core/include/experimental/xrt_aie.h): Documents the AI Engine XRT API calls

### [Vitis Unified Software Development Platform 2021.2 Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/index.html)
The following are links to Vitis related information referenced in this tutorial:

* [Vitis Application Acceleration Development Flow Documentation](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/kme1569523964461.html)

* [Vitis Application Acceleration Development Flow Tutorials](https://github.com/Xilinx/Vitis-Tutorials)

* [Vitis HLS](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/irn1582730075765.html)

## Revision History
* October 2021 - Optimized AI Engine Datamovers + Added more clarifications in this README.md + Improved petalinux version check
* September 2021 - Initial Release

 
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
