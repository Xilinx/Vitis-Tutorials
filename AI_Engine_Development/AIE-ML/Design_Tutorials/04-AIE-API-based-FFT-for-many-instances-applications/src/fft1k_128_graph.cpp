//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Donato Carlo Giorgio

#include "fft1k_128_graph.h"
#include <string>

// Wrapping the graph
class dut_graph : public graph {
    public:
        input_plio  din[N_IO];
        output_plio dout[N_IO];
        fft1k_128_graph fft_graph;
        dut_graph(void)
        {
            std::string plio_i;
            std::string plio_o;
            std::string file_i;
            std::string file_o;
            for(int i=0; i<N_IO; i++){
                plio_i = "PLIO_i[" + std::to_string(i) + "]";
                plio_o = "PLIO_o[" + std::to_string(i) + "]";
                file_i = "src/verif_i_128/PLIO_i[" + std::to_string(i) + "].txt";
                file_o = "verif_o_128/PLIO_o[" + std::to_string(i) + "].txt";

                din[i]     = input_plio::create(plio_i, plio_64_bits, file_i);
                dout[i]    = output_plio::create(plio_o, plio_64_bits, file_o);
                connect(din[i].out[0], fft_graph.din[i]);
                connect(fft_graph.dout[i], dout[i].in[0]);
            }
        }
};

dut_graph fft1k_128_dut;

int main(void)
{
    fft1k_128_dut.init();
    fft1k_128_dut.run(3);
    fft1k_128_dut.end();

    return 0;
}