<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Dual Implementation of a Super Sampling Rate FIR Filter with Dual-Stream Input

***Version: Vitis 2025.2***

The purpose of this fifth part of the tutorial is to implement and run in hardware two instances of SSR with dual-stream I/Os.

Navigate to the `DualSSR16_hw` directory to continue.

## Goal of This Hardware Implementation

This section implements cosmetic changes that enable multiple implementations of the same filter with different names for the input and output data.

The two instances, G1 and G2, span from column 15 to 22 and from column 25 to 32.

Use this design to check in hardware the latency and throughput at some PLIO ports.

## Compile the Graph

The first step is to compile, simulate and visualize the graph with:

```BASH
make clean data aie aiesim aiesimviz
```

The output display (in the console or log section of the Vitis Analyzer) shows different average throughput values for the two instances. This occurs because the reference time for G1 and G2 is the same, while G1 starts before G2:

- G1 (filenames PhaseOut_N_K_15 finishing with 15) output throughput is around 3900 MB/s
- G2 (filenames PhaseOut_N_K_25 finishing with 25) output throughput is around 4000 MB/s

In the 'Array view', you can see the two instances with all input and output filenames.

## Build Hardware and Generate `sd_card.img`

Type `make all_hw` and go grab a coffee! This takes 1 to 2 hours to compile graph and PL kernels, link the system, and generate an SD card image.

After you have flashed an SD card with the generated image, you can use it to test various XRT profile and trace features.

### Logging in for the First Time

Copy your sd_card.img to a micro-SD card and insert it in the board.

- First boot: default user is `petalinux` and you set the password to p.
- sudo su and change the password of root `passwd root`: set it to r.
- Allow permission to login as root: vi /etc/ssh/sshd_config and change `PermitRootLogin` to `yes`.
- Reboot the board: reboot
- login as root
- cd /run/media/mmcblk0p1
- ./newdir
- cd pest1
- ./embedded_exec.sh

Each time you run `./newdir`, the script creates a new ptest directory with a copy of the original files. You can edit the `xrt.ini` file to profile and trace various parameters.

## Support

GitHub issues are used to track requests and bugs. For questions, go to [support.amd.com](https://adaptivesupport.amd.com/s/?language=en_US).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc</sub><br></br></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
