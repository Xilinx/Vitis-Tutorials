//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "interp1.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
interp1<REPEAT,NSAMP>::interp1( float (&lut_copy_i)[NLUT], float (&m_slopeBuff_i)[NSAMP],
                                float (&m_offsetBuff_i)[NSAMP] )
  : lut_copy(lut_copy_i), m_slopeBuff(m_slopeBuff_i), m_offsetBuff(m_offsetBuff_i)
{
  aie::set_rounding(aie::rounding_mode::floor);
  aie::set_saturation(aie::saturation_mode::saturate);
  state = 0;
  count = 0;
}

// ------------------------------------------------------------
// Copy LUT
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
inline __attribute__ ((always_inline))
void interp1<REPEAT,NSAMP>::copy_lut(input_async_buffer<float>& lut_i)
{
  auto itr = aie::begin_restrict_vector<8>(lut_i);
  auto itw = aie::begin_restrict_vector<8>(lut_copy);
  for (unsigned rr=0; rr < NLUT/8; rr++)
    chess_prepare_for_pipelining
  {
    *itw++ = *itr++;
  } // rr
}

// ------------------------------------------------------------
// Compute Workload
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
inline __attribute__ ((always_inline))
void interp1<REPEAT,NSAMP>::compute_workload(input_async_buffer<float>& lut_i,
					     input_buffer<float>& domain_i,
                                             output_buffer<float>& interp_o)
{
  using dataVect_t = ::aie::vector<float,8>;
  using accVect_t = ::aie::accum<accfloat,8>;

  dataVect_t dataVect, offsetVect, slopeVect;
  float* __restrict slopePtr = &m_offsetBuff[0];
  float* __restrict offsetPtr = &m_slopeBuff[0];
  dataVect_t* __restrict slopeVectPtr = (dataVect_t*)m_offsetBuff;
  dataVect_t* __restrict offsetVectPtr = (dataVect_t*)m_slopeBuff;

  ::aie::vector<int32,8> idxVect, intDataVect;
  accVect_t acc;
  dataVect_t* __restrict inPtr = (dataVect_t*)domain_i.data();
  dataVect_t* __restrict outPtr = (dataVect_t*)interp_o.data();

  cfloat* __restrict ptr1 = reinterpret_cast<cfloat*>(lut_i.data());
  cfloat* __restrict ptr2 = reinterpret_cast<cfloat*>(&lut_copy[0]);

  for (int i = 0; i < (NSAMP/8); i++)
    chess_prepare_for_pipelining chess_loop_range((NSAMP/8), )
  {
    // Load vector of x (floats)
    dataVect = *inPtr++;
    // cast vector to int16 and downshift to find LUT index
    intDataVect =
      ::aie::to_fixed<int32,8>(dataVect, TP_FINE_BITS + TP_COARSE_BITS);
    idxVect = ::aie::detail::shift<int32,8>::run(intDataVect, 0, TP_FINE_BITS);
    // idxVect = ::aie::downshift(intDataVect, TP_FINE_BITS);
#pragma unroll(8)
    for (int j = 0; j < 8; j++) {
      *slopePtr++  = ptr1[idxVect[j]].real;
      *offsetPtr++ = ptr2[idxVect[j]].imag;
    }
  }
  chess_separator();
  
  inPtr -= (NSAMP/8);
  dataVect = *inPtr++;
  offsetVect = *offsetVectPtr++;
  slopeVect = *slopeVectPtr++;
  for (int i = 0; i < (NSAMP/8); i++)
    chess_prepare_for_pipelining chess_loop_range((NSAMP/8), )
  {
    acc = ::aie::mac(::aie::from_vector<accfloat>(offsetVect), dataVect, slopeVect);
    *outPtr++ = acc.template to_vector<float>();
    dataVect = *inPtr++;
    offsetVect = *offsetVectPtr++;
    slopeVect = *slopeVectPtr++;
  }
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

template<unsigned REPEAT,unsigned NSAMP>
inline __attribute__((noinline))
void interp1<REPEAT,NSAMP>::run( input_async_buffer<float>& lut_i, input_buffer<float>& domain_i,
                                 output_buffer<float>& interp_o )
{
  // Copy LUT if new contents (once per radar pulse):
  if (state == 0) {
    lut_i.acquire();
    copy_lut(lut_i);
    state = 1; // Move to run
  }

  // Perform compute workload:
  compute_workload(lut_i,domain_i,interp_o);

  // Update state:
  if ( count == REPEAT-1 ) {
    lut_i.release();
    state = 0;
    count = 0;
  }
  else {
    count++;
  }
}


