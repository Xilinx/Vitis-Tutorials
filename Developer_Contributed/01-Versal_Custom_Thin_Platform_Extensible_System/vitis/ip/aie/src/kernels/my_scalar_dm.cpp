//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include "kernels.hpp"

// Constructor
template <typename TT_DATA, size_t BUF_SIZE>
my_scalar_dm<TT_DATA, BUF_SIZE>::my_scalar_dm()
{
}
// Run:
template <typename TT_DATA, size_t BUF_SIZE>
void __attribute__ ((noinline)) my_scalar_dm<TT_DATA, BUF_SIZE>::run(adf::input_buffer<TT_DATA, adf::extents<BUF_SIZE>> & __restrict sig_i, adf::output_buffer<TT_DATA, adf::extents<BUF_SIZE>> & __restrict sig_o )
{
  int my_size = sig_o.size();

  // Declare iterators
  auto in_iter =  aie::begin(sig_i);
  auto out_iter =  aie::begin(sig_o);

  for (int i = 0; i < my_size; i++)
  {
    *out_iter++ = *in_iter++;
  }
}

