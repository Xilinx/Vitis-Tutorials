# Complex FIR design

## Scope

The tutorial implements one fixed block of a causal, 8-tap complex FIR on an AIE2 tile:

- input and output: 4096 interleaved `bfloat16` elements;
- complex samples: 2048;
- taps: eight complex `float32` constants compiled into the kernel;
- startup state: zero history;
- execution: one IRON worker and one input/output object FIFO pair.

The example does not retain history between calls, accept runtime taps, flush a trailing tail, or measure throughput.

## Mathematical specification

Let

\[
x[n] = I_x[n] + jQ_x[n], \qquad h[k] = I_h[k] + jQ_h[k].
\]

The output is

\[
y[n] = \sum_{k=0}^{7} h[k]x[n-k], \qquad x[n]=0 \text{ for } n<0.
\]

Expanding one complex product gives

\[
(I_x+jQ_x)(I_h+jQ_h)
= (I_xI_h-Q_xQ_h) + j(I_xQ_h+Q_xI_h).
\]

The kernel therefore computes

```text
Iout[n] = sum(Ix[n-k] * Ih[k] - Qx[n-k] * Qh[k], k=0..7)
Qout[n] = sum(Ix[n-k] * Qh[k] + Qx[n-k] * Ih[k], k=0..7)
```

The history arrays begin at zero. At iteration `n`, the new sample enters slot 7, so slot `7-k` contains sample `x[n-k]`.

## Tap set

```text
Ih = (+0.05, +0.10, +0.20, +0.30, +0.30, +0.20, +0.10, +0.05)
Qh = (+0.05, +0.10, +0.20, +0.30, -0.30, -0.20, -0.10, -0.05)
```

The nonzero real and imaginary components exercise all four terms of the complex product. The tap set is instructional; the tutorial does not claim a standards-qualified low-pass or Hilbert-transformer response.

## Data representation

Input and output use:

```text
[I0, Q0, I1, Q1, ..., I2047, Q2047]
```

The C++ kernel reads each `bfloat16` component into a `float`, performs source-level `float32` arithmetic with `float32` tap constants, then converts each result to `bfloat16` for storage.

The NumPy reference performs the same mathematical FIR and data-layout mapping. Its loop reduction and the compiler-generated AIE reduction are not asserted to have identical intermediate rounding. Compiler contraction or reassociation and host/device conversion details can produce small numerical differences.

## Validation rule

The silicon acceptance test is deliberately explicit and fail-closed:

```text
shape(actual) == shape(reference)
all values are finite
abs(actual[i] - reference[i]) <= 0.01 for every i
```

There is no relative-tolerance term. A constant absolute threshold is not a uniform bfloat16-ULP threshold because representable spacing varies with exponent. Accordingly, the tutorial makes neither a bitwise-equality claim nor a one-ULP claim.

The single silicon stimulus is a deterministic random vector generated with NumPy `RandomState(456)`. The directed impulse, DC, complex-tone, and real-FIR-reduction checks validate host reference behavior before compilation; they do not constitute additional NPU dispatches.

## IRON mapping

`test_fir_complex_m19.py` constructs:

- one full-block input `ObjectFifo`;
- one full-block output `ObjectFifo`;
- one `ExternalFunction` compiled from `fir_complex_kernel.cc`;
- one `Worker` with a 16 KiB stack request;
- one `Runtime` sequence that fills input and drains output; and
- one `Program` resolved for `iron.get_current_device()`.

The host enforces the fixed ABI before program construction:

- `N` must be 4096; and
- `element_type` must be `ml_dtypes.bfloat16`.

After dispatch, `XRTTensor.numpy()` uses the public pinned API to reconcile device output to host memory.

## References

- NIST Digital Library of Mathematical Functions, complex-number conventions: https://dlmf.nist.gov/1.9
- MLIR-AIE repository: https://github.com/Xilinx/mlir-aie
- MLIR-AIE v1.4.1 native Windows guide: https://xilinx.github.io/mlir-aie/1.4.1/buildHostWinNative/
- `ml_dtypes` bfloat16 implementation: https://github.com/jax-ml/ml_dtypes
- AMD XDNA architecture overview: https://www.amd.com/en/technologies/xdna.html
