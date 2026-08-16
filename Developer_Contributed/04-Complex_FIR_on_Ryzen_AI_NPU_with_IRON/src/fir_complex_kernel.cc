// Purpose: Bit-accurate complex FIR filter kernel for AIE2 (Milestone 19).
//          Applies an 8-tap complex FIR h[k] = Ih[k] + j*Qh[k] to an
//          interleaved bfloat16 I/Q input vector (4096 bf16 elements =
//          2048 complex I/Q samples), producing an interleaved bfloat16
//          I/Q output vector of the same shape.
// Target operating system: Windows 11 Pro 25H2.
// Target architecture: AMD Ryzen 9 7940HS Phoenix / XDNA1 / AIE2.
// Input types: bfloat16 I/Q interleaved input (4096 samples = 2048 complex pairs).
// Output types: bfloat16 I/Q interleaved output (4096 samples).
// Scaling: Direct bfloat16 operand load, float32 multiply-accumulate,
//          single bfloat16 truncation on store, matching M5/M6.
// Complex multiply identity (Oppenheim & Schafer, DTSP 3e, section 2.2;
// NIST DLMF section 1.9):
//   (Ix + j Qx) * (Ih + j Qh) = (Ix*Ih - Qx*Qh) + j*(Ix*Qh + Qx*Ih).
// Alignment assumptions: 64-byte aligned vector memory (IRON XRTTensor).
// State requirements: Stateless across kernel invocations; internal state
//                     is two 8-float shift registers (hist_i, hist_q).
// Error handling: Zero-history warmup (first L-1 outputs are transient),
//                 matching the M8 pipeline kernel convention.
//
// Design note: shift-and-ingest organization (M8 convention).
//
// This kernel follows tests/m8_pipeline/pipeline_kernel.cc line-for-line
// in loop shape - a single 2048-iteration flat loop, no separate prime or
// tail phase. At the start of the loop hist_i and hist_q are zero, and
// each iteration:
//   1. reads one (I, Q) pair from in_iq into scalars ii, qq;
//   2. shifts hist_i and hist_q left by one slot;
//   3. writes ii and qq into hist_i[L-1] and hist_q[L-1];
//   4. computes the two dot products
//        Iacc = sum_{k=0..L-1} ( hist_i[L-1-k]*cIk - hist_q[L-1-k]*cQk )
//        Qacc = sum_{k=0..L-1} ( hist_i[L-1-k]*cQk + hist_q[L-1-k]*cIk )
//      which is the textbook direct-form convolution
//        out[i] = sum_{k=0..L-1} h[k] * x[i-k]
//      (Oppenheim & Schafer, DTSP 3e, section 5.2) with x[n] = 0 for n < 0.
//   5. stores Iacc and Qacc into out_iq[2i], out_iq[2i+1].
//
// The first L-1 outputs are the transient response of the filter to a
// zero-history startup; they still match the reference in
// test_fir_complex_m19.py term-for-term because the reference performs
// the same shift-and-ingest walk.
//
// This shape is silicon-validated at 16/16 PASS in v0.4.0 through M8, and
// requires no separate prime or tail loop, so there is no data-dependent
// branch in the main body.

#define NOCPP

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <aie_api/aie.hpp>
#include "sdr_dsp_common.hpp"

extern "C" {

void fir_complex_kernel(
    bfloat16 *__restrict in_iq,
    bfloat16 *__restrict out_iq
) {
    event0();

    // 8 complex taps h[k] = Ih[k] + j*Qh[k].
    // Ih matches tests/m5_fir/fir_kernel.cc exactly.
    // Qh is an antisymmetric Hilbert-transformer-flavoured sequence
    // chosen to exercise all four terms of the complex multiply.
    const float cI0 =  0.05f;
    const float cI1 =  0.10f;
    const float cI2 =  0.20f;
    const float cI3 =  0.30f;
    const float cI4 =  0.30f;
    const float cI5 =  0.20f;
    const float cI6 =  0.10f;
    const float cI7 =  0.05f;

    const float cQ0 =  0.05f;
    const float cQ1 =  0.10f;
    const float cQ2 =  0.20f;
    const float cQ3 =  0.30f;
    const float cQ4 = -0.30f;
    const float cQ5 = -0.20f;
    const float cQ6 = -0.10f;
    const float cQ7 = -0.05f;

    // Shift-register windows. Zero-history warmup: the first L-1 outputs
    // are computed with some slots still zero, matching the reference
    // in test_fir_complex_m19.py which walks the same schedule.
    float hist_i[8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float hist_q[8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

#pragma clang loop unroll_count(4)
    for (int i = 0; i < 2048; ++i) {
        float ii = (float)in_iq[2 * i    ];
        float qq = (float)in_iq[2 * i + 1];

        // Shift the window left by one, ingesting the new sample at slot 7.
        // This exactly mirrors tests/m8_pipeline/pipeline_kernel.cc lines 52-56.
        hist_i[0] = hist_i[1]; hist_i[1] = hist_i[2]; hist_i[2] = hist_i[3]; hist_i[3] = hist_i[4];
        hist_i[4] = hist_i[5]; hist_i[5] = hist_i[6]; hist_i[6] = hist_i[7]; hist_i[7] = ii;

        hist_q[0] = hist_q[1]; hist_q[1] = hist_q[2]; hist_q[2] = hist_q[3]; hist_q[3] = hist_q[4];
        hist_q[4] = hist_q[5]; hist_q[5] = hist_q[6]; hist_q[6] = hist_q[7]; hist_q[7] = qq;

        // Textbook direct-form: out[i] = sum_{k=0..L-1} h[k] * x[i-k].
        // Newest sample hist[7] pairs with tap 0; oldest sample hist[0]
        // pairs with tap L-1.
        float Iacc = hist_i[7] * cI0 - hist_q[7] * cQ0
                   + hist_i[6] * cI1 - hist_q[6] * cQ1
                   + hist_i[5] * cI2 - hist_q[5] * cQ2
                   + hist_i[4] * cI3 - hist_q[4] * cQ3
                   + hist_i[3] * cI4 - hist_q[3] * cQ4
                   + hist_i[2] * cI5 - hist_q[2] * cQ5
                   + hist_i[1] * cI6 - hist_q[1] * cQ6
                   + hist_i[0] * cI7 - hist_q[0] * cQ7;

        float Qacc = hist_i[7] * cQ0 + hist_q[7] * cI0
                   + hist_i[6] * cQ1 + hist_q[6] * cI1
                   + hist_i[5] * cQ2 + hist_q[5] * cI2
                   + hist_i[4] * cQ3 + hist_q[4] * cI3
                   + hist_i[3] * cQ4 + hist_q[3] * cI4
                   + hist_i[2] * cQ5 + hist_q[2] * cI5
                   + hist_i[1] * cQ6 + hist_q[1] * cI6
                   + hist_i[0] * cQ7 + hist_q[0] * cI7;

        out_iq[2 * i    ] = (bfloat16)Iacc;
        out_iq[2 * i + 1] = (bfloat16)Qacc;
    }

    event1();
}

}
