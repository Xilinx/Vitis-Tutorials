//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#include "svd.h"

template<unsigned IN_V_FLAG, unsigned OUT_V_START, unsigned P0, unsigned Q0, unsigned P1, unsigned Q1, unsigned P2, unsigned Q2>
void SVD<IN_V_FLAG,OUT_V_START,P0,Q0,P1,Q1,P2,Q2>::run(adf::input_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict in, adf::output_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict out) {
  // read in R-matrix data to populate W-matrix columns
  auto inIterator  = aie::begin_vector<N>(in);
  auto outIterator = aie::begin_vector<N>(out);
  for (unsigned kk = 0; kk < COL; ++kk)
    chess_prepare_for_pipelining
    chess_flatten_loop
  {
    W[kk] = *inIterator++;
  }
  if constexpr (IN_V_FLAG == 0) {
    for (unsigned kk = 0; kk < COL; ++kk) {
      V[kk]     = aie::zeros<cfloat,N>();
      V[kk][kk] = cfloat({1.0f, 0.0f});
    }
  }
  else if constexpr (IN_V_FLAG > 0) {
    for (unsigned kk = 0; kk < COL; ++kk)
      chess_prepare_for_pipelining
      chess_flatten_loop
    {
      V[kk] = *inIterator++;
    }
  }

//  for iter = 1 : nIter
//    for p = 1 : nUla-1
//      for q = p+1 : nUla
//        Rot = jacobiRotation(W(:,p), W(:,q));
//        V(:,[p,q]) = V(:,[p,q]) * Rot;
//        W(:,[p,q]) = W(:,[p,q]) * Rot;
//      end
//    end
//  end

  compute_rotation(P0, Q0);
  compute_VW(P0, Q0);
  if constexpr (P1 != COL_NULL) {
    compute_rotation(P1, Q1);
    compute_VW(P1, Q1);
  }
  if constexpr (P2 != COL_NULL) {
    compute_rotation(P2, Q2);
    compute_VW(P2, Q2);
  }

  if constexpr (OUT_V_START == 0) {  // output both W and V in full
    for (unsigned col = 0; col < COL; ++col)
      chess_prepare_for_pipelining
      chess_flatten_loop
    {
      *outIterator++ = W[col];
    }
    for (unsigned col = 0; col < COL; ++col)
      chess_prepare_for_pipelining
      chess_flatten_loop
    {
      *outIterator++ = V[col];
    }
  }
  else if constexpr (OUT_V_START > 0) {  // output partial V of noise sub-space only
    for (unsigned col = 0; col < COL; ++col) {
      const cfloat s_complex = dot_product(W[col], W[col]);
      S[col] = s_complex.real;
    }
    const float scaling_factor = inv(S[0]);
    S = aie::mul(S, scaling_factor);
    const aie::mask<N> compare_mask = aie::gt(S, NOISE_SUBSPACE_THRESHOLD);
    unsigned nSources = compare_mask.count();

    // N x N matrix: {nSources, 0.0f}, V[nSources] to V[N-1], {0.0f, 0.0f}, ...
    // Note: if nSources is 8 (N), there is actually no signal source. The NxN matrix
    // is { {{8.0f, 0.0f}, 7 x {0.0f, 0.0f}}, 7 x { 8 x {0.0f, 0.0f} }. The pseudo
    // spectrum calculations do NOT use nSources and all bins will be zeros.
    aie::vector<cfloat, N> scratchpad = aie::zeros<cfloat,N>();
    scratchpad[0]  = cfloat({float(nSources), 0.0f});
    *outIterator++ = scratchpad;

    for (unsigned col = nSources; col < COL; ++col)
      chess_prepare_for_pipelining
    {
      *outIterator++ = V[col];
    }
    scratchpad[0] = cfloat({0.0f, 0.0f});  // reset scratchpad
    for (unsigned i = 0; i < nSources - 1; ++i)
      chess_prepare_for_pipelining
    {
      *outIterator++ = scratchpad;
    }
  }
}

template<unsigned IN_V_FLAG, unsigned OUT_V_START, unsigned P0, unsigned Q0, unsigned P1, unsigned Q1, unsigned P2, unsigned Q2>
inline cfloat SVD<IN_V_FLAG,OUT_V_START,P0,Q0,P1,Q1,P2,Q2>::dot_product(aie::vector<cfloat,N> A, aie::vector<cfloat,N> B) {
  aie::accum<caccfloat,N> acc = aie::mul(A, aie::op_conj(B));
  const cfloat dot_product = aie::reduce_add(aie::vector<cfloat,N>(acc));

  return dot_product;
}

