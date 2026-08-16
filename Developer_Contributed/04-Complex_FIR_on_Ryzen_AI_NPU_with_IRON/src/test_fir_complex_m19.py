# Copyright (c) 2026 Midhat Nashar
# SPDX-License-Identifier: MIT
#
# Fixed-size complex FIR example for AMD Phoenix XDNA1 / AIE2.
#
# The device kernel consumes 4096 interleaved bfloat16 I/Q elements
# (2048 complex samples), computes an 8-tap causal complex FIR with float32
# taps and accumulation, and converts each result to bfloat16 on store.
#
# Validation contract:
#   * deterministic host-side checks run before compilation;
#   * one deterministic random-I/Q vector is dispatched to the NPU;
#   * silicon and reference outputs must be finite and differ by no more than
#     ABS_TOL in absolute value, with no relative-tolerance term.
#
# This is a functional validation example, not a bitwise-conformance test or
# a performance benchmark. See ../doc/M19_DESIGN.md for the numerical model.

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
from ml_dtypes import bfloat16

N_ELEMENTS = 4096
N_COMPLEX = N_ELEMENTS // 2
L = 8
ABS_TOL = np.float32(0.01)
RANDOM_SEED = 456

# These float32 values match the constants in fir_complex_kernel.cc.
COEFFS_I_F = np.array(
    [0.05, 0.10, 0.20, 0.30, 0.30, 0.20, 0.10, 0.05],
    dtype=np.float32,
)
COEFFS_Q_F = np.array(
    [0.05, 0.10, 0.20, 0.30, -0.30, -0.20, -0.10, -0.05],
    dtype=np.float32,
)


def _f32_coeffs(Ih=None, Qh=None):
    """Return validated float32 tap arrays."""
    Ih_out = COEFFS_I_F if Ih is None else np.asarray(Ih, dtype=np.float32)
    Qh_out = COEFFS_Q_F if Qh is None else np.asarray(Qh, dtype=np.float32)
    if Ih_out.shape != (L,) or Qh_out.shape != (L,):
        raise ValueError(f"Ih and Qh must both have shape ({L},)")
    return Ih_out, Qh_out


def _validate_interleaved_input(in_bf16):
    """Validate the reference model's interleaved I/Q input."""
    arr = np.asarray(in_bf16)
    if arr.ndim != 1 or arr.size == 0 or arr.size % 2 != 0:
        raise ValueError("input must be a non-empty 1-D array of interleaved I/Q")
    if arr.dtype != np.dtype(bfloat16):
        raise TypeError(f"input dtype must be bfloat16; got {arr.dtype}")
    return arr


def complex_fir_reference(in_bf16, Ih=None, Qh=None):
    """Compute the causal complex FIR in float32 and return bfloat16 output.

    The mathematical operation is

        y[n] = sum(h[k] * x[n-k], k=0..L-1), with x[n] = 0 for n < 0.

    Input samples are promoted from bfloat16 to float32. Taps remain float32.
    Results are converted to bfloat16 once after each output accumulation.
    The AIE compiler may choose a different legal float32 reduction schedule,
    so device validation uses the explicit absolute tolerance ABS_TOL.
    """
    arr = _validate_interleaved_input(in_bf16)
    Ih_f32, Qh_f32 = _f32_coeffs(Ih, Qh)

    in_f32 = arr.astype(np.float32)
    Ix = in_f32[0::2]
    Qx = in_f32[1::2]
    count = Ix.size

    hist_i = np.zeros(L, dtype=np.float32)
    hist_q = np.zeros(L, dtype=np.float32)
    ref = np.zeros(2 * count, dtype=np.float32)

    for i in range(count):
        hist_i[:-1] = hist_i[1:]
        hist_q[:-1] = hist_q[1:]
        hist_i[-1] = Ix[i]
        hist_q[-1] = Qx[i]

        Iacc = np.float32(0.0)
        Qacc = np.float32(0.0)
        for k in range(L):
            si = hist_i[L - 1 - k]
            sq = hist_q[L - 1 - k]
            Iacc += si * Ih_f32[k] - sq * Qh_f32[k]
            Qacc += si * Qh_f32[k] + sq * Ih_f32[k]

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
    """Build the fixed-size IRON program used by this tutorial."""
    if N != N_ELEMENTS:
        raise ValueError(
            f"complex_fir requires N={N_ELEMENTS}; got N={N}. "
            "The C++ kernel has a fixed 2048-complex-sample loop bound."
        )
    if element_type is not bfloat16:
        raise TypeError(
            f"complex_fir requires element_type=bfloat16; got {element_type}"
        )

    in_ty = np.ndarray[(N,), np.dtype[element_type]]
    out_ty = np.ndarray[(N,), np.dtype[element_type]]

    of_in = ObjectFifo(in_ty, name="in_iq")
    of_out = ObjectFifo(out_ty, name="out_iq")
    current_dir = Path(__file__).parent.resolve()

    fir_func = ExternalFunction(
        "fir_complex_kernel",
        source_file=str(current_dir / "fir_complex_kernel.cc"),
        arg_types=[in_ty, out_ty],
        include_dirs=[cxx_header_path()],
    )

    def core_body(of_in, of_out, fir_func):
        elem_in = of_in.acquire(1)
        elem_out = of_out.acquire(1)
        fir_func(elem_in, elem_out)
        of_in.release(1)
        of_out.release(1)

    worker = Worker(
        core_body,
        fn_args=[of_in.cons(), of_out.prod(), fir_func],
        stack_size=0x4000,
    )

    def sequence(a_in, c_out, in_prod, out_cons):
        in_prod.fill(a_in)
        out_cons.drain(c_out, wait=True)

    runtime = Runtime(
        sequence,
        [in_ty, out_ty, of_in.prod(), of_out.cons()],
    )
    program = Program(iron.get_current_device(), runtime, workers=[worker])
    return program.resolve_program()


