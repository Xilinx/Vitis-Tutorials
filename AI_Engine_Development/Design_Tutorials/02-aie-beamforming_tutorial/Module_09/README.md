# Building the Design
Now, we'll package the Linux System and run it on the VCK190 board. To package the Linux system for hw with a single command. 
```
make all
```

# Running the System
Now, we'll run the Linux System on the VCK190 board. For the hardware build, the –-package command creates a sd_card.img file which contains the contents of the sd_card/ folder.  Next, we will copy the sd_card.img file onto an SD Card which will become the boot device of our system. 

Now follow **Steps 1-9** to run the `beamforming_linux.elf` excutable on your VCK190 board. 

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

**Step 8.** Wait until you see the `root@versal-rootfs-common-2020_2` Linux command prompt. Press enter a few times to get past any `xinit` errors. 

**Step 9.** Run the following commands into the TeraTerm terminal: 
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
cd into the package folder `build/hw/package` directory and then `systest` for a `vck190` board 
```bash
cd build/hw/package
/proj/systest/bin/cluster-ping vck190
/proj/systest/bin/systest vck190-19 (for example)
```
Open a new terminal. This terminal will be referred to as Terminal 2. The previous terminal running `systest` will be referred to as Terminal 1.

### Terminal 2:
In Terminal 2, clone this repo, copy the following files into your `package\` directory and source env script: 
```
cp -r internal_only/* . 
source ../setup_env.sh
```

### Terminal 1:
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

Obtain your full path to the `package\` directory:
```bash
pwd
```
	
Set the nfsroot and copy `package\` directory contents to the board:
```
nfsroot "<path-to-package-directory>/"
tftpd "<path-to-package-directory>/"

```
Note: Your `<path-to-package-directory>` is the result of the `pwd` command.

View the `com0` port of the board. 
```
connect com0
```

The output you should see of the above command is below: 
```
Connecting to device com0.  Use Ctrl-\ to escape.
```
Switch to Terminal 2. 

### Terminal 2: 
Use the XSCT tool to execute the `jtag.tcl` script and boot the Linux base image (from the `base_images` folder).  
```bash
pwd
xsct jtag.tcl <hostname> <path-to-package-directory>/base_images
```
Note: The `<hostname>` can be seen from Terminal 1. The `<path-to-package-directory>` is the result of the `pwd` command. 

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
This will take a few mins to complete. 

If you get an error executing the `jtag.tcl` script, and don't see any output on terminal 1, try to systest into a different VCK190 board. 

Eventually, the output you should see on Terminal 1 at the end of the above command is below: 
```
PetaLinux 2020.2 xilinx-vck190-2020_2 /dev/ttyAMA0

xilinx-vck190-2020_2 login: 
```

### Terminal 1:
When the script is run, the board boots through a pre-built Linux Base Image. Use `root` as the login and password. At the command prompt, run the following commands: 
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

### Terminal 2: 
Use the XSCT tool to execute the `sd_mode.tcl` script. 
```
xsct sd_mode.tcl <hostname>
```
While the script is running, you will see output on the com0 port in Terminal 1.

The output you should see on Terminal 1 at the end of the above command is as follows: 
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

### Terminal 1: 
Now your board should have booted Linux from the SD mode, run the `tx_chain_xrt.elf` executable. 
```bash
cd /mnt/sd-mmcblk0p1
./init.sh
./tx_chain_xrt.elf a.xclbin
```

At the end of the run, you will see `"TEST PASSED for DPD0"`.

To exit out of the `root@versal-rootfs-common-2020_2` prompt: 
```
Ctrl + \ 
```

To exit `Systest`: 
```
exit 
```

</details>
