<table>
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Post-Link Recompile of an AI Engine Application</h1>
   </td>
 </tr>
 <tr>
 <td align="center"><h1>Lab 1: Direct AI Engine Recompile Makefile Flow</h1>
 </td>
 </tr>

</table>

# Initialization


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


# Phase 1: Compile AI Engine application and PL Kernels and Link the System

In this lab, all the operations are completed within a terminal. A Makefile contains all the operations for the phases of this lab.  Open it in any editor or display it in the terminal using `more`, `less`, or `cat`.

Phase 1 is completed in the following stages:

1. `kernels`: Build the PL kernels. The output is a number of XO files, which are packaged RTL kernels.

2. `aie`: Build the AI Engine graph. The output is the file `libadf.a`, which is the compiled graph with all the PL/AI Engine interfaces.

3. `link`: Link the AI Engine array design with the PL design. This stage creates the XCLBIN and the XSA files.

There are also optional stages to simulate the AI Engine application using the `aiesim` target and `run_emu` to launch hardware emulation that includes all the PL kernels.

The platform directory, `$PLATFORM_REPO_PATHS`, is initialized in  `env_setup.h`. It should contain the platform `xilinx_vck190_base_202210_1`.

In the terminal, type `make clean phase1`. All the required stages (`kernels`, `aie`, and `link`) will be run in the terminal as well as in hardware emulation (`package` and `run_emu`).

Check the placement of the AI Engine kernels using Vitis Analyzer and opening the `graph.aiecompile_summary` file.

**Note:** The hardware emulation does not launch automatically. You have to launch it manually.

The following is the last message in the terminal:

```
[  OK  ] Mounted /run/media/mmcblk0p1.
[  OK  ] Finished Record Runlevel Change in UTMP.

PetaLinux 2022.1_release_S03221922 versal-rootfs-common-20221 ttyAMA0

versal-rootfs-common-20221 login: root (automatic login)

[   69.227997] audit: type=1006 audit(1648486440.428:2): pid=596 uid=0 old-auid=4294967295 auid=0 tty=(none) old-ses=4294967295 ses=1 res=1
[   69.233487] audit: type=1300 audit(1648486440.428:2): arch=c00000b7 syscall=64 success=yes exit=1 a0=8 a1=fffff3d2d410 a2=1 a3=ffffba2fa6b0 items=0 ppid=1 pid=596 auid=0 uid=0 gid=0 euid=0 suid=0 fsuid=0 egid=0 sgid=0 fsgid=0 tty=(none) ses=1 comm="(systemd)" exe="/lib/systemd/systemd" key=(null)
[   69.234988] audit: type=1327 audit(1648486440.428:2): proctitle="(systemd)"
root@versal-rootfs-common-20221:~#

```

To launch the simulation, type the following:

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
root@versal-rootfs-common-20221:/run/media/mmcblk0p1#
```

You can get out of QEMU by hitting **Ctrl** + **a x**.


# Phase 2: Recompile the AI Engine Application, Package the New System, and Rerun Hardware Emulation

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

 1. `aie2`: As in the previous phase, this stage builds an AI Engine application, but in this case it uses the new interface constraints. The kernel itself has not changed but a kernel location constraint has been added to the graph so that you can see a big difference in the kernel placement. Check this new placement using Vitis Analyzer on the compile summary.

 2. `package2`: Create the PDI to load onto the device (`hw_emu`).

 3. `run_emu`: Launch the simulation and verify that the output is still correct.

Run `make phase2` to go over all steps. Simulation is done in the same way as in phase 1, but it takes more time due to the longer routing path.

# Perform On-Board Testing

1. To perform on-board testing, the same stages can be replicated but with a different target:

- For phase 1, type `make TARGET=hw clean phase1`.
- For phase 2, type `make TARGET=hw phase2`.

2. When the two phases are completed, there will be a `sd_card.img` file in the directory. Use a standard SD card imager with this file to initialize an SD card.

3. Place the SD card in the top SD card slot of your board.

4. Open a serial terminal connected to the right COM port of your VCK190 and switch on your board.

5. The system boots. When this is finished, change the directory before launching the PS application.

```
cd /run/media/mmcblk0p1
./host.exe a.xclbin
```


### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

### License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )


Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD039 | &copy; Copyright 2021–2022 Xilinx, Inc.</sup></p>