def _pack_iq(Ix_f32, Qx_f32):
    """Pack equal-length float arrays as interleaved bfloat16 I/Q."""
    Ix = np.asarray(Ix_f32, dtype=np.float32)
    Qx = np.asarray(Qx_f32, dtype=np.float32)
    if Ix.ndim != 1 or Qx.ndim != 1 or Ix.shape != Qx.shape:
        raise ValueError("Ix and Qx must be equal-length 1-D arrays")
    packed = np.empty(2 * Ix.size, dtype=np.float32)
    packed[0::2] = Ix
    packed[1::2] = Qx
    return packed.astype(bfloat16)


def _assert_abs_close(actual, expected, label, atol=ABS_TOL):
    """Fail closed on shape, non-finite, or absolute-tolerance violations."""
    actual_arr = np.asarray(actual)
    expected_arr = np.asarray(expected)
    if actual_arr.shape != expected_arr.shape:
        raise AssertionError(
            f"{label}: shape mismatch {actual_arr.shape} != {expected_arr.shape}"
        )

    actual_f32 = actual_arr.astype(np.float32)
    expected_f32 = expected_arr.astype(np.float32)
    if not np.isfinite(actual_f32).all():
        bad = int(np.flatnonzero(~np.isfinite(actual_f32))[0])
        raise AssertionError(f"{label}: non-finite actual output at index {bad}")
    if not np.isfinite(expected_f32).all():
        bad = int(np.flatnonzero(~np.isfinite(expected_f32))[0])
        raise AssertionError(f"{label}: non-finite reference output at index {bad}")

    abs_error = np.abs(actual_f32 - expected_f32)
    max_error = float(np.max(abs_error))
    failing = np.flatnonzero(abs_error > np.float32(atol))
    if failing.size:
        index = int(failing[0])
        raise AssertionError(
            f"{label}: absolute error exceeds {float(atol):.6f} at index {index}; "
            f"actual={float(actual_f32[index]):.8g}, "
            f"expected={float(expected_f32[index]):.8g}, "
            f"abs_error={float(abs_error[index]):.8g}, "
            f"max_abs_error={max_error:.8g}"
        )
    return max_error


def _local_i_impulse_check():
    """A unit I impulse must produce the complex tap sequence."""
    Ix = np.zeros(N_COMPLEX, dtype=np.float32)
    Qx = np.zeros(N_COMPLEX, dtype=np.float32)
    Ix[0] = 1.0
    actual = complex_fir_reference(_pack_iq(Ix, Qx))

    expected = np.zeros(N_ELEMENTS, dtype=np.float32)
    expected[0 : 2 * L : 2] = COEFFS_I_F
    expected[1 : 2 * L : 2] = COEFFS_Q_F
    expected = expected.astype(bfloat16)
    if not np.array_equal(actual, expected):
        raise AssertionError("I-impulse reference check failed")
    print("[reference] I impulse: PASS")


def _local_q_impulse_check():
    """A unit Q impulse exercises both cross terms of complex multiply."""
    Ix = np.zeros(N_COMPLEX, dtype=np.float32)
    Qx = np.zeros(N_COMPLEX, dtype=np.float32)
    Qx[0] = 1.0
    actual = complex_fir_reference(_pack_iq(Ix, Qx))

    expected = np.zeros(N_ELEMENTS, dtype=np.float32)
    expected[0 : 2 * L : 2] = -COEFFS_Q_F
    expected[1 : 2 * L : 2] = COEFFS_I_F
    expected = expected.astype(bfloat16)
    if not np.array_equal(actual, expected):
        raise AssertionError("Q-impulse reference check failed")
    print("[reference] Q impulse: PASS")


def _local_dc_check():
    """After seven warm-up samples, DC output must equal the tap sums."""
    Ix = np.ones(N_COMPLEX, dtype=np.float32)
    Qx = np.zeros(N_COMPLEX, dtype=np.float32)
    actual = complex_fir_reference(_pack_iq(Ix, Qx)).astype(np.float32)

    expected_i = float(bfloat16(np.sum(COEFFS_I_F, dtype=np.float32)))
    expected_q = float(bfloat16(np.sum(COEFFS_Q_F, dtype=np.float32)))
    steady_i = actual[2 * (L - 1) :: 2]
    steady_q = actual[2 * (L - 1) + 1 :: 2]
    err_i = _assert_abs_close(steady_i, np.full_like(steady_i, expected_i), "DC I")
    err_q = _assert_abs_close(steady_q, np.full_like(steady_q, expected_q), "DC Q")
    print(
        "[reference] DC: PASS "
        f"(sum Ih={expected_i:.6f}, sum Qh={expected_q:.6f}, "
        f"max_abs_error={max(err_i, err_q):.6f})"
    )


