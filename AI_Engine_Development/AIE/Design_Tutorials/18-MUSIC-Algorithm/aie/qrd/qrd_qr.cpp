//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#include "qrd_qr.h"

//  for kk = 1 : C
//    R(kk,kk) = norm(Q(:,kk))
//    Q(:,kk) = Q(:,kk) * (1.0/R(kk,kk))
//    for ii = kk+1 : C
//      R(kk,ii) = transpose(conj(Q(:,kk))) * Q(:,ii)
//      Q(:,ii) = Q(:,ii) - R(kk,ii) * Q(:,kk)
//    end
//  end

template<unsigned COL_NORM, unsigned COL_QR, unsigned IN_TOTAL, unsigned OUT_START, unsigned OUT_END>
void QRD_QR<COL_NORM,COL_QR,IN_TOTAL,OUT_START,OUT_END>::run(adf::input_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict in, adf::output_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict out) {
  auto inIterator  = aie::begin_vector<SEGMENT_SIZE>(in);
  auto outIterator = aie::begin_vector<SEGMENT_SIZE>(out);

  // read in A-matrix data to populate Q-matrix columns
  for (unsigned n = 0; n < IN_TOTAL; ++n)
    chess_prepare_for_pipelining
    chess_flatten_loop
  {
    Q[n] = *inIterator++;
  }
  for (unsigned n = 0; n < COL; ++n)
    chess_prepare_for_pipelining
    chess_flatten_loop
  {
    R[n] = *inIterator++;
  }

  aie::accum<caccfloat,SEGMENT_SIZE> acc = aie::zeros<caccfloat,SEGMENT_SIZE>();
  for (unsigned i = 0, m = SEGMENT_OFFSET; i < NUM_SEGMENTS_PER_COLUMN; ++i, ++m)
    chess_flatten_loop
  {
    acc = aie::mac(acc, aie::op_conj(Q[i]), Q[m]);
  }
  cfloat R_kk_ii = aie::reduce_add(aie::vector<cfloat,SEGMENT_SIZE>(acc));
  for (unsigned i = 0, m = SEGMENT_OFFSET; i < NUM_SEGMENTS_PER_COLUMN; ++i, ++m)
    chess_flatten_loop
  {
    aie::vector<cfloat,SEGMENT_SIZE> R_kk_ii_Q_j = aie::mul(Q[i], R_kk_ii);
    Q[m] = aie::sub(Q[m], R_kk_ii_Q_j);
  }
  R[COL_QR][COL_NORM] = R_kk_ii;  // kk-th element of column vector R[ii]

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
