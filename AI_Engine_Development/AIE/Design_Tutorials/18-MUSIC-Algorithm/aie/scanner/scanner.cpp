//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#include "scanner.h"

template<unsigned START, unsigned END>
void Scanner<START,END>::run(adf::input_buffer< cfloat, adf::extents<adf::inherited_extent>> & __restrict in,
                             adf::output_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict out) {
  auto inIterator  = aie::begin_vector<N>(in);
  auto outIterator = aie::begin_vector<N>(out);

  // noiseSS is not used by the Scanner class
  if constexpr (START == 0) {
    for (unsigned i = 0; i < N; ++i)
      // chess_prepare_for_pipelining
      // chess_flatten_loop
    {
      noiseSS = *inIterator++;
    }
  }

  // read in and write out Pm_null[NUM_POINTS]
  unsigned count = 0;
  for (unsigned i = 0; i < NUM_REGIONS; ++i)
    // chess_prepare_for_pipelining
    // chess_flatten_loop
  {
    Pm_null_cfloat[i] = *inIterator++;
    *outIterator++ = Pm_null_cfloat[i];
  }

  // read in the first half of tags and write them out when calculating the second half of tags
  if constexpr (START != 0) {
    *outIterator++ = *inIterator++;
    *outIterator++ = *inIterator++;
  }

  const aie::vector<float,N> THRESHOLD_VECTOR = aie::broadcast<float,N>(DOA_MIN_THRESHOLD);
  aie::vector<float,N>       Pm_null_float;
  unsigned                   tag[NUM_REGIONS];
  for (unsigned i = START; i < END; ++i)
    // chess_prepare_for_pipelining
    // chess_flatten_loop
  {
    for (unsigned j = 0; j < N; ++j) {
      const cfloat tmp = Pm_null_cfloat[i][j];
      Pm_null_float[j] = tmp.real;
    }
    const aie::vector<float,N> compare_result = aie::min(Pm_null_float, THRESHOLD_VECTOR);
    tag[i] = !(aie::equal(compare_result, THRESHOLD_VECTOR));
  }

  aie::vector<cfloat, N> tag_cfloat;
  for (unsigned i = 0, index = START; i < 2; ++i) {
    for (unsigned j = 0; j < N; ++j) {
      tag_cfloat[j] = cfloat({float(tag[index++]), 0.0f});
    }
    *outIterator++ = tag_cfloat;
  }
}
