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

# Iteration Guidelines

When using the Vitis flow to create a VEK280 DFX platform and develop acceleration applications, iteration falls into three categories: hardware updates, software updates, and application updates. Follow the rules below to avoid recompiling more than necessary.

## 1. Hardware Updates (Vivado design changes)

- Re-export the **XSAs** (`vek280_custom_dfx_static.xsa`, `vek280_custom_dfx_rp.xsa`, `vek280_custom_dfx_hw_emu.xsa`) from Vivado after any block-design change in [Step 1](./step1.md).
- Re-run [Step 2](./step2.md) to rebuild the Vitis platform against the new XSAs. The `generate_platform.py` and `build_platform.py` scripts pick up the new static / RP XSAs from `step1_hw/build/vivado/`.
- Clean and rebuild the validation applications in [Step 3](./step3.md):

  ```bash
  cd ref_files/step3_validate
  make ultraclean
  make all
  ```

## 2. Software Component Updates

- If you change `system-user.dtsi` or the common image, copy the updated component into `step2_sw/build/boot/` (or `sd_dir/`), then re-run `make` in `step2_sw/`. For a device-tree change specifically, you also need to regenerate `system.dtb` via `sdtgen` — the Makefile handles this automatically.
- After the platform is rebuilt, clean and rebuild the applications in [Step 3](./step3.md) so they pick up the new XPFM.

## 3. Application Updates

### `vadd` (PL-only)

- Pure host change: re-link the host. The Makefile's `cpVaddCode` target only copies sources; deleting `vadd_work/simple_vadd` before `make vadd_emu` (or `make vadd_hw`) is enough to force a relink.
- Kernel change: delete the kernel XO and the linked XSA, then rebuild:

  ```bash
  cd ref_files/step3_validate/vadd_work
  rm -f _x.hw/vadd.xo build_dir.hw.*/vadd.link.xsa build_dir.hw.*/vadd.xclbin
  cd ..
  make vadd_hw
  ```

### AIE-ML (`aieml_system`)

The AIE-ML sources live under `ref_files/step3_validate/aie_src/`. Edit them in place — `make` re-stages them into `aie_work/` on every invocation because `cpAieCode` is a phony-style copy.

| Change                                                       | Minimum rebuild                                                                 |
| ------------------------------------------------------------ | ------------------------------------------------------------------------------- |
| Host-only (`src/host.cpp`, `system_settings*.h`)             | `rm aie_work/aieml_system && make aie_adder_emu`                                |
| PL HLS kernel (`src/mm2s_8_128.cpp`, `src/s2mm_*_128.cpp`)   | `rm aie_work/<kernel>.xo aie_work/aieml.xsa aie_work/krnl_aieml.xclbin && make aie_adder_emu` |
| AIE-ML graph (`src/graph.cpp`, `src/MultGraph.h`, `src/matmult.cpp`, `src/kernels.h`, `src/tiling_parameters.h`) | `rm -rf aie_work/Work0 aie_work/libadf*.a aie_work/aieml.xsa aie_work/krnl_aieml.xclbin && make aie_adder_emu` |
| `system.cfg` (connectivity / clock)                          | Same as PL kernel change — re-link the xclbin.                                  |
| Reference data (`data/*.txt`)                                | Delete `aie_work/data/` and let `cpAieCode` re-stage it.                        |

For a completely fresh start, `make ultraclean` removes both `vadd_work/` and `aie_work/`.

## Iterating on DFX Reconfiguration Behaviour

If you are exercising the DFX path on hardware (loading multiple RMs against the same boot image), no rebuild of the boot image or platform is needed when you switch between `xclbin` files at runtime. Only re-package (`make vadd_hw` / `make aie_adder_hw`) when the underlying RM hardware contract changes (kernel signatures, stream topology, AIE-ML graph shape).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
