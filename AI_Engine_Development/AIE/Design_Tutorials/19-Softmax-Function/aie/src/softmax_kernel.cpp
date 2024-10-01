//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#include "softmax_kernel.h"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>

void softmax_kernel::softmax(input_stream<int32>* in, output_stream<int32>* out)
{
    // work buffers in data memory
    auto pWbufA = aie::begin_restrict_vector<8>(wbufA);
    auto pWbufB = aie::begin_restrict_vector<8>(wbufB);
    auto pWbufC = aie::begin_restrict_vector<8>(wbufC);
    auto pWbufD = aie::begin_restrict_vector<8>(wbufD);

    // read and store data while searching for maximum value
    float max_val = -2 ^ 127;

    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        aie::vector<float,8> vin = aie::vector_cast<float>(readincr_v<8>(in));
        float vmax = aie::reduce_max(vin);
        if (vmax > max_val) {
            max_val = vmax;
        }
        *pWbufB++ = vin;
    }

    pWbufB -= (BUFFSZ/8);

    chess_separator();

    /****** Start of computation of exponential function of all input values ******/
    // subtract maximum value from all input values and scale result by log2(e)
    aie::accum<accfloat,8> acc_init;
    acc_init.from_vector(aie::broadcast<float,8>(-log2e*max_val));

    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        *pWbufA++ = aie::mac(acc_init, *pWbufB++, log2e);
    }

    pWbufA -= (BUFFSZ/8);
    pWbufB -= (BUFFSZ/8);

    chess_separator();

    // compute integer part of scaled inputs, equivalent to floor()
    // Note: Not strictly a floor(), fails for negative numbers with very small magnitudes
    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        aie::vector<float,8> vecA = aie::vector_cast<float>(*pWbufA++);
        aie::vector<float,8> xshft = aie::sub(vecA, 0.5f);
        aie::vector<int32,8> xfloor = aie::to_fixed(xshft,0);
        *pWbufC++ = aie::to_float(xfloor);
    }

    pWbufA -= (BUFFSZ/8);
    pWbufC -= (BUFFSZ/8);

    chess_separator();

    // compute the fractional part of scaled input
    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        *pWbufB++ = aie::sub(*pWbufA++, *pWbufC++);
    }

    pWbufA -= (BUFFSZ/8);
    pWbufB -= (BUFFSZ/8);
    pWbufC -= (BUFFSZ/8);

    chess_separator();

    /****** Start of correction polynomial computation ******/
    // using Horner's method, break polynomial evaluation into separate loops for improved pipelining

    // polynomial loop 1
    aie::accum<accfloat,8> p_acc;
    p_acc.from_vector(aie::broadcast<float,8>(p3));

    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        *pWbufC++ = aie::mac(p_acc, *pWbufB++, p4);
    }

    pWbufB -= (BUFFSZ/8);
    pWbufC -= (BUFFSZ/8);

    chess_separator();
  
    // polynomial loop 2
    p_acc.from_vector(aie::broadcast<float,8>(p2));

    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        *pWbufD++ = aie::mac(p_acc, *pWbufB++, *pWbufC++);
    }

    pWbufB -= (BUFFSZ/8);
    pWbufC -= (BUFFSZ/8);
    pWbufD -= (BUFFSZ/8);

    chess_separator();

    // polynomial loop 3
    p_acc.from_vector(aie::broadcast<float,8>(p1));

    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        *pWbufC++ = aie::mac(p_acc, *pWbufB++, *pWbufD++);
    }

    pWbufB -= (BUFFSZ/8);
    pWbufC -= (BUFFSZ/8);
    pWbufD -= (BUFFSZ/8);

    chess_separator();

    // polynomial loop 4
    p_acc.from_vector(aie::broadcast<float,8>(p0));

    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        *pWbufD++ = aie::mac(p_acc, *pWbufB++, *pWbufC++);
    }

    pWbufB -= (BUFFSZ/8);
    pWbufC -= (BUFFSZ/8);
    pWbufD -= (BUFFSZ/8);

    /****** End of correction polynomial computation ******/

    chess_separator();

    // apply correction term to scaled input
    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        *pWbufB++ = aie::sub(*pWbufA++, *pWbufD++);
    }

    pWbufA -= (BUFFSZ/8);
    pWbufB -= (BUFFSZ/8);
    pWbufD -= (BUFFSZ/8);

    chess_separator();

    // convert results to IEEE 754 format - use 2 loops
    aie::accum<accfloat,8> b_acc;
    b_acc.from_vector(aie::broadcast<float,8>(exp_B));

    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        *pWbufC++ = aie::mac(b_acc,*pWbufB++,exp_S);
    }

    pWbufB -= (BUFFSZ/8);
    pWbufC -= (BUFFSZ/8);

    chess_separator();

    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        aie::vector<int32,8> exp_i = aie::to_fixed(*pWbufC++,0);

        // integer values should be in the range [0, 1,065,353,216], find outliers and set to zero
        aie::mask<8>  msk_neg = aie::lt(exp_i,0);
        aie::vector<int32,8> exp_bnd = aie::select(exp_i, aie::zeros<int32,8>(), msk_neg);
        aie::mask<8>  msk_pos = aie::gt(exp_bnd,1065353216);
        exp_bnd = aie::select(exp_bnd, aie::zeros<int32,8>(), msk_pos);

        *pWbufA++ = exp_bnd.cast_to<float>();
    }

    pWbufA -= (BUFFSZ/8);
    pWbufC -= (BUFFSZ/8);

    /****** End of computation of exponential functions of all input values ******/

    chess_separator();
   
    // accumulate all vectors to determine scale factor
    auto vsum = aie::zeros<float,8>();

    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        vsum = aie::add(vsum, *pWbufA++);
    }

    pWbufA -= (BUFFSZ/8);

    chess_separator();

    // compute inverse
    float scl_fctr = aie::inv(aie::reduce_add(vsum));

    // scale values and write to output
    for (unsigned i=0; i < BUFFSZ/8; i++) 
        chess_prepare_for_pipelining
		chess_loop_count(BUFFSZ/8)
    {
        aie::vector<float,8> vout = aie::mul(*pWbufA++, scl_fctr);
        writeincr(out, aie::vector_cast<int32>(vout));
    }
}