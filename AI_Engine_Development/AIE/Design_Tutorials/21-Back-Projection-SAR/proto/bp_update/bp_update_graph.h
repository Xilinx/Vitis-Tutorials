//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "bp_update.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class bp_update_graph : public graph {
public:
  kernel      kk;
  input_plio  pixel_i;
  input_plio  dist_i;
  input_plio  phcorr_i;
  output_plio pixel_o;

  // Constructor:
  bp_update_graph(void)
  {
    pixel_i  =  input_plio::create("PLIO_pixel_i", plio_64_bits, "data/pixel_i.txt" );
    dist_i   =  input_plio::create("PLIO_dist_i",  plio_64_bits, "data/dist_i.txt" );
    phcorr_i =  input_plio::create("PLIO_phcorr_i",plio_64_bits, "data/phcorr_i.txt" );
    pixel_o  = output_plio::create("PLIO_pixel_o", plio_64_bits, "data/pixel_o.txt" );

    kk = kernel::create_object<bp_update>();
    source(kk) = "bp_update.cpp";
    runtime<ratio>(kk) = 0.9;

    connect<>( pixel_i.out[0],  kk.in[0] );       dimensions(kk.in[0])  = { bp_update::NSAMP };
    connect<>( dist_i.out[0],   kk.in[1] );       dimensions(kk.in[1])  = { bp_update::NSAMP };
    connect<>( phcorr_i.out[0], kk.in[2] );       dimensions(kk.in[2])  = { bp_update::NSAMP };
    connect<>( kk.out[0],       pixel_o.in[0] );  dimensions(kk.out[0]) = { bp_update::NSAMP };
  }
};


