<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Building the Design

The next step is to package the Linux system and run it on the VCK190 board. To package the Linux system for hardware with a single command, run the following command:

```
make all
```

## Running the System

For the hardware build, the ``–-package`` command creates an ``sd_card.img`` file, which contains the contents of the ``sd_card/`` folder. Copy the ``sd_card.img`` file onto an SD card. This becomes the boot device of your system. Perform the following steps to run the `test_bf_app.exe` executable on the VCK190 board.

1. Ensure that your board is powered off.
2. Use an SD card writer (such as balenaEtcher) to flash the `sd_card.img` file to an SD card.
3. Plug the flashed SD card into the top slot of the VCK190 board.
4. Set the switch SW1 Mode\[3:0\]=1110 = OFF OFF OFF ON.
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
8. Wait until you see the `root@versal-rootfs-common-2023_1` Linux command prompt. Press enter a few times to get past any `xinit` errors.
9. Run the following commands into the TeraTerm terminal:

```
cd /mnt/sd-mmcblk0p1
./test_bf_app.exe 0 aie.xclbin
```
The `test_bf_app.exe` should execute functional and performance tests. At the end of the application, the following final output is generated:

```
[ULBF] Read first 4 iterations of Slave-11 errCount:0...
[DLBF+ULBF] - ***** TEST PASSED *****
 ***** END TEST *****
```

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
