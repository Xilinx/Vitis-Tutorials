//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <vector>

#include "dft7_graph.h"
#include "transpose0_graph.h"
#include "dft9_graph.h"
#include "transpose1_graph.h"
#include "dft16_graph.h"

using namespace adf;

template<unsigned X, unsigned Y>
class pfa1008_graph : public graph {
public:
   input_plio sig_i;
  output_plio sig_o;

  dft7_graph        dft7;
  transpose0_graph  transpose0;
  dft9_graph        dft9;
  transpose1_graph  transpose1;
  dft16_graph       dft16;

  pfa1008_graph( void )
  {
#ifdef AIE_SIM_ONLY
    sig_i =  input_plio::create("PLIO_sig_i",plio_64_bits,"data/sig_i_aie.txt");
    sig_o = output_plio::create("PLIO_sig_o",plio_64_bits,"data/sig_o_aie.txt");
#else
    sig_i =  input_plio::create("PLIO_sig_i",plio_64_bits);
    sig_o = output_plio::create("PLIO_sig_o",plio_64_bits);
#endif
    connect<>( sig_i.out[0],      dft7.sig_i       );
    connect<>( dft7.sig_o,        transpose0.sig_i );
    connect<>( transpose0.sig_o,  dft9.sig_i       );
    connect<>( dft9.sig_o,        transpose1.sig_i );
    connect<>( transpose1.sig_o,  dft16.sig_i      );
    connect<>( dft16.sig_o,       sig_o.in[0]      );

    // Location Constraints:
    location<kernel>(dft7.kk0) = tile(X+0,Y+0);
    location<kernel>(dft7.kk1) = tile(X+1,Y+0);
    location<kernel>(dft7.kk2) = tile(X+2,Y+0);

    location<kernel>(dft9.kk0) = tile(X+0,Y+1);
    location<kernel>(dft9.kk1) = tile(X+1,Y+1);
    location<kernel>(dft9.kk2) = tile(X+2,Y+1);
    location<kernel>(dft9.kk3) = tile(X+3,Y+1);

    location<kernel>(dft16.kk0) = tile(X+3,Y+0);
    location<kernel>(dft16.kk1) = tile(X+4,Y+0);
  }
};

