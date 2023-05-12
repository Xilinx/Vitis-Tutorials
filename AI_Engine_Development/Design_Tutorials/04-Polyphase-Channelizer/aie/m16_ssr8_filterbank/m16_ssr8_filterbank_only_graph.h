//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include "m16_ssr8_filterbank_graph.h"

class m16_ssr8_filterbank_only_graph : public graph {
public:
  static constexpr unsigned FIR_X = 22;
  static constexpr unsigned FIR_Y = 0;
  std::array<input_plio,8>  fbank_i;
  std::array<output_plio,8> fbank_o;
  m16_ssr8_filterbank_graph dut;
  // Constructor:
  m16_ssr8_filterbank_only_graph(void) {
    for (int ii=0; ii < 8; ii++) {
      std::string plio_i = "PLIO_fbank_i_"+std::to_string(ii);
      std::string plio_o = "PLIO_fbank_o_"+std::to_string(ii);
      std::string file_i = "data/sig"+std::to_string(ii)+"_i.txt";
      std::string file_o = "data/sig"+std::to_string(ii)+"_o.txt";
      fbank_i[ii] =  input_plio::create(plio_i,plio_64_bits,file_i);
      fbank_o[ii] = output_plio::create(plio_o,plio_64_bits,file_o);
      connect<stream>(fbank_i[ii].out[0], dut.sig_i[ii]);
      connect<stream>(dut.sig_o[ii],      fbank_o[ii].in[0]);
    }
    location<graph>(dut.dut_graph0) = area_group( {{aie_tile,FIR_X,FIR_Y,FIR_X+3,FIR_Y+1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut.dut_graph1) = area_group( {{aie_tile,FIR_X,FIR_Y,FIR_X+3,FIR_Y+1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut.dut_graph2) = area_group( {{aie_tile,FIR_X,FIR_Y,FIR_X+3,FIR_Y+1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut.dut_graph3) = area_group( {{aie_tile,FIR_X,FIR_Y,FIR_X+3,FIR_Y+1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut.dut_graph4) = area_group( {{aie_tile,FIR_X,FIR_Y,FIR_X+3,FIR_Y+1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut.dut_graph5) = area_group( {{aie_tile,FIR_X,FIR_Y,FIR_X+3,FIR_Y+1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut.dut_graph6) = area_group( {{aie_tile,FIR_X,FIR_Y,FIR_X+3,FIR_Y+1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut.dut_graph7) = area_group( {{aie_tile,FIR_X,FIR_Y,FIR_X+3,FIR_Y+1},{shim_tile,FIR_X,0,FIR_X+3,0}} );

    location<PLIO>(fbank_i[0]) = shim(FIR_X+0);  location<PLIO>(fbank_o[0]) = shim(FIR_X+0);
    location<PLIO>(fbank_i[1]) = shim(FIR_X+0);  location<PLIO>(fbank_o[1]) = shim(FIR_X+0);
    location<PLIO>(fbank_i[2]) = shim(FIR_X+1);  location<PLIO>(fbank_o[2]) = shim(FIR_X+1);
    location<PLIO>(fbank_i[3]) = shim(FIR_X+1);  location<PLIO>(fbank_o[3]) = shim(FIR_X+1);
    location<PLIO>(fbank_i[4]) = shim(FIR_X+2);  location<PLIO>(fbank_o[4]) = shim(FIR_X+2);
    location<PLIO>(fbank_i[5]) = shim(FIR_X+2);  location<PLIO>(fbank_o[5]) = shim(FIR_X+2);
    location<PLIO>(fbank_i[6]) = shim(FIR_X+3);  location<PLIO>(fbank_o[6]) = shim(FIR_X+3);
    location<PLIO>(fbank_i[7]) = shim(FIR_X+3);  location<PLIO>(fbank_o[7]) = shim(FIR_X+3);
  }
};
