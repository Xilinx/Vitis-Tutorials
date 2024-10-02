//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#include "finder.h"

template<unsigned REGION_ID_0, unsigned REGION_ID_1>
void Finder<REGION_ID_0,REGION_ID_1>::run(adf::input_buffer< cfloat, adf::extents<adf::inherited_extent>> & __restrict in,
                                          adf::output_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict out) {
  auto inIterator  = aie::begin_vector<N>(in);
  auto outIterator = aie::begin_vector<N>(out);

  // interleave input and output to improve performance a bit
  for (unsigned i = 0; i < NUM_REGIONS; ++i)
    // chess_prepare_for_pipelining
    // chess_flatten_loop
  {
    Pm_null[i]     = *inIterator++;
    *outIterator++ = Pm_null[i];
  }

  for (unsigned i = 0; i < REGION_ID_0 / N; ++i)
    chess_prepare_for_pipelining
    chess_flatten_loop
  {
    *outIterator++ = *inIterator++;
  }

  aie::vector<cfloat,N> tag_cfloat = *inIterator++;
  float tag[ANGLE_FINDER_REGION_LEN];
  for (unsigned i = 0; i < ANGLE_FINDER_REGION_LEN; ++i)
  {
    cfloat tmp = tag_cfloat[i];
    tag[i]     = tmp.real;
  }

  // testbed[1 + N + 1]: 1 from previous segment, N (8) from current one, and 1 from next one
  // Initialize testbed[10] with non-zero numbers to avoid a linker error
  float testbed[1 + ANGLE_FINDER_REGION_LEN + 1] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};

  for (unsigned i = REGION_ID_0; i <= REGION_ID_1; ++i)
    // chess_prepare_for_pipelining
    // chess_flatten_loop
  {
    // ignore the left most and right most regions
    if (i == 0 || i == NUM_REGIONS - 1) continue;

    if (tag[i % N] == 0.0f) continue;

    unsigned index = 0;
    cfloat previous = Pm_null[i-1][N-1];
    testbed[index++] = previous.real;
    for (unsigned j = 0; j < N; ++j) {
      cfloat tmp = Pm_null[i][j];
      testbed[index++] = tmp.real;
    }
    cfloat tmp = Pm_null[i+1][0];
    testbed[index] = tmp.real;

    for (unsigned index = 1; index < N + 1; ++index) {
      if ((testbed[index] < DOA_MIN_THRESHOLD) && (testbed[index-1] > testbed[index]) && (testbed[index] < testbed[index+1])) {
        const unsigned n = i * N + index - 1;
        tag_cfloat[i % N] = cfloat({float(n), 0.0f});
      }
    }
  }
  *outIterator++ = tag_cfloat;

  for (unsigned i = REGION_ID_0 / N + 1; i < NUM_REGIONS / N; ++i)
    chess_prepare_for_pipelining
    chess_flatten_loop
  {
    *outIterator++ = *inIterator++;
  }
}
