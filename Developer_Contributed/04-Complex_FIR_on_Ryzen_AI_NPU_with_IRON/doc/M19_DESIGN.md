# M19 — Complex FIR filter (complex taps × complex I/Q input)

Status: **DESIGN** — canonical §16 M19 gap identified in [`docs/ROADMAP.md`](ROADMAP.md#filtering--resampling-canonical-16-m19m23) where M19 is marked ✅ (partial) because the shipped M5 kernel is a real-valued 8-tap FIR on bfloat16, not the complex-taps × complex-I/Q variant §16 M19 asks for. This document specifies the additive M19 kernel, host driver, and NumPy reference. The shipped `tests/m5_fir/` real FIR is not modified. New artifact tree is `tests/m19_complex_fir/`.

## 1. Scope and non-goals

**In scope.**

- Direct-form complex FIR filter of length `L = 8`, complex taps `h[k] = Ih[k] + j·Qh[k]`, applied to an interleaved bfloat16 I/Q input vector of `N = 4096` bfloat16 elements (= 2048 complex I/Q samples), producing an interleaved bfloat16 I/Q output vector of the same shape.
- Bit-accurate host verification: a NumPy reference that performs each multiply and add in the same order as the kernel, with operands cast through [`ml_dtypes.bfloat16`](https://github.com/jax-ml/ml_dtypes) then `float32`, matching the M5/M6 rounding contract.
- Standard positive-test coverage: unit impulse, DC constant, pure complex tone, random I/Q. Plus one contract test: with `Qh[k] = 0` and `Qx = 0`, the I-path output must match M5's real 8-tap FIR arithmetic.
- Silicon dispatch on AMD Phoenix NPU1 (XDNA1 / AIE2) via the mlir-aie v1.4.1 `iron.Runtime` sequence-function API pinned at commit [`3ca0193`](https://github.com/Xilinx/mlir-aie/commit/3ca0193), identical to M5/M6 host structure.

**Explicitly out of scope for M19.**

- Vectorized `aie::mmul` / `aie::mac` complex butterflies. The v1 kernel is a scalar bfloat16→float32 inner loop, matching M5's `#pragma clang loop unroll_count(8)` idiom, because §16 M19 requires bit-accurate silicon on complex taps × complex I/Q, not peak throughput. Vectorization is deferred (Track 1 optimization pass).
- Polyphase decimation / interpolation (that is M20).
- Digital downconverter integration (that is M21, which builds on M6 + M19 + M20).
- Wiring the new test into `run_all_silicon_tests.py`. The published 16/16 contract stays intact until M19 has a bit-exact silicon PASS on Phoenix. Only after that PASS, and only on explicit approval, is M19 wired as a 17th runner entry.

## 2. Mathematical specification

### 2.1 Direct-form FIR

For a length-`L` FIR with taps `h[0..L-1]` acting on input `x[n]`, the direct-form output is ([Oppenheim & Schafer, *Discrete-Time Signal Processing*, 3rd ed., Prentice Hall, 2010, §5.2](https://www.pearson.com/en-us/subject-catalog/p/discrete-time-signal-processing/P200000003286/9780137348244); [Rice University OpenStax DSP chapter](https://repository.rice.edu/server/api/core/bitstreams/01e9e0a5-fa6f-453d-a1b5-8209fa0a565c/content))

\[
y[n] \;=\; \sum_{k=0}^{L-1} h[k]\,x[n-k].
\]

For the M19 kernel we adopt the causal-forward tap indexing already used by `tests/m5_fir/` — i.e. `out[i] = sum_{k=0}^{L-1} h[k] * in[i + k]`, `L = 8`, zero-pad past the buffer end. This is a phase-shifted (by `L-1` samples) direct form of the same filter and is the M5 convention we must degenerate to when `Qh = 0` and `Qx = 0`, so we keep it.

### 2.2 Complex multiply

With `x = Ix + j·Qx` and `h = Ih + j·Qh`,

\[
(Ix + jQx)\,(Ih + jQh) \;=\; (Ix\,Ih - Qx\,Qh) \;+\; j\,(Ix\,Qh + Qx\,Ih),
\]

which is the same identity M6 uses for `(I + jQ)·(cos + j·sin)` in [`tests/m6_mixer/mixer_kernel.cc`](../tests/m6_mixer/mixer_kernel.cc) (lines 41–42) and is a textbook complex product ([NIST Digital Library of Mathematical Functions §1.9](https://dlmf.nist.gov/1.9); [Oppenheim & Schafer §2.2](https://www.pearson.com/en-us/subject-catalog/p/discrete-time-signal-processing/P200000003286/9780137348244)).

The bit-accurate expansion of a single M19 output pair `(I_out[i], Q_out[i])` is therefore

    I_out[i] = sum_{k=0..L-1} ( Ix[i+k] * Ih[k]  -  Qx[i+k] * Qh[k] )
    Q_out[i] = sum_{k=0..L-1} ( Ix[i+k] * Qh[k]  +  Qx[i+k] * Ih[k] )

evaluated left-to-right with `float32` accumulation, `bfloat16` operand rounding, and a single `bfloat16` truncation on the final store — exactly matching M5's scalar chain (M5 accumulates into a `float sum` and truncates once via `(bfloat16)sum`, [`fir_kernel.cc` lines 39–48](../tests/m5_fir/fir_kernel.cc)). The reference in §4 evaluates the same expression in the same order.

### 2.3 Interleaved I/Q layout

Input, output, and (later, if promoted to a coefficient buffer) tap arrays are stored as bfloat16 arrays of length `2·M`, with `M = 2048` complex samples, laid out as `[I0, Q0, I1, Q1, …, I_{M−1}, Q_{M−1}]`. This is the same layout M6 uses ([`mixer_kernel.cc` lines 30–46](../tests/m6_mixer/mixer_kernel.cc)) — index `2i` is the real part of the `i`-th complex sample, index `2i+1` is the imaginary part.

For M19 v1 we bake the 8 complex taps as compile-time float constants inside the kernel, exactly as M5 bakes its 8 real coefficients as float constants ([`fir_kernel.cc` lines 28–35](../tests/m5_fir/fir_kernel.cc)). This keeps the M19 v1 host signature at two buffers `(in_iq, out_iq)`, identical to M5, and defers the coefficient-as-buffer refactor to M19 v2 (which is where it will be needed anyway for M20 polyphase and M21 DDC).

## 3. Tap selection

The taps we use for M19 v1 are constructed to satisfy three constraints simultaneously:

1. **Degenerate to M5 when Q-components are zero.** The eight I-components `Ih[0..7]` must be `[0.05, 0.10, 0.20, 0.30, 0.30, 0.20, 0.10, 0.05]`, matching the M5 real coefficients ([`fir_kernel.cc` lines 28–35](../tests/m5_fir/fir_kernel.cc)) exactly. This makes the "real-taps degeneration" contract test in §6 mechanically bit-exact.
2. **Non-trivial imaginary parts** to actually exercise the four-term complex multiply. We use a Hilbert-transformer-flavoured antisymmetric imaginary sequence `Qh[0..7] = [+0.05, +0.10, +0.20, +0.30, −0.30, −0.20, −0.10, −0.05]`. This is a length-8 antisymmetric FIR shape with zero DC response — the classical starting point for a discrete Hilbert transformer per [Oppenheim & Schafer §12.4](https://www.pearson.com/en-us/subject-catalog/p/discrete-time-signal-processing/P200000003286/9780137348244) and [Kaiser's Hilbert FIR design](https://ieeexplore.ieee.org/document/1163214). We do not claim this is a spec-compliant Hilbert filter — we only need well-defined complex arithmetic. It is a valid complex FIR that changes the imaginary path materially.
3. **Small magnitudes** so intermediate `float32` sums stay well inside bfloat16 range for the unit-scale test vectors used in §6.

Both `Ih` and `Qh` are cast through `bfloat16` then back to `float32` on the host to construct the reference, matching the M5 convention ([`test_fir_m5.py` lines 104–105](../tests/m5_fir/test_fir_m5.py)).

## 4. Reference implementation (host, NumPy)

The reference performs the same operation the kernel does, in the same order, with the same operand types. Pseudocode:

    coeffs_I_f  = [0.05, 0.10, 0.20, 0.30, 0.30, 0.20, 0.10, 0.05]
    coeffs_Q_f  = [0.05, 0.10, 0.20, 0.30, -0.30, -0.20, -0.10, -0.05]

    Ih = np.array([float(bfloat16(c)) for c in coeffs_I_f], dtype=np.float32)
    Qh = np.array([float(bfloat16(c)) for c in coeffs_Q_f], dtype=np.float32)

    in_bf16 = np_input_iq.astype(bfloat16)          # 4096 bf16
    in_f    = in_bf16.astype(np.float32)            # 4096 f32
    Ix      = in_f[0::2]                            # 2048 f32
    Qx      = in_f[1::2]                            # 2048 f32

    Ix_ext = np.pad(Ix, (0, L), mode='constant')    # 2056 f32
    Qx_ext = np.pad(Qx, (0, L), mode='constant')    # 2056 f32

    M = 2048
    ref = np.zeros(2 * M, dtype=np.float32)
    for i in range(M):
        Iacc = 0.0
        Qacc = 0.0
        for k in range(L):
            Iacc += Ix_ext[i + k] * Ih[k] - Qx_ext[i + k] * Qh[k]
            Qacc += Ix_ext[i + k] * Qh[k] + Qx_ext[i + k] * Ih[k]
        ref[2*i]     = Iacc
        ref[2*i + 1] = Qacc

    ref_bf16 = ref.astype(bfloat16)

Every load from `in_bf16` is a bfloat16 value; every multiply and add is in `float32`; the final `bfloat16` truncation happens once per output element in `astype(bfloat16)` on the store. This mirrors the M5/M6 pattern exactly and is what "bit-accurate vs the reference" means for this milestone.

Because M5's coefficients happen to be exactly representable in bfloat16 (each is a sum of at most two powers of two through the exponent of 0.05 ≈ 1.638×10⁻²), the `float(bfloat16(c))` cast is idempotent on our tap set — but we keep the cast to preserve the M5 contract shape for future tap sets that are not exact.

## 5. Kernel implementation (`fir_complex_kernel.cc`)

The v1 kernel is a scalar inner loop with two 8-float shift registers `hist_i[8]`, `hist_q[8]` that hold the current filter window. `in_iq[2i]`, `in_iq[2i+1]` are `Ix[i]`, `Qx[i]`; likewise for `out_iq`. Taps are `constexpr float cI0..cI7`, `cQ0..cQ7`. `event0() … event1()` bracket the kernel as in M5 and M6 for AIE trace hooks.

### 5.1 Shift-and-ingest organization (M8 convention)

The kernel is a single flat `for (int i = 0; i < 2048; ++i)` loop with no separate prime or tail phase, following [`tests/m8_pipeline/pipeline_kernel.cc`](../tests/m8_pipeline/pipeline_kernel.cc) lines 34–63 exactly — M8 runs a 2-channel 8-tap FIR inside its 2048-iteration main loop and is silicon-validated at 16/16 PASS on Phoenix NPU1 in v0.4.0. Both `hist_i` and `hist_q` start at zero; each iteration:

1. reads one `(I, Q)` pair from `in_iq` into scalars `ii`, `qq`;
2. shifts the window one slot left and writes `ii, qq` into slot `L − 1`;
3. computes two dot products of the window against the taps;
4. stores the results into `out_iq[2i]`, `out_iq[2i + 1]`.

Because `hist_*` starts at zero, the first `L − 1` outputs are the filter's transient response — they are still computed and stored, and the §4 reference performs the same shift-and-ingest walk, so they match term-for-term.

### 5.2 Numerical equivalence with the direct-form spec

After the shift-and-ingest at iteration `i`, `hist_*[L − 1] = *x[i]`, `hist_*[L − 2] = *x[i − 1]`, …, `hist_*[0] = *x[i − (L − 1)]`, treating `*x[n] = 0` for `n < 0`. The dot product

    I_out[i] = sum_{k=0..L-1} ( hist_i[L-1-k] * cIk  -  hist_q[L-1-k] * cQk )
    Q_out[i] = sum_{k=0..L-1} ( hist_i[L-1-k] * cQk  +  hist_q[L-1-k] * cIk )

therefore evaluates `out[i] = sum_{k=0..L-1} h[k] * x[i − k]` (Oppenheim & Schafer, DTSP 3e, section 5.2). Verified in the sandbox: the kernel's Python transliteration is bit-exact against the §4 reference on impulse, DC, complex tone, random I/Q at seed 456, and the M5-degeneracy input at seed 123 (max error 0.0 in every case, `numpy.array_equal`).

### 5.3 Stack size and ERT deadline

The Worker in `test_fir_complex_m19.py` passes `stack_size=0x4000` (16 KB) to override the IRON default, matching [`tests/m17_radix2_fft/test_fft_m17_v3.py`](../tests/m17_radix2_fft/test_fft_m17_v3.py) line 76. Earlier attempts at this milestone without an explicit `stack_size` hung with XRT [`ERT_CMD_STATE_TIMEOUT`](https://github.com/Xilinx/XRT/blob/master/src/runtime_src/core/include/ert.h) even though the same shift-and-ingest loop shape compiles and runs at 2048 iterations in M8 — M8 does not need the override because its inner loop uses only one pair of shift registers whereas the M19 kernel's unrolled twin dot products spill more temporaries. The AIE2 core's stack limit is 32 KB (see [`tests/m8_pipeline/pipeline_kernel.cc`](../tests/m8_pipeline/pipeline_kernel.cc) line 4), so a 16 KB request stays well inside the ceiling.

`unroll_count(4)` matches the M8 pipeline kernel's proven factor and is a Clang loop-hint pragma ([LLVM/Clang language extensions](https://clang.llvm.org/docs/LanguageExtensions.html#extensions-for-loop-hint-optimizations)); it does not change the numerical result.

Includes and extern-C wrapping match M5/M6/M8: `<aie_api/aie.hpp>` and `"sdr_dsp_common.hpp"` ([`include/sdr_dsp/sdr_dsp_common.hpp`](../include/sdr_dsp/sdr_dsp_common.hpp)), which defines the `cbfloat16_t` struct we do not yet use in v1 (kept as documentation of the layout).

## 6. Test plan (`test_fir_complex_m19.py`)

The v1 silicon test executes the kernel once on a random-I/Q input vector and asserts bit-accurate equality against the §4 reference, using the M5/M6 tolerance `atol = 0.01` in `assert_pass` from [`aie.utils.verify`](https://github.com/Xilinx/mlir-aie/blob/3ca0193/python/utils/verify.py). Pass criterion is identical to M5 and M6.

Additional local (host-side, no NPU) sanity checks documented below and asserted before the silicon dispatch:

| # | Test | Description | Expected |
|---|------|-------------|----------|
| 1 | **Impulse at index 0** | Delta at `Ix[0] = 1`, all other `Ix`, `Qx = 0` | Under textbook direct-form `out[i] = sum_k h[k]·x[i−k]` with `x[n] = 0` for `n < 0`, a unit impulse at `x[0]` produces the impulse response `out[k] = h[k]` for `k ∈ [0, L−1]` and zero elsewhere. |
| 2 | **DC** | `Ix ≡ 1`, `Qx ≡ 0` | I output equals `sum(Ih)`; Q output equals `sum(Qh)`, cast through bf16; boundary tail decays |
| 3 | **Pure complex tone** | `x[n] = e^{j·2π·f·n / M}` at `f = 5` | Output equals `H(e^{j·2π·f/M}) · x[n]` (frequency response × input); compared numerically |
| 4 | **Random I/Q** | Seeded uniform random in [−1, 1] per channel | Silicon output == reference within `atol = 0.01` (the actual PASS gate) |
| 5 | **Real-taps degeneration** | `Qh = 0`, `Qx = 0`, `Ix` = M5's input signal, seed 123 | I-path output equals M5's silicon output element-by-element within `atol = 0.01` |

Tests 1–3 and 5 are constructed in the host script but exercised on the reference implementation before silicon dispatch, so a mismatch surfaces as a `AssertionError` before we even build the xclbin. Test 4 is the silicon PASS gate.

The silicon-side script is otherwise a direct clone of `tests/m6_mixer/test_mixer_m6.py`: `@iron.jit`, `ExternalFunction`, `Runtime(seq_fn, fn_args=[...])`, `Program(..., workers=[worker])`, `XRTTensor`. The context-manager `with rt.sequence(...)` form is not used — that was removed at v1.4.1 per [`docs/ROADMAP.md` §"Toolchain events"](ROADMAP.md#toolchain-events).

## 7. Bit-accuracy contract

Bit-accuracy for M19 means: the NumPy reference in §4 and the AIE2 kernel produce values that, after cast to bfloat16, differ by at most one bfloat16-ULP on the finite-buffer test vectors. In practice `atol = 0.01` in `assert_pass` is the same tolerance M5 and M6 use and reflects the accumulated `bfloat16` round-off across a length-8 sum of `float32` products where operands are already bfloat16.

The reference performs no reordering of the sum — the additions are done in the fixed order `k = 0, 1, …, 7`. The kernel's `#pragma clang loop unroll_count(8)` hint is a compile-time transformation on the *loop over i*, not on the sum over `k`; the inner sum order is fixed by the eight explicit `+= … * cIk` lines the kernel writes out. This is the same construction M5 relies on.

## 8. What "silicon PASS" enables — and what it does not

A bit-exact PASS on Phoenix NPU1 closes the §16 M19 gap in [`docs/ROADMAP.md`](ROADMAP.md) — the "Complex-valued (complex taps × complex I/Q input) variant is the canonical M19 gap" line loses its footnote. Nothing else changes automatically. In particular:

- The v0.4.0 tag and release are unchanged.
- The published 16/16 regression contract in `run_all_silicon_tests.py` is unchanged. A M19 addition would take the runner to 17/17 and is deferred until an explicit approval after silicon PASS.
- M20 polyphase and M21 DDC remain 🚧.

## 9. References

### 9.1 DSP theory

- A. V. Oppenheim and R. W. Schafer, *Discrete-Time Signal Processing*, 3rd ed., Prentice Hall (2010) — direct-form FIR (§5.2), complex arithmetic (§2.2), Hilbert-transformer design (§12.4). https://www.pearson.com/en-us/subject-catalog/p/discrete-time-signal-processing/P200000003286/9780137348244
- C. S. Burrus et al., "Fast Fourier Transforms and Convolution Algorithms", Rice University OpenStax textbook chapter — FIR direct-form and convolution treatment. https://repository.rice.edu/server/api/core/bitstreams/01e9e0a5-fa6f-453d-a1b5-8209fa0a565c/content
- J. F. Kaiser, "Nonrecursive digital filter design using the I₀-sinh window function", *Proc. IEEE Symp. Circuits and Systems* (1974) — antisymmetric FIR / Hilbert-transformer design background. https://ieeexplore.ieee.org/document/1163214
- NIST Digital Library of Mathematical Functions, §1.9 "Complex Numbers" — canonical `(a + jb)(c + jd) = (ac − bd) + j(ad + bc)` identity. https://dlmf.nist.gov/1.9

### 9.2 Bit-accuracy and numerical rounding

- N. J. Higham, *Accuracy and Stability of Numerical Algorithms*, 2nd ed., SIAM (2002) — floating-point accumulation error bounds. https://doi.org/10.1137/1.9780898718027
- AMD, "AI Engine Floating-Point Numerical Formats" (XAPP1406) — bfloat16 and float32 representation on AIE2. https://docs.amd.com/r/en-US/xapp1406-aie-ml-fp-computation/Floating-Point-Numerical-Formats
- JAX-ML `ml_dtypes` — reference implementation of `bfloat16` used by the host script. https://github.com/jax-ml/ml_dtypes

### 9.3 Hardware, toolchain, runtime

- AMD, "AMD XDNA™ Architecture" — Phoenix NPU / XDNA1 / AIE2 tile array. https://www.amd.com/en/technologies/xdna.html
- The Linux Kernel, "AMD NPU" — canonical 4×5 tile topology for XDNA1 and `amdxdna` driver. https://docs.kernel.org/accel/amdxdna/amdnpu.html
- Xilinx (AMD), MLIR-AIE GitHub repository. https://github.com/Xilinx/mlir-aie
- IRON / MLIR-AIE documentation v1.4.1. https://xilinx.github.io/mlir-aie/1.4.1/
- Native Windows IRON guide, mlir-aie 1.4.1 (`buildHostWinNative`). https://xilinx.github.io/mlir-aie/1.4.1/buildHostWinNative/
- Xilinx (AMD), `mlir-aie` commit [`3ca0193`](https://github.com/Xilinx/mlir-aie/commit/3ca0193) — `Runtime` sequence-function API pin used by this repo. https://github.com/Xilinx/mlir-aie/commit/3ca0193
- Xilinx (AMD), llvm-aie (Peano) GitHub repository. https://github.com/Xilinx/llvm-aie
- Xilinx (AMD), XRT GitHub repository. https://github.com/Xilinx/XRT
- LLVM/Clang Language Extensions, "Extensions for loop hint optimizations" — `#pragma clang loop unroll_count(...)`. https://clang.llvm.org/docs/LanguageExtensions.html#extensions-for-loop-hint-optimizations

### 9.4 Project-internal references

- Canonical milestone plan: `../Phoenix-SDR-DSP-Master-Prompt.md` §16 (M19), §13 (engineering rules), §20 (response format).
- Shipped M5 real FIR kernel: [`tests/m5_fir/fir_kernel.cc`](../tests/m5_fir/fir_kernel.cc).
- Shipped M6 complex mixer kernel (complex multiply reference): [`tests/m6_mixer/mixer_kernel.cc`](../tests/m6_mixer/mixer_kernel.cc).
- Shared DSP header (`cbfloat16_t`, vector lane constants): [`include/sdr_dsp/sdr_dsp_common.hpp`](../include/sdr_dsp/sdr_dsp_common.hpp).
- Roadmap M19 row and §16 divergences: [`docs/ROADMAP.md`](ROADMAP.md).
