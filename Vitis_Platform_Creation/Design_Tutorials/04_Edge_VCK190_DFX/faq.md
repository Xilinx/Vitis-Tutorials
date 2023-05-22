<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Versal Custom Platform Creation Tutorial</h1>
    </td>
 </tr>
</table>

## Frequently Asked Questions

### SError Interrupt on xrt_cu_hls_init

Issue: When the application configuring the XCLBIN, it halts on `pc : xrt_cu_hls_init+0xc8/0x120 [zocl]`

Full Log:
```
root@versal-rootfs-common-20221:/mnt# ./run_app.sh
[   38.206468] zocl-drm amba_pl@0:zyxclmm_drm: zocl_create_client: created KDS c                                                              lient for pid(566), ret: 0
[   38.215933] zocl-drm amba_pl@0:zyxclmm_drm: zocl_destroy_client: client exits                                                               pid(566)
[   38.233391] zocl-drm amba_pl@0:zyxclmm_drm: zocl_create_client: created KDS c                                                              lient for pid(566), ret: 0
[   38.243329] zocl-drm amba_pl@0:zyxclmm_drm: zocl_create_client: created KDS c                                                              lient for pid(566), ret: 0
[   38.253842] zocl-drm amba_pl@0:zyxclmm_drm: zocl_destroy_client: client exits                                                               pid(566)
[   38.262481] zocl-drm amba_pl@0:zyxclmm_drm: zocl_create_client: created KDS c                                                              lient for pid(566), ret: 0
[   38.271905] zocl-drm amba_pl@0:zyxclmm_drm: zocl_destroy_client: client exits                                                               pid(566)
INFO: Reading krnl_vadd.xclbin
Loading: 'krnl_vadd.xclbin'
[   38.556872] zocl-drm amba_pl@0:zyxclmm_drm: zocl_create_client: created KDS c                                                              lient for pid(566), ret: 0
[   38.566335] zocl-drm amba_pl@0:zyxclmm_drm: zocl_destroy_client: client exits                                                               pid(566)
[   38.574454] zocl-drm amba_pl@0:zyxclmm_drm: zocl_create_client: created KDS c                                                              lient for pid(566), ret: 0
Trying to program device[0]: edge
[   38.616196] [drm] skip kind 29(AIE_RESOURCES) return code: -22
[   38.616209] [drm] skip kind 0(BITSTREAM) return code: -22
[48326.785]Loading PDI from DDR
[48326.852]Monolithic/Master Device
[48329.995]3.184 ms: PDI initialization time
[48333.849]+++Loading Image#: 0x0, Name: pl_cfi, Id: 0x18700000
[48339.362]---Loading Partition#: 0x0, Id: 0x103
[48343.784] 0.158 ms for Partition#: 0x0, Size: 34720 Bytes
[48348.776]---Loading Partition#: 0x1, Id: 0x105
[48353.288] 0.247 ms for Partition#: 0x1, Size: 4656 Bytes
[48358.108]---Loading Partition#: 0x2, Id: 0x205
[48364.146] 1.773 ms for Partition#: 0x2, Size: 33248 Bytes
[48367.522]---Loading Partition#: 0x3, Id: 0x203
[48371.811] 0.028 ms for Partition#: 0x3, Size: 544 Bytes
[48376.764]---Loading Partition#: 0x4, Id: 0x303
[48412.069] 31.040 ms for Partition#: 0x4, Size: 5896080 Bytes
[48414.725]---Loading Partition#: 0x5, Id: 0x305
[48420.493] 1.501 ms for Partition#: 0x5, Size: 43856 Bytes
[48424.139]---Loading Partition#: 0x6, Id: 0x403
[48428.563] 0.161 ms for Partition#: 0x6, Size: 239344 Bytes
[48433.636]---Loading Partition#: 0x7, Id: 0x405
[48438.145] 0.247 ms for Partition#: 0x7, Size: 4656 Bytes
[48443.011]Subsystem PDI Load: Done
[   38.622056] [drm] found kind 19(BITSTREAM_PARTIAL_PDI)
[   38.754431] [drm] FPGA Manager load DONE
[   38.762520] [drm] skip kind 18(PDI) return code: -22
[   38.766440] [drm] found kind 8(IP_LAYOUT)
[   38.771410] [drm] found kind 9(DEBUG_IP_LAYOUT)
[   38.775426] [drm] skip kind 25(AIE_METADATA) return code: -22
[   38.779952] [drm] found kind 7(CONNECTIVITY)
[   38.785694] [drm] found kind 6(MEM_TOPOLOGY)
[   38.790001] [drm] Memory 0 is not reserved in device tree. Will allocate memo                                                              ry from CMA
[   38.802365] [drm] Memory 1 is not reserved in device tree. Will allocate memo                                                              ry from CMA
[   38.810728] SError Interrupt on CPU1, code 0xbf000000 -- SError
[   38.810735] CPU: 1 PID: 566 Comm: simple_vadd Tainted: G           O      5.1                                                              5.19-xilinx-v2022.1 #1
[   38.810741] Hardware name: Xilinx Versal vck190 Eval board revA (QSPI) (DT)
[   38.810744] pstate: 80000005 (Nzcv daif -PAN -UAO -TCO -DIT -SSBS BTYPE=--)
[   38.810749] pc : xrt_cu_hls_init+0xc8/0x120 [zocl]
[   38.810771] lr : xrt_cu_hls_init+0x64/0x120 [zocl]
[   38.810784] sp : ffff800009cc3600
[   38.810786] x29: ffff800009cc3600 x28: ffff0008009cc080 x27: ffff00080396f810
[   38.810794] x26: ffff00080396f810 x25: ffff80000917c518 x24: ffff0008009d8000
[   38.810799] x23: ffff000803a9fd00 x22: ffff0008009cc340 x21: ffff800009c9d000
[   38.810804] x20: ffff0008009d8000 x19: ffff000805565100 x18: ffffffffffffffff
[   38.810810] x17: 6d656d2065746163 x16: 6f6c6c61206c6c69 x15: ffff800089cc355d
[   38.810815] x14: 0000000000000001 x13: ffff800008000000 x12: ffff80000928c540
[   38.810820] x11: ffff80000928c540 x10: ffff800009500558 x9 : 000080009d36b000
[   38.810826] x8 : ffff800009ca5000 x7 : ffff00080095b528 x6 : 0000000000000001
[   38.810831] x5 : 00000002ffffffff x4 : 0000000000000005 x3 : 0000000000000001
[   38.810836] x2 : ffff800000e37000 x1 : 0000000000000000 x0 : 0000000000000000
[   38.810842] Kernel panic - not syncing: Asynchronous SError Interrupt
[   38.810844] CPU: 1 PID: 566 Comm: simple_vadd Tainted: G           O      5.1                                                              5.19-xilinx-v2022.1 #1
[   38.810848] Hardware name: Xilinx Versal vck190 Eval board revA (QSPI) (DT)
[   38.810850] Call trace:
[   38.810852]  dump_backtrace+0x0/0x190
[   38.810864]  show_stack+0x18/0x30
[   38.810869]  dump_stack_lvl+0x7c/0xa0
[   38.810875]  dump_stack+0x18/0x34
[   38.810878]  panic+0x14c/0x30c
[   38.810884]  add_taint+0x0/0xb0
[   38.810888]  arm64_serror_panic+0x6c/0x7c
[   38.810891]  do_serror+0x28/0x60
[   38.810894]  el1h_64_error_handler+0x30/0x50
[   38.810899]  el1h_64_error+0x78/0x7c
[   38.810902]  xrt_cu_hls_init+0xc8/0x120 [zocl]
[   38.810915]  cu_probe+0x190/0x27c [zocl]
[   38.810928]  platform_probe+0x68/0xe0
[   38.810934]  really_probe.part.0+0x9c/0x30c
[   38.810938]  __driver_probe_device+0x98/0x144
[   38.810942]  driver_probe_device+0x44/0x11c
[   38.810945]  __device_attach_driver+0xb4/0x120
[   38.810949]  bus_for_each_drv+0x78/0xd0
[   38.810955]  __device_attach+0xdc/0x184
[   38.810958]  device_initial_probe+0x14/0x20
[   38.810962]  bus_probe_device+0x9c/0xa4
[   38.810965]  device_add+0x36c/0x860
[   38.810970]  platform_device_add+0x114/0x234
[   38.810974]  subdev_create_cu+0xec/0x1b0 [zocl]
[   38.810987]  zocl_xclbin_read_axlf+0x8a8/0xc0c [zocl]
[   38.811000]  zocl_read_axlf_ioctl+0x18/0x24 [zocl]
[   38.811013]  drm_ioctl_kernel+0xc4/0x11c
[   38.811020]  drm_ioctl+0x214/0x44c
[   38.811025]  __arm64_sys_ioctl+0xb8/0xe0
[   38.811031]  invoke_syscall+0x54/0x124
[   38.811036]  el0_svc_common.constprop.0+0xd4/0xfc
[   38.811040]  do_el0_svc+0x48/0xb0
[   38.811045]  el0_svc+0x28/0x80
[   38.811048]  el0t_64_sync_handler+0xa4/0x130
[   38.811052]  el0t_64_sync+0x1a0/0x1a4
[   38.811056] SMP: stopping secondary CPUs
[   38.811062] Kernel Offset: disabled
[   38.811063] CPU features: 0x10000011,20000842
[   38.811066] Memory Limit: none
[   39.099261] ---[ end Kernel panic - not syncing: Asynchronous SError Interrup                                                              t ]---

```

