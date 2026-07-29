//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz
//
// Channelizer VSS - Combined AI Engine top-level graph
//
// Instantiates the polyphase filterbank and DFT sub-graphs.
// Both sub-graphs use generic ports (port<input>/port<output>); PLIO
// ports are created here with names matching the stream_connect
// directives in system.cfg.
//
// Signal flow:
//   m16_ssr8_permute_fb_i_wrapper (HLS)  -->  [filterbank_in_1..8]   -->  AIE Filterbank
//   AIE Filterbank                       -->  [filterbank_out_1..8]  -->  m16_ssr8_permute_fb_o_wrapper (HLS)
//   m16_ssr8_cyclic_shift_wrapper (HLS)  -->  [dft_in_1..8]          -->  AIE DFT
//   AIE DFT                              -->  [dft_out_1..8]         -->  VSS output
//
// PLIO naming:
//   filterbank input  : filterbank_in_<n>   (n = 1..8)
//   filterbank output : filterbank_out_<n>  (n = 1..8)
//   DFT input         : dft_in_<n>          (n = 1..8)
//   DFT output        : dft_out_<n>         (n = 1..8)

#include <adf.h>
#include "m16_ssr8_filterbank_graph.h"
#include "m16_ssr8_dft_graph.h"

using namespace adf;

class channelizer_graph : public graph {
public:
  std::array< input_plio,8> fbank_i;
  std::array<output_plio,8> fbank_o;
  std::array< input_plio,8> dft_i;
  std::array<output_plio,8> dft_o;

  // Inner graphs use port<input>/port<output> — no PLIO internals
  m16_ssr8_filterbank_graph dut_fbank;
  m16_ssr8_dft_graph        dut_dft;

  channelizer_graph() : dut_fbank(), dut_dft()
  {
    for (int ii = 0; ii < 8; ii++) {
      std::string plio_fbank_i = "filterbank_in_"  + std::to_string(ii+1);
      std::string plio_fbank_o = "filterbank_out_" + std::to_string(ii+1);
      std::string plio_dft_i   = "dft_in_"         + std::to_string(ii+1);
      std::string plio_dft_o   = "dft_out_"        + std::to_string(ii+1);

      fbank_i[ii] =  input_plio::create(plio_fbank_i, plio_64_bits);
      fbank_o[ii] = output_plio::create(plio_fbank_o, plio_64_bits);
      dft_i[ii]   =  input_plio::create(plio_dft_i,   plio_64_bits);
      dft_o[ii]   = output_plio::create(plio_dft_o,   plio_64_bits);

      connect<stream>(fbank_i[ii].out[0],  dut_fbank.sig_i[ii]);
      connect<stream>(dut_fbank.sig_o[ii], fbank_o[ii].in[0]);
      connect<stream>(dft_i[ii].out[0],    dut_dft.sig_i[ii]);
      connect<stream>(dut_dft.sig_o[ii],   dft_o[ii].in[0]);
    }
  }
};

channelizer_graph mygraph;

int main(int argc, char **argv)
{
  mygraph.init();
  mygraph.run(2);
  mygraph.end();
  return 0;
}
