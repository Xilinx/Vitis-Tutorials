//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "m16_ssr8_dft_graph.h"
#include "m16_ssr8_filterbank_graph.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class m16_ssr8_graph : public graph {
public:
  static constexpr unsigned FIR_X = 22;
  static constexpr unsigned DFT_X = 11;
  std::array< input_plio,8> dft_i;
  std::array<output_plio,8> dft_o;
  std::array< input_plio,8> fbank_i;
  std::array<output_plio,8> fbank_o;

  // Position filterbank to the right side of DFT:
  m16_ssr8_dft_graph        dut_dft;
  m16_ssr8_filterbank_graph dut_fbank;

  // Constructor:
  m16_ssr8_graph( void ) : dut_dft(), dut_fbank()
  {
    // Connections:
    for (int ii=0; ii < 8; ii++) {
      // Note: There are no filenames on these ::create() routines because we either connect these up to
      //       the actual PL HLS blocks in 'system.cfg' or External Traffic Generators for debug
      std::string plio_fbank_i = "PLIO_fbank_i_"+std::to_string(ii);
      std::string plio_fbank_o = "PLIO_fbank_o_"+std::to_string(ii);
      std::string plio_dft_i = "PLIO_dft_i_"+std::to_string(ii);
      std::string plio_dft_o = "PLIO_dft_o_"+std::to_string(ii);
#ifdef AIE_SIM_ONLY
      std::string fname_fbank_i = "data/fir_i_"+std::to_string(ii)+".txt";
      std::string fname_fbank_o = "data/fir_o_"+std::to_string(ii)+".txt";
      std::string fname_dft_i = "data/dft_i_"+std::to_string(ii)+".txt";
      std::string fname_dft_o = "data/dft_o_"+std::to_string(ii)+".txt";
      fbank_i[ii] =  input_plio::create(plio_fbank_i,plio_64_bits,fname_fbank_i);
      fbank_o[ii] = output_plio::create(plio_fbank_o,plio_64_bits,fname_fbank_o);
      dft_i[ii]   =  input_plio::create(plio_dft_i,  plio_64_bits,fname_dft_i);
      dft_o[ii]   = output_plio::create(plio_dft_o,  plio_64_bits,fname_dft_o);
#else
      fbank_i[ii] =  input_plio::create(plio_fbank_i,plio_64_bits);
      fbank_o[ii] = output_plio::create(plio_fbank_o,plio_64_bits);
      dft_i[ii]   =  input_plio::create(plio_dft_i,plio_64_bits);
      dft_o[ii]   = output_plio::create(plio_dft_o,plio_64_bits);
#endif
      connect<stream>(fbank_i[ii].out[0],  dut_fbank.sig_i[ii]);
      connect<stream>(dut_fbank.sig_o[ii], fbank_o[ii].in[0]);
      connect<stream>(dft_i[ii].out[0],    dut_dft.sig_i[ii]);
      connect<stream>(dut_dft.sig_o[ii],   dft_o[ii].in[0]);
    }

    location<graph>(dut_dft.dut_graphA) = area_group( {{aie_tile,DFT_X,0,DFT_X+3,0},{shim_tile,DFT_X,0,DFT_X+3,0}} );
    location<graph>(dut_dft.dut_graphB) = area_group( {{aie_tile,DFT_X,1,DFT_X+3,1},{shim_tile,DFT_X,0,DFT_X+3,0}} );
    location<graph>(dut_dft.dut_graphC) = area_group( {{aie_tile,DFT_X,2,DFT_X+3,2},{shim_tile,DFT_X,0,DFT_X+3,0}} );
    location<graph>(dut_dft.dut_graphD) = area_group( {{aie_tile,DFT_X,3,DFT_X+3,3},{shim_tile,DFT_X,0,DFT_X+3,0}} );

    location<graph>(dut_fbank.dut_graph0) = area_group( {{aie_tile,FIR_X,0,FIR_X+3,1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut_fbank.dut_graph1) = area_group( {{aie_tile,FIR_X,0,FIR_X+3,1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut_fbank.dut_graph2) = area_group( {{aie_tile,FIR_X,0,FIR_X+3,1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut_fbank.dut_graph3) = area_group( {{aie_tile,FIR_X,0,FIR_X+3,1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut_fbank.dut_graph4) = area_group( {{aie_tile,FIR_X,0,FIR_X+3,1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut_fbank.dut_graph5) = area_group( {{aie_tile,FIR_X,0,FIR_X+3,1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut_fbank.dut_graph6) = area_group( {{aie_tile,FIR_X,0,FIR_X+3,1},{shim_tile,FIR_X,0,FIR_X+3,0}} );
    location<graph>(dut_fbank.dut_graph7) = area_group( {{aie_tile,FIR_X,0,FIR_X+3,1},{shim_tile,FIR_X,0,FIR_X+3,0}} );

    location<PLIO>(dft_i[0]) = shim(DFT_X+1);    location<PLIO>(dft_o[0]) = shim(DFT_X+3);
    location<PLIO>(dft_i[1]) = shim(DFT_X+1);    location<PLIO>(dft_o[1]) = shim(DFT_X+3);
    location<PLIO>(dft_i[2]) = shim(DFT_X+1);    location<PLIO>(dft_o[2]) = shim(DFT_X+0);
    location<PLIO>(dft_i[3]) = shim(DFT_X+1);    location<PLIO>(dft_o[3]) = shim(DFT_X+0);
    location<PLIO>(dft_i[4]) = shim(DFT_X+2);    location<PLIO>(dft_o[4]) = shim(DFT_X+3);
    location<PLIO>(dft_i[5]) = shim(DFT_X+2);    location<PLIO>(dft_o[5]) = shim(DFT_X+3);
    location<PLIO>(dft_i[6]) = shim(DFT_X+2);    location<PLIO>(dft_o[6]) = shim(DFT_X+0);
    location<PLIO>(dft_i[7]) = shim(DFT_X+2);    location<PLIO>(dft_o[7]) = shim(DFT_X+0);

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




