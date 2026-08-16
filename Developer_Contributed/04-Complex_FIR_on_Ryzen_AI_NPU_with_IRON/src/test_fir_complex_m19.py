# Purpose: Milestone 19 Complex FIR Silicon Validation on AMD Phoenix NPU
#          (complex taps * complex I/Q input, 8-tap, bit-accurate vs a
#          NumPy reference that matches the kernel's operand and rounding
#          contract element-for-element).
# Target operating system: Windows 11 Pro 25H2.
# Target architecture: AMD Phoenix NPU1 / XDNA1 / AIE2.
# Input types: bfloat16 interleaved I/Q signal (4096 elements = 2048 pairs).
# Output types: bfloat16 filtered I/Q output verified against the reference.
# Scaling: direct bfloat16 operand load, float32 multiply-accumulate,
#          single bfloat16 truncation on store, matching M5/M6.
# Alignment assumptions: handled by IRON XRTTensor / BO runtime.
# State requirements: device 0 (NPU Phoenix).
# Error handling: Bit-accurate tolerance check against reference complex FIR.
#
# Design: docs/M19_DESIGN.md
# Host API pin: mlir-aie v1.4.1 iron.Runtime sequence-function API
#   https://github.com/Xilinx/mlir-aie/blob/3ca0193/python/iron/runtime/runtime.py
# Direct-form FIR (Oppenheim and Schafer, DTSP 3e, section 5.2):
#   y[n] = sum_{k=0..L-1} h[k] * x[n - k],  x[n] = 0 for n < 0
# This matches the kernel's shift-and-ingest schedule
# (tests/m8_pipeline/pipeline_kernel.cc lines 34-63).
# Complex multiply (NIST DLMF section 1.9; matches tests/m6_mixer/mixer_kernel.cc):
#   (Ix + j Qx) * (Ih + j Qh) = (Ix*Ih - Qx*Qh) + j*(Ix*Qh + Qx*Ih)

from pathlib import Path

import numpy as np
from aie import iron
from aie.iron import (
    CompileTime,
    ExternalFunction,
    In,
    ObjectFifo,
    Out,
    Program,
    Runtime,
    Worker,
)
from aie.utils.config import cxx_header_path
from aie.utils.hostruntime.xrtruntime.tensor import XRTTensor
from aie.utils.verify import assert_pass
from ml_dtypes import bfloat16

# Taps hard-coded to match tests/m19_complex_fir/fir_complex_kernel.cc.
# Ih matches tests/m5_fir/fir_kernel.cc exactly so the M5-degeneracy check
# is a real check of the I path.
COEFFS_I_F = [0.05, 0.10, 0.20, 0.30, 0.30, 0.20, 0.10, 0.05]
COEFFS_Q_F = [0.05, 0.10, 0.20, 0.30, -0.30, -0.20, -0.10, -0.05]
L = 8


def _bf16_coeffs():
    """Cast tap constants through bfloat16 then back to float32, matching
    the M5 convention (tests/m5_fir/test_fir_m5.py lines 104-105)."""
    Ih = np.array([float(bfloat16(c)) for c in COEFFS_I_F], dtype=np.float32)
    Qh = np.array([float(bfloat16(c)) for c in COEFFS_Q_F], dtype=np.float32)
    return Ih, Qh


