//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

// This version of the design is hard-coded to support the following channelizer system parameters:
// --> M  = 16          (# of channels)
// --> Fs = 10.5 Gsps   (input sampling rate)
// --> P  = 8           (Numerator of oversampling ratio)
// --> Q  = 7           (Denominator of oversampling ratio)
// --> K = 8            (# of taps per phase)
//
// --> This model implements a single tile design capable of services 4 FIR in the filter bank

template<unsigned pattern,unsigned NSAMP>
class polyphase_fir {
public:
  typedef cint16 TT_DATA;
  typedef int16  TT_COEFF;
  // System parameters (hard-coded):
  static constexpr unsigned MCHAN = 16; // Total # of channels in polyphase filterbank
  static constexpr unsigned HOP = 14;   // Matches downshift of circular buffer, valid for P/Q = 8/7
  static constexpr unsigned TAPS_PER_PHASE = 8;
  static constexpr unsigned NUM_POLY = 8;
  // Implementation parameters:
  static constexpr unsigned BIT_SHIFT = 15;

  // Check parameters:
  static_assert( (NSAMP % 4) == 0, "NSAMP must be a multiple of 4");

private:

  // Each filter processes 8 unique polyphases over time:
  alignas(32) TT_COEFF (&taps0)[NUM_POLY*TAPS_PER_PHASE];
  alignas(32) TT_COEFF (&taps1)[NUM_POLY*TAPS_PER_PHASE];

  // Filter state:
  alignas(32) aie::vector<TT_DATA,TAPS_PER_PHASE> state0;
  alignas(32) aie::vector<TT_DATA,TAPS_PER_PHASE> state1;

public:
  // Constructor:
  polyphase_fir( TT_COEFF (&taps0_i)[NUM_POLY*TAPS_PER_PHASE],
                 TT_COEFF (&taps1_i)[NUM_POLY*TAPS_PER_PHASE] );

  // Run (Intrinsics Version):
  void run_i( input_stream<TT_DATA>* __restrict sig_i,
              output_stream<TT_DATA>* __restrict sig_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( polyphase_fir::run_i );
    REGISTER_PARAMETER( taps0 );
    REGISTER_PARAMETER( taps1 );
  }
};

// ------------------------------------------------------------
// Compute Patterns
// ------------------------------------------------------------

#define RESET_COEFF\
  ptr0 = (v16int16*)taps0;\
  ptr1 = (v16int16*)taps1


// For this pattern, sample 'd0' is repeated --> discard 2nd one
// | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 |
// | d3 | d2 | d0 | s0 | s1 | s2 | s3 | s4 | s5 | s6 | s7 |
#define REPEATED_D0(BUFF,COEFF,PTR,ACC)                         \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  discard = readincr(sig_i);                                    \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  COEFF = *PTR++;                                               \
  ACC = mul4(BUFF,0,0x0122,1,COEFF,0,0xC840,1);                 \
  COEFF = *PTR++;                                               \
  ACC = mac4(ACC,BUFF,2,0x2344,1,COEFF,0,0xC840,1);             \
  WRITEINCRW(WMS_rsrc1,sig_o,as_v4int32(srs(ACC,BIT_SHIFT)))


  // For this pattern, sample 'd1' is repeated --> discard 2nd one
  // | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 |
  // | d2 | d1 | d0 | s0 | s1 | s2 | s3 | s4 | s5 | s6 | s7 |
#define REPEATED_D1(BUFF,COEFF,PTR,ACC)                         \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  discard = readincr(sig_i);                                    \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  COEFF = *PTR++;                                               \
  ACC = mul4(BUFF,0,0x0112,1,COEFF,0,0xC840,1);                 \
  COEFF = *PTR++;                                               \
  ACC = mac4(ACC,BUFF,2,0x2334,1,COEFF,0,0xC840,1);             \
  WRITEINCRW(WMS_rsrc1,sig_o,as_v4int32(srs(ACC,BIT_SHIFT)))


  // For this pattern, sample 'd2' is repeated --> discard 2nd one
  // | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 |
  // | d2 | d1 | d0 | s0 | s1 | s2 | s3 | s4 | s5 | s6 | s7 |
#define REPEATED_D2(BUFF,COEFF,PTR,ACC)                         \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  discard = readincr(sig_i);                                    \
  COEFF = *PTR++;                                               \
  ACC = mul4(BUFF,0,0x0012,1,COEFF,0,0xC840,1);                 \
  COEFF = *PTR++;                                               \
  ACC = mac4(ACC,BUFF,2,0x2234,1,COEFF,0,0xC840,1);             \
  WRITEINCRW(WMS_rsrc1,sig_o,as_v4int32(srs(ACC,BIT_SHIFT)))


  // For this pattern, sample 'd0' is repeated --> discard it (this 'd0' is 's0' from last time)
  // | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 |
  // | d3 | d2 | d1 | s0 | s1 | s2 | s3 | s4 | s5 | s6 | s7 |
#define HOLDOVER_D0(BUFF,COEFF,PTR,ACC)                         \
  discard = readincr(sig_i);                                    \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  COEFF = *PTR++;                                               \
  ACC = mul4(BUFF,0,0x0123,1,COEFF,0,0xC840,1);                 \
  COEFF = *PTR++;                                               \
  ACC = mac4(ACC,BUFF,3,0x1234,1,COEFF,0,0xC840,1);             \
  WRITEINCRW(WMS_rsrc1,sig_o,as_v4int32(srs(ACC,BIT_SHIFT)))


  // For Compute 1, its a normal FIR
  // | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 |
  // | d3 | d2 | d1 | d0 | s0 | s1 | s2 | s3 | s4 | s5 | s6 |
#define NORMAL_FIR(BUFF,COEFF,PTR,ACC)                          \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  BUFF = shft_elem(BUFF,readincr(sig_i));                       \
  COEFF = *PTR++;                                               \
  ACC = mul4(BUFF,0,0x0123,1,COEFF,0,0xC840,1);                 \
  COEFF = *PTR++;                                               \
  ACC = mac4(ACC,BUFF,4,0x0123,1,COEFF,0,0xC840,1);             \
  WRITEINCRW(WMS_rsrc1,sig_o,as_v4int32(srs(ACC,BIT_SHIFT)))
