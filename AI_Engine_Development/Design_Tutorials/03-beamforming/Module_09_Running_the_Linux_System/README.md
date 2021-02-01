# Building the Design

The next step is to package the Linux system and run it on the VCK190 board. To package the Linux system for hardware with a single command, run the following:

```
make all
```

# Running the System

For the hardware build, the ``–-package`` command creates an ``sd_card.img`` file which contains the contents of the ``sd_card/`` folder. Copy the ``sd_card.img`` file onto an SD card. This will become the boot device of your system. Perform the following steps to run the `beamforming_linux.elf` executable on the VCK190 board.

1. Ensure your board is powered off.
2. Use an SD card writer (such as balenaEtcher) to flash the `sd_card.img` file to an SD card.
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
8. Wait until you see the `root@versal-rootfs-common-2020_2` Linux command prompt. Press enter a few times to get past any `xinit` errors.
9. Run the following commands into the TeraTerm terminal:

```
cd /mnt/sd-mmcblk0p1
export XILINX_XRT=/usr
./beamforming_linux.elf 1 a.xclbin
```
The `beamforming_linux.elf` should execute functional and performance tests. At the end of the application, you should see the following final output:

```
[ULBF] Read first 4 iterations of Slave-11 errCount:0...
[DLBF+ULBF] - ***** TEST PASSED *****
 ***** END TEST *****
```
<details>
  <summary> Running the System on BOARDFARM - INTERNAL ONLY</summary>

# Running the System on BOARDFARM - INTERNAL ONLY

Open a terminal (referred to as terminal 1) which is running `systest`. ``cd`` into the package folder `Module_09_Running_the_Linux_System/ps_pl/` directory and then `systest` for a `vck190` board.

```bash
cd Module_09_Running_the_Linux_System/ps_pl
/proj/systest/bin/cluster-ping vck190
/proj/systest/bin/systest vck190-19 (for example)
```
Open an LSF xterm terminal. This terminal will be referred to as terminal 2.

## Terminal 2

In terminal 2, ``cd`` into the scripts folder and source the ``env`` script:
```
cd Module_09_Running_the_Linux_System/scripts/
source setup_env.sh
```

## Terminal 1

Launch hardware server on your VCK190 board.

```bash
power 0 power 1 hw_server
```

The output of this command will show the board hostname.

```
Starting Vivado hardware server.
You can connect to this machine using the remote cable:
    <hostname>:3121
```

Obtain your full path to the `ps_pl/` directory:
```bash
pwd
```

Set the nfsroot to the absolute path of `ps_pl/` directory:
```
nfsroot "<path-to-ps_pl directory>/"
```

Obtain your full path to the `Module_07_Petalinux/build/vck190_linux/images/linux/` directory:

```
tftpd "<path-to-Module_07_Petalinux/build/vck190_linux/images/linux/ directory>/"
```

View the `com0` port of the board.

```
connect com0
```

The output you should see of the above command is shown below:
```
Connecting to device com0.  Use Ctrl-\ to escape.
```
Switch to Terminal 2.

## Terminal 2

Use the XSCT tool to execute the `jtag.tcl` script and boot the Linux images (from the custom Linux images folder).  

```bash
pwd
xsct jtag.tcl <hostname> <path-to-Module_07_Petalinux/build/vck190_linux/images/linux/-directory>
```
Note: The `<hostname>` can be seen from Terminal 1.

While the script is running, you will see the following output on the com0 port in Terminal 1.

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

Eventually, the output you see on terminal 1 at the end of the above command should be as shown below:

```
PetaLinux 2020.2 xilinx-vck190-2020_2 /dev/ttyAMA0
root@xilinx-vck190-es1-2020_2
```

## Terminal 1

When the script is run, the board boots through a pre-built Linux base image. Use `root` as the login and password. At the command prompt, run the following commands:

```
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

## Terminal 2

Use the XSCT tool to execute the `sd_mode.tcl` script.
```
xsct sd_mode.tcl <hostname>
```
While the script is running, you will see output on the com0 port in terminal 1.

The output you should see on terminal 1 at the end of the above command is as follows:

```
PetaLinux 2020.2 versal-rootfs-common-2020_2 ttyAMA0

root@versal-rootfs-common-2020_2:~#
```

## Terminal 1

Now that your board should have booted Linux from the SD mode, run the `tx_chain_xrt.elf` executable.

```bash
cd /mnt/sd-mmcblk0p1
./test_bf_app.exe 0 aie.xclbin
```

**Note:** You are using ``aie.xclbin`` and not ``a.xclbin``.

At the end of the run, you will see the following:

```
"[Beamformer-PERF] - ***** TEST PASSED *****
[main] Beamformer End... "
```

To exit out of the `root@versal-rootfs-common-2020_2` prompt:

```
Ctrl + \
```

To exit `systest`:
```
exit
```

</details>

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
