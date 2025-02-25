//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include <matrix_mult_graph.hpp>

using namespace adf;
namespace dsplib = xf::dsp::aie::blas::matrix_mult;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class gemm_16x32x8_graph : public graph {
public:
  typedef int32 TT_DATA_A;
  typedef int32 TT_DATA_B;
  static constexpr unsigned            TP_DIM_A = 16;
  static constexpr unsigned           TP_DIM_AB = 32;
  static constexpr unsigned            TP_DIM_B = 8;
  static constexpr unsigned            TP_SHIFT = 27;
  static constexpr unsigned              TP_RND = 6; // aie::rounding_mode::rnd_conv_even
  static constexpr unsigned    TP_DIM_A_LEADING = 1; // Row major (0) or Col major (1)
  static constexpr unsigned    TP_DIM_B_LEADING = 0; // Row major (0) or Col major (1)
  static constexpr unsigned  TP_DIM_OUT_LEADING = 1; // Row major (0) or Col major (1)
  static constexpr unsigned     TP_ADD_TILING_A = 0; // Include (1) or exclude (0) tiling kernel
  static constexpr unsigned     TP_ADD_TILING_B = 0; // Include (1) or exclude (0) tiling kernel
  static constexpr unsigned TP_ADD_DETILING_OUT = 0; // Include (1) or exclude (0) tiling kernel
  static constexpr unsigned TP_INPUT_WINDOW_VSIZE_A = TP_DIM_A * TP_DIM_AB;
  static constexpr unsigned TP_INPUT_WINDOW_VSIZE_B = TP_DIM_B * TP_DIM_AB;
  static constexpr unsigned             TP_CASC_LEN = 1;
  static constexpr unsigned                  TP_SAT = 1;
  static constexpr unsigned                  TP_SSR = 1;
  static constexpr unsigned                 NPORT_I = TP_CASC_LEN * TP_SSR;
  static constexpr unsigned                 NPORT_O = TP_SSR;

  std::array< input_plio,NPORT_I> sigA_i;
  std::array< input_plio,NPORT_I> sigB_i;
  std::array<output_plio,NPORT_O> sig_o;

  using TT_GEMM = dsplib::matrix_mult_graph<TT_DATA_A,TT_DATA_B,TP_DIM_A,TP_DIM_AB,TP_DIM_B,TP_SHIFT,TP_RND,
                                            TP_DIM_A_LEADING,TP_DIM_B_LEADING,TP_DIM_OUT_LEADING,
                                            TP_ADD_TILING_A,TP_ADD_TILING_B,TP_ADD_DETILING_OUT,
                                            TP_INPUT_WINDOW_VSIZE_A, TP_INPUT_WINDOW_VSIZE_B,
                                            TP_CASC_LEN,TP_SAT,TP_SSR>;
  TT_GEMM gemm;

  // Constructor:
  gemm_16x32x8_graph( void )
  {
    for (unsigned ii=0; ii < NPORT_I; ii++) {
      std::string pnameA = "PLIO_A_" + std::to_string(ii) + "_i";
      std::string fnameA = "data/sigA_" + std::to_string(ii) + "_i.txt";
      std::string pnameB = "PLIO_B_" + std::to_string(ii) + "_i";
      std::string fnameB = "data/sigB_" + std::to_string(ii) + "_i.txt";
      sigA_i[ii] = input_plio::create(pnameA, plio_64_bits, fnameA);
      sigB_i[ii] = input_plio::create(pnameB, plio_64_bits, fnameB);
      connect<>(sigA_i[ii].out[0],gemm.inA[ii]);
      connect<>(sigB_i[ii].out[0],gemm.inB[ii]);
    }
    for (unsigned ii=0; ii < NPORT_O; ii++) {
      std::string pname = "PLIO_" + std::to_string(ii) + "_o";
      std::string fname = "data/sig_" + std::to_string(ii) + "_o.txt";
      sig_o[ii] = output_plio::create(pname, plio_64_bits, fname);
      connect<>(gemm.out[ii],sig_o[ii].in[0]);
    }
  }
};


