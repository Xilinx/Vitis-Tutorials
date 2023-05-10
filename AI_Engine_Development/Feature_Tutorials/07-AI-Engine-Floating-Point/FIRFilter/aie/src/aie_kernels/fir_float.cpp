/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "aie_kernels.h"
#include "system_settings.h"

// Real floating point operations are on 8 lanes in parallel
#define VECTOR_LEN 8

alignas(32) float coeff_float[COEFF_NUM] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

#define __USE_AIE_API__

#ifndef __USE_AIE_API__
// Intrinsics version
void firfloat(input_circular_buffer<float,adf::extents<NSAMPLES_WINDOW>,adf::margin<NSAMPLES_OVERLAP>> & datain, output_circular_buffer<float,adf::extents<NSAMPLES_WINDOW>> & dataout)
{
    unsigned int xoffsets = 0x76543210; // Data Lane offset
    unsigned int zoffsets  = 0x00000000; // Process the same coefficient on all lanes

    auto pin = begin_vector_random_circular<8>(datain);
    auto pout = begin_vector_random_circular<8>(dataout);

    vector<float,16> sbuff;
    vector<float,8> coe1 = load_v<8>(coeff_float);
    vector<float,8> coe2 = load_v<8>(coeff_float+8);

    vector<float,8> acc0;
    vector<float,8> acc1;
    vector<float,8> acc2;

    const unsigned LSIZE = (dataout.size() / VECTOR_LEN);

    sbuff.insert(0,*pin++);

    for(unsigned i=0; i<LSIZE; i++)
    chess_prepare_for_pipelining
    chess_loop_range(8,)
    {
        sbuff.insert(1,*pin++);

        acc0 = fpmul(       sbuff, 1,xoffsets, coe1, 0, zoffsets);
        acc1 = fpmul(       sbuff, 2,xoffsets, coe1, 1, zoffsets);
        acc0 = fpmac( acc0, sbuff, 3,xoffsets, coe1, 2, zoffsets);
        acc1 = fpmac( acc1, sbuff, 4,xoffsets, coe1, 3, zoffsets);
        acc0 = fpmac( acc0, sbuff, 5,xoffsets, coe1, 4, zoffsets);
        acc1 = fpmac( acc1, sbuff, 6,xoffsets, coe1, 5, zoffsets);
        acc0 = fpmac( acc0, sbuff, 7,xoffsets, coe1, 6, zoffsets);
        acc1 = fpmac( acc1, sbuff, 8,xoffsets, coe1, 7, zoffsets);

        sbuff.insert(0,*pin--);

        acc0 = fpmac( acc0, sbuff,  9,xoffsets, coe2, 0, zoffsets);
        acc1 = fpmac( acc1, sbuff, 10,xoffsets, coe2, 1, zoffsets);
        acc0 = fpmac( acc0, sbuff, 11,xoffsets, coe2, 2, zoffsets);
        acc1 = fpmac( acc1, sbuff, 12,xoffsets, coe2, 3, zoffsets);
        acc0 = fpmac( acc0, sbuff, 13,xoffsets, coe2, 4, zoffsets);
        acc1 = fpmac( acc1, sbuff, 14,xoffsets, coe2, 5, zoffsets);
        acc0 = fpmac( acc0, sbuff, 15,xoffsets, coe2, 6, zoffsets);
        acc1 = fpmac( acc1, sbuff,  0,xoffsets, coe2, 7, zoffsets);

        sbuff.insert(0,*pin++);

        acc2 = fpadd( acc1, xset_w(0,acc0), 0, xoffsets);
        *pout++ = acc2;
    }

}

#else

// AIE API version
void firfloat(input_circular_buffer<float,adf::extents<NSAMPLES_WINDOW>,adf::margin<NSAMPLES_OVERLAP>> & datain, output_circular_buffer<float,adf::extents<NSAMPLES_WINDOW>> & dataout)
{
    auto pin = begin_vector_random_circular<8>(datain);
    auto pout = begin_vector_random_circular<8>(dataout);

    vector<float,16> sbuff;
    vector<float,8> coe1 = load_v<8>(coeff_float);
    vector<float,8> coe2 = load_v<8>(coeff_float+8);

    const unsigned LSIZE = (dataout.size() / VECTOR_LEN);

    // Define the operator
    constexpr unsigned Lanes=8, Points=8, CoeffStep=1, DataStepX=1, DataStepY=1;

    using mul_ops = aie::sliding_mul_ops<Lanes, Points, CoeffStep, DataStepX, DataStepY, float, float>;

    sbuff.insert(0,*pin++);

    for(unsigned i=0; i<LSIZE; i++)
    chess_prepare_for_pipelining
    chess_loop_range(8,)
    {
        sbuff.insert(1,*pin++);

        auto acc0 = mul_ops::mul(coe1,0,sbuff,1);

        sbuff.insert(0,*pin--);

        acc0 = mul_ops::mac(acc0,coe2,0,sbuff,9);

        sbuff.insert(0,*pin++);

        *pout++ = acc0;
    }

}

#endif
