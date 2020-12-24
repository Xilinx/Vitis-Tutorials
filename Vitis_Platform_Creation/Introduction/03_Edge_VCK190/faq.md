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
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>Versal Custom Platform Creation Tutorial</h1>
    </td>
 </tr>
</table>

## Frequently Asked Questions

### Why can't my platform boot?

The Versal platform has various boot stages. We need to isolate the failing stage to find more details of trouble shooting.

- For ***Xilinx Versal Platform Loader and Manager*** stage issues, it loads PDI file and initialize each components. Please check the Vivado settings if this step halts or report errors.
- U-boot stage, the u-boot code is not changed, it's more likely a device setup issue. Please check the device tree configuration.
- Linux booting stage, please check step2 PetaLinux settings.
  - If CIPS to AIE interface is not enabled in platform PFM.AXI, system won't be able to connect PL kernels to AIE. Linux boot will hang.



### How to verify XRT is setup properly on device?

There are several check points to verify XRT setup

- Check ***XILINX_XRT*** environment variable and ***/etc/xocl.txt*** file. ***XILINX_XRT*** should points to ***/usr***. ***/etc/xocl.txt*** should contain the custom platform name which matches the platform used for creating the XCLBIN file. They are needed for OpenCL applications to make use of XRT functions. They are setup by ***init.sh*** in this example. Since init.sh is not run automatically after boot, user needs to source it manually every time after boot.

- XRT libraries are located in /usr/lib directory. Please make sure these library files and soft links exist in target rootfs.

  ```
  root@petalinux:/usr/lib# ls -l | grep xrt
  lrwxrwxrwx    1 root     root            22 Jan 19 03:14 libxrt++.so -> /usr/lib/libxrt++.so.2
  lrwxrwxrwx    1 root     root            17 May 27  2020 libxrt++.so.2 -> libxrt++.so.2.6.0
  -rwxr-xr-x    1 root     root        341944 May 27  2020 libxrt++.so.2.6.0
  lrwxrwxrwx    1 root     root            25 Jan 19 03:14 libxrt_core.so -> /usr/lib/libxrt_core.so.2
  lrwxrwxrwx    1 root     root            20 May 27  2020 libxrt_core.so.2 -> libxrt_core.so.2.6.0
  -rwxr-xr-x    1 root     root        403384 May 27  2020 libxrt_core.so.2.6.0
  lrwxrwxrwx    1 root     root            29 Jan 19 03:14 libxrt_coreutil.so -> /usr/lib/libxrt_coreutil.so.2
  lrwxrwxrwx    1 root     root            24 May 27  2020 libxrt_coreutil.so.2 -> libxrt_coreutil.so.2.6.0
  -rwxr-xr-x    1 root     root        740376 May 27  2020 libxrt_coreutil.so.2.6.0
  lrwxrwxrwx    1 root     root            21 May 27  2020 libxrt_hwemu.so.2 -> libxrt_hwemu.so.2.6.0
  -rwxr-xr-x    1 root     root        387008 May 27  2020 libxrt_hwemu.so.2.6.0
  lrwxrwxrwx    1 root     root            21 May 27  2020 libxrt_swemu.so.2 -> libxrt_swemu.so.2.6.0
  -rwxr-xr-x    1 root     root        198576 May 27  2020 libxrt_swemu.so.2.6.0
  drwxr-xr-x    3 root     root          1024 Sep  3  2020 xrt
  
  root@petalinux:/usr/lib/xrt/module# ls -l
  total 415
  -rwxr-xr-x    1 root     root        224784 May 27  2020 libxdp_appdebug_plugin.so
  -rwxr-xr-x    1 root     root         42920 May 27  2020 libxdp_debug_plugin.so
  -rwxr-xr-x    1 root     root         42944 May 27  2020 libxdp_hal_api_interface_plugin.so
  -rwxr-xr-x    1 root     root         83880 May 27  2020 libxdp_hal_plugin.so
  -rwxr-xr-x    1 root     root         34952 May 27  2020 libxdp_lop_plugin.so
  
  ```

- zocl kernel module is loaded at boot time. Please make sure this module is loaded properly.

  ```
  root@petalinux:/usr/lib# lsmod
      Tainted: G
  zocl 98304 0 - Live 0xffffffc008910000 (O)
  
  root@petalinux:/usr/lib# dmesg | grep zocl
  [    5.244767] zocl: loading out-of-tree module taints kernel.
  [    5.262207] [drm] Probing for xlnx,zocl-versal
  [    5.266833] zocl-drm amba:zyxclmm_drm: IRQ index 32 not found
  [    5.303733] [drm] Initialized zocl 2018.2.1 20180313 for amba:zyxclmm_drm on minor 0
  [    5.952938] [drm] Probing for xlnx,zocl-versal
  [    5.957547] zocl-drm amba:zyxclmm_drm: IRQ index 32 not found
  [    5.969346] [drm] Initialized zocl 2018.2.1 20180313 for amba:zyxclmm_drm on minor 0
  ```

  

### When is the AIE app and PL kernel being programmed?

Since the platform we create is not a DFX platform, PL kernels are implemented and packaged with platform all the times. They are included in BOOT.BIN. They are configured during boot time.

When host application is launched, it reads the kernel metadata from xclbin file. This step won't configure PL bit stream.

AI Engine has two run methods: configure during boot or load graph in host application. In this test AIE_RTP test application, it configures AIE at run time.


### Why my AIE system application hangs?

If you test the platform with PL+AIE system application and it hangs on the board with following error message, pressing Ctrl+C reports command stall, you are probably using the **sd_card.img** in **package** directory rather than **package_no_aie_debug** directory. 

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


<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
