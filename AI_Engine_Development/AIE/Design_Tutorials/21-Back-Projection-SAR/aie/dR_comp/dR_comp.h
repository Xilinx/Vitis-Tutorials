//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

template<unsigned REPEAT,unsigned NSAMP>
class dR_comp {
public:
  static constexpr float      c_light = 299792458.0;        // Speed of light
  static constexpr float         Fmin = 9.288080384e+09;    // Min freq of radar (assume common across pulses)
  static constexpr float  scale_theta = 2*Fmin/c_light;     // Scale 'dR' to produce 'theta'
  static constexpr float       deltaF = 1.471301625e+06;    // Frequency step of radar (assume common)
  static constexpr float        maxWr = c_light/(2*deltaF); // Max scene size of radar image (m)
  static constexpr float scale_domain = 1.0/maxWr;          // Scale 'dR' to produce 'domain'
  static constexpr float  sqrt_expand = 9.874208984e+03;    // Expand sqrt() output using sqrt(97.5e6)
  static constexpr unsigned    NPULSE = 586;

  unsigned pulse;
  unsigned count;

  dR_comp(void);

  // Run:
  void run( input_buffer<float>& sig_i, const float (&R0_range)[NPULSE],
            output_buffer<float>& domain_o, output_buffer<float>& theta_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dR_comp::run );
  }
};

