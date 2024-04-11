//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins
#include <algorithm>
#include <array>
#include <adf.h>
#include <aie_api/aie.hpp>

#include "sortcpp_fp1024.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

sortcpp_fp1024::sortcpp_fp1024(void )
{
}

// ------------------------------------------------------------
// Load Data
// ------------------------------------------------------------

void __attribute__((always_inline))
sortcpp_fp1024::load_data( input_stream<float>* sig_i )
{
  auto it = aie::begin_restrict_vector<8>(data.data());
  aie::vector<float,8> vec;
  for (unsigned ii=0; ii < SIZE/8; ii++)
    chess_prepare_for_pipelining
    //      chess_unroll_loop(2)
  {
    vec.insert(0,readincr_v<4>(sig_i));
    vec.insert(1,readincr_v<4>(sig_i));
    *it++ = vec;
  }
}

// ------------------------------------------------------------
// Store Data
// ------------------------------------------------------------

void __attribute__((always_inline))
sortcpp_fp1024::store_data( output_stream<float>* sig_o )
{
  auto it = aie::begin_restrict_vector<8>(data.data());
  aie::vector<float,8> vec;
  for (unsigned ii=0; ii < SIZE/8; ii++)
    chess_prepare_for_pipelining
  {
    vec = *it++;
    writeincr(sig_o,vec.extract<4>(0));
    writeincr(sig_o,vec.extract<4>(1));
  }
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void sortcpp_fp1024::run( input_stream<float>* sig_i, output_stream<float>* sig_o )
{
  load_data(sig_i);
  std::sort(data.begin(),data.end());
  store_data(sig_o);
}



