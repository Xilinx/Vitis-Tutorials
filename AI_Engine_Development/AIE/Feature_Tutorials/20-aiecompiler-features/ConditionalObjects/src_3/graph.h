// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

// Test conditional sub-graph.
#include "adf.h"

using namespace adf;

template<int ID>
void f0(input_stream<int32> *, output_stream<int32> *);

struct Sub0: public graph
{
    input_port _in0;
    input_port _out0;
    kernel     _k0;
    Sub0()
    {
        _k0 = kernel::create(f0<0>);
        connect(_in0, _k0.in[0]);
        connect(_k0.out[0], _out0);
        runtime<ratio>(_k0) = 0.9;
        source(_k0) = "k0.cpp";
    }
};

struct Sub1: public graph
{
    input_port _in0;
    input_port _out0;
    kernel     _k0;
    Sub1()
    {
        _k0 = kernel::create(f0<1>);
        connect(_in0, _k0.in[0]);
        connect(_k0.out[0], _out0);
        runtime<ratio>(_k0) = 0.8;
        source(_k0) = "k0.cpp";
    }
};

template<int ID>
struct MyGraph: public graph
{
    input_plio _plioI;
    output_plio _plioO;

    constexpr static bool hasSub0() {return ID & 0x1;}
    constexpr static bool hasSub1() {return ID & 0x2;}

    typename std::conditional<hasSub0(), Sub0, int >::type _sub0;
    typename std::conditional<hasSub1(), Sub1, int >::type _sub1;

    MyGraph()
    {
        _plioI = input_plio::create("plio_I"+std::to_string(ID), plio_32_bits, "data/input"+std::to_string(ID)+".txt");
        _plioO = output_plio::create("plio_O"+std::to_string(ID), plio_64_bits, "data/output"+std::to_string(ID)+".txt");
        if constexpr (hasSub0() && hasSub1())
        {
            connect(_plioI.out[0], _sub0._in0);
            connect(_sub0._out0, _sub1._in0);
            connect(_sub1._out0, _plioO.in[0]);
        }
        else if constexpr (hasSub0())
        {
            connect(_plioI.out[0], _sub0._in0);
            connect(_sub0._out0, _plioO.in[0]);
        }
        else if constexpr (hasSub1())
        {
            connect(_plioI.out[0], _sub1._in0);
            connect(_sub1._out0, _plioO.in[0]);
        }
    }
};
