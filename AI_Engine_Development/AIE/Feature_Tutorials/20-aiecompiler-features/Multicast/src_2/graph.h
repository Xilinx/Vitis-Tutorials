// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "kernels.h"

namespace testcase {

    class broadcast_buffer : public adf::graph {

    private:
        adf::kernel mk;
        adf:: kernel tk1, tk2;

    public:
        adf::port<input> in;
        adf::port<output> out[2];

        broadcast_buffer() {
            mk = adf::kernel::create(passthrough_buffer);
            adf::source(mk) = "passthrough.cpp";

            adf::runtime<ratio>(mk) = 0.1;
            adf::connect(in, mk.in[0]);
            dimensions(mk.in[0]) = {2*BaseInputSize};
            dimensions(mk.out[0]) = {2*BaseInputSize};

            tk1 = adf::kernel::create(passthrough_buffer);
            adf::source(tk1) = "passthrough.cpp";

            adf::runtime<ratio>(tk1) = 0.1;
            adf::connect(mk.out[0], tk1.in[0]);
            adf::connect(tk1.out[0], out[0]);
            dimensions(tk1.in[0]) = {BaseInputSize};
            dimensions(tk1.out[0]) = {BaseInputSize};

            tk2 = adf::kernel::create(passthrough_buffer);
            adf::source(tk2) = "passthrough.cpp";

            adf::runtime<ratio>(tk2) = 0.1;
            adf::connect(mk.out[0], tk2.in[0]);
            adf::connect(tk2.out[0], out[1]);
            dimensions(tk2.in[0]) = {3*BaseInputSize};
            dimensions(tk2.out[0]) = {3*BaseInputSize};

        };
    };

    class BroadcastGraph : public adf::graph {
    public:
        broadcast_buffer Bb;
        adf::input_plio inb;
        adf::output_plio outb[2];

        BroadcastGraph()
        {
            inb = adf::input_plio::create("Inputb",adf::plio_32_bits,"data/input.txt");
            for(int i=0;i<2;i++)
            {
                outb[i] = adf::output_plio::create("Outputb_"+std::to_string(i),adf::plio_32_bits,"data/outputb"+std::to_string(i)+".txt");
            }
            adf::connect<> (inb.out[0],Bb.in);


            for(int i=0;i<2;i++)
            {
                adf::connect<> (Bb.out[i],outb[i].in[0]);
            }

        };
    };

};
