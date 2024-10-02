//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#include "io_adapter.h"

void IO_Adapter::run(input_stream<cfloat> * __restrict sig_i_0,
                     input_stream<cfloat> * __restrict sig_i_1,
                     adf::output_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict out) {
  static constexpr unsigned IO_BUFFER_BUS_WIDTH = 256;  // 256 bits
  static constexpr unsigned NUM_ELEMENTS = IO_BUFFER_BUS_WIDTH / (sizeof(cfloat) * 8);  // 8 * 8 = 64 bits for cfloat

  auto outIterator = aie::begin_vector<NUM_ELEMENTS>(out);

  aie::vector<cfloat, NUM_ELEMENTS> vec_0, vec_1;
  for (unsigned i = 0; i < (ROW * COL) / (2 * NUM_ELEMENTS); ++i)
    chess_prepare_for_pipelining
    chess_flatten_loop
  {
    vec_0.insert(0, readincr_v<NUM_ELEMENTS, aie_stream_resource_in::a>(sig_i_0));
    *outIterator++ = vec_0;
    vec_1.insert(0, readincr_v<NUM_ELEMENTS, aie_stream_resource_in::b>(sig_i_1));
    *outIterator++ = vec_1;
  }
}
