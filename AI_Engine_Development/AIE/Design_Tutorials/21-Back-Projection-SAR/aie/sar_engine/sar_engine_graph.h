//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include "range_gen_graph.h"
#include "diff3dsq_graph.h"
#include "sqrt_lib_graph.h"
#include "dR_comp_graph.h"
#include "fmod_floor_graph.h"
#include "expjx_graph.h"
#include "interp1_graph.h"
#include "bp_update_graph.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP,unsigned ID>
class sar_engine_graph : public graph {
public:
  port<input>      coord_i;
  port<input>      image_i;
  port<input>      ifft_real_i;
  port<input>      ifft_imag_i;
  port<input>      R0_range_i;
  port<output>     image_o;

  range_gen_graph<REPEAT,NSAMP,ID> range;
  diff3dsq_graph<REPEAT,NSAMP>     diff3d;
  sqrt_lib_graph<REPEAT,NSAMP>     sqrt;
  dR_comp_graph<REPEAT,NSAMP>      dR_comp;
  fmod_floor_graph<REPEAT,NSAMP>   fmod;
  expjx_graph<REPEAT,NSAMP>        expjx;
  interp1_graph<REPEAT,NSAMP>      interp1_real;
  interp1_graph<REPEAT,NSAMP>      interp1_imag;
  bp_update_graph<REPEAT,NSAMP>    bp_update;

  sar_engine_graph( void )
  {
    // Connect RTP's:
    connect<>( coord_i,           diff3d.coord_i     );
    connect<>( R0_range_i,        dR_comp.R0_range_i );

    // Connect signals:
    connect<>  s0( range.sig_o,           diff3d.sig_i            );
    connect<>  s1( diff3d.sig_o,          sqrt.sig_i              );
    connect<>  s2( sqrt.sig_o,            dR_comp.sig_i           );
    connect<>  s3( dR_comp.theta_o,       fmod.sig_i              );
    connect<>  s4( fmod.sig_o,            expjx.sig_i             );
    connect<>  s5( expjx.sig_o,           bp_update.phase_i       );
    connect<>  s6( image_i,               bp_update.image_i       );
    connect<>  s7( ifft_real_i,           interp1_real.lut_i      );
    connect<>  s8( dR_comp.domain_o,      interp1_real.domain_i   );
    connect<>  s9( interp1_real.interp_o, bp_update.interp_real_i );
    connect<> s10( ifft_imag_i,           interp1_imag.lut_i      );
    connect<> s11( dR_comp.domain_o,      interp1_imag.domain_i   );
    connect<> s12( interp1_imag.interp_o, bp_update.interp_imag_i );
    connect<> s13( bp_update.image_o,     image_o                 );
  }
};


