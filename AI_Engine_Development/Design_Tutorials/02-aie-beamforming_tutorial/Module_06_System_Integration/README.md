# Introduction: Packaging and Running the System  
After the AI Engine graph is compiled and linked with the PL kernels, the PS application is compiled, and all the required outputs are generated, the next step in the build process is to package the required files to configure and boot the Versal device. This requires the use of the v++ --package command. For Versal ACAPs, the programmable device image (PDI) file is used to boot and program the hardware device. 

In this module, we will package the baremetal system to generate the final boot-able image (PDI) for running hardware emulation and for running it on the VCK190 board. Refer to UG1076 Packaging the System for Hardware Emulation or Hardware for more details on this package process. 

# Building the Design: Hardware Emulation 
For hardware Emulation, the –-package command adds the PDI and EMULATION_DATA sections to the XCLBIN file and outputs a new XCLBIN file (a.xclbin). 

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

Notice the absence of the -D__PS_ENABLE_AIE__ (in Module 05) and the absence of the –package.defer_aie_run option (Module 06). Our baremetal application is loaded and launched at boot time and these options are not required. In Module 08/09, we’ll compile the Linux Host application and package it. These options are required at that time since the Linux Host Application launches are controls the graph.

## Dependencies

|Filename|Description|
|  ---  |  ---  |
|vck190_v1_0.xpfm|The custom embedded platform we created in Module 01.| 
|main_partial_ps.elf|The baremetal PS Host Application we created in Module05.|
|beamforming.rev1.hw_emu.xclbin|The XCLBIN file we created in Module 04. |
|libadf.a|The AI Engine Application we created in Module02.|

## Build Products 
|Filename|Description|
|  ---  |  ---  |
|build/hw_emu_main_partial/hw_emu_main_partial.package/||
|build/hw_emu_main_partial/a.xclbin||
|build/hw_emu_main_partial/v++\_package.log||
|build/hw_emu_main_partial/v++.package_summary||
|build/hw_emu_main_partial/\_x||


# Running the System: Hardware Emulation 
Running the system depends on the build target. The process of running the hardware emulation build is different from running the hardware build. For hardware emulation, the --package command generates the launch_hw_emu.sh script. We will use this script the launch the emulation environment. Hardware emulation runs the AI Engine simulator for the graph application, runs the Vivado logic simulator (XSIM) for the PL kernels, and runs QEMU for the PS Host Application.  

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
Note this step takes several hours to complete. You can open a new shell and continue with the next section while hardware emulation completes.  

TODO: Issue: Exiting hw_emu need to close the shell. 

# Building the Design: Hardware
For hardware builds, the package process creates an XCLBIN file containing the ELF files and graph configuration data objects (CDOs) for the AI Engine Application. Since we already ran the main_partial.elf host application during hardware emulation, we'll run use the main_full.elf for the hardware package. 

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
    --package.ps_elf ../Module_05_Baremetal_Host_Application/build/main_partial_ps.elf,a72-0     \	
    ../Module_04_AI_Engine_and_PL_Integration/build/rev1/hw_emu/beamforming.rev1.hw_emu.xclbin   \
    ../Module_02_AI_Engine_Design/build/libadf.a                                                     
```
## Dependencies and Build Products 
Pretty much the same as the hw_emu build except the build products are in the build/hw_main_full folder. 

# Running the System: Hardware
For the hardware build, the –-package command creates a sd_card.img file which contains the contents of the sd_card/ folder.  Next, we will copy the sd_card.img file onto an SD Card which will become the boot device of our system. 

Now follow **Steps 1-9** to run the `beamforming_baremetal.elf` excutable on your VCK190 board. 

**Step 1.** Ensure your board is powered off. 

**Step 2.** Use an SD card writer (such as balenaEtcher) to flash the `sd_card.img` file an SD card. 

**Step 3.** Plug the flashed SD card into the top slot of the VCK190 board. 

**Step 4.** Set the switch SW1 Mode\[3:0\]=1110 = OFF OFF OFF ON

**Step 5.** Connect your computer to the VCK190 board using the included USB cable. 

**Step 6.** Open a TeraTerm terminal and select the correct COM port. Set the port settings to the following: 
```
Port: <COMMXX>
Speed: 115200
Data: 8 bit
Parity: none
Stop Bits: 1 bit
Flow control: none
Transmit delay: 0 msec/char 0 msec/line
```

**Step 7.** Power on the board.

**Step 8.** Output of Baremetal PS Host Application
The `beamforming_baremetal.elf` should execute immediately after booting and you should see output on the console for each phase in the PS Host Application execution flow. At the end of the application, you should see the following final output: 
```
[ULBF] Read first 4 iterations of Slave-11 errCount:0...
[DLBF+ULBF] - ***** TEST PASSED ***** 
 ***** END TEST ***** 
```

# References

UG1076 Packaging the System for Hardware Emulation or Hardware. 

UG1076 Deploying the System

Vitis Compiler Command UG1416 