Solution:

This error can happen when XRT tries to access kernel register but it is not accessible. When visiting a non-existing address in AXI, the Linux system will report an SError. The path for accessing the kernel register goes across the DFX boundary. The following reasons can cause this error:

- Aperture setup of the PL_CTRL_S_AXI interface. Make sure the APERTURE and HDL_ATTRIBUTE.LOCKED properties are applied to the PL_CTRL_AXI_S interface. If it is incorrect, the v++ link will assign incorrect address to kernels.

- PL_CTRL_S_AXI interface width. Make sure the PL_CTRL_AXI_S interface is freezed after doing validation because the validation step calculates the bus width. You can check the with by right click the VitisRegion.bd, select Generate Output Products. If you select "Do not launch", Vivado will generate the wrapper and skip IP synthesis. Check VitisRegion.v. If the PL_CTRL_S_AXI_araddr is defined as a bus like `input [43:0]PL_CTRL_S_AXI_araddr`, it is correct. If it is `input PL_CTRL_S_AXI_araddr`, it is incorrect.

### ERROR: [BD 41-2666] NoC BDC Aperture Error: Dead-end Aperture

Issue: When running Generate Block Design in step 1, Vivado reports the following error message:

```
ERROR: [BD 41-2666] NoC BDC Aperture Error: Dead-end aperture. BDC Aperture 0x800000000 [0x180000000] on /VitisRegion/DDR_2 is not used by any downstream NoC endpoints or BDC boundary apertures. A NoC BDC aperture must have a path to a NoC endpoint or downstream BDC aperture that covers the upstream BDC aperture. Downstream endpoints: /noc_ddr/MC0_2
```

Solution:

The error message shows that the BDC Aperture `0x800000000 [0x180000000]` on the BDC interface `/VitisRegion/DDR_2` is not used by downstream NOC endpoint `/noc_ddr/MC0_2`. It is because the noc_ddr only enables 2 GB memory space. Check that you followed the noc_ddr setup instructions to enable `DDR LOW 1` address region.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
