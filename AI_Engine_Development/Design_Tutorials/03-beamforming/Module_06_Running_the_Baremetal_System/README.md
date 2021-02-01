# Introduction: Packaging and Running the System  

After the AI Engine graph is compiled and linked with the PL kernels, the PS application is compiled, and all the required outputs are generated, the next step in the build process is to package the required files to configure and boot the Versal™ device. This requires the use of the Vitis™ compiler ``--package`` command. For Versal ACAPs, the programmable device image (PDI) file is used to boot and program the hardware device.

In this module, you will package the bare-metal system to generate the final bootable image (PDI) for running hardware emulation and for running it on the VCK190 board. Refer to [AI Engine Documentation: Packaging](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/integrate_ai_engine_application.html#ariaid-title8) for more details on this packaging process.

# Building the Design: Hardware Emulation

For hardware emulation, the ``–-package`` command adds the PDI and EMULATION_DATA sections to the XCLBIN file and outputs a new XCLBIN file (``a.xclbin``):

```bash
make package
```
or

```bash
make package TARGET=hw_emu MAIN=main_partial
```
or

```bash
v++ -p                                                                                           \
    -t hw_emu                                                                                    \
    -f ../Module_01_Custom_Platform/build/output/vck190_v1_0/export/vck190_v1_0/vck190_v1_0.xpfm \
    --package.out_dir build/hw_emu_main_partial/hw_emu_main_partial.package                      \
    --package.domain aiengine                                                                    \
    --package.boot_mode=sd                                                                       \
    --package.image_format=fat32                                                                 \
    --package.ps_elf ../Module_05_Baremetal_Host_Application/build/main_partial_ps.elf,a72-0     \
    ../Module_04_AI_Engine_and_PL_Integration/build/rev1/hw_emu/beamforming.rev1.hw_emu.xclbin   \
    ../Module_02_AI_Engine_Design/build/libadf.a                                                     
```

Notice the absence of the ``-D__PS_ENABLE_AIE__`` (Module 05) and ``–package.defer_aie_run`` options (Module 06). This bare-metal application is loaded and launched at boot time and these options are not required. In Modules 08 and 09, you will compile the Linux host application and package; these options will be required at this point because the Linux host application launches and controls the graph.

## Dependencies

|Filename|Description|
|  ---  |  ---  |
|vck190_v1_0.xpfm|The custom embedded platform created in Module 01.|
|main_partial_ps.elf|The baremetal PS host application created in Module 05.|
|beamforming.rev1.hw_emu.xclbin|The XCLBIN file created in Module 04. |
|libadf.a|The AI Engine application created in Module 02.|

## Build Products
|Filename|Description|
|  ---  |  ---  |
|build/hw_emu_main_partial/hw_emu_main_partial.package/|Package directory containing the files needed to run hardware emulation.|

# Running the System: Hardware Emulation

Running the system depends on the build target. The process of running the hardware emulation build is different from that of running the hardware build. For hardware emulation, the ``--package`` command generates the ``launch_hw_emu.sh`` script. This script is used to launch the emulation environment. Hardware emulation runs the AI Engine simulator for the graph application, the Vivado® logic simulator (XSIM) for the PL kernels, and QEMU for the PS host application.  

Use the following command to launch hardware emulation from the command line.

```
make run_emu
```
or
```
cd build/hw_emu_main_partial/hw_emu_main_partial.package
export XILINX_XRT/usr
./launch_hw_emu.sh
```
This step takes several hours to complete. You can open a new shell and continue with the next section while hardware emulation completes.  

To exit QEMU simulation, press **Ctrl+A**, let go of the keyboard, and then press **x**.

# Building the Design: Hardware

For hardware builds, the package process creates an XCLBIN file containing the ELF files and graph configuration data objects (CDOs) for the AI Engine application. Because you already ran the ``main_partial.elf`` host application during hardware emulation, use the ``main_full.elf`` for the hardware package:

```bash
make package TARGET=hw MAIN=main_full
```
or

```bash
v++ -p                                                                                           \
    -t hw                                                                                        \
    -f ../Module_01_Custom_Platform/build/output/vck190_v1_0/export/vck190_v1_0/vck190_v1_0.xpfm \
    --package.out_dir build/hw_main_full/hw_main_full.package                                    \
    --package.domain aiengine                                                                    \
    --package.boot_mode=sd                                                                       \
    --package.image_format=fat32                                                                 \
    --package.ps_elf ../Module_05_Baremetal_Host_Application/build/main_full_ps.elf,a72-0     \
    ../Module_04_AI_Engine_and_PL_Integration/build/rev1/hw_emu/beamforming.rev1.hw_emu.xclbin   \
    ../Module_02_AI_Engine_Design/build/libadf.a                                                     
```
## Dependencies

|Filename|Description|
|  ---  |  ---  |
|vck190_v1_0.xpfm|The custom embedded platform created in Module 01.|
|main_full_ps.elf|The bare-metal PS host application created in Module 05.|
|beamforming.rev1.hw.xclbin|The XCLBIN file created in Module 04. |
|libadf.a|The AI Engine application created in Module 02.|

## Build Products
|Filename|Description|
|  ---  |  ---  |
|build/hw_main_full/hw_main_full.package/|Package directory containing files needed to run hardware (VCK190 board).|

# Running the System: Hardware

For the hardware build, the ``–-package`` command creates an ``sd_card.img`` file which contains the contents of the ``sd_card/`` folder.  The next step is to copy the ``sd_card.img`` file onto an SD card, which will become the boot device of your system.

Perform the following steps to run the `hw_main_full_ps.elf` excutable on your VCK190 board.

1. Ensure your board is powered off.
2. Use an SD card writer (such as balenaEtcher) to flash the `sd_card.img` file an SD card.
3. Plug the flashed SD card into the top slot of the VCK190 board.
4. Set the switch SW1 Mode\[3:0\]=1110 = OFF OFF OFF ON
5. Connect your computer to the VCK190 board using the included USB cable.
6. Open a TeraTerm terminal and select the correct COM port. Set the port settings to the following:
```
Port: <COMMXX>
Speed: 115200
Data: 8 bit
Parity: none
Stop Bits: 1 bit
Flow control: none
Transmit delay: 0 msec/char 0 msec/line
```

7. Power on the board.

8. Output of bare-metal PS host application: the `*.elf` should execute immediately after booting and you should see output on the console for each phase in the PS host application execution flow. At the end of the application, you should see the following final output:

```
[ULBF] Read first 4 iterations of Slave-11 errCount:0...
[DLBF+ULBF] - ***** TEST PASSED *****
 ***** END TEST *****
```

# References

* [AI Engine Documentation: Integrating the Application Using the Vitis Tools Flow, Packaging](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/integrate_ai_engine_application.html#ariaid-title8)
* [AI Engine Documentation: Integrating the Application Using the Vitis Tools Flow, Running the System](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/integrate_ai_engine_application.html#ariaid-title11)
* [AI Engine Documentation: Integrating the Application Using the Vitis Tools Flow, Deploying the System](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/integrate_ai_engine_application.html#ariaid-title12)
* [Vitis Compiler Command Options](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/vitiscommandcompiler.html#wrj1504034328013)

© Copyright 2021 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
