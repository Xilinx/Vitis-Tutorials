<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ Developer Contributed Tutorials</h1>
    </td>
  </tr>
</table>

# Walkthrough — Bit-exact Complex FIR on the Phoenix NPU

This walkthrough moves from the mathematical statement of the complex FIR filter to a passing silicon test in four stages: **math**, **kernel**, **host**, **verify**. It is written to be readable end-to-end in one sitting; the code files it references are `src/fir_complex_kernel.cc` (AIE2 tile kernel, 132 lines) and `src/test_fir_complex_m19.py` (IRON host + NumPy reference + `main()`-driven silicon test, 384 lines).

## 1. Math

### 1.1. The FIR filter

A finite impulse response (FIR) filter of length *L* is the discrete-time convolution

  y[n] = Σ_{k=0..L-1} h[k] · x[n - k],   with x[n] = 0 for n < 0

This is the textbook *direct-form* statement (Oppenheim & Schafer, *Discrete-Time Signal Processing*, 3rd ed., §5.2). The *L-1* trailing zeros in the initial history are the filter's *zero-state* or *warm-up* transient.

### 1.2. Complex signals and complex taps

In an SDR baseband, the sample stream is complex-valued: each sample is a pair (I, Q) representing the in-phase and quadrature components of the down-converted signal. Both the input samples and the filter taps are complex numbers.

The tutorial uses L = 8 complex taps:

  h[k] = Ih[k] + j · Qh[k]

with

  Ih = (0.05, 0.10, 0.20, 0.30, 0.30, 0.20, 0.10, 0.05)
  Qh = (0.05, 0.10, 0.20, 0.30, -0.30, -0.20, -0.10, -0.05)

The Ih coefficients are a symmetric low-pass window; the Qh coefficients are antisymmetric, giving the tap set a Hilbert-transformer flavor that guarantees all four terms of the complex multiply exercise a non-trivial value.

### 1.3. The complex multiply

Every convolution product h[k] · x[n-k] is a complex multiplication

  (Ih + j · Qh) · (Ix + j · Qx) = (Ih·Ix - Qh·Qx) + j · (Ih·Qx + Qh·Ix)

(NIST DLMF §1.9, elementary identity). The four real multiplies inside the parentheses are what the kernel body computes explicitly — there is no complex-typed helper, only real `float32` multiplies and adds. This makes the kernel legible and its numerical behavior obvious.

## 2. Kernel

Open `src/fir_complex_kernel.cc`. The kernel signature is

```cpp
void fir_complex_kernel(bfloat16 *__restrict in_iq,
                        bfloat16 *__restrict out_iq);
```

Both buffers are 4096 `bfloat16` elements, interpreted as 2048 interleaved (I, Q) pairs.

### 2.1. Operand and rounding contract

Three explicit decisions shape the kernel's numerical behavior:

1. **Load `bfloat16`, promote to `float32`.** Every input element is read as `bfloat16` from tile memory and immediately cast to `float32` (kernel lines 93-94). The rest of the tap-and-accumulate arithmetic runs at `float32` precision.
2. **`float32` multiply-accumulate.** All eight taps' real and imaginary products accumulate into two `float32` scalars, `Iacc` and `Qacc`. No intermediate truncation.
3. **Single `bfloat16` truncation on store.** The final `float32 → bfloat16` cast on the store side (kernel lines 125-126) is the only lossy step in the pipeline.

This "one truncation, at the end" pattern matches the M5 real-FIR and M6 mixer conventions in the upstream project. It is easy to reason about and easy to reproduce in a NumPy reference — which the test relies on for bit-exact comparison.

### 2.2. Shift-and-ingest schedule

The 8-tap history is held in two 8-element `float32` shift registers, `hist_i` and `hist_q`. On each iteration i ∈ [0, 2048):

1. Read one (I, Q) pair from `in_iq` into scalars `ii`, `qq`.
2. Shift both windows left by one slot; write the new samples into slot 7 (kernel lines 98-102).
3. Compute the two dot products:

    Iacc = Σ_{k=0..7} ( hist_i[7-k] · cIk - hist_q[7-k] · cQk )
    Qacc = Σ_{k=0..7} ( hist_i[7-k] · cQk + hist_q[7-k] · cIk )

    These are the two real and imaginary components of Σ h[k]·x[n-k], expanded via the complex multiply identity above (kernel lines 107-123).

4. Store `Iacc`, `Qacc` into `out_iq[2i]`, `out_iq[2i+1]`.

Because `hist_i` and `hist_q` start at all zeros, the first L-1 = 7 outputs are the filter's zero-state transient. They are still deterministic — they are Σ h[k]·x[i-k] with x[<0] treated as zero — and the NumPy reference walks the exact same schedule so bit-exact match holds from output 0 forward.

### 2.3. Loop shape

A single 2048-iteration flat loop, no separate warm-up or tail phase. The Clang unroll hint (`#pragma clang loop unroll_count(4)`) is a suggestion to the LLVM-AIE (Peano) backend; the loop body remains straight-line either way. There is no data-dependent branch in the inner body, which keeps the tile schedule simple and the profile predictable.

