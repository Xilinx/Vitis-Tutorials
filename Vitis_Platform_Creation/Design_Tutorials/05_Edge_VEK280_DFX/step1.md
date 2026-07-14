<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ Platform Creation Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Step 1: Create the Hardware Design

This hardware design illustrates how to create a DFX hardware platform for the AMD Versal™ AI Edge VEK280 (`xcve2802`) board. You will use the AMD Versal™ DFX Extensible Embedded Example Design from the Vivado example store and target it at the VEK280 board, which contains an AI Engine-ML (AIE-ML) array.

The major difference between an AMD Vitis™ DFX platform and a flat Vitis platform is in the AMD Vivado™ design: a DFX platform needs to define the Block Design Container (BDC) for the Reconfigurable Partition (RP). The BDC defines the dynamic region.

Versal devices support partial reconfiguration for almost all component types. Refer to *Vivado Design Suite User Guide: Dynamic Function eXchange* [(UG909)](https://docs.amd.com/r/en-US/ug909-vivado-partial-reconfiguration/Design-Elements-Inside-Reconfigurable-Modules) for the list of supported element types.

Common partitioning practice in a Vitis platform design:

| IP Type                   | Static Region        | Dynamic Region                |
| :------------------------ | :------------------- | :---------------------------- |
| CIPS                      | Y                    |                               |
| AI Engine-ML              |                      | Y                             |
| Memory                    | Memory Controller    | Export platform interfaces    |
| Clock                     | Y                    | Y                             |
| Reset                     | Y                    | Y                             |
| Interrupt                 | Interrupt Controller | Export platform interfaces    |
| AXI Master for IP Control |                      | Export platform interfaces    |
| Decoupling for DFX        | DFX Decoupler        | Slice Registers when required |

- **Control, Interface & Processing System (CIPS)** must stay in the static region because its hard blocks cannot be reconfigured.
- **AI Engine-ML** must stay in the dynamic region because `v++` links acceleration kernels into it at application development time.
- If the **memory controllers** are shared by the static and dynamic regions, they should be instantiated in the static region.
- The static and dynamic regions can have their own **clock** generator and **reset** logic. Dynamic-region signals can be independent or derived from the static region.
- The **Interrupt Controller** should be instantiated in the static region to avoid reloading its driver after every DFX reconfiguration. Interrupt signals and platform AXI interface signals can be exported from the BDC.
- The **decoupling** logic on the boundary between the static and dynamic regions helps with timing closure and prevents metastability. XRT enables DFX Decoupler isolation before reconfiguration and disables it afterwards.

## Vivado Design Creation

### Project Creation

1. Create a workspace and launch Vivado:

   ```bash
   mkdir WorkSpace
   cd WorkSpace
   source <Vitis_Install_Directory>/settings64.sh
   vivado
   ```

2. Download the Versal Extensible Embedded Platform Example:

   1. Click **Tools → Vivado Store...**
   2. Click **OK** to agree to download open-source examples from the web.
   3. Select **Example Designs → Platforms → Versal DFX Extensible Embedded Platform** and click the download button on the toolbar.
   4. Click **Close** after installation completes.

3. Create the Versal DFX Extensible Embedded Platform example project:

   1. Click **File → Project → Open Example...**
   2. Click **Next**.
   3. Select **Versal DFX Extensible Embedded Platform** in the Select Project Template window.
   4. Enter the **project name** and **project location**. Keep **Create project subdirectory** checked. Click **Next**.
   5. Select **Versal AI Edge VEK280 Evaluation Platform** in the Default Part window. Click **Next**.
   6. Configure the Clocks settings. The default settings are sufficient for this tutorial; the dynamic clock used by Vitis acceleration kernels is `clk_out2` (156.25 MHz).
   7. Configure the Interrupt settings (63 interrupts use two cascaded AXI_INTC blocks; keep the default).
   8. Configure the Memory settings. The example design enables the on-board DDR4 by default; enable LPDDR4 if you need additional on-board memory.
   9. Click **Next**.
   10. Review the new project summary and click **Finish**.
   11. After a short delay, Vivado generates the example design.

At this stage, Vivado's block automation has already created the top-level block design and a dynamic-region block. It added the CIPS, AXI NoC, DFX Decoupler, and supporting logic blocks to the top diagram, and added the AIE-ML, AXI NoC, and clock IP to the dynamic region (`VitisRegion.bd`). It also applied all board presets for VEK280, generated the block design output products, and configured platform address apertures.

Open the `VitisRegion` block design from the Flow Navigator (**IP Integrator → Open Block Design**), then in **Platform Setup → Clock** mark `clk_out2` (156.25 MHz) as the default clock if it is not already selected.

### DFX Platform Submodule Notes for VEK280 / AIE-ML

The CED-generated DFX template is structurally the same as the VCK190 DFX template, with the following VEK280-specific notes:

- **Target part**: `xcve2802-vsvh1760-2MP-e-S` (AI Edge / AIE-ML).
- **AI Engine-ML subsystem**: the AIE-ML array replaces the classic AIE array. Memory bandwidth, tile organization, and the `aie-ml_sys_design` programming model differ from the classic AIE flow used on VCK190. The `system-user.dtsi` shipped under `ref_files/step2_sw/` declares the AIE-ML aperture (`xlnx,aie-gen = [02]`).
- **Board DTSI**: the platform is created with `board_dtsi="versal-vek280-revb"` (see `ref_files/step2_sw/generate_platform.py`).
- **Memory**: the example design enables DDR4 UDIMM and LPDDR4. Both are routed through the NoC to the dynamic-region NoC stub.

### Modifying the IP Address Map (Optional)

This step is not mandatory. If you need to relocate an IP in the address map:

1. Determine the address space of the IP.
2. Open the address editor (**Window → Address Editor**).
3. Edit the base address inside the allowed FPD or LPD aperture. Refer to the [Versal Technical Reference Manual](https://docs.amd.com/r/en-US/am011-versal-acap-trm/High-level-Address-Map) for the device-level aperture list.

> **Note:** Modified base/high addresses must stay inside the original (FPD or LPD) aperture.

### Export Hardware Emulation XSA

Vitis hardware emulation requires SystemC TLM simulation models for CIPS, NoC, and AI Engine-ML. The Versal Extensible Platform Example pre-sets all three to `tlm`. To verify, select the CIPS instance in the Vivado canvas, open the **Block Properties** window, and confirm that `ALLOWED_SIM_MODELS` is `tlm,rtl` and `SELECTED_SIM_MODEL` is `tlm`. Repeat the check for the NoC and AI Engine-ML blocks.

Generate the emulation XSA:

```tcl
set_property PREFERRED_SIM_MODEL "tlm" [current_project]
generate_switch_network_for_noc
launch_simulation -scripts_only
launch_simulation -step compile
launch_simulation -step elaborate
set_property platform.platform_state "pre_synth" [current_project]
write_hw_platform -hw_emu -force -file vek280_custom_dfx_hw_emu.xsa
```

### Export Hardware XSA

1. Run implementation:

   1. Click **Generate Device Image** in the Flow Navigator and wait until implementation completes.
   2. Click **Open Implemented Design**.

2. Export the hardware XSA:

   ```tcl
   set_property platform.platform_state "impl" [current_project]
   write_hw_platform -force -fixed -static -file vek280_custom_dfx_static.xsa
   write_hw_platform -force -rp versal_dfx_platform_i/VitisRegion vek280_custom_dfx_rp.xsa
   ```

   - The static region is exported in **fixed XSA** format; this is what `bootgen` consumes when building the static `BOOT.BIN`.
   - The dynamic region is exported in **extensible XSA** format; this is what the `v++` linker uses when building each `xclbin` for the reconfigurable region.

### Fast Track

Scripts are provided to re-create the project and generate outputs in a single command. The Vivado run takes 45–60 minutes on a typical workstation.

To build the hardware design from the shell:

```bash
source <Vitis_Install_Directory>/settings64.sh
cd ref_files/step1_hw
make all
```

To clean the generated files:

```bash
make clean
```

A top-level all-in-one driver is also provided. To run steps 1 → 2 → 3 with one command, from `ref_files/`:

```bash
cd ref_files
make all COMMON_IMAGE_VERSAL=<path/to/xilinx-versal-common-v2026.1/>
```

To clean every step:

```bash
make clean
```

### Next Step

You have completed the hardware platform creation flow. Continue with [Step 2: Create the Software Components and the Vitis DFX Platform](./step2.md).

## References

- Turn a Block Design Container into a Reconfigurable Partition in *Vivado Design Suite User Guide: Dynamic Function eXchange* [(UG909)](https://docs.amd.com/r/en-US/ug909-vivado-partial-reconfiguration/Turn-a-Block-Design-Container-into-a-Reconfigurable-Partition)
- Create a Floorplan for the Reconfigurable Region in *Vivado Design Suite User Guide: Dynamic Function eXchange* [(UG909)](https://docs.amd.com/r/en-US/ug909-vivado-partial-reconfiguration/Create-a-Floorplan-for-the-Reconfigurable-Region)
- [Versal AI Edge Series VEK280 Evaluation Kit User Guide (UG1612)](https://docs.amd.com/r/en-US/ug1612-vek280-eval-bd)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
