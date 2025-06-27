//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef FIR_Symmetric_d2ee5c2f_GRAPH_H_
#define FIR_Symmetric_d2ee5c2f_GRAPH_H_

#include <adf.h>
#include "fir_sr_sym_graph.hpp"


class FIR_Symmetric_d2ee5c2f : public adf::graph {
public:
  static constexpr unsigned int TP_SSR = 1;
  template <typename dir>
  using ssr_port_array = std::array<adf::port<dir>, TP_SSR>;

  ssr_port_array<input> in;
  // No dual input
  ssr_port_array<input> coeff;
  ssr_port_array<output> out;
  // No dual output

  std::vector<int32> taps = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  xf::dsp::aie::fir::sr_sym::fir_sr_sym_graph<
    cint16, //TT_DATA
    int32, //TT_COEFF
    24, //TP_FIR_LEN
    9, //TP_SHIFT
    2, //TP_RND
    64, //TP_INPUT_WINDOW_VSIZE
    1, //TP_CASC_LEN
    0, //TP_DUAL_IP
    1, //TP_USE_COEFF_RELOAD
    1, //TP_NUM_OUTPUTS
    0, //TP_API
    1, //TP_SSR
    0 //TP_SAT
  > filter;

  FIR_Symmetric_d2ee5c2f() : filter() {
    adf::kernel *filter_kernels = filter.getKernels();
    for (int i=0; i < 1; i++) {
      adf::runtime<ratio>(filter_kernels[i]) = 0.9;
    }
    for (int i=0; i < TP_SSR; i++) {
      adf::connect<> net_in(in[i], filter.in[i]);
      // No dual input
      adf::connect<> net_coeff(coeff[i], filter.coeff[i]);
      adf::connect<> net_out(filter.out[i], out[i]);
      // No dual output
    }
  }

};


#endif // FIR_Symmetric_d2ee5c2f_GRAPH_H_
