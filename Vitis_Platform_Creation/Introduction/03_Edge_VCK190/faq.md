<!-- 
# Copyright 2020 Xilinx Inc.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
-->


<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Versal Custom Platform Creation Tutorial</h1>
    </td>
 </tr>
</table>

## Frequently Asked Questions

### Why can't my platform boot on hardware?

The Versal platform has various boot stages. We need to isolate the failing stage to find more details of trouble shooting.

- For ***Xilinx Versal Platform Loader and Manager*** stage issues, it loads PDI file and initialize each components. Please check the Vivado settings if this step halts or report errors.
- If the boot hangs at U-boot stage, please check the device tree configuration. If SD card circuit doesn't support high speed, please add no-1-8-v for SD controller node. U-boot will look for boot.scr and system.dtb on the fat32 partition of SD card if it's in SD boot mode. Please make sure these two files are available.

### Why can't my platform boot on hardware emulation?

- Please make sure the SELECTED_SIM_MODEL is set to TLM for CIPS, NOC and AI Engine.

### When is the AIE app and PL kernel being programmed?

Since the platform we create is not a DFX platform, PL kernels are implemented and packaged with platform all the times. They are included in BOOT.BIN. They are downloaded to hardware during boot time.

When host application is launched, it reads the kernel metadata from xclbin file to understand the kernel topology and parameters. This step won't configure PL bit stream.

AI Engine has two run methods: configure during boot or load graph in host application.


### Why my AIE system application hangs?

If you test the platform with PL+AIE system application and it hangs on the board with following error message, pressing Ctrl+C reports command stall, you are probably using the **sd_card.img** in **package** directory rather than **package_no_aie_debug** directory. The application in package directory is used for debugging. It will stop after loading and wait for debugger to connect to it. 

```
[   52.262013] [drm] Pid 715 opened device
[   52.265891] [drm] Pid 715 closed device
[   52.299093] [drm] Pid 715 opened device
[   52.302958] [drm] Pid 715 closed device
[   52.307012] [drm] Pid 715 opened device
Loading: 'binary_container_1.xclbin'
[   52.579320] [drm] zocl_xclbin_read_axlf fbc72c83-5ab0-43f2-97aa-60c7672e835a ret: 0
[   52.584870] [drm] bitstream fbc72c83-5ab0-43f2-97aa-60c7672e835a locked, ref=1
[   52.592572] [drm] No ERT scheduler on MPSoC, using KDS
[   52.604924] [drm] Interrupt is not enabled for at least one kernel. Fall back to polling mode  .
[   52.604934] [drm] 12 non-zero interrupt-id CUs out of 13 CUs
[   52.613562] [drm] CU ffffff02 is free-running.
[   52.619222] [drm] scheduler config ert(0)
[   52.623663] [drm]   cus(3)
[   52.627666] [drm]   slots(16)
[   52.630369] [drm]   num_cu_masks(1)
[   52.633330] [drm]   cu_shift(16)
[   52.636807] [drm]   cu_base(0xa4010000)
[   52.640030] [drm]   polling(1)
[   52.643894] [drm] bitstream fbc72c83-5ab0-43f2-97aa-60c7672e835a unlocked, ref=0
^C[   52.654496] [drm] bitstream fbc72c83-5ab0-43f2-97aa-60c7672e835a locked, ref=1
[   56.707230] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   57.241186] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   57.753177] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   58.265177] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   58.777175] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   60.313187] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   60.825177] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   61.337174] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   61.849185] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   62.361191] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   62.873190] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   63.385190] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   63.897188] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   64.409185] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   64.921183] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   65.433177] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   65.945174] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   66.457185] [drm] pid(715) waiting for outstanding 1 cmds to finish
[   66.969193] [drm:zocl_client_release [zocl]] *ERROR* Please investigate stale cmds
[   66.983017] [drm] addr 0xa4020000, status 0x4
[   66.983019] [drm] addr 0xa4010000, status 0x1
[   66.987375] [drm] bitstream fbc72c83-5ab0-43f2-97aa-60c7672e835a unlocked, ref=0
[   66.991728] [drm] Pid 715 closed device

```


<p align="center"><sup>Copyright&copy; 2021 Xilinx</sup></p>