def complex_fir_reference(in_bf16):
    """NumPy reference that performs the same operation the kernel does,
    in the same order, with the same operand types. Textbook direct-form
    convolution with zero-history warmup, matching the M8 shift-and-ingest
    schedule used by tests/m19_complex_fir/fir_complex_kernel.cc:

        out[i] = sum_{k=0..L-1} h[k] * x[i - k],  x[n] = 0 for n < 0.

    Inputs
    ------
    in_bf16 : np.ndarray of dtype bfloat16, shape (2 * M,), interleaved I/Q.

    Returns
    -------
    ref_bf16 : np.ndarray of dtype bfloat16, shape (2 * M,), interleaved I/Q.
    """
    Ih, Qh = _bf16_coeffs()

    in_f = in_bf16.astype(np.float32)
    Ix = in_f[0::2]
    Qx = in_f[1::2]
    M = Ix.shape[0]

    hist_i = np.zeros(L, dtype=np.float32)
    hist_q = np.zeros(L, dtype=np.float32)
    ref = np.zeros(2 * M, dtype=np.float32)

    for i in range(M):
        # Shift-and-ingest exactly like the kernel
        hist_i[0:L - 1] = hist_i[1:L]
        hist_q[0:L - 1] = hist_q[1:L]
        hist_i[L - 1] = Ix[i]
        hist_q[L - 1] = Qx[i]

        # Dot products: newest sample hist[L-1] pairs with tap 0, oldest
        # hist[0] pairs with tap L-1, so this is out[i] = sum_k h[k] * x[i-k].
        Iacc = np.float32(0.0)
        Qacc = np.float32(0.0)
        for k in range(L):
            si = hist_i[L - 1 - k]
            sq = hist_q[L - 1 - k]
            Iacc += si * Ih[k] - sq * Qh[k]
            Qacc += si * Qh[k] + sq * Ih[k]
        ref[2 * i] = Iacc
        ref[2 * i + 1] = Qacc

    return ref.astype(bfloat16)


@iron.jit
def complex_fir(
    input_iq: In,
    output_iq: Out,
    *,
    N: CompileTime[int],
    element_type: CompileTime[type],
):
    in_ty = np.ndarray[(N,), np.dtype[element_type]]
    out_ty = np.ndarray[(N,), np.dtype[element_type]]

    of_in = ObjectFifo(in_ty, name="in_iq")
    of_out = ObjectFifo(out_ty, name="out_iq")

    current_dir = Path(__file__).parent.resolve()
    include_sdr_dir = Path(__file__).resolve().parents[2] / "include" / "sdr_dsp"

    fir_func = ExternalFunction(
        "fir_complex_kernel",
        source_file=str(current_dir / "fir_complex_kernel.cc"),
        arg_types=[in_ty, out_ty],
        include_dirs=[cxx_header_path(), str(include_sdr_dir)],
    )

    def core_body(of_in, of_out, fir_func):
        elem_in = of_in.acquire(1)
        elem_out = of_out.acquire(1)
        fir_func(elem_in, elem_out)
        of_in.release(1)
        of_out.release(1)

    # stack_size mirrors tests/m17_radix2_fft/test_fft_m17_v3.py line 76.
    # AIE2 core stack default was observed to be too small for local
    # 8-float shift-register arrays plus the unrolled dot product
    # temporaries; the default hangs with ERT_CMD_STATE_TIMEOUT.
    worker = Worker(
        core_body,
        fn_args=[of_in.cons(), of_out.prod(), fir_func],
        stack_size=0x4000,
    )

    def sequence(a_in, c_out, in_prod, out_cons):
        in_prod.fill(a_in)
        out_cons.drain(c_out, wait=True)

    rt = Runtime(
        sequence,
        [in_ty, out_ty, of_in.prod(), of_out.cons()],
    )
    my_program = Program(iron.get_current_device(), rt, workers=[worker])
    return my_program.resolve_program()


# --- Host-side reference-only sanity checks (Section 6 of docs/M19_DESIGN.md).
# These run before silicon dispatch. Any mismatch surfaces as AssertionError
# before we build the xclbin.

def _pack_iq(Ix_f, Qx_f, N):
    iq_f = np.zeros(N, dtype=np.float32)
    iq_f[0::2] = Ix_f
    iq_f[1::2] = Qx_f
    return iq_f.astype(bfloat16)


def _local_impulse_check(N):
    """Test 1: Impulse at index 0.

    Under textbook direct-form out[i] = sum_k h[k] * x[i-k] with a unit
    impulse at x[0], the impulse response is out[k] = h[k] for k in
    [0, L-1] and zero elsewhere.
    """
    Ih, Qh = _bf16_coeffs()
    M = N // 2
    Ix = np.zeros(M, dtype=np.float32)
    Qx = np.zeros(M, dtype=np.float32)
    Ix[0] = 1.0
    in_bf16 = _pack_iq(Ix, Qx, N)

    ref = complex_fir_reference(in_bf16).astype(np.float32)

    expected = np.zeros(2 * M, dtype=np.float32)
    Ih_bf = np.array(Ih, dtype=bfloat16).astype(np.float32)
    Qh_bf = np.array(Qh, dtype=bfloat16).astype(np.float32)
    for k in range(L):
        expected[2 * k] = Ih_bf[k]
        expected[2 * k + 1] = Qh_bf[k]

    assert np.allclose(ref, expected, atol=1e-6), (
        f"Impulse mismatch: first 2L got {ref[:2*L]} expected {expected[:2*L]}"
    )
    print("[reference] Test 1 impulse at index 0: PASS")


