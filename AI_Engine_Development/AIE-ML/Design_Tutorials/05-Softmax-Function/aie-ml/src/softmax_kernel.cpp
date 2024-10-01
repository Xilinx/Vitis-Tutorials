//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#include "softmax_kernel.h"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>

void softmax_kernel::softmax(input_stream<int16>* in, output_stream<int16>* out)
{
    // set rounding to match MATLAB generated test vectors
    aie::set_rounding(aie::rounding_mode::symmetric_inf);
    aie::set_saturation(aie::saturation_mode::saturate);

    // work buffers in data memory
    auto pWbufA = aie::begin_restrict_vector<16>(wbufA);
    auto pWbufB = aie::begin_restrict_vector<16>(wbufB);

    // read and store data while searching for maximum value
    float max_val = -2 ^ 127;

    for (unsigned i=0; i < BUFFSZ/16; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/16)
    {
        aie::vector<bfloat16,16> vin = v16bfloat16(readincr_v<16>(in));
        float vmax = aie::reduce_max(vin);
        if (vmax > max_val) {
            max_val = vmax;
        }
        *pWbufA++ = v16int16(vin);
    }

    pWbufA -= (BUFFSZ/16);

    chess_separator();

    // subtract maximum value from all input values
    aie::accum<accfloat,16> accM;
    accM.from_vector(aie::broadcast<float,16>(max_val));

    for (unsigned i=0; i < BUFFSZ/16; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/16)
    {
        aie::vector<bfloat16,16> vecA = v16bfloat16(*pWbufA++);
        aie::accum<accfloat,16> accA;
        accA.from_vector(vecA);
        *pWbufB++ = v16int16(aie::to_vector<bfloat16>(aie::sub(accA, accM)));
    }

    pWbufA -= (BUFFSZ/16);
    pWbufB -= (BUFFSZ/16);

    chess_separator();

    /****** Start of computation of exponential functions of all input values ******/
    // convert results to IEEE 754 format - use 2 loops
    aie::accum<accfloat,16> b_acc;
    b_acc.from_vector(aie::broadcast<float,16>(exp_B));

    for (unsigned i=0; i < BUFFSZ/16; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/16)
    {
        aie::vector<bfloat16,16> vecB = v16bfloat16(*pWbufB++);
        aie::accum<accfloat,16> aout = aie::mac(b_acc, vecB, exp_S);
        *pWbufA++ = v16int16(aie::to_vector<bfloat16>(aout));
    }

    pWbufA -= (BUFFSZ/16);
    pWbufB -= (BUFFSZ/16);

    chess_separator();

    for (unsigned i=0; i < BUFFSZ/16; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/16)
    {
        aie::vector<bfloat16,16> vecA = v16bfloat16(*pWbufA++);
        aie::vector<int16,16> exp_i = aie::to_fixed<int16>(vecA,0);

        // integer values should be in the range [0, 16,256], find outliers and set to zero
        aie::mask<16>  msk_neg = aie::lt(exp_i,int16(0));
        aie::vector<int16,16> exp_bnd = aie::select(exp_i, aie::zeros<int16,16>(), msk_neg);
        aie::mask<16>  msk_pos = aie::gt(exp_bnd, int16(16256));
        exp_bnd = aie::select(exp_bnd, aie::zeros<int16,16>(), msk_pos);
        *pWbufB++ = exp_bnd;
    }

    pWbufA -= (BUFFSZ/16);
    pWbufB -= (BUFFSZ/16);

    /****** End of computation of exponential functions of all input values ******/

    chess_separator();

    // accumulate all vectors to determine scale factor
    aie::accum<accfloat,16> accsum;
    accsum.from_vector(aie::zeros<bfloat16,16>());

    for (unsigned i=0; i < BUFFSZ/16; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/16)
    {
        aie::vector<bfloat16,16> vecB = v16bfloat16(*pWbufB++);
        accsum = aie::add(accsum, vecB);
    }

    pWbufB -= (BUFFSZ/16);

    chess_separator();

    // compute inverse
    bfloat16 scl_fctr = aie::inv(aie::reduce_add(aie::to_vector<bfloat16>(accsum)));

    // scale values and write to output
    for (unsigned i=0; i < BUFFSZ/16; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/16)
    {
        aie::vector<bfloat16,16> vecB = v16bfloat16(*pWbufB++);
        aie::vector<int16,16> vout = v16int16(aie::to_vector<bfloat16>(aie::mul(vecB, scl_fctr)));
        writeincr(out, vout);
    }
}