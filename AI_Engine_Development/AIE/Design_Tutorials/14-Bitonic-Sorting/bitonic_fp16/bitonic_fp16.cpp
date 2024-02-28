//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "bitonic_fp16.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

bitonic_fp16::bitonic_fp16(void )
{
}

// ------------------------------------------------------------
// Stage 0
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp16::stage0_intrinsic( aie::vector<float,16>& vec )
{
  static constexpr unsigned BFLY_STAGE0_TOP_I = 0xECA86420;
  static constexpr unsigned BFLY_STAGE0_BOT_I = 0xFDB97531;
  static constexpr unsigned BFLY_STAGE0_TOP_O = 0xB3A29180;
  static constexpr unsigned BFLY_STAGE0_BOT_O = 0xF7E6D5C4;
  vec = fpshuffle16(vec,0,BFLY_STAGE0_TOP_I,BFLY_STAGE0_BOT_I);
  aie::vector<float,8> v_top = vec.extract<8>(0);
  aie::vector<float,8> v_bot = vec.extract<8>(1);
  aie::vector<float,8> v_mx = fpmax(v_top,v_bot);
  aie::vector<float,8> v_mn = fpmin(v_top,v_bot);
  vec = aie::concat(v_mn,v_mx);
  vec = fpshuffle16(vec,0,BFLY_STAGE0_TOP_O,BFLY_STAGE0_BOT_O);
}

void __attribute__((always_inline))
bitonic_fp16::stage0_api( aie::vector<float,16>& vec )
{
  aie::vector<float,8> v_top = aie::filter_even(vec);
  aie::vector<float,8> v_bot = aie::filter_odd(vec);
  aie::vector<float,8> v_mx = aie::max(v_top,v_bot);
  aie::vector<float,8> v_mn = aie::min(v_top,v_bot);
  std::tie(v_mn,v_mx) = aie::interleave_zip(v_mn,v_mx,1);
  vec = aie::concat(v_mn,v_mx);
}

// ------------------------------------------------------------
// Stage 1a
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp16::stage1a( aie::vector<float,16>& vec )
{
  static constexpr unsigned BFLY_STAGE1a_TOP_I = 0xDC985410;
  static constexpr unsigned BFLY_STAGE1a_BOT_I = 0xEFAB6723;
  static constexpr unsigned BFLY_STAGE1a_TOP_O = 0xAB328910;
  static constexpr unsigned BFLY_STAGE1a_BOT_O = 0xEF76CD54;
  vec = fpshuffle16(vec,0,BFLY_STAGE1a_TOP_I,BFLY_STAGE1a_BOT_I);
  aie::vector<float,8>  v_top = vec.extract<8>(0);
  aie::vector<float,8>  v_bot = vec.extract<8>(1);
  aie::vector<float,8>  v_mx = aie::max(v_top, v_bot);
  aie::vector<float,8>  v_mn = aie::min(v_top, v_bot);
  vec = aie::concat(v_mn,v_mx);
  vec = fpshuffle16(vec,0,BFLY_STAGE1a_TOP_O,BFLY_STAGE1a_BOT_O);
}

// ------------------------------------------------------------
// Stage 1
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp16::stage1( aie::vector<float,16>& vec )
{
  stage1a( vec );
  stage0_intrinsic(vec);            // Stage 1b
}

// ------------------------------------------------------------
// Stage 2a
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp16::stage2a( aie::vector<float,16>& vec )
{
  static constexpr unsigned BFLY_STAGE2a_TOP_I = 0xBA983210;
  static constexpr unsigned BFLY_STAGE2a_BOT_I = 0xCDEF4567;
  static constexpr unsigned BFLY_STAGE2a_TOP_O = 0x89AB3210;
  static constexpr unsigned BFLY_STAGE2a_BOT_O = 0xCDEF7654;
  aie::vector<float,8> v_mx;
  aie::vector<float,8> v_mn;
  vec = fpshuffle16(vec,0,BFLY_STAGE2a_TOP_I,BFLY_STAGE2a_BOT_I);
  v_mx = aie::max(vec.extract<8>(0),vec.extract<8>(1));
  v_mn = aie::min(vec.extract<8>(0),vec.extract<8>(1));
  vec = aie::concat(v_mn,v_mx);
  vec = fpshuffle16(vec,0,BFLY_STAGE2a_TOP_O,BFLY_STAGE2a_BOT_O);
}

