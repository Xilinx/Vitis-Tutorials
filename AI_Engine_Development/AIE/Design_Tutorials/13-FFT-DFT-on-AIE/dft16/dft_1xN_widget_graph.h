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
    for (unsigned kk=0; kk < NT; kk++) {
      if ( kk == 0 ) {
        connect<stream,stream>( sig_i[2*kk+0], k_input.in[0] );
        connect<stream,stream>( sig_i[2*kk+1], k_input.in[1] );
      }
      else if ( kk == NT-1 ) {
        connect<stream,stream>( sig_i[2*kk+0], k_output.in[0] );
        connect<stream,stream>( sig_i[2*kk+1], k_output.in[1] );
      }
      else {
        connect<stream,stream>( sig_i[2*kk+0], k_middle[kk-1].in[0] );
        connect<stream,stream>( sig_i[2*kk+1], k_middle[kk-1].in[1] );
      }
    } // kk
    // Connect cascade input ports:
    for (unsigned kk=0; kk < NT-2; kk++) {
      if ( kk==0 ) {
        connect<cascade,cascade>( k_input.out[0], k_middle[0].in[2] );
      }
      else {
        connect<cascade,cascade>( k_middle[kk-1].out[0], k_middle[kk].in[2] );
      }
    } // kk
    // Connect final cascade output port:
    connect<cascade,cascade>( k_middle[NT-3].out[0], k_output.in[2] );
    // Connect final output port:
    for (unsigned kk=0; kk < 2; kk++) {
      connect<stream,stream>( k_output.out[kk], sig_o[kk] );
    } // kk
  }
};


