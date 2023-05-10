/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "aie_kernels.h"
#include "system_settings.h"

cfloat coeff_cfloat[COEFF_NUM] = {{1,16},{2,15},{3,14},{4,13},{5,12},{6,11},{7,10},{8,9},{9,8},{10,7},{11,6},{12,5},{13,4},{14,3},{15,2},{16,1}};

// Complex floating point operations are on 4 lanes in parallel
#define VECTOR_LEN 4

#define __USE_AIE_API__

#ifndef __USE_AIE_API__

// Intrinsics version
void fircfloat(input_circular_buffer<cfloat,adf::extents<NSAMPLES_WINDOW>,adf::margin<NSAMPLES_OVERLAP>> & datain, output_circular_buffer<cfloat,adf::extents<NSAMPLES_WINDOW>> & dataout)
{
    unsigned int xoffsets = 0x3210; // Data Lane offset
    unsigned int zoffsets  = 0x0000; // Process the same coefficient on all lanes

    auto pin  = begin_vector_random_circular<4>(datain);
    auto pout = begin_vector_random_circular<4>(dataout);

    vector<cfloat,8> sbuff;
    vector<cfloat,4> coe;

    vector<cfloat,4> acc0;
    vector<cfloat,4> acc1;
    vector<cfloat,4> acc2;

    const unsigned LSIZE = (dataout.size() / VECTOR_LEN);

    sbuff.insert(0,*pin++);
    sbuff.insert(1,*pin++);

    for(unsigned i=0; i<LSIZE; i++)
    chess_prepare_for_pipelining
    chess_loop_range(8,)
    {
        coe = load_v<4>(coeff_cfloat);

        acc0 = fpmul(       sbuff, 1,xoffsets, coe, 0, zoffsets);
        acc1 = fpmul(       sbuff, 2,xoffsets, coe, 1, zoffsets);
        acc0 = fpmac( acc0, sbuff, 3,xoffsets, coe, 2, zoffsets);
        acc1 = fpmac( acc1, sbuff, 4,xoffsets, coe, 3, zoffsets);

        sbuff.insert(0,*pin++);
        coe = load_v<4>(coeff_cfloat+4);

        acc0 = fpmac( acc0, sbuff, 5,xoffsets, coe, 0, zoffsets);
        acc1 = fpmac( acc1, sbuff, 6,xoffsets, coe, 1, zoffsets);
        acc0 = fpmac( acc0, sbuff, 7,xoffsets, coe, 2, zoffsets);
        acc1 = fpmac( acc1, sbuff, 0,xoffsets, coe, 3, zoffsets);

        sbuff.insert(1,*pin++);

        coe = load_v<4>(coeff_cfloat+8);

        acc0 = fpmac( acc0, sbuff, 1, xoffsets, coe, 0, zoffsets);
        acc1 = fpmac( acc1, sbuff, 2, xoffsets, coe, 1, zoffsets);
        acc0 = fpmac( acc0, sbuff, 3,xoffsets, coe, 2, zoffsets);
        acc1 = fpmac( acc1, sbuff, 4,xoffsets, coe, 3, zoffsets);

        sbuff.insert(0,*pin);
        coe = load_v<4>(coeff_cfloat+12);

        acc0 = fpmac( acc0, sbuff, 5,xoffsets, coe, 0, zoffsets);
        acc1 = fpmac( acc1, sbuff, 6,xoffsets, coe, 1, zoffsets);
        pin -=3;
        acc0 = fpmac( acc0, sbuff, 7,xoffsets, coe, 2, zoffsets);
        acc1 = fpmac( acc1, sbuff, 0,xoffsets, coe, 3, zoffsets);

        sbuff.insert(0,*pin++);
        sbuff.insert(1,*pin++);

        acc2 = fpadd( acc1, xset_w(0,acc0), 0, xoffsets);

        *pout++ = acc2;
    }

}

#else
// AIE API version

void fircfloat(input_circular_buffer<cfloat,adf::extents<NSAMPLES_WINDOW>,adf::margin<NSAMPLES_OVERLAP>> & datain, output_circular_buffer<cfloat,adf::extents<NSAMPLES_WINDOW>> & dataout)
{
    auto pin = begin_vector_random_circular<4>(datain);
    auto pout = begin_vector_random_circular<4>(dataout);

    vector<cfloat,8> sbuff;
    vector<cfloat,4> coe;

    const unsigned LSIZE = (dataout.size() / VECTOR_LEN);

    // Define the operator
    constexpr unsigned Lanes=4, Points=4, CoeffStep=1, DataStepX=1, DataStepY=1;

    using mul_ops = aie::sliding_mul_ops<Lanes, Points, CoeffStep, DataStepX, DataStepY, cfloat, cfloat>;


    sbuff.insert(0,*pin++);
    sbuff.insert(1,*pin++);

    for(unsigned i=0; i<LSIZE; i++)
    chess_prepare_for_pipelining
    chess_loop_range(8,)
    {
        coe = load_v<4>(coeff_cfloat);

        auto acc0 = mul_ops::mul(coe,0,sbuff,1);

        // sbuff = upd_w(sbuff,0 ,window_readincr_v4(datain));
        sbuff.insert(0,*pin++);
        coe = load_v<4>(coeff_cfloat+4);

        acc0 = mul_ops::mac(acc0,coe,0,sbuff,5);

        // sbuff = upd_w(sbuff,1 ,window_readincr_v4(datain));
        sbuff.insert(1,*pin++);

        coe = load_v<4>(coeff_cfloat+8);

        acc0 = mul_ops::mac(acc0,coe,0,sbuff,1);

        // sbuff = upd_w(sbuff,0 ,window_read_v4(datain));
        sbuff.insert(0,*pin);
        coe = load_v<4>(coeff_cfloat+12);

        acc0 = mul_ops::mac(acc0,coe,0,sbuff,5);
        pin -=3;

        sbuff.insert(0,*pin++);
        sbuff.insert(1,*pin++);

        *pout++ = acc0;
    }

}

#endif
