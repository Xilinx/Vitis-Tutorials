//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "dft_1xN_widget.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template <unsigned NSAMP>
class dft_1xN_widget_graph : public graph {
public:
  static constexpr unsigned NT = 4;      // Number of tiles
  typedef cint16 TT_DATA;
  typedef cint16 TT_COEFF;
  typedef cacc48 TT_ACC;
private:
  kernel k_input;
  kernel k_middle[NT-2];
  kernel k_output;

public:
  std::array<port<input>,2*NT>  sig_i;
  std::array<port<output>,2>    sig_o;

  using TT_KERNEL_I = dft_1xN_input <TT_DATA,TT_COEFF,TT_ACC,NSAMP>;
  using TT_KERNEL_M = dft_1xN_middle<TT_DATA,TT_COEFF,TT_ACC,NSAMP>;
  using TT_KERNEL_O = dft_1xN_output<TT_DATA,TT_COEFF,TT_ACC,NSAMP>;

  // Constructor:
  dft_1xN_widget_graph( std::vector<std::vector<TT_COEFF>> twid0,
                        std::vector<std::vector<TT_COEFF>> twid1 ) // These twiddles are vectors of vectors
  {
    // Create kernels on the ends:
    k_input  = kernel::create_object<TT_KERNEL_I>( twid0[0],    twid1[0]    );
    k_output = kernel::create_object<TT_KERNEL_O>( twid0[NT-1], twid1[NT-1] );
    source(k_input)  = "dft_1xN_widget.cpp";
    source(k_output) = "dft_1xN_widget.cpp";
    runtime<ratio>(k_input) = 0.9;
    runtime<ratio>(k_output) = 0.9;
    // Create kernels in the middle:
    for (unsigned kk=0; kk < NT-2; kk++) {
      k_middle[kk] = kernel::create_object<TT_KERNEL_M>( twid0[kk+1], twid1[kk+1] );
      source(k_middle[kk]) = "dft_1xN_widget.cpp";
      runtime<ratio>(k_middle[kk]) = 0.9;
    }

    // Stream inputs to all tiles:
    connect<> n00( sig_i[0], k_input.in[0] );
    connect<> n01( sig_i[1], k_input.in[1] );
    connect<> n10( sig_i[2], k_middle[0].in[0] );
    connect<> n11( sig_i[3], k_middle[0].in[1] );
    connect<> n20( sig_i[4], k_middle[1].in[0] );
    connect<> n21( sig_i[5], k_middle[1].in[1] );
    connect<> n30( sig_i[6], k_output.in[0] );
    connect<> n31( sig_i[7], k_output.in[1] );

    fifo_depth(n00) = (8);
    fifo_depth(n01) = (8);
    fifo_depth(n10) = (8);
    fifo_depth(n11) = (8);
    fifo_depth(n20) = (8);
    fifo_depth(n21) = (8);
    fifo_depth(n30) = (8);
    fifo_depth(n31) = (8);

    connect<>( k_input.out[0],     k_middle[0].in[2] );
    connect<>( k_middle[0].out[0], k_middle[1].in[2] );
    connect<>( k_middle[1].out[0], k_output.in[2]    );

    connect<>( k_output.out[0], sig_o[0] );
    connect<>( k_output.out[1], sig_o[1] );

  }
};


