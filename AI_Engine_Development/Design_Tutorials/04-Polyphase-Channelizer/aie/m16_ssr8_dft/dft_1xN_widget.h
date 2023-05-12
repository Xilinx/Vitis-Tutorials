//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

// ============================================================
// Input Tile
// ============================================================

// Performs two (1x2) x (2x4) OPS performed in as many cycles

template<class TT_DATA,class TT_COEFF,class TT_ACC,unsigned NSAMP>
class dft_1xN_input {
private:
  // Filter tap weights:
  alignas(16) TT_COEFF (&coeff0)[8];
  alignas(16) TT_COEFF (&coeff1)[8];

public:

  // Check parameters:
  static_assert( (NSAMP % 8) == 0, "NSAMP must be a multiple of 8");

  // Constructor:
  dft_1xN_input( TT_COEFF (&coeff0_i)[8], TT_COEFF (&coeff1_i)[8] );

  // Run:
  void run_input( input_stream<TT_DATA>* __restrict sig0_i,
                  input_stream<TT_DATA>* __restrict sig1_i,
                  output_stream<TT_ACC>* __restrict acc_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dft_1xN_input::run_input );
    REGISTER_PARAMETER( coeff0 );
    REGISTER_PARAMETER( coeff1 );
  }
};

// ============================================================
// Middle Tile
// ============================================================

// Performs two (1x2) x (2x4) OPS performed in as many cycles

template<class TT_DATA,class TT_COEFF,class TT_ACC,unsigned NSAMP>
class dft_1xN_middle {
private:
  // Filter tap weights:
  alignas(16) TT_COEFF (&coeff0)[8];
  alignas(16) TT_COEFF (&coeff1)[8];

public:
  // Constructor:
  dft_1xN_middle( TT_COEFF (&coeff0_i)[8], TT_COEFF (&coeff1_i)[8] );

  // Run:
  void run_middle( input_stream<TT_DATA>* __restrict sig0_i,
                   input_stream<TT_DATA>* __restrict sig1_i,
                   input_stream<TT_ACC>*  __restrict acc_i,
                   output_stream<TT_ACC>* __restrict acc_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dft_1xN_middle::run_middle );
    REGISTER_PARAMETER( coeff0 );
    REGISTER_PARAMETER( coeff1 );
  }
};

// ============================================================
// Output Tile
// ============================================================

// Performs two (1x2) x (2x4) OPS performed in as many cycles

template<class TT_DATA,class TT_COEFF,class TT_ACC,unsigned NSAMP>
class dft_1xN_output {
private:
  static constexpr unsigned BIT_SHIFT = 15; // Assumes Q.15 x Q.15 (twiddles x data)
  // Filter tap weights:
  alignas(16) TT_COEFF (&coeff0)[8];
  alignas(16) TT_COEFF (&coeff1)[8];

public:
  // Constructor:
  dft_1xN_output( TT_COEFF (&coeff0_i)[8], TT_COEFF (&coeff1_i)[8] );

  // Run:
  void run_output( input_stream<TT_DATA>*  __restrict sig0_i,
                   input_stream<TT_DATA>*  __restrict sig1_i,
                   input_stream<TT_ACC>*   __restrict acc_i,
                   output_stream<TT_DATA>* __restrict sig0_o,
                   output_stream<TT_DATA>* __restrict sig1_o );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( dft_1xN_output::run_output );
    REGISTER_PARAMETER( coeff0 );
    REGISTER_PARAMETER( coeff1 );
  }
};