// ------------------------------------------------------------
// Stage 2b
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp16::stage2b( aie::vector<float,16>& vec )
{
  static constexpr unsigned BFLY_STAGE2b_TOP_I = 0xDC985410;
  static constexpr unsigned BFLY_STAGE2b_BOT_I = 0xFEBA7632;
  static constexpr unsigned BFLY_STAGE2b_TOP_O = 0xBA329810;
  static constexpr unsigned BFLY_STAGE2b_BOT_O = 0xFE76DC54;
  aie::vector<float,8> v_mx;
  aie::vector<float,8> v_mn;
  vec = fpshuffle16(vec,0,BFLY_STAGE2b_TOP_I,BFLY_STAGE2b_BOT_I); // Combine these two shuffles
  v_mx = aie::max(vec.extract<8>(0),vec.extract<8>(1));
  v_mn = aie::min(vec.extract<8>(0),vec.extract<8>(1));
  vec = aie::concat(v_mn,v_mx);
  vec = fpshuffle16(vec,0,BFLY_STAGE2b_TOP_O,BFLY_STAGE2b_BOT_O);
}

// ------------------------------------------------------------
// Stage 2
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp16::stage2( aie::vector<float,16>& vec )
{
  stage2a(vec);
  stage2b(vec);
  stage0_intrinsic(vec);            // Stage 2c
}

// ------------------------------------------------------------
// Stage 3a
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp16::stage3a( aie::vector<float,16>& vec )
{
  aie::vector<float,8>  v_top = vec.extract<8>(0);
  aie::vector<float,8>  v_bot = aie::reverse(vec.extract<8>(1));;
  aie::vector<float,8>  v_mx  = aie::max(v_top,v_bot);
  aie::vector<float,8>  v_mn  = aie::min(v_top,v_bot);
  vec   = aie::concat(v_mn,aie::reverse(v_mx));
}

// ------------------------------------------------------------
// Stage 3b
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp16::stage3b( aie::vector<float,16>& vec )
{
  static constexpr unsigned BFLY_STAGE3b_TOP_I = 0xBA983210;
  static constexpr unsigned BFLY_STAGE3b_BOT_I = 0xFEDC7654;
  static constexpr unsigned BFLY_STAGE3b_TOP_O = 0xBA983210;
  static constexpr unsigned BFLY_STAGE3b_BOT_O = 0xFEDC7654;
  aie::vector<float,8> v_mx;
  aie::vector<float,8> v_mn;
  vec = fpshuffle16(vec,0,BFLY_STAGE3b_TOP_I,BFLY_STAGE3b_BOT_I); // Combine these two shuffles
  v_mx = aie::max(vec.extract<8>(0),vec.extract<8>(1));
  v_mn = aie::min(vec.extract<8>(0),vec.extract<8>(1));
  vec = aie::concat(v_mn,v_mx);
  vec = fpshuffle16(vec,0,BFLY_STAGE3b_TOP_O,BFLY_STAGE3b_BOT_O);
}

// ------------------------------------------------------------
// Stage 3
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp16::stage3( aie::vector<float,16>& vec )
{
  aie::vector<float,8> v_mx;
  aie::vector<float,8> v_mn;
  stage3a(vec);
  stage3b(vec);
  stage2b(vec);           // Stage 3c
  stage0_intrinsic(vec);  // Stage 3d
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void bitonic_fp16::run( input_stream<float>* sig_i, output_stream<float>* sig_o )
{
  // Input:
  aie::vector<float,16> vec = readincr_v<16>(sig_i);
  // Sort:
  stage0_intrinsic( vec );
  stage1( vec );
  stage2( vec );
  stage3( vec );
  // Output:
  writeincr(sig_o,vec);
}



