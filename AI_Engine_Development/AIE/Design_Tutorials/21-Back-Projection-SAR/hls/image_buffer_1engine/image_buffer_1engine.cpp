//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "image_buffer_template.h"
#include "image_buffer_1engine.h"
using namespace image_buffer_1engine;

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void image_buffer_1engine_wrapper( TT_DATA    mem[DEPTH],
                                   unsigned   NPULSE_USE,
                                   TT_STREAM& buff_to_aie,
                                   TT_STREAM& aie_to_buff )
{
#pragma HLS interface m_axi      port=mem         bundle=gmem    offset=slave   depth=DEPTH
#pragma HLS interface axis       port=buff_to_aie
#pragma HLS interface axis       port=aie_to_buff
#pragma HLS interface s_axilite  port=NPULSE_USE  bundle=control
#pragma HLS interface s_axilite  port=mem         bundle=control
#pragma HLS interface s_axilite  port=return      bundle=control

  static image_buffer_template<TT_DATA,DEPTH> kk;

  kk.run( mem, NPULSE_USE, buff_to_aie, aie_to_buff );
}