def _local_complex_tone_check():
    """Cross-check the loop reference against NumPy's complex convolution."""
    sample_index = np.arange(N_COMPLEX, dtype=np.float32)
    phase = np.float32(2.0 * np.pi * 5.0 / N_COMPLEX) * sample_index
    ideal = np.exp(1j * phase).astype(np.complex64)
    packed = _pack_iq(ideal.real, ideal.imag)
    quantized = packed.astype(np.float32)
    x_quantized = quantized[0::2].astype(np.complex64) + 1j * quantized[1::2].astype(
        np.complex64
    )
    taps = (COEFFS_I_F + 1j * COEFFS_Q_F).astype(np.complex64)
    expected_complex = np.convolve(x_quantized, taps, mode="full")[:N_COMPLEX]
    expected = _pack_iq(expected_complex.real, expected_complex.imag)
    actual = complex_fir_reference(packed)
    max_error = _assert_abs_close(actual, expected, "complex-tone convolution")
    print(f"[reference] Complex tone convolution: PASS (max_abs_error={max_error:.6f})")


def _local_real_fir_reduction_check():
    """With zero imaginary taps/input, the formula reduces to a real FIR."""
    rng = np.random.RandomState(123)
    Ix = rng.uniform(0.1, 1.0, N_COMPLEX).astype(np.float32)
    Qx = np.zeros(N_COMPLEX, dtype=np.float32)
    packed = _pack_iq(Ix, Qx)
    zero_q_taps = np.zeros(L, dtype=np.float32)
    actual = complex_fir_reference(
        packed,
        Ih=COEFFS_I_F,
        Qh=zero_q_taps,
    ).astype(np.float32)[0::2]

    quantized_i = packed.astype(np.float32)[0::2]
    expected_f32 = np.convolve(quantized_i, COEFFS_I_F, mode="full")[:N_COMPLEX]
    expected = expected_f32.astype(bfloat16).astype(np.float32)
    max_error = _assert_abs_close(actual, expected, "real-FIR reduction")
    print(
        "[reference] Real-FIR algebraic reduction: PASS "
        f"(max_abs_error={max_error:.6f})"
    )


def _run_local_reference_checks():
    print("Running host-side reference checks before NPU compilation...")
    _local_i_impulse_check()
    _local_q_impulse_check()
    _local_dc_check()
    _local_complex_tone_check()
    _local_real_fir_reduction_check()


def main():
    print("=== Complex FIR on AMD Ryzen AI NPU with IRON ===")
    print(f"Target device: {iron.get_current_device()}")
    print(
        f"Vector: {N_ELEMENTS} bfloat16 elements "
        f"({N_COMPLEX} interleaved complex I/Q samples)"
    )
    print(f"Complex taps: {L}; silicon acceptance: max absolute error <= {ABS_TOL}")

    _run_local_reference_checks()

    rng = np.random.RandomState(RANDOM_SEED)
    Ix = rng.uniform(-1.0, 1.0, N_COMPLEX).astype(np.float32)
    Qx = rng.uniform(-1.0, 1.0, N_COMPLEX).astype(np.float32)
    input_bf16 = _pack_iq(Ix, Qx)
    output_bf16 = np.zeros(N_ELEMENTS, dtype=bfloat16)

    input_tensor = XRTTensor(input_bf16, dtype=bfloat16)
    output_tensor = XRTTensor(output_bf16, dtype=bfloat16)

    print("Compiling the 8-tap complex FIR and dispatching to the NPU...")
    result = complex_fir(
        input_tensor,
        output_tensor,
        N=N_ELEMENTS,
        element_type=bfloat16,
    )
    print(f"Kernel execution result: {result}")

    # XRTTensor.numpy() is the public API in the pinned MLIR-AIE v1.4.1 release. It
    # reconciles device output to host memory before returning the NumPy view.
    actual = np.asarray(output_tensor.numpy())
    expected = complex_fir_reference(input_bf16)
    max_error = _assert_abs_close(
        actual,
        expected,
        "silicon random-I/Q validation",
    )

    print(f"Input sample [0..3]:  {input_bf16[:4]}")
    print(f"Reference [0..3]:     {expected[:4]}")
    print(f"Silicon output [0..3]: {actual[:4]}")
    print(f"Maximum absolute error: {max_error:.6f}")
    print(
        "SUCCESS: the NPU output is finite and satisfies the "
        f"absolute-tolerance contract (<= {float(ABS_TOL):.6f})."
    )
    print("PASS!")


if __name__ == "__main__":
    main()
