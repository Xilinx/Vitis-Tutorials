//
// Copyright (C) 2023-2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
// Author Derek Hagen

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include "kernels.hpp"

// Constructor
template <typename TT_DATA>
my_stream_dm<TT_DATA>::my_stream_dm()
{
  // Drop samples before starting kernel to start fill fifos.
  // This is usually needed when using broadcast to several kernels.
  //int const smpl_drop = 4;
  //for (int i = 0; i < smpl_drop; i++)
  //{
  //  // Drop only on stream port 0
  //  get_ss(0);
  //}
}
// Run:
template <typename TT_DATA>
void __attribute__ ((noinline)) my_stream_dm<TT_DATA>::run(input_stream<TT_DATA> * __restrict sig_i, output_stream<TT_DATA> * __restrict sig_o )
{

  //TT_DATA tmp_data;
  // Use two 128 bit vector registers as ping/pong buffers
  //aie::vector<TT_DATA, 4> chess_storage(vrl0) d0;
  //aie::vector<TT_DATA, 4> chess_storage(vrh0) d1;
  aie::vector<TT_DATA, 4> d0;
  aie::vector<TT_DATA, 4> d1;
  //aie::vector<TT_DATA, 8> dvec;
  //aie::locate_in_register<0>(dvec);  // Bind register

  // Preload vector regs
  //d0 = readincr_v<4>(sig_i);
//  dvec.insert(0, readincr_v<4>(sig_i));

  // For comparing scalar stream datamover.
  // This will no be able to keep up with data due to register spilling.
//  while (true)
//  chess_prepare_for_pipelining
//  {
//    tmp_data = readincr(sig_i);
//    writeincr(sig_o, tmp_data);
//  }

  //for (int i = 0; i < 16; i++)
  while (true)
  chess_prepare_for_pipelining
  {
    // Acquiring 128-bit vector takes 4 clock cycles.
    // By using two vector register alternating beteen read and write, the operation can be pipelined with no gaps on the AXI Stream port
    // Each loop iteration move 8 samples, 4 samples per vector, unrolled twice.
    // This gives an average throughput of one sample per clock.
//    dvec.insert(1, readincr_v<4>(sig_i));
    d1 = readincr_v<4>(sig_i);
    writeincr(sig_o, d0); //dvec.extract(0));
    //writeincr_v<4>(sig_o, d0); //dvec.extract(0));
//    dvec.insert(0, readincr_v<4>(sig_i));
    d0 = readincr_v<4>(sig_i);
    writeincr(sig_o, d1); //dvec.extract(1));
    //writeincr_v<4>(sig_o, d1); //dvec.extract(1));
  }
  // Write to keep balance between reading/writing due to preload.
  //writeincr_v<4>(sig_o, d0);
}

