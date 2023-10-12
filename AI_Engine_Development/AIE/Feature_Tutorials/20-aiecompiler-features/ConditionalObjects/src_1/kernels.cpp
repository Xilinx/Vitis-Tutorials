// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "kernels.h"


alignas(32) int16 coeffs[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};


void k0(input_stream<int16> *sin, output_stream<int32> *sout)
{
    aie::vector<int16,16> c = aie::load_v<16>(coeffs);
    aie::accum<acc48,8> acc;
    aie::vector<int16,32> data;

    constexpr unsigned Lanes=8, Points=16, CoeffStep=1, DataStepX=1, DataStepY=1;
    using mul_ops = aie::sliding_mul_ops<Lanes, Points, CoeffStep, DataStepX, DataStepY, int16, int16, acc48>;

    data.insert(0,readincr_v<8>(sin));
    data.insert(1,readincr_v<8>(sin));

    for(int i=0;i<FRAME_LENGTH/32;i++)
    {
        data.insert(2,readincr_v<8>(sin));
        acc = mul_ops::mul(c,0,data,1);
        writeincr(sout,acc.to_vector<int32>(0));

        data.insert(3,readincr_v<8>(sin));
        acc = mul_ops::mul(c,0,data,9);
        writeincr(sout,acc.to_vector<int32>(0));

        data.insert(0,readincr_v<8>(sin));
        acc = mul_ops::mul(c,0,data,17);
        writeincr(sout,acc.to_vector<int32>(0));

        data.insert(1,readincr_v<8>(sin));
        acc = mul_ops::mul(c,0,data,25);
        writeincr(sout,acc.to_vector<int32>(0));
    }
}


void k0_cascin(input_stream<int16> * sin, output_stream<int32> * sout, input_cascade<acc48>* cin)
{
    aie::vector<int16,16> c = aie::load_v<16>(coeffs);
    aie::accum<acc48,8> acc;
    aie::vector<int16,32> data;

    constexpr unsigned Lanes=8, Points=16, CoeffStep=1, DataStepX=1, DataStepY=1;
    using mul_ops = aie::sliding_mul_ops<Lanes, Points, CoeffStep, DataStepX, DataStepY, int16, int16, acc48>;

    data.insert(0,readincr_v<8>(sin));
    data.insert(1,readincr_v<8>(sin));

    for(int i=0;i<FRAME_LENGTH/32;i++)
    {
        data.insert(2,readincr_v<8>(sin));
        acc = readincr_v<8>(cin);
        acc = mul_ops::mac(acc,c,0,data,1);
        writeincr(sout,acc.to_vector<int32>(0));

        data.insert(3,readincr_v<8>(sin));
        acc = readincr_v<8>(cin);
        acc = mul_ops::mac(acc,c,0,data,9);
        writeincr(sout,acc.to_vector<int32>(0));

        data.insert(0,readincr_v<8>(sin));
        acc = readincr_v<8>(cin);
        acc = mul_ops::mac(acc,c,0,data,17);
        writeincr(sout,acc.to_vector<int32>(0));

        data.insert(1,readincr_v<8>(sin));
        acc = readincr_v<8>(cin);
        acc = mul_ops::mac(acc,c,0,data,25);
        writeincr(sout,acc.to_vector<int32>(0));
    }

}


void k0_cascout(input_stream<int16> *sin, output_cascade<acc48>* cout)
{
    aie::vector<int16,16> c = aie::load_v<16>(coeffs);
    aie::accum<acc48,8> acc;
    aie::vector<int16,32> data;

    constexpr unsigned Lanes=8, Points=16, CoeffStep=1, DataStepX=1, DataStepY=1;
    using mul_ops = aie::sliding_mul_ops<Lanes, Points, CoeffStep, DataStepX, DataStepY, int16, int16, acc48>;

    data.insert(0,readincr_v<8>(sin));
    data.insert(1,readincr_v<8>(sin));

    for(int i=0;i<FRAME_LENGTH/32;i++)
    {
        data.insert(2,readincr_v<8>(sin));
        acc = mul_ops::mul(c,0,data,1);
        writeincr(cout,acc);

        data.insert(3,readincr_v<8>(sin));
        acc = mul_ops::mul(c,0,data,9);
        writeincr(cout,acc);

        data.insert(0,readincr_v<8>(sin));
        acc = mul_ops::mul(c,0,data,17);
        writeincr(cout,acc);

        data.insert(1,readincr_v<8>(sin));
        acc = mul_ops::mul(c,0,data,25);
        writeincr(cout,acc);
    }

}


void k0_cascin_cascout(input_stream<int16> *sin,  input_cascade<acc48>* cin, output_cascade<acc48>* cout)
{
    aie::vector<int16,16> c = aie::load_v<16>(coeffs);
    aie::accum<acc48,8> acc;
    aie::vector<int16,32> data;

    constexpr unsigned Lanes=8, Points=16, CoeffStep=1, DataStepX=1, DataStepY=1;
    using mul_ops = aie::sliding_mul_ops<Lanes, Points, CoeffStep, DataStepX, DataStepY, int16, int16, acc48>;

    data.insert(0,readincr_v<8>(sin));
    data.insert(1,readincr_v<8>(sin));

    for(int i=0;i<FRAME_LENGTH/32;i++)
    {
        data.insert(2,readincr_v<8>(sin));
        acc = readincr_v<8>(cin);
        acc = mul_ops::mac(acc,c,0,data,1);
        writeincr(cout,acc);

        data.insert(3,readincr_v<8>(sin));
        acc = readincr_v<8>(cin);
        acc = mul_ops::mac(acc,c,0,data,9);
        writeincr(cout,acc);

        data.insert(0,readincr_v<8>(sin));
        acc = readincr_v<8>(cin);
        acc = mul_ops::mac(acc,c,0,data,17);
        writeincr(cout,acc);

        data.insert(1,readincr_v<8>(sin));
        acc = readincr_v<8>(cin);
        acc = mul_ops::mac(acc,c,0,data,25);
        writeincr(cout,acc);
    }

}
