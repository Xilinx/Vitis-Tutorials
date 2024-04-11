/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once

#include <adf.h>
#include <fir_sr_sym_graph.hpp>
#include "fir2_coeff.h"

using namespace adf;

using namespace xf::dsp::aie::fir::sr_sym;

// ------------------------------------------------------------
// FIR Namespace
// ------------------------------------------------------------

namespace fir2 {
  static constexpr int  WIN_SIZE = 2048;
  typedef cint16                                TT_DATA;
  typedef int16                                 TT_COEFF;
  static constexpr int  TP_FIR_LEN            = 29;     // Total number of taps
  static constexpr int  TP_SHIFT              = 15;     // Depends on FXP properties
  static constexpr int  TP_RND                = 5;      // how to set this
  static constexpr int  TP_INPUT_WINDOW_VSIZE = WIN_SIZE;
  static constexpr int  TP_CASC_LEN           = 1;      // # of tiles in a cascade
  static constexpr int  TP_DUAL_IP            = 0;      // Only used in stream mode (TP_API = 1)
  static constexpr int  TP_USE_COEFF_RELOAD   = 0;      // Set to 1 to use coefficient reload feature
  static constexpr int  TP_NUM_OUTPUTS        = 1;      // Add optional 2nd output port for windows
  static constexpr int  TP_API                = 1;      // Set to 1 for stream mode (windows otherwise)
  static constexpr int  TP_SSR                = 4;      // Scale throughput using parallelization

  static constexpr int NPORTS_I = TP_SSR * (TP_DUAL_IP + 1);
  static constexpr int NPORTS_O = TP_SSR * TP_NUM_OUTPUTS;
  static constexpr int NTILES   = TP_CASC_LEN * TP_SSR * TP_SSR;

  static_assert( NPORTS_I == NPORTS_O, "Must have same # of I/O ports" );
};

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<int FIR_X, int FIR_Y>
class fir2_graph : public graph {
private:

  // Filter taps:
  std::vector<fir2::TT_COEFF> m_taps = std::vector<fir2::TT_COEFF>(FIR2_COEFF);

  // Filter class:
  using TT_FIR = fir_sr_sym_graph<fir2::TT_DATA,fir2::TT_COEFF,fir2::TP_FIR_LEN,fir2::TP_SHIFT,fir2::TP_RND,
                                  fir2::TP_INPUT_WINDOW_VSIZE,fir2::TP_CASC_LEN,fir2::TP_DUAL_IP,
                                  fir2::TP_USE_COEFF_RELOAD,fir2::TP_NUM_OUTPUTS,fir2::TP_API,fir2::TP_SSR>;

public:
  std::array<port<input>, fir2::NPORTS_I> filt_i;
  std::array<port<output>,fir2::NPORTS_I> filt_o;

  TT_FIR fir_dut;

  fir2_graph(void) : fir_dut( m_taps )
  {
    for (int ii=0; ii < fir2::NPORTS_I; ii++) {
      connect<stream>( filt_i[ii],      fir_dut.in[ii] );
      connect<stream>( fir_dut.out[ii], filt_o[ii] );
    }
    // Placement:
    //    Constraint everything to fit in an 8 x 2 rectangle:
    location<graph>(fir_dut) = area_group( {{aie_tile, FIR_X,FIR_Y,FIR_X+7,FIR_Y+1},
                                            {shim_tile,FIR_X,0,    FIR_X+7,0      }} );
  }
};
