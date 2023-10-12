// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "adf.h"
#include <stdlib.h>
#include "aie_kernels.h"

using namespace adf;


constexpr int getNum(int id) {return id == 1? 2: 3;}

template<int ID>
void f0(input_stream<int32> *, output_stream<int32> *);

template<int ID>
struct Sub0: public graph
{
    input_port _in;
    output_port _out;
    kernel _k;
    Sub0()
    {
        _k = kernel::create(f0<ID>);
        runtime<ratio>(_k) = 0.9;
        source(_k) = "k0.cpp";
        connect(_in, _k.in[0]);
        connect(_k.out[0], _out);
    }
};

template<int ID>
struct MyGraph: public graph
{
    std::array<input_plio , getNum(ID)> _plioIs;
    std::array<output_plio, getNum(ID)> _plioOs;

    typename std::conditional<ID==1,
                              std::array<Sub0<1>, 2>,
                              std::array<Sub0<2>, 3>>::type _subs;

    MyGraph()
    {
        for (int ind = 0; ind < getNum(ID); ++ind)
        {
            char iName[40];
            char oName[40];
            sprintf(iName, "data/i_%d_%d.txt", ID, ind);
            sprintf(oName, "data/o_%d_%d.txt", ID, ind);

            _plioIs[ind] = input_plio::create(adf::plio_32_bits, iName);
            _plioOs[ind] = output_plio::create(adf::plio_32_bits, oName);

            connect(_plioIs[ind].out[0], _subs[ind]._in);
            connect(_subs[ind]._out, _plioOs[ind].in[0]);
        }
    }
};
