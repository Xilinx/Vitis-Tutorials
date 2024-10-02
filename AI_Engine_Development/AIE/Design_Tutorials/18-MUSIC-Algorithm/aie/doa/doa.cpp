//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#include "doa.h"

#ifndef M_PI
static constexpr float M_PI  = 3.1415926f;
#endif

#ifndef M_2PI
static constexpr float M_2PI = 6.2831852f;
#endif

template<unsigned N, unsigned INPUT_PM_NULL_FLAG, unsigned START, unsigned END>
DOA<N,INPUT_PM_NULL_FLAG,START,END>::DOA(void) {
  for (unsigned n = START; n < END; ++n) {
    // sweepPts[n] = (-90.0f + n * 180.0f / (nPts - 1)) * M_PI / 180.0f;
    // const float theta = (-0.5f + float(n) / float(DOA::NUM_POINTS - 1)) * M_PI;
    // const float theta = -1.5707963f + float(n) * 0.01231997f;  // NUM_POINTS = 256
    const float theta = -1.5707963f + float(n) * M_PI / float(NUM_POINTS-1);

    const float tmp2  = M_PI * aie::sin(theta);
    steeringVector[n-START][0] = cfloat({1.0f, 0.0f});
    for (unsigned k = 1; k < N; ++k) {
      float phase = float(k) * tmp2;
      float sign  = 1.0f;
      if (phase < 0) {
        phase = -phase;
        sign = -1.0f;
      }
      while (phase > M_PI)
        phase -= M_2PI;

      steeringVector[n-START][k] = cfloat({aie::cos(phase), sign * aie::sin(phase)});
    }
  }
}

template<unsigned N, unsigned INPUT_PM_NULL_FLAG, unsigned START, unsigned END>
void DOA<N,INPUT_PM_NULL_FLAG,START,END>::run(adf::input_buffer< cfloat, adf::extents<adf::inherited_extent>> & __restrict in,
                                              adf::output_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict out) {
  auto inIterator_noiseSS  = aie::begin_vector<N>(in);
  auto outIterator_noiseSS = aie::begin_vector<N>(out);

  auto inIterator_Pm_null  = aie::begin_vector<DOA_INTERVAL_LEN>(in);
  auto outIterator_Pm_null = aie::begin_vector<DOA_INTERVAL_LEN>(out);

  static constexpr unsigned multiplier = COL / DOA_INTERVAL_LEN;
  static constexpr unsigned advance = N * multiplier;
  inIterator_Pm_null  += advance;
  outIterator_Pm_null += advance;

  cfloat                  norm_square;
  aie::accum<caccfloat,N> acc0, acc1;
  aie::vector<cfloat,N>   scratchpad = aie::zeros<cfloat,N>();

  // read in nSources and pass it through
  *outIterator_noiseSS++ = *inIterator_noiseSS++;

  // steeringVector[0]
  {
    noiseSS[0] = *inIterator_noiseSS++;
    acc0 = aie::mul(noiseSS[0], steeringVector[0]);
    scratchpad[0] = aie::reduce_add(aie::vector<cfloat,N>(acc0));
    *outIterator_noiseSS++ = noiseSS[0];

    noiseSS[1] = *inIterator_noiseSS++;
    acc1 = aie::mul(noiseSS[1], steeringVector[0]);
    scratchpad[1] = aie::reduce_add(aie::vector<cfloat,N>(acc1));
    *outIterator_noiseSS++ = noiseSS[1];

    noiseSS[2] = *inIterator_noiseSS++;
    acc0 = aie::mul(noiseSS[2], steeringVector[0]);
    scratchpad[2] = aie::reduce_add(aie::vector<cfloat,N>(acc0));
    *outIterator_noiseSS++ = noiseSS[2];

    noiseSS[3] = *inIterator_noiseSS++;
    acc1 = aie::mul(noiseSS[3], steeringVector[0]);
    scratchpad[3] = aie::reduce_add(aie::vector<cfloat,N>(acc1));
    *outIterator_noiseSS++ = noiseSS[3];

    noiseSS[4] = *inIterator_noiseSS++;
    acc0 = aie::mul(noiseSS[4], steeringVector[0]);
    scratchpad[4] = aie::reduce_add(aie::vector<cfloat,N>(acc0));
    *outIterator_noiseSS++ = noiseSS[4];

    noiseSS[5] = *inIterator_noiseSS++;
    acc1 = aie::mul(noiseSS[5], steeringVector[0]);
    scratchpad[5] = aie::reduce_add(aie::vector<cfloat,N>(acc1));
    *outIterator_noiseSS++ = noiseSS[5];

    noiseSS[6] = *inIterator_noiseSS++;
    acc0 = aie::mul(noiseSS[6], steeringVector[0]);
    scratchpad[6] = aie::reduce_add(aie::vector<cfloat,N>(acc0));
    *outIterator_noiseSS++ = noiseSS[6];

    acc0 = aie::mul(aie::op_conj(scratchpad), scratchpad);
    norm_square = aie::reduce_add(aie::vector<cfloat,N>(acc0));
  }

  if constexpr (INPUT_PM_NULL_FLAG != 0) {
    for (unsigned i = 0; i < NUM_DOA_INTERVALS; ++i)
      chess_prepare_for_pipelining
      chess_flatten_loop
    {
      Pm_null[i] = *inIterator_Pm_null++;
    }
  }

  static constexpr unsigned DOA_INTERVAL_ID = START / DOA_INTERVAL_LEN;
  Pm_null[DOA_INTERVAL_ID][0] = norm_square;

  // steeringVector[1] to steeringVector[7]
  for (unsigned n = 1; n < END - START; ++n)
    chess_prepare_for_pipelining
    chess_flatten_loop
  {
    acc0 = aie::mul(noiseSS[0], steeringVector[n]);
    scratchpad[0] = aie::reduce_add(aie::vector<cfloat,N>(acc0));
    acc1 = aie::mul(noiseSS[1], steeringVector[n]);
    scratchpad[1] = aie::reduce_add(aie::vector<cfloat,N>(acc1));
    acc0 = aie::mul(noiseSS[2], steeringVector[n]);
    scratchpad[2] = aie::reduce_add(aie::vector<cfloat,N>(acc0));
    acc1 = aie::mul(noiseSS[3], steeringVector[n]);
    scratchpad[3] = aie::reduce_add(aie::vector<cfloat,N>(acc1));
    acc0 = aie::mul(noiseSS[4], steeringVector[n]);
    scratchpad[4] = aie::reduce_add(aie::vector<cfloat,N>(acc0));
    acc1 = aie::mul(noiseSS[5], steeringVector[n]);
    scratchpad[5] = aie::reduce_add(aie::vector<cfloat,N>(acc1));
    acc0 = aie::mul(noiseSS[6], steeringVector[n]);
    scratchpad[6] = aie::reduce_add(aie::vector<cfloat,N>(acc0));

    acc1 = aie::mul(aie::op_conj(scratchpad), scratchpad);
    norm_square = aie::reduce_add(aie::vector<cfloat,N>(acc1));
    Pm_null[DOA_INTERVAL_ID][n] = norm_square;
  }

  for (unsigned i = 0; i < NUM_DOA_INTERVALS; ++i)
    chess_prepare_for_pipelining
    chess_flatten_loop
  {
    *outIterator_Pm_null++ = Pm_null[i];
  }
}
