//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#include "qrd_norm.h"

//  for kk = 1 : C
//    R(kk,kk) = norm(Q(:,kk))
//    Q(:,kk) = Q(:,kk) * (1.0/R(kk,kk))
//    for ii = kk+1 : C
//      R(kk,ii) = transpose(conj(Q(:,kk))) * Q(:,ii)
//      Q(:,ii) = Q(:,ii) - R(kk,ii) * Q(:,kk)
//    end
//  end

template<unsigned COL_NORM, unsigned IN_TOTAL, unsigned OUT_START, unsigned OUT_END>
void QRD_Norm<COL_NORM,IN_TOTAL,OUT_START,OUT_END>::run(adf::input_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict in, adf::output_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict out) {
  auto inIterator  = aie::begin_vector<SEGMENT_SIZE>(in);
  auto outIterator = aie::begin_vector<SEGMENT_SIZE>(out);

  // read in A-matrix data to populate Q-matrix columns
  for (unsigned n = 0; n < IN_TOTAL; ++n)
    chess_prepare_for_pipelining
    chess_flatten_loop
  {
    Q[n] = *inIterator++;
  }
  if constexpr (COL_NORM == 0) {
    for (unsigned n = 0; n < COL; ++n)
      chess_prepare_for_pipelining
      chess_flatten_loop
    {
      R[n] = aie::zeros<cfloat,COL>();
    }
  }
  else if constexpr (COL_NORM > 0) {
    for (unsigned n = 0; n < COL; ++n)
      chess_prepare_for_pipelining
      chess_flatten_loop
    {
      R[n] = *inIterator++;
    }
  }
  aie::accum<caccfloat,SEGMENT_SIZE> acc = aie::zeros<caccfloat,SEGMENT_SIZE>();
  for (unsigned i = 0; i < NUM_SEGMENTS_PER_COLUMN; ++i)
    chess_flatten_loop
  {
    acc = aie::mac(acc, aie::op_conj(Q[i]), Q[i]);
  }
  cfloat norm_square = aie::reduce_add(aie::vector<cfloat,SEGMENT_SIZE>(acc));
  float  norm        = aie::sqrt(norm_square.real);
  float  norm_inv    = aie::inv(norm);
  R[COL_NORM][COL_NORM] = cfloat({norm, 0.0f});
  for (unsigned i = 0; i < NUM_SEGMENTS_PER_COLUMN; ++i)
    chess_flatten_loop
  {
    Q[i] = aie::mul(Q[i], norm_inv);
  }

  for (unsigned n = OUT_START; n < OUT_END; ++n)
    chess_prepare_for_pipelining
    chess_flatten_loop
  {
    *outIterator++ = Q[n];
  }
  for (unsigned n = 0; n < COL; ++n)
    chess_prepare_for_pipelining
    chess_flatten_loop
  {
    *outIterator++ = R[n];
  }
}
