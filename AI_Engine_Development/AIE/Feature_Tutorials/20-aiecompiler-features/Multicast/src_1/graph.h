// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "kernels.h"

#include <adf.h>

template<int NTK>  // 1 Maker for NTK takers
class broadcast_buffer : public adf::graph {

public:
    adf::kernel mk;
    adf::kernel tk[NTK];

    adf::port<input> in;
    adf::port<output> out[NTK];

    broadcast_buffer() {
        const int Lmaker = 8;
        mk = adf::kernel::create(passthrough_buffer);
        adf::source(mk) = "passthrough.cpp";
        adf::runtime<ratio>(mk) = 0.8;
        adf::connect(in, mk.in[0]);
        dimensions(mk.in[0]) = {InputSize};
        dimensions(mk.out[0]) = {InputSize};

        for(int i=0;i<NTK;i++)
        {
            tk[i] = adf::kernel::create(passthrough_buffer);
            adf::source(tk[i]) = "passthrough.cpp";
            adf::runtime<ratio>(tk[i]) = 0.8;
            adf::connect(mk.out[0], tk[i].in[0]);
            adf::connect(tk[i].out[0], out[i]);
            dimensions(tk[i].in[0]) = {InputSize};
            dimensions(tk[i].out[0]) = {InputSize};
        }
    };
};

template<int NTK>  // 1 Maker for NTK takers
class broadcast_stream : public adf::graph {

public:
    adf::kernel mk;
    adf::kernel tk[NTK];

    adf::port<input> in;
    adf::port<output> out[NTK];

    broadcast_stream() {
        const int Lmaker = 8;
        mk = adf::kernel::create(passthrough_stream<InputSize>);
        adf::source(mk) = "passthrough.cpp";
        adf::runtime<ratio>(mk) = 0.8;
        adf::connect(in, mk.in[0]);

        for(int i=0;i<NTK;i++)
        {
            tk[i] = adf::kernel::create(passthrough_stream<InputSize>);
            adf::source(tk[i]) = "passthrough.cpp";
            adf::runtime<ratio>(tk[i]) = 0.8;
            adf::connect(mk.out[0], tk[i].in[0]);
            adf::connect(tk[i].out[0], out[i]);
        }
    };
};

template<int NTK>  // 1 Maker for NTK buffer takers and NTK stream takers
class broadcast_buffer_stream : public adf::graph {

public:
    adf::kernel mk;
    adf::kernel tk[NTK];

    adf::port<input> in;
    adf::port<output> out[NTK];

    broadcast_buffer_stream() {
        const int Lmaker = 8;
        mk = adf::kernel::create(passthrough_buffer);
        adf::source(mk) = "passthrough.cpp";
        adf::runtime<ratio>(mk) = 0.8;
        adf::connect(in, mk.in[0]);
        dimensions(mk.in[0]) = {InputSize};
        dimensions(mk.out[0]) = {InputSize};

        for(int i=0;i<NTK;i++)
        {
            tk[i] = adf::kernel::create(passthrough_buffer);
            adf::source(tk[i]) = "passthrough.cpp";
            adf::runtime<ratio>(tk[i]) = 0.8;
            adf::connect(mk.out[0], tk[i].in[0]);
            adf::connect(tk[i].out[0], out[i]);
            dimensions(tk[i].in[0]) = {InputSize};
            dimensions(tk[i].out[0]) = {InputSize};
        }
    };
};

template<int NTK>
class BroadcastGraph : public adf::graph {
public:
    broadcast_buffer<NTK> Bb;
    broadcast_stream<NTK> Bs;
    adf::input_plio inb,ins;
    adf::output_plio outb[NTK];
    adf::output_plio outs[NTK];

    BroadcastGraph()
    {
        inb = adf::input_plio::create("Inputb",adf::plio_32_bits,"data/input.txt");
        ins = adf::input_plio::create("Inputs",adf::plio_32_bits,"data/input.txt");
        for(int i=0;i<NTK;i++)
        {
            outb[i] = adf::output_plio::create("Outputb_"+std::to_string(i),adf::plio_32_bits,"data/outputb"+std::to_string(i)+".txt");
            outs[i] = adf::output_plio::create("Outputs_"+std::to_string(i),adf::plio_32_bits,"data/outputs"+std::to_string(i)+".txt");
        }
        adf::connect<> (inb.out[0],Bb.in);
        adf::connect<> (ins.out[0],Bs.in);

        adf::location<adf::kernel>(Bb.mk) = adf::tile(10,3);
        adf::location<adf::kernel>(Bs.mk) = adf::tile(30,3);

        for(int i=0;i<NTK;i++)
        {
            adf::connect<> (Bb.out[i],outb[i].in[0]);
            adf::connect<> (Bs.out[i],outs[i].in[0]);

            adf::location<adf::kernel>(Bb.tk[i]) = adf::tile(12,i);
            adf::location<adf::kernel>(Bs.tk[i]) = adf::tile(32,i);

        }

    };
};
