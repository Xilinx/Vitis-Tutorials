//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "ifft256p4_rot_graph.h"
#include "ifft256p4_graph.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class ifft64k_graph : public graph {
public:
  static constexpr unsigned   NFFT = 256;
  static constexpr unsigned  EXTRA = 4;
  static constexpr unsigned  NSAMP = NFFT + EXTRA;
  static constexpr unsigned NFRONT = 5;
  static constexpr unsigned  NBACK = 5;

  std::array<port< input>,NFRONT> front_i;
  std::array<port<output>,NFRONT> front_o;
  std::array<port< input>,NBACK>  back_i;
  std::array<port<output>,NBACK>  back_o;

  ifft256p4_rot_graph<NFRONT,0> g_front0;
  ifft256p4_rot_graph<NFRONT,1> g_front1;
  ifft256p4_rot_graph<NFRONT,2> g_front2;
  ifft256p4_rot_graph<NFRONT,3> g_front3;
  ifft256p4_rot_graph<NFRONT,4> g_front4;

  std::array<ifft256p4_graph,NBACK> g_back;

  // Constructor:
  ifft64k_graph( void )
  {
    // Connections to "Row" transforms:
    connect<>( front_i[0], g_front0.sig_i );
    connect<>( front_i[1], g_front1.sig_i );
    connect<>( front_i[2], g_front2.sig_i );
    connect<>( front_i[3], g_front3.sig_i );
    connect<>( front_i[4], g_front4.sig_i );

    connect<>( g_front0.sig_o, front_o[0] );
    connect<>( g_front1.sig_o, front_o[1] );
    connect<>( g_front2.sig_o, front_o[2] );
    connect<>( g_front3.sig_o, front_o[3] );
    connect<>( g_front4.sig_o, front_o[4] );


    // Connections to "Column" transforms:
    for (unsigned bb=0; bb < NBACK; bb++) {
      connect<>( back_i[bb],       g_back[bb].sig_i );
      connect<>( g_back[bb].sig_o, back_o[bb]       );
    }
  }
};


