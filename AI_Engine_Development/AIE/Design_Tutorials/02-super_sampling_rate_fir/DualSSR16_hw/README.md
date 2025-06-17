<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Dual Implementation of a Super Sampling Rate FIR Filter with Dual-Stream Input

***Version: Vitis 2025.1***

The purpose of this fith part of the tutorial is to implement and run in hardware two instances of SSR with dual-stream I/Os.

Navigate to the `DualSSR16_hw` directory to continue.

## Goal of this hardware implementation

In this section we are doing some cosmetic changes to be able to implement multiple times the same filter with different names for the input and output data.

The 2 instances, G1 and G2, will span from column 15 to 22 and from column 25 to 32.

We will use this design to check in hardware the latency and throughput at some PLIO ports.


## Compile the graph

The first step is to compile, simulate and visualize the graph with:

```BASH
make clean data aie aiesim aiesimviz
```
The output display of the average throughput gives different values for the 2 instances just because the reference time taken for G1 and G2 is the same while G1 starts before G2:

- G1 (filenames finishing with 15) output throughput is around 3900MB/s
- G2 (filenames finishing with 25) output throughput is around 4000MB/s

In the 'Array view' one can see clearly the 2 instances with all input and output filenames.


## Build hardware and generate `sd_card.img`

Type `make all_hw` and go grab a coffee! This will take 1 or 2h to compile graph and PL kernels, link the system and generate an SD card image.

Once you have flashed an SDCard with the generated image, you can use it to test various XRT profile and trace features.

### Logging in for the first time

copy your sd_card.img on a micro-sd card and insert it in the board. You may have your own way to do this, but let me give you mine.

- First boot: default user is `petalinux` and you set the password to p.
- sudo su and change the password of root `passwd root`: set it to r.
- Allow permission to login as root: vi /etc/ssh/sshd_config and change `PermitRootLogin` to `yes`.
- Reboot the board: reboot
- login as root
- cd /run/media/mmcblk0p1
- ./newdir
- cd pest1
- ./embedded_exec.sh

Each time you run `./newdir` a new ptest directory is created with a copy of the original files in it. You can edit the `xrt.ini` file to profile and trace various parameters.





## Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](https://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2024 Advanced Micro Devices, Inc</sub><br><sup>XD020</sup></br></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