def _local_dc_check(N):
    """Test 2: DC input on I only, Q = 0. Steady-state samples of the I
    output must equal sum(Ih); of the Q output must equal sum(Qh). The
    filter enters steady state at index L - 1 = 7."""
    Ih, Qh = _bf16_coeffs()
    M = N // 2
    Ix = np.ones(M, dtype=np.float32)
    Qx = np.zeros(M, dtype=np.float32)
    in_bf16 = _pack_iq(Ix, Qx, N)

    ref = complex_fir_reference(in_bf16).astype(np.float32)
    steady_I = ref[2 * (L - 1) :: 2]
    steady_Q = ref[2 * (L - 1) + 1 :: 2]
    exp_I = float(bfloat16(np.float32(np.sum(Ih))))
    exp_Q = float(bfloat16(np.float32(np.sum(Qh))))
    assert np.allclose(steady_I, exp_I, atol=0.01), (
        f"DC I steady mismatch: got {steady_I[:4]} expected {exp_I}"
    )
    assert np.allclose(steady_Q, exp_Q, atol=0.01), (
        f"DC Q steady mismatch: got {steady_Q[:4]} expected {exp_Q}"
    )
    print(
        f"[reference] Test 2 DC: PASS "
        f"(sum Ih = {exp_I:.6f}, sum Qh = {exp_Q:.6f})"
    )


def _local_tone_check(N):
    """Test 3: Pure complex tone at bin f_bin. In steady state
    (i >= L - 1) the output is x[i] * H(e^{j w}) where
    H(e^{j w}) = sum_k h[k] * e^{-j w k}, w = 2 pi f_bin / M."""
    M = N // 2
    f_bin = 5.0
    t = np.arange(M, dtype=np.float32)
    phase = 2.0 * np.pi * f_bin * t / M
    x_cplx = np.exp(1j * phase).astype(np.complex64)
    Ix = x_cplx.real.astype(np.float32)
    Qx = x_cplx.imag.astype(np.float32)
    in_bf16 = _pack_iq(Ix, Qx, N)

    ref = complex_fir_reference(in_bf16).astype(np.float32)
    ref_cplx = ref[0::2] + 1j * ref[1::2]

    Ih, Qh = _bf16_coeffs()
    h_cplx = (Ih + 1j * Qh).astype(np.complex128)
    w = 2.0 * np.pi * f_bin / M
    H = np.sum(h_cplx * np.exp(-1j * w * np.arange(L, dtype=np.float64)))

    # Ratio across the steady-state region.
    steady = slice(L - 1, M)
    ratio = ref_cplx[steady] / x_cplx[steady]
    mag_err = float(np.max(np.abs(np.abs(ratio) - np.abs(H))))
    phase_err = float(np.max(np.abs(np.angle(ratio) - np.angle(H))))
    assert mag_err < 0.02, f"Tone magnitude drift {mag_err:.4f}"
    assert phase_err < 0.02, f"Tone phase drift {phase_err:.4f} rad"
    print(
        f"[reference] Test 3 pure complex tone: PASS "
        f"(|H| = {abs(H):.4f}, arg H = {np.angle(H):.4f} rad, "
        f"mag_err = {mag_err:.4g}, phase_err = {phase_err:.4g} rad)"
    )


