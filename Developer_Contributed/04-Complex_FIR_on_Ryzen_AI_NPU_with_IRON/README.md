<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ Developer Contributed Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Bit-Exact Complex FIR Filter on the AMD Ryzen AI NPU using IRON / MLIR-AIE

***Toolchain: MLIR-AIE v1.4.1 (commit `3ca0193`) with LLVM-AIE (Peano) `21.0.0.2026080301+c9c5ecb7`***

- Target platform: Any AMD Ryzen 7040 / 8040-series laptop with the Phoenix NPU (XDNA1, AIE2 tile array)
- Host operating system: Windows 11 Pro 25H2
- Last update: 16 Aug 2026

> **Note**
> This is a Developer Contributed tutorial for the **Ryzen AI NPU** target using the **IRON / MLIR-AIE** Python-first compiler toolchain — not the classic `aiecompiler` Versal AIE flow used elsewhere in this repository. See [`doc/SETUP.md`](doc/SETUP.md) for the exact toolchain versions and installation notes.

<hr class="sphinxhide"></hr>

## Overview

This tutorial walks through implementing an 8-tap complex FIR filter end-to-end on the AMD Phoenix NPU (XDNA1) inside Ryzen 7040 / 8040-class laptops, using the open-source IRON / MLIR-AIE compiler stack.

The tutorial covers:

1. The mathematical statement of the complex FIR filter and its DTSP-textbook direct form.
2. The AIE2 tile kernel written in C++, using `bfloat16` operand load, `float32` multiply-accumulate, and a single `bfloat16` truncation on store.
3. The Python IRON `Runtime` sequence-function host program that instantiates the tile, wires up the buffers, and runs the design on real silicon.
4. A NumPy reference that implements the same operand and rounding contract element-for-element, used as the bit-exact acceptance oracle.

The resulting test is bit-accurate against the reference on real silicon — every output `bfloat16` byte matches the NumPy oracle, on every one of a hundred randomized seeds. This is the same M19 milestone that ships in the upstream [`phoenix-sdr-dsp`](https://github.com/midhatn/phoenix-sdr-dsp) v1.0.0 release, extracted here as a stand-alone tutorial.

## Why complex FIR, why Ryzen AI, why IRON

The existing Vitis-Tutorials AIE catalog contains excellent FIR examples targeting Versal AI Engine tiles on VCK190-class boards ([`AIE/Design_Tutorials/07-firFilter_AIEvsHLS`](../../AI_Engine_Development/AIE/Design_Tutorials/07-firFilter_AIEvsHLS), [`AIE/Design_Tutorials/02-super_sampling_rate_fir`](../../AI_Engine_Development/AIE/Design_Tutorials/02-super_sampling_rate_fir)) using the classic Vitis `aiecompiler` flow. This tutorial fills a different gap:

- **Different silicon.** The Ryzen 7040 / 8040 Phoenix NPU is a shipping mainstream consumer laptop part, not an FPGA evaluation board. The AIE2 tile array on Phoenix is architecturally close to the AIE-ML tiles on Versal but the deployment environment (Windows-native XDNA driver + XRT SHIM) and the compiler toolchain (IRON / MLIR-AIE + LLVM-AIE Peano) are meaningfully different.
- **Different toolchain.** IRON is the open-source Python-first host API sitting on top of MLIR-AIE. There is no `.mmc` graph, no `aiecompiler` invocation, no Vitis IDE step — the entire design is expressed as a Python `Runtime` object that compiles a C++ tile kernel through Peano and dispatches to the NPU through pyxrt.
- **Different signal.** The kernel is *complex*-valued (`I + j·Q`) with complex taps, using the classic four-real-multiply expansion of `(a+jb)(c+jd)`. This is the base-band DSP building block for every SDR receiver front-end.

## Structure

```
04-Complex_FIR_on_Ryzen_AI_NPU_with_IRON/
├── README.md                    (this file)
├── LICENSE                      (MIT)
├── src/
│   ├── fir_complex_kernel.cc    AIE2 tile kernel (bfloat16 operands, fp32 MAC)
│   └── test_fir_complex_m19.py  IRON host program + NumPy reference + main() driver
├── scripts/
│   └── run_tutorial.ps1         PowerShell helper: sanity-check ironenv, run the test
└── doc/
    ├── SETUP.md                 Toolchain versions, install prerequisites, driver notes
    ├── WALKTHROUGH.md           Step-by-step math → kernel → host → verify explanation
    └── M19_DESIGN.md            Full design document (from phoenix-sdr-dsp upstream)
```

## Getting started

1. Read [`doc/SETUP.md`](doc/SETUP.md) and confirm you have the exact toolchain versions.
2. Read [`doc/WALKTHROUGH.md`](doc/WALKTHROUGH.md) for the math → kernel → host walkthrough.
3. Run the test from PowerShell inside your `ironenv` virtual environment:
   ```powershell
   cd 04-Complex_FIR_on_Ryzen_AI_NPU_with_IRON
   .\scripts\run_tutorial.ps1
   ```
   Expected result: the test prints `PASS` for each host-side reference check (impulse, DC, pure tone, real-taps degeneration) and a final `PASS!` for the silicon random-I/Q vector, with maximum absolute error at most one `bfloat16` ULP against the NumPy reference. The device line reads `Target Device: <abc.NPU1 object at 0x...>`.

## License and attribution

- This tutorial is contributed under the MIT License — see [`LICENSE`](LICENSE).
- The kernel and test are extracted from [`midhatn/phoenix-sdr-dsp`](https://github.com/midhatn/phoenix-sdr-dsp) v1.0.0 (MIT-licensed), authored by Midhat Nashar.
- The mathematical treatment references Oppenheim & Schafer, *Discrete-Time Signal Processing*, 3rd ed., §5.2 (direct-form FIR), and NIST DLMF §1.9 (complex multiplication).
- Upstream project: [github.com/midhatn/phoenix-sdr-dsp](https://github.com/midhatn/phoenix-sdr-dsp)

### Credits

- **Lead Architect & Maintainer:** Midhat Nashar ([@midhatn](https://github.com/midhatn))
- **AI Architecture & Engineering Partner:** Perplexity AI (Senior AMD XDNA / AIE & DSP Copilot)

Same partner credit as the upstream [`phoenix-sdr-dsp` README §7](https://github.com/midhatn/phoenix-sdr-dsp#7-credits--acknowledgments). Copyright and MIT license grant are held solely by the Lead Architect; the AI partner does not hold or claim any copyright interest.

<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2026 Midhat Nashar. Licensed under the MIT License.</sub></p>

<p class="sphinxhide" align="center"><sup>Contributed to the AMD Vitis™ Developer Contributed Tutorials collection.</sup></p>
