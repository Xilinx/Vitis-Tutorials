// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT


#include "adf.h"

using namespace adf;

#include "kernels.h"

template<bool HAS_CASCADE_IN, bool HAS_CASCADE_OUT>
struct SubGraph: public graph
{
    input_port strmIn;
    typename std::conditional<!HAS_CASCADE_OUT,output_port,int>::type strmOut;
    typename std::conditional<HAS_CASCADE_IN, input_port, int>::type cascIn;
    typename std::conditional<HAS_CASCADE_OUT, output_port, int>::type cascOut;

    kernel     k1;

    SubGraph()
    {
        if constexpr (HAS_CASCADE_IN && HAS_CASCADE_OUT)
        {
            k1 = kernel::create(k0_cascin_cascout);
            connect(cascIn, k1.in[1]);
            connect(k1.out[0], cascOut);

        }
        else if constexpr (HAS_CASCADE_IN)
        {
            k1 = kernel::create(k0_cascin);
            connect(cascIn, k1.in[1]);
            connect(k1.out[0], strmOut);

        }
        else if constexpr (HAS_CASCADE_OUT)
        {
            k1 = kernel::create(k0_cascout);
            connect(k1.out[0], cascOut);

        }
        else
        {
            k1 = kernel::create(k0);
            connect(k1.out[0], strmOut);

        }
        connect(strmIn, k1.in[0]);
        source(k1) = "kernels.cpp";
        runtime<ratio>(k1) = 0.6;
    }
};

struct TestGraph1: public graph
{
    SubGraph<false,false> sub0;
    SubGraph<false,true> sub1;
    SubGraph<true,true> sub2;
    SubGraph<true,false> sub3;

    input_plio plioIns[4];
    output_plio plioOuts[2];

    TestGraph1()
    {
        for(int i=0;i<4;i++)
            plioIns[i] = input_plio::create("plio_ins"+std::to_string(i), plio_32_bits, "data/input"+std::to_string(i)+".txt", 100, true);
        plioOuts[0] = output_plio::create("plio_outs0", plio_64_bits, "data/output0.txt", 125.5);
        plioOuts[1] = output_plio::create("plio_outs1", plio_64_bits, "data/output1.txt", 125.5);

        connect(plioIns[0].out[0], sub0.strmIn);
        connect(plioIns[1].out[0], sub1.strmIn);
        connect net2(plioIns[2].out[0], sub2.strmIn);
        connect net3(plioIns[3].out[0], sub3.strmIn);

        fifo_depth(net2) = 32;
        fifo_depth(net3) = 128;

        connect(sub1.cascOut, sub2.cascIn);
        connect(sub2.cascOut, sub3.cascIn);

        connect(sub0.strmOut, plioOuts[0].in[0]);
        connect(sub3.strmOut, plioOuts[1].in[0]);

    }
};
