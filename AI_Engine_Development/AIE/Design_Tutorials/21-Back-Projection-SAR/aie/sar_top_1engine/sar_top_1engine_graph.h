//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once
#include <adf.h>
#include "ifft2k_async_graph.h"
#include "sar_engine_graph.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP,unsigned ID>
class sar_top_1engine_graph : public graph {
public:
  ifft2k_async_graph                ifft;
  sar_engine_graph<REPEAT,NSAMP,ID> engine;

  input_gmio  ifft_i;
  input_plio  image_i;
  input_port  coord_i;          // RTP for radar pulse coordinates
  input_port  R0_range_i;       // RTP for range to target
  output_plio image_o;

  // Constructor:
  sar_top_1engine_graph( void )
  {
    ifft_i  =  input_gmio::create("GMIO_i_ifft_" +std::to_string(ID),64,1000);
    image_i =  input_plio::create("PLIO_i_image_"+std::to_string(ID),plio_64_bits);
    image_o = output_plio::create("PLIO_o_image_"+std::to_string(ID),plio_64_bits);

    connect<>( coord_i,        engine.coord_i    );
    connect<>( R0_range_i,     engine.R0_range_i );

    connect<>( ifft_i.out[0],  ifft.sig_i         );
    connect<>( ifft.real_o,    engine.ifft_real_i );
    connect<>( ifft.imag_o,    engine.ifft_imag_i );
    connect<>( image_i.out[0], engine.image_i     );
    connect<>( engine.image_o, image_o.in[0]      );
  }
};