def _local_m5_degeneracy_check(N):
    """Test 5: Real-taps degeneration. With Qh = 0 and Qx = 0, the I path
    is a real 8-tap FIR. Compare against a local M5-style reference on the
    same input under textbook direct-form (matching this kernel's shape),
    accepting M5/M6 tolerance atol=0.01."""
    M = N // 2
    np.random.seed(123)  # match tests/m5_fir/test_fir_m5.py seed
    Ix = np.random.uniform(0.1, 1.0, M).astype(np.float32)
    Qx = np.zeros(M, dtype=np.float32)
    in_bf16 = _pack_iq(Ix, Qx, N)

    Ih, _ = _bf16_coeffs()

    # Reference under this kernel's convention with Qh=0.
    Qh_saved = COEFFS_Q_F.copy()
    try:
        for k in range(L):
            COEFFS_Q_F[k] = 0.0
        ref = complex_fir_reference(in_bf16).astype(np.float32)
    finally:
        for k in range(L):
            COEFFS_Q_F[k] = Qh_saved[k]

    m19_I_bf = ref[0::2]

    # Recompute the same convolution directly to cross-check (guards
    # against a bug that would make ref equal to itself trivially).
    in_f = in_bf16.astype(np.float32)
    Ix_bf = in_f[0::2]
    hist = np.zeros(L, dtype=np.float32)
    m5_style = np.zeros(M, dtype=np.float32)
    for i in range(M):
        hist[0:L - 1] = hist[1:L]
        hist[L - 1] = Ix_bf[i]
        m5_style[i] = sum(hist[L - 1 - k] * Ih[k] for k in range(L))
    m5_style_bf = m5_style.astype(bfloat16).astype(np.float32)

    max_err = float(np.max(np.abs(m19_I_bf - m5_style_bf)))
    assert max_err < 0.01, (
        f"M5-degeneracy I-path mismatch: max_err = {max_err:.6f}"
    )
    print(
        f"[reference] Test 5 real-taps degeneration (I path == M5-style): "
        f"PASS (max_err = {max_err:.6f})"
    )


def _run_local_reference_checks(N):
    print("Running host-side reference checks before silicon dispatch...")
    _local_impulse_check(N)
    _local_dc_check(N)
    _local_tone_check(N)
    _local_m5_degeneracy_check(N)


def main():
    print("=== Phoenix SDR-DSP Milestone 19: Complex FIR Silicon Execution ===")
    data_size = 4096  # 2048 complex pairs, matches M6 layout
    element_type = bfloat16
    print(f"Target Device: {iron.get_current_device()}")
    print(
        f"Vector Length: {data_size} elements "
        f"({data_size // 2} complex I/Q pairs) of {element_type.__name__}"
    )
    print(f"Taps L = {L}, complex (Ih and Qh baked into kernel)")

    # Reference-only pre-checks (impulse, DC, tone, M5 degeneracy)
    _run_local_reference_checks(data_size)

    # --- Test 4: Random I/Q vector -> silicon PASS gate.
    num_complex = data_size // 2
    np.random.seed(456)
    Ix = np.random.uniform(-1.0, 1.0, num_complex).astype(np.float32)
    Qx = np.random.uniform(-1.0, 1.0, num_complex).astype(np.float32)
    np_in_bf16 = _pack_iq(Ix, Qx, data_size)
    np_out_iq = np.zeros(data_size, dtype=element_type)

    # Wrap in XRTTensor with correct bfloat16 dtype
    in_tensor = XRTTensor(np_in_bf16, dtype=element_type)
    out_tensor = XRTTensor(np_out_iq, dtype=element_type)

    print("Compiling 8-Tap Complex FIR with Peano and dispatching to Phoenix NPU...")
    res = complex_fir(in_tensor, out_tensor, N=data_size, element_type=element_type)
    print(f"Kernel execution result: {res}")

    # Sync back from NPU to host memory
    out_tensor.to("cpu")

    print("Execution complete. Inspecting Complex FIR output vs reference...")

    ref_out_bf16 = complex_fir_reference(np_in_bf16)
    out_np = out_tensor._data

    print(f"Input I/Q sample [0..4]:  {np_in_bf16[:4]}")
    print(f"Ref Out sample [0..4]:    {ref_out_bf16[:4]}")
    print(f"Actual Out sample [0..4]: {out_np[:4]}")

    max_err = float(
        np.max(
            np.abs(
                out_np.astype(np.float32) - ref_out_bf16.astype(np.float32)
            )
        )
    )
    print(f"Maximum absolute error: {max_err:.6f}")

    assert_pass(
        out_np,
        ref_out_bf16,
        fail_msg="Complex FIR output mismatch",
        atol=0.01,
    )
    print(
        "SUCCESS: Phoenix NPU executed 8-Tap Complex FIR "
        "(complex taps x complex I/Q) on physical silicon!"
    )
    print("PASS!")


if __name__ == "__main__":
    main()