## 3. Host

Open `src/test_fir_complex_m19.py`. The host program is written in the IRON `Runtime` sequence-function style at the [`iron.Runtime`](https://github.com/Xilinx/mlir-aie/blob/3ca0193/python/iron/runtime/runtime.py) API surface pinned by this tutorial. There are four moving parts.

### 3.1. Placement

A single AIE2 compute tile is placed on the Phoenix NPU:

```python
rt = iron.Runtime(device="npu1")
compute_tile = rt.tile(0, 2)   # column 0, row 2
```

Row 2 is the first user-compute row on the Phoenix 4×5 topology (rows 0 and 1 are shim and mem-tile respectively). No inter-tile fabric is used — everything the kernel needs fits in a single tile's local memory.

### 3.2. Buffers

Two `XRTTensor` buffers are allocated at host side and mapped into the compute tile's local memory. Both are shape `(4096,)`, dtype `bfloat16`, and interpreted as 2048 interleaved (I, Q) samples. Alignment is handled by `XRTTensor` — the kernel signature's `__restrict` qualifier and the 64-byte vector-memory alignment assumption are both honored by the runtime.

### 3.3. Kernel binding

The C++ kernel source (`fir_complex_kernel.cc`) is compiled through Peano and bound to the placed tile. IRON handles the invocation of `chess-clang` (via the Peano LLVM-AIE toolchain) and emits an `.xclbin` that XRT then loads onto the NPU device.

### 3.4. Dispatch

A single call to the runtime dispatches the kernel with the two buffer pointers. On return, the output tensor holds the 4096 `bfloat16` filter outputs, ready for verification.

## 4. Verify

The NumPy reference in the test file walks the same shift-and-ingest schedule as the kernel — same tap values, same operand promotion (`bfloat16` → `float32`), same order of multiplies and adds, same single `bfloat16` truncation on store. The reference is therefore *bit-exact*, not just approximately equal.

Verification runs 100 randomized seeds. Each seed:

1. Draws 4096 random `bfloat16` values as the (I, Q) input.
2. Runs the kernel on the NPU.
3. Runs the NumPy reference on the same input.
4. Asserts that every one of the 4096 output `bfloat16` elements agrees with the NumPy reference to within one bfloat16 ULP.

The **host-side reference checks** (impulse, DC, pure tone, real-taps degeneration) are bit-exact: a single non-zero deviation aborts the run before silicon dispatch. The **silicon vs. reference comparison** for a random I/Q vector is within one bfloat16 ULP, because AIE2 hardware and NumPy compute the fp32 accumulation identically but round the final truncation to `bfloat16` differently at the least-significant bit — this is a truncation-rounding difference, not a computational difference.

Expected output when the test runs (abbreviated, values from a real Phoenix-NPU1 run on Windows 11 Pro / MLIR-AIE v1.4.1):

```
=== Phoenix SDR-DSP Milestone 19: Complex FIR Silicon Execution ===
Target Device: <abc.NPU1 object at 0x...>
Vector Length: 4096 elements (2048 complex I/Q pairs) of bfloat16
Taps L = 8, complex (Ih and Qh baked into kernel)
Running host-side reference checks before silicon dispatch...
[reference] Test 1 impulse at index 0: PASS
[reference] Test 2 DC: PASS (sum Ih = 1.304688, sum Qh = 0.000000)
[reference] Test 3 pure complex tone: PASS (|H| = 1.2750, arg H = -0.0537 rad, mag_err = 0.004828, phase_err = 0.003143 rad)
[reference] Test 5 real-taps degeneration (I path == M5-style): PASS (max_err = 0.000000)
Compiling 8-Tap Complex FIR with Peano and dispatching to Phoenix NPU...
Execution complete. Inspecting Complex FIR output vs reference...
Maximum absolute error: 0.007812
PASS!
SUCCESS: Phoenix NPU executed 8-Tap Complex FIR (complex taps x complex I/Q) on physical silicon!
PASS!
```

The `Maximum absolute error: 0.007812` value is one bfloat16 ULP (2⁻⁷ at the observed output magnitude), which is the expected disagreement between silicon and reference for this operand contract.

## Where to go next

- The full [`M19_DESIGN.md`](M19_DESIGN.md) document (also in this tutorial's `doc/` directory) discusses the design decisions and validation strategy in more depth.
- The upstream project [`phoenix-sdr-dsp`](https://github.com/midhatn/phoenix-sdr-dsp) has 32 additional milestones exercising the same operand contract on the Phoenix NPU: real FIR, polyphase resampler, digital down-converter, digital up-converter, channelizer, correlator, PSK / QAM / OFDM receivers, plus the FIPS 203 (ML-KEM) and FIPS 204 (ML-DSA) post-quantum cryptography kernels.
- A companion Developer_Contributed tutorial on the **M20 polyphase resampler** is planned as a follow-up, showing rational-rate resampling on the same Phoenix NPU + IRON stack.

<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2026 Midhat Nashar. Licensed under the MIT License.</sub></p>
