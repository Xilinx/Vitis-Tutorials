//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include "firbank_graph.h"
#include "ifft4096_2d_graph.h"
#include "channelizer_init_taps.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class channelizer_graph : public graph {
public:

  std::array< port< input>,firbank_graph::TP_SSR>  sig_i;
  std::array<port< output>,firbank_graph::TP_SSR>  sig_o;
  std::array< port< input>,ifft4096_2d_graph::TP_SSR>  front_i;
  std::array< port< input>,ifft4096_2d_graph::TP_SSR>   back_i;
  std::array<port< output>,ifft4096_2d_graph::TP_SSR>  front_o;
  std::array<port< output>,ifft4096_2d_graph::TP_SSR>   back_o;

  firbank_graph firbank;
  ifft4096_2d_graph ifft4096_2d;
  
  // Constructor:
  channelizer_graph( void ) : firbank{TAPS_INIT_0}
  {
    for ( unsigned ff=0; ff < firbank_graph::TP_SSR; ff++) {
      connect<>( sig_i[ff],       firbank.sig_i[ff] );
      connect<>( firbank.sig_o[ff], sig_o[ff] );
    }
    for ( unsigned ff=0; ff < ifft4096_2d_graph::TP_SSR; ff++) {
      connect<>( front_i[ff],             ifft4096_2d.front_i[ff] );
      connect<>( ifft4096_2d.front_o[ff], front_o[ff] );
    }
    for ( unsigned ff=0; ff < ifft4096_2d_graph::TP_SSR; ff++) {
      connect<>( back_i[ff],              ifft4096_2d.back_i[ff] );
      connect<>( ifft4096_2d.back_o[ff],  back_o[ff] );
    }
  }
};