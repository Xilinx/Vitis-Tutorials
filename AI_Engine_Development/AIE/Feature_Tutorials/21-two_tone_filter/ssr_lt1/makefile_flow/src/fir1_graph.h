/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once

#include <adf.h>
#include <fir_sr_sym_graph.hpp>
#include <fft_ifft_dit_1ch_graph.hpp>
#include "fir1_coeff.h"

using namespace adf;

using namespace xf::dsp::aie::fir::sr_sym;
using namespace xf::dsp::aie::fft::dit_1ch;

// ------------------------------------------------------------
// FIR Namespace
// ------------------------------------------------------------

namespace fir1 {
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
  static constexpr int  TP_API                = 0;      // Set to 1 for stream mode (windows otherwise)
  static constexpr int  TP_SSR                = 1;      // Scale throughput using parallelization
};

// ------------------------------------------------------------
// FFT Namespace
// ------------------------------------------------------------

namespace fft1 {
  typedef cint16                              TT_TYPE;
  typedef cint16                              TT_TWIDDLE;
  static constexpr int  TP_POINT_SIZE         = 256;
  static constexpr int  TP_FFT_NIFFT          = 1;
  static constexpr int  TP_SHIFT              = 6; // Excludes twiddle shift
  static constexpr int  TP_CASC_LEN           = 1;
  static constexpr int  TP_DYN_PT_SIZE        = 0;
  static constexpr int  TP_WINDOW_SIZE        = fir1::WIN_SIZE;
  static constexpr int  TP_API                = 0; // Windows
  static constexpr int  TP_PARALLEL_POWER     = 0;
};

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class fir1_graph : public graph {
public:

private:
  // Filter taps:
  std::vector<fir1::TT_COEFF> m_taps = std::vector<fir1::TT_COEFF>(FIR1_COEFF);

  // Filter class:
  using TT_FIR = fir_sr_sym_graph<fir1::TT_DATA,fir1::TT_COEFF,fir1::TP_FIR_LEN,fir1::TP_SHIFT,
                                  fir1::TP_RND,fir1::TP_INPUT_WINDOW_VSIZE,fir1::TP_CASC_LEN,
                                  fir1::TP_DUAL_IP,fir1::TP_USE_COEFF_RELOAD,fir1::TP_NUM_OUTPUTS,
                                  fir1::TP_API,fir1::TP_SSR>;
  // FFT class:
  using TT_FFT = fft_ifft_dit_1ch_graph<fft1::TT_TYPE,fft1::TT_TWIDDLE,fft1::TP_POINT_SIZE,
                                        fft1::TP_FFT_NIFFT,fft1::TP_SHIFT,fft1::TP_CASC_LEN,
                                        fft1::TP_DYN_PT_SIZE,fft1::TP_WINDOW_SIZE,fft1::TP_API,
                                        fft1::TP_PARALLEL_POWER>;
public:
  input_plio  filt_i;
  output_plio filt_o;
  output_plio fft_o;

  TT_FIR fir_dut;
  TT_FFT fft_dut;

  fir1_graph(void) : fir_dut( m_taps ), fft_dut()
  {
    filt_i =  input_plio::create("PLIO_fir_i",plio_64_bits,"data/sig_i.txt");
    filt_o = output_plio::create("PLIO_fir_o",plio_64_bits,"data/fir_o.txt");
    fft_o  = output_plio::create("PLIO_fft_o",plio_64_bits,"data/fft_o.txt");
    connect<window<4*fir1::WIN_SIZE>>( filt_i.out[0],  fir_dut.in[0] );
    connect<window<4*fir1::WIN_SIZE>>( fir_dut.out[0], fft_dut.in[0] );
    connect<window<4*fir1::WIN_SIZE>>( fir_dut.out[0], filt_o.in[0] );
    connect<window<4*fir1::WIN_SIZE>>( fft_dut.out[0], fft_o.in[0] );
  }
};
