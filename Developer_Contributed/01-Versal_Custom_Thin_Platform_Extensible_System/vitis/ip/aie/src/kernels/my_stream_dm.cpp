// /**********
// © Copyright 2023 Advanced Micro Devices (AMD), Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// **********/

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
  int const smpl_drop = 4;
  for (int i = 0; i < smpl_drop; i++)
  {
    // Drop only on stream port 0
    get_ss(0);
  }
}
// Run:
template <typename TT_DATA>
void __attribute__ ((noinline)) my_stream_dm<TT_DATA>::run(input_stream<TT_DATA> * __restrict sig_i, output_stream<TT_DATA> * __restrict sig_o )
{
  TT_DATA tmp_data;

  while (true)
  chess_prepare_for_pipelining
  {
    tmp_data = readincr(sig_i);
    writeincr(sig_o, tmp_data);
  }
}