template<unsigned IN_V_FLAG, unsigned OUT_V_START, unsigned P0, unsigned Q0, unsigned P1, unsigned Q1, unsigned P2, unsigned Q2>
inline cfloat SVD<IN_V_FLAG,OUT_V_START,P0,Q0,P1,Q1,P2,Q2>::calc_ei_2t(const float x, const float y) {
  static constexpr float epsilon = 6.561e-15f;                // invsqrt(epsilon) = 12345679.01234568
  const float R_inv = aie::invsqrt(x * x + y * y + epsilon);  // add an epsilon to prevent invsqrt(0.0f)

  return cfloat({x * R_inv, y * R_inv});
}

template<unsigned IN_V_FLAG, unsigned OUT_V_START, unsigned P0, unsigned Q0, unsigned P1, unsigned Q1, unsigned P2, unsigned Q2>
inline cfloat SVD<IN_V_FLAG,OUT_V_START,P0,Q0,P1,Q1,P2,Q2>::calc_ei_t(const float sin_2t, const float cos_2t) {
  return calc_ei_2t(1.0f + cos_2t, sin_2t);
}

template<unsigned IN_V_FLAG, unsigned OUT_V_START, unsigned P0, unsigned Q0, unsigned P1, unsigned Q1, unsigned P2, unsigned Q2>
inline void SVD<IN_V_FLAG,OUT_V_START,P0,Q0,P1,Q1,P2,Q2>::calc_rot(const cfloat eit, const float c, const float s) {
  Rot[0] =           eit  * cfloat({0.0f, s});
  Rot[1] = aie::conj(eit) * cfloat({-c,   0.0f});
  Rot[2] =           eit  * cfloat({c,    0.0f});
  Rot[3] = aie::conj(eit) * cfloat({0.0f, -s});
}

//function [Rot] = compute_rotation( Xv, Yv )
//   Hpp = real(transpose(Xv)*conj(Xv));
//   Hqq = real(transpose(Yv)*conj(Yv));
//   tmp = transpose(Xv)*conj(Yv);
//   Hrr = real(tmp);
//   Hjj = imag(tmp);
//
//   ei_2t1 = calc_ei_2t(Hjj,Hrr);
//   ei_t1  = calc_ei_t(imag(ei_2t1),real(ei_2t1));
//
//   tx = 0.5*(Hqq-Hpp);
//   ty = Hrr * imag(ei_2t1) + Hjj * real(ei_2t1);
//
//   ei_2t2 = calc_ei_2t(tx,ty);
//   ei_t2  = calc_ei_t(imag(ei_2t2), real(ei_2t2));
//
//   Rot = calc_rot( ei_t1, real(ei_t2), imag(ei_t2) );
//end

template<unsigned IN_V_FLAG, unsigned OUT_V_START, unsigned P0, unsigned Q0, unsigned P1, unsigned Q1, unsigned P2, unsigned Q2>
inline void SVD<IN_V_FLAG,OUT_V_START,P0,Q0,P1,Q1,P2,Q2>::compute_rotation(const unsigned p, const unsigned q) {
  const cfloat dotWp  = dot_product(W[p], W[p]);
  const cfloat dotWq  = dot_product(W[q], W[q]);
  const cfloat dotWpq = dot_product(W[p], W[q]);

  const float  Hrr    = dotWpq.real;
  const float  Hjj    = dotWpq.imag;
  const cfloat ei_2t1 = calc_ei_2t(Hjj, Hrr);
  const cfloat ei_t1  = calc_ei_t(ei_2t1.imag, ei_2t1.real);

  const float  tx     = 0.5f * (dotWq.real - dotWp.real);
  const float  ty     = Hrr * ei_2t1.imag + Hjj * ei_2t1.real;
  const cfloat ei_2t2 = calc_ei_2t(tx, ty);
  const cfloat ei_t2  = calc_ei_t(ei_2t2.imag, ei_2t2.real);

  calc_rot(ei_t1, ei_t2.real, ei_t2.imag);
}

template<unsigned IN_V_FLAG, unsigned OUT_V_START, unsigned P0, unsigned Q0, unsigned P1, unsigned Q1, unsigned P2, unsigned Q2>
inline void SVD<IN_V_FLAG,OUT_V_START,P0,Q0,P1,Q1,P2,Q2>::compute_VW(const unsigned p, const unsigned q) {
  aie::vector<cfloat,N> op0 = aie::mul(V[p], Rot[0]);
  aie::vector<cfloat,N> op1 = aie::mul(V[q], Rot[1]);
  aie::vector<cfloat,N> op2 = aie::mul(V[p], Rot[2]);
  aie::vector<cfloat,N> op3 = aie::mul(V[q], Rot[3]);
  V[p] = aie::add(op0, op1);
  V[q] = aie::add(op2, op3);

  op0 = aie::mul(W[p], Rot[0]);
  op1 = aie::mul(W[q], Rot[1]);
  op2 = aie::mul(W[p], Rot[2]);
  op3 = aie::mul(W[q], Rot[3]);
  W[p] = aie::add(op0, op1);
  W[q] = aie::add(op2, op3);
}
