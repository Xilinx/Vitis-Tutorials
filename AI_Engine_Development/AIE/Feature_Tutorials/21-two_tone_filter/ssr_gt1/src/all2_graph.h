/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once

#include <adf.h>
#include "fir2_graph.h"
#include "fft2_graph.h"

using namespace adf;

// Make sure our FIR and FFT designs are compatible:
static_assert( fir2::WIN_SIZE == fft2::WIN_SIZE, "Must have matching window sizes" );
static_assert( fir2::NPORTS_O == fft2::NPORTS_O, "Must have matching port sizes" );

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class all2_graph : public graph {
private:
  static constexpr int FIR_X = 15; // Bottom-left corner
  static constexpr int FIR_Y = 0;
  static constexpr int FFT_X = 24; // Bottom-left corner
  static constexpr int FFT_Y = 0;

public:
  std::array< input_plio,fir2::NPORTS_I> filt_i;
  std::array<output_plio,fir2::NPORTS_O> filt_o;
  std::array<output_plio,fft2::NPORTS_O> fft_o;

  fir2_graph<FIR_X,FIR_Y> fir_dut;
  fft2_graph<FFT_X,FFT_Y> fft_dut;

  all2_graph(void)
  {
    for (int ii=0; ii < fir2::NPORTS_I; ii++) {
      std::string plio_fir_i = "PLIO_fir_i_" + std::to_string(ii);
      std::string plio_fir_o = "PLIO_fir_o_" + std::to_string(ii);
      std::string plio_fft_o = "PLIO_fft_o_" + std::to_string(ii);

      std::string fname_fir_i = "data/sig_i_" + std::to_string(ii) + ".txt";
      std::string fname_fir_o = "data/fir_o_" + std::to_string(ii) + ".txt";
      std::string fname_fft_o = "data/fft_o_" + std::to_string(ii) + ".txt";

      filt_i[ii] =  input_plio::create(plio_fir_i, plio_64_bits, fname_fir_i);
      filt_o[ii] = output_plio::create(plio_fir_o, plio_64_bits, fname_fir_o);
      fft_o[ii]  = output_plio::create(plio_fft_o, plio_64_bits, fname_fft_o);

      connect<stream>( filt_i[ii].out[0],  fir_dut.filt_i[ii] );
      connect<stream>( fir_dut.filt_o[ii], filt_o[ii].in[0]   ); // Note: broadcast net
      connect<stream>( fir_dut.filt_o[ii], fft_dut.fft_i[ii]  ); // Note: broadcast net
      connect<stream>( fft_dut.fft_o[ii],  fft_o[ii].in[0]    );
    }
  }
};
