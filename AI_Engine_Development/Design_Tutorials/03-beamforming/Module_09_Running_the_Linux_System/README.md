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

For the hardware build, the ``–-package`` command creates an ``sd_card.img`` file which contains the contents of the ``sd_card/`` folder. Copy the ``sd_card.img`` file onto an SD card. This will become the boot device of your system. Perform the following steps to run the `test_bf_app.exe` executable on the VCK190 board.

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
8. Wait until you see the `xilinx-vck190-20221 login` Linux command prompt. Press enter a few times to get past any `xinit` errors.
9. Give login as `petalinux` and password as `petalinux`.
10. Run the following commands into the TeraTerm terminal:


```
sudo su
mount /dev/mmcblk0p1 /mnt
cd /mnt
./test_bf_app.exe 0 aie.xsa
```
The `test_bf_app.exe` should execute functional and performance tests. At the end of the application, you should see the following final output:

```
[ULBF] Read first 4 iterations of Slave-11 errCount:0...
[DLBF+ULBF] - ***** TEST PASSED *****
 ***** END TEST *****
```

# Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )



Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"> XD016 | &copy; Copyright 2021 Xilinx, Inc.</p>
