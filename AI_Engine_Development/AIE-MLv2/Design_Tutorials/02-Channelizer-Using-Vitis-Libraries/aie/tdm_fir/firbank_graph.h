//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include <fir_tdm_graph.hpp>
#include "pkt_switch_graph.hpp"


using namespace adf;
namespace dsplib_tdm = xf::dsp::aie::fir::tdm;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class firbank_graph : public graph {
public:
  typedef cint16                      TT_DATA;
  typedef cint32                      TT_OUT_DATA;
  typedef int32                       TT_COEFF;
  static constexpr unsigned           TP_FIR_LEN            = 36;
  static constexpr unsigned           TP_SHIFT              = 15;
  static constexpr unsigned           TP_RND                = 12;
  static constexpr unsigned           TP_NUM_OUTPUTS        = 1;
  static constexpr unsigned           TP_DUAL_IP            = 0;
  static constexpr unsigned           TP_SAT                = 1;
  static constexpr unsigned           TP_TDM_CHANNELS       = 4096;
  static constexpr unsigned           TP_SSR                = 32;
  static constexpr unsigned           TP_INPUT_WINDOW_VSIZE = 4096;
  static constexpr unsigned           TP_CASC_LEN           = 1;
  
  using TT_FIR = dsplib_tdm::fir_tdm_graph<TT_DATA,TT_COEFF,TP_FIR_LEN,TP_SHIFT,TP_RND,TP_INPUT_WINDOW_VSIZE,
                                       TP_TDM_CHANNELS,TP_NUM_OUTPUTS,TP_DUAL_IP,TP_SSR,TP_SAT,TP_CASC_LEN,TT_OUT_DATA>;

  static constexpr unsigned           NPORT_I = 2;
  static constexpr unsigned           NPORT_O = 4;
  xf::dsp::aie::pkt_switch_graph<TP_SSR, NPORT_I, NPORT_O, TT_FIR> tdmfir;

  std::array< port< input>,NPORT_I> sig_i;
  std::array<port< output>,NPORT_O> sig_o;

  // Constructor:
  firbank_graph( std::vector<typename firbank_graph::TT_COEFF> TAPS_INIT_0 ) : tdmfir(TAPS_INIT_0)
  {
    for (unsigned ii=0; ii < NPORT_I; ii++) {
      connect<>( sig_i[ii], tdmfir.pkt_in[ii] );
    }
    for (unsigned ii=0; ii < NPORT_O; ii++) {
      connect<>( tdmfir.pkt_out[ii], sig_o[ii] );
    }
  }
};


