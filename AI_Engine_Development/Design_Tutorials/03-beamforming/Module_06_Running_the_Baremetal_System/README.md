# Introduction: Packaging and Running the System  

After the AI Engine graph is compiled and linked with the PL kernels, the PS application is compiled, and all the required outputs are generated, the next step in the build process is to package the required files to configure and boot the Versal™ device. This requires the use of the Vitis™ compiler ``--package`` command. For Versal ACAPs, the programmable device image (PDI) file is used to boot and program the hardware device.

In this module, you will package the bare-metal system to generate the final bootable image (PDI) for running hardware emulation and for running it on the VCK190 board. Refer to [AI Engine Documentation: Packaging](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/integrate_ai_engine_application.html#ariaid-title8) for more details on this packaging process.

# Building the Design: Hardware Emulation

For hardware emulation, the ``–-package`` command adds the PDI and EMULATION_DATA sections to the XCLBIN file and outputs a new XCLBIN file (``a.xclbin``).

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

TODO: Issue: Exiting hw_emu need to close the shell.

# Building the Design: Hardware

For hardware builds, the package process creates an XCLBIN file containing the ELF files and graph configuration data objects (CDOs) for the AI Engine application. Since you already ran the ``main_partial.elf`` host application during hardware emulation, use the ``main_full.elf`` for the hardware package.

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
|main_full_ps.elf|The baremetal PS Host application created in Module 05.|
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

<details>
  <summary> Running the Bare-Metal System on BOARDFARM - INTERNAL ONLY</summary>


## INTERNAL ONLY: IF USING BOARDFARM

To run your design and package it for hardware, ``cd`` into the package folder `build/hw_main_full/hw_main_full.package` directory and then `systest` for a `vck190` board.

```bash
cd build/hw_main_full/hw_main_full.package
/proj/systest/bin/cluster-ping vck190
/proj/systest/bin/systest vck190-19 (for example)
```
Open a new terminal. This terminal will be referred to as terminal 2. The previous terminal running `systest` will be referred to as terminal 1.

### Terminal 2

In terminal 2, copy the contents of the ``internal_only`` folder into the `hw_main_full.package\` directory and source the env script again:

```
cp ../../internal_only/* .
source ../../env_setup_2020_internal_only.sh
```

### Terminal 1

Launch the hardware server on your VCK190 board.

```bash
power 0 power 1 hw_server
```
The output of this command will show the board hostname.

```
Starting Vivado hardware server.
You can connect to this machine using the remote cable:
    <hostname>:3121
```

Obtain your full path to the `package\` directory:

```bash
pwd
```

Set the ``nfsroot`` and copy the `hw_main_full.package\` directory contents to the board:

```
nfsroot "<path-to-package-directory>/"
tftpd "<path-to-package-directory>/"

```
**Note:** Your `<path-to-package-directory>` is the result of the `pwd` command.

View the `com0` port of the board.

```
connect com0
```

The output you should see of the above command is below:

```
Connecting to device com0.  Use Ctrl-\ to escape.
```

Switch to terminal 2.

### Terminal 2

Use the XSCT tool to execute the `jtag.tcl` script and boot the Linux base image (from the `base_images` folder).  

```bash
pwd
xsct jtag.tcl <hostname> <path-to-package-directory>/base_images
```
**Note:** The `<hostname>` can be seen from terminal 1. The `<path-to-package-directory>` is the result of the `pwd` command.

While the script is running, you will see the following output on the com0 port in terminal 1.

```
[5.495715]****************************************
[7.150615]Xilinx Versal Platform Loader and Manager
[11.769859]Release 2020.1   Apr 16 2020  -  05:10:55
[16.389928]Platform Version: v1.0 PMC: v1.0, PS: v1.0
[21.96325]STDOUT: PS UART
[23.406453]****************************************
....
```
This will take a few minutes to complete.

If you get an error executing the `jtag.tcl` script, and don't see any output on terminal 1, try to ``systest`` into a different VCK190 board.

Eventually, the output you should see on terminal 1 at the end of the above command is as shown below:

```
PetaLinux 2020.2 xilinx-vck190-2020_2 /dev/ttyAMA0

xilinx-vck190-2020_2 login:
```

### Terminal 1

When the script is run, the board boots through a pre-built Linux base image. Use `root` as the login and password. At the command prompt, run the following commands:

```
xilinx-vck190-2020_1 login: root
Password: root
mkdir /mnt_nfsroot
mount -o port=2049,nolock,proto=tcp,vers=2 10.10.70.101:/exports/root /mnt_nfsroot
dd if=/mnt_nfsroot/sd_card.img of=/dev/mmcblk0
```
This will take a few minutes to complete.

The output you should see at the end of the above command is as follows:

```
4194304+0 records in
4194304+0 records out
```

Power cycle the board to boot through the SD mode.

```bash
Press CTRL-\ #(to exit the root@xilinx-vck190-2020_2 command prompt and go back to systest)
power 0 power 1 connect com0
```

### Terminal 2

Use the XSCT tool to execute the `sd_mode.tcl` script.

```
xsct sd_mode.tcl <hostname>
```

While the script is running, you will see output on the ``com0`` port in terminal 1.

The output you should see on terminal 1 at the end of the above command is as follows:

```
PetaLinux 2020.2 versal-rootfs-common-2020_2 ttyAMA0


root@versal-rootfs-common-2020_2:~# Enabling notebook extension jupyter-js-widgets/extension...
      - Validating: OK
[C 21:08:32.781 NotebookApp] Bad config encountered during initialization:
[C 21:08:32.782 NotebookApp] No such notebook dir: ''/usr/share/example-notebooks''

root@versal-rootfs-common-2020_2:~# xinit: giving up
xinit: unable to connect to X server: Connection refused
xinit: server error

root@versal-rootfs-common-2020_2:~#
```

You might need to press enter a few times to get to the `root@versal-rootfs-common-2020_2:~#` prompt.

### Terminal 1

Now your board should have booted Linux from the SD mode, and the `hw_main_full_ps.elf` executable should automatically start running.

At the end of the run, you will see `"TEST PASSED"`.

To exit out of the `root@versal-rootfs-common-2020_2` prompt:

```
Ctrl + \
```

To exit `Systest`:
```
exit
```
</details>


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
