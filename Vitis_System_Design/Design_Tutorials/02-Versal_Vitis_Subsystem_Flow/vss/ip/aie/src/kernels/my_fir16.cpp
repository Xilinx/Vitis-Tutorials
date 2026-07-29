//
// Copyright (C) 2023-2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
// Author Derek Hagen

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include "kernels.hpp"

// Parameters for the sliding multiplication operations:
// With cint16 data and int16 coefficients, the AIE tile can compute 16 MUL/MAC operations per clock cycle
// Considering only the inner loop, this means on average we can compute the result of a 16 tap FIR filter per clock cycle.
// To translate this into vector operation, we compute 8 results over 8 clock cycles.
// The parameters below is used to set sliding mul op either to a 8 lane single cycle operation computing 2 taps per clock cycle (using P=2)
// or as a 4 clock cycle operation, computing 8 taps in 4 clock cycle (using P8=8).
// The design is set to use the 4 clock cycle operation to demonstrate how AIE API reduces the number of lines to code.

#define L   8
#define P   2
#define P8  8
#define CS  1
#define DSX 1
#define DSY 1
#define N_UNROLL 4

// Constructor
template <typename TT_DATA, size_t BUF_SIZE, size_t MAR_SIZE>
my_fir16<TT_DATA, BUF_SIZE, MAR_SIZE>::my_fir16()
{
}
// Run:
template <typename TT_DATA, size_t BUF_SIZE, size_t MAR_SIZE>
void __attribute__ ((noinline)) my_fir16<TT_DATA, BUF_SIZE, MAR_SIZE>::run(const int16 (&coeffs)[16], adf::input_buffer<TT_DATA, adf::extents<BUF_SIZE>, adf::margin<MAR_SIZE>> & __restrict sig_i, adf::output_buffer<TT_DATA, adf::extents<BUF_SIZE>> & __restrict sig_o )
{
  const int shift = 14; //MOVER_SHIFT ;
  int my_size = sig_o.size();

  // All 16 taps fit into a single 256-bit vector register.
  // This means we only need to load the coeffs once each kernel invocation.
  //aie::vector<int16, 16> chess_storage(wc0) coe = aie::load_v<16>(coeffs);  // chess directive for wc0 only for AIE, not AIE-ML.
  aie::vector<int16, 16> coe = aie::load_v<16>(coeffs);
  //aie::locate_in_register<4>(coe);  // Bind register
  // Accumulator register for storing intermediate results
  aie::accum<cacc48, 8> acc;
  // Use 1024-bit vector register as a circular buffer for input data.
  // The permutation network is configured by the sliding mul op to advance the data.
  // The data is replaced in the vector register every 4th clock cycle ahead of being used by the sliding mul op.
  // For the buffer to nicely return to same starting point, the operations is unrolled manually 4 times (N_UNROLL).
  aie::vector<cint16, 32> dvec;
  //aie::locate_in_register<0>(dvec);  // Bind register

  //  Alternative method for accessing parts of the 1024-bit vector register using pointers and alias.
  //  Not used here
  //  aie::vector<cint16, 8> dvec0;
  //  aie::vector<cint16, 8> dvec1;
  //  aie::vector<cint16, 8> dvec2;
  //  aie::vector<cint16, 8> dvec3;
  //  #define dvec aie::concat(dvec0, dvec1, dvec2, dvec3)

  // Declare iterators to advance input and output data
  auto in_iter =  aie::begin_vector<8>(sig_i);
  auto out_iter =  aie::begin_vector<8>(sig_o);

  // Preload vector regs
  dvec.insert(0, *in_iter++);
  dvec.insert(1, *in_iter++);

  for (int i = 0; i < my_size/L/N_UNROLL; i++)
  chess_prepare_for_pipelining
  {
    acc  = aie::sliding_mul_ops<L, P8, CS, DSX, DSY, int16, TT_DATA, cacc48>::mul(coe, 0,  dvec, 1);  // 4 clock cycle operation
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mul(     coe, 0,  dvec, 1); // 1 cc operations
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 2,  dvec, 3);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 4,  dvec, 5);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 6,  dvec, 7);

    dvec.insert(2, *in_iter++); // Loading next set of data into vector register
    //dvec2 = *in_iter++;
    acc  = aie::sliding_mul_ops<L, P8, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 8,  dvec, 9);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 8,  dvec, 9);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 10, dvec, 11);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 12, dvec, 13);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 14, dvec, 15);
    *out_iter++ = aie::to_vector<cint16>(acc, shift); // Writing 8 results.

    acc  = aie::sliding_mul_ops<L, P8, CS, DSX, DSY, int16, TT_DATA, cacc48>::mul(coe, 0,  dvec, 9);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mul(     coe, 0,  dvec, 9);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 2,  dvec, 11);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 4,  dvec, 13);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 6,  dvec, 15);

    dvec.insert(3, *in_iter++);
    //dvec3 = *in_iter++;
    acc  = aie::sliding_mul_ops<L, P8, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 8,  dvec, 17);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 8,  dvec, 17);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 10, dvec, 19);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 12, dvec, 21);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 14, dvec, 23);
    *out_iter++ = aie::to_vector<cint16>(acc, shift);

    acc  = aie::sliding_mul_ops<L, P8, CS, DSX, DSY, int16, TT_DATA, cacc48>::mul(coe, 0,  dvec, 17);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mul(     coe, 0,  dvec, 17);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 2,  dvec, 19);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 4,  dvec, 21);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 6,  dvec, 23);

    dvec.insert(0, *in_iter++);
    //dvec0 = *in_iter++;
    acc  = aie::sliding_mul_ops<L, P8, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 8,  dvec, 25);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 8,  dvec, 25);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 10, dvec, 27);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 12, dvec, 29);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 14, dvec, 31);
    *out_iter++ = aie::to_vector<cint16>(acc, shift);

    acc  = aie::sliding_mul_ops<L, P8, CS, DSX, DSY, int16, TT_DATA, cacc48>::mul(coe, 0,  dvec, 25);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mul(     coe, 0,  dvec, 25);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 2,  dvec, 27);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 4,  dvec, 29);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 6,  dvec, 31);

    dvec.insert(1, *in_iter++);
    //dvec1 = *in_iter++;
    acc  = aie::sliding_mul_ops<L, P8, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 8,  dvec, 1);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 8,  dvec, 1);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 10, dvec, 3);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 12, dvec, 5);
    //acc  = aie::sliding_mul_ops<L, P, CS, DSX, DSY, int16, TT_DATA, cacc48>::mac(acc, coe, 14, dvec, 7);
    *out_iter++ = aie::to_vector<cint16>(acc, shift);
  }
}

