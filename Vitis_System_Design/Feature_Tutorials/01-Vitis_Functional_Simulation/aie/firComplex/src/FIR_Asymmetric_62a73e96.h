//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef FIR_Asymmetric_62a73e96_GRAPH_H_
#define FIR_Asymmetric_62a73e96_GRAPH_H_

#include <adf.h>
#include "fir_sr_asym_graph.hpp"


class FIR_Asymmetric_62a73e96 : public adf::graph {
public:
  static constexpr unsigned int TP_SSR = 1;
  template <typename dir>
  using ssr_port_array = std::array<adf::port<dir>, TP_SSR>;

  ssr_port_array<input> in;
  // No dual input
  //No coeff port
  ssr_port_array<output> out;
  // No dual output

  std::vector<cint16> taps = {{-13 , 0}, {569 , 0}, {95 , 0}, {-427 , 0}, {-289 , 0}, {504 , 0}, {601 , 0}, {-495 , 0}, {-1052 , 0}, {323 , 0}, {1700 , 0}, {169 , 0}, {-2777 , 0}, {-1562 , 0}, {5883 , 0}, {13536 , 0}, {13536 , 0}, {5883 , 0}, {-1562 , 0}, {-2777 , 0}, {169 , 0}, {1700 , 0}, {323 , 0}, {-1052 , 0}, {-495 , 0}, {601 , 0}, {504 , 0}, {-289 , 0}, {-427 , 0}, {95 , 0}, {569 , 0}, {-13 , 0}};
  xf::dsp::aie::fir::sr_asym::fir_sr_asym_graph<
    cint16, //TT_DATA
    cint16, //TT_COEFF
    32, //TP_FIR_LEN
    16, //TP_SHIFT
    0, //TP_RND
    256, //TP_INPUT_WINDOW_VSIZE
    6, //TP_CASC_LEN
    0, //TP_USE_COEFF_RELOAD
    1, //TP_NUM_OUTPUTS
    0, //TP_DUAL_IP
    0, //TP_API
    1, //TP_SSR
    0 //TP_SAT
  > filter;

  FIR_Asymmetric_62a73e96() : filter(taps) {
    adf::kernel *filter_kernels = filter.getKernels();
    for (int i=0; i < 1; i++) {
      adf::runtime<ratio>(filter_kernels[i]) = 0.9;
    }
    for (int i=0; i < TP_SSR; i++) {
      adf::connect<> net_in(in[i], filter.in[i]);
      // No dual input
      //No coeff port
      adf::connect<> net_out(filter.out[i], out[i]);
      // No dual output
    }
  }

};


#endif // FIR_Asymmetric_62a73e96_GRAPH_H_
