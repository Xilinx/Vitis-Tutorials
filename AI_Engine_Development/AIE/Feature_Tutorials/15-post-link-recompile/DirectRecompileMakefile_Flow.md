</table>
<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Lab 1: Direct AI Engine Recompile Makefile Flow

## Initialization


All labs rely on a set of source files that are available in the `Files` sub-directory:

- `AIE` contains the source files for the AI Engine application.
- `AIE2` contains the source files for the AI Engine application with location constraints for one of the AI Engine kernels.
- `Constraints` contains the PLIO constraints file for the second phase.
- `data` contains data files used to verify application functionality.
- `HwLink` contains a system configuration file to link AI Engine and PL kernels.
- `PL` contains all PL kernels (`mm2s`, `s2mm`, and `polar_clip`).
- `PS` contains the host (A72) application that runs the PL kernels, launches the graph, and verifies the output.

Go to the Lab directory:
```BASH
cd WithoutIntermediatePlatform
```


## Phase 1: Compile AI Engine application and PL Kernels and Link the System

In this lab, all the operations are completed within a terminal. A Makefile contains all the operations for the phases of this lab.  Open it in any editor or display it in the terminal using `more`, `less`, or `cat`.

Phase 1 is completed in the following stages:

1. `kernels`: Build the PL kernels. The output is a number of XO files, which are packaged RTL kernels.

2. `aie`: Build the AI Engine graph. The output is the file `libadf.a`, which is the compiled graph with all the PL/AI Engine interfaces.

3. `link`: Link the AI Engine array design with the PL design. This stage creates the XCLBIN and the XSA files.

There are also optional stages to simulate the AI Engine application using the `aiesim` target and `run_emu` to launch hardware emulation that includes all the PL kernels.

In the terminal, type `make clean phase1`. All the required stages (`kernels`, `aie`, and `link`) are run in the terminal as well as in hardware emulation (`package` and `run_emu`).

Check the placement of the AI Engine kernels using an AMD Vitis&trade; Analyzer and opening the `graph.aiecompile_summary` file.

**Note:** The hardware emulation does not launch automatically. Launch it manually:



```
cd /run/media/mmcblk0p1
./host.exe a.xclbin
```

At the end of the simulation, the following message is displayed:

```
mm2s (DataIn1) completed with status(4)
[59672.228524] zocl-drm axi:zyxclmm_drm:  ffff0008003cd410 kds_del_context: Client pid(2424) del context CU(0x1)
polar_clip completed with status(4)
[59672.230697] zocl-drm axi:zyxclmm_drm:  ffff0008003cd410 kds_del_context: Client pid(2424) del context CU(0x2)
s2mm (DataOut1) completed with status(4)
[59674.351664] zocl-drm axi:zyxclmm_drm:  ffff0008003cd410 kds_del_context: Client pid(2424) del context CU(0xffffffff)
[59674.353188] zocl-drm axi:zyxclmm_drm:  ffff0008003cd410 kds_del_context: Client pid(2424) del context CU(0x0)
TEST PASSED
Releasing remaining XRT objects...
[59674.355157] [drm] bitstream 8ae06494-3226-bda4-94c7-906e136725ad unlocked, ref=0
[59674.452197] zocl-drm axi:zyxclmm_drm: zocl_destroy_client: client exits pid(2424)
root@versal-rootfs-common-20231:/run/media/mmcblk0p1#
```

You can get out of QEMU by hitting **Ctrl** + **a x**.


## Phase 2: Recompile the AI Engine Application, Package the New System, and Rerun Hardware Emulation

AI Engine compilation in phase 1 generates numerous files in the `Work` directory. The `Work/temp/graph_aie_routed.aiecst` file contains all the constraints that can be used during AI Engine compilation to get the exact same placement. From this file, extract the constraints related to the PLIO interface nodes, removing the other nodes and all the port constraints:

```BASH
{
  "NodeConstraints": {
    "DataIn1": {
      "shim": {
        "column": 24,
        "channel": 0
      }
    },
    "clip_in": {
      "shim": {
        "column": 24,
        "channel": 0
      }
    },
    "clip_out": {
      "shim": {
        "column": 25,
        "channel": 0
      }
    },
    "DataOut1": {
      "shim": {
        "column": 25,
        "channel": 0
      }
    }
  }
}
```

Phase 2 contains the following stages:

 1. `aie2`: As in the previous phase, this stage builds an AI Engine application, but in this case, it uses new interface constraints. The kernel itself has not changed but a kernel location constraint has been added to the graph so that you can see a big difference in the kernel placement. Check this new placement using Vitis Analyzer on the compile summary.

 2. `package2`: Create the PDI to load onto the device (`hw_emu`).

 3. `run_emu`: Launch the simulation and verify that the output is still correct.

Run `make phase2` to go over all steps. Simulation is done in the same way as in phase 1, but it takes more time due to the longer routing path.

## Perform On-Board Testing

1. To perform on-board testing, the same stages can be replicated but with a different target:

- For phase 1, type `make TARGET=hw clean phase1`.
- For phase 2, type `make TARGET=hw phase2`.

2. When the two phases are completed, there will be an `sd_card.img` file in the directory. Use a standard SD card imager with this file to initialize an SD card.

3. Place the SD card in the top SD card slot of your board.

4. Open a serial terminal connected to the right COM port of your VCK190 and switch on your board.

5. The system boots. When this is finished, change the directory before launching the PS application.

```
cd /run/media/mmcblk0p1
./host.exe a.xclbin
```


### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).


## License

___

The MIT License (MIT)

Copyright (c) 2023 Advanced Micro Devices, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



<p align="center"><sup>XD039 | &copy; Copyright 2021–2023 Xilinx, Inc.</sup></p>
