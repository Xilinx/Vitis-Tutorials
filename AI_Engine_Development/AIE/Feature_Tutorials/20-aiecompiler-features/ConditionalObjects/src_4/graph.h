// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

// Test conditional std::array<sub-graph,N>
#include "adf.h"
#include <stdlib.h>
#include "aie_kernels.h"

using namespace adf;



std::string
nextOutputFilename()
{
    static int ind = -1;
    ++ind;
    std::string result = std::string("data/o") + std::to_string(ind) + ".txt";
    return result;
}

template<bool HAS_RTPS>
struct Sub0: public graph
{
    input_port _in0;
    output_port _out0;
    typename std::conditional< HAS_RTPS, input_port, std::tuple<>>::type _in1;
    typename std::conditional< HAS_RTPS, inout_port, std::tuple<>>::type _inout0;
    kernel _k;
    Sub0()
    {
        if constexpr (HAS_RTPS) {
            _k = kernel::create(f1);
        } else {
            _k = kernel::create(f0);
        }
        runtime<ratio>(_k) = 0.9;
        source(_k) = "k0.cpp";
        connect(_in0, _k.in[0]);
        connect(_k.out[0], _out0);

        if constexpr (HAS_RTPS) {
            connect(_in1, _k.in[1]);
            connect(_k.inout[0], _inout0);
        }
    }
};

template<bool HAS_RTPS>
struct MyGraph: public graph
{
    input_plio _plioI0;
    output_plio _plioO0;
    Sub0<HAS_RTPS> _sub;

    MyGraph()
    {
        {
            _plioI0 = input_plio::create(adf::plio_32_bits, "data/i0.txt");
            _plioO0 = output_plio::create(adf::plio_32_bits, nextOutputFilename().c_str());

            connect(_plioI0.out[0], _sub._in0);
            connect(_sub._out0, _plioO0.in[0]);
        }
    }
};
