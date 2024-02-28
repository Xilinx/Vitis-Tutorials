//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <adf.h>
#include <aie_api/aie.hpp>

#include "bitonic_fp1024.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

bitonic_fp1024::bitonic_fp1024( float (&data_i)[SIZE] ) : data(data_i)
{
}

// ------------------------------------------------------------
// Load Data
// ------------------------------------------------------------

void __attribute__((noinline))
bitonic_fp1024::load_data( input_stream<float>* sig_i )
{
  auto it = aie::begin_restrict_vector<8>(data);
  aie::vector<float,8> vec;
  for (unsigned ii=0; ii < SIZE/8; ii++) // Expect II=8
    chess_prepare_for_pipelining
    //      chess_unroll_loop(2)
  {
    vec.insert(0,readincr_v<4>(sig_i));
    vec.insert(1,readincr_v<4>(sig_i));
    *it++ = vec;
  }
}

// ------------------------------------------------------------
// Store Data
// ------------------------------------------------------------

void __attribute__((noinline))
bitonic_fp1024::store_data( output_stream<float>* sig_o )
{
  auto it = aie::begin_restrict_vector<8>(data);
  aie::vector<float,8> vec;
  for (unsigned ii=0; ii < SIZE/8; ii++) // Expect II=8
    chess_prepare_for_pipelining
  {
    vec = *it++;
    writeincr(sig_o,vec.extract<4>(0));
    writeincr(sig_o,vec.extract<4>(1));
  }
}

// ------------------------------------------------------------
// Stage 0 All
// ------------------------------------------------------------

void __attribute__((noinline))
bitonic_fp1024::stage0_all( void )
{
  auto itr = aie::begin_restrict_vector<8>(data);
  auto itw = aie::begin_restrict_vector<8>(data);
  aie::vector<float,32> vec;
  aie::vector<float,8> v_top;
  aie::vector<float,8> v_bot;
  aie::vector<float,8> v_mx;
  aie::vector<float,8> v_mn;
  for (unsigned ii=0; ii < SIZE/32; ii++) // Process two groups of 16 samples per loop body
    chess_prepare_for_pipelining
    chess_unroll_loop(1)
  {
    vec.insert(0,*itr++);
    vec.insert(1,*itr++);
    vec.insert(2,*itr++);
    vec.insert(3,*itr++);
    v_top = aie::filter_even(vec.extract<16>(0));
    v_bot = aie::filter_odd(vec.extract<16>(0));
    v_mx = aie::max(v_top,v_bot);
    v_mn = aie::min(v_top,v_bot);
    std::tie(v_mn,v_mx) = aie::interleave_zip(v_mn,v_mx,1);
    vec.insert(0,aie::concat(v_mn,v_mx));
    v_top = aie::filter_even(vec.extract<16>(1));
    v_bot = aie::filter_odd(vec.extract<16>(1));
    v_mx = aie::max(v_top,v_bot);
    v_mn = aie::min(v_top,v_bot);
    std::tie(v_mn,v_mx) = aie::interleave_zip(v_mn,v_mx,1);
    vec.insert(1,aie::concat(v_mn,v_mx));
    *itw++ = vec.extract<8>(0);
    *itw++ = vec.extract<8>(1);
    *itw++ = vec.extract<8>(2);
    *itw++ = vec.extract<8>(3);
  }
}

// ------------------------------------------------------------
// Stage 1a All
// ------------------------------------------------------------

void __attribute__((noinline))
bitonic_fp1024::stage1a_all( void )
{
  static constexpr unsigned BFLY_STAGE1a_TOP_I = 0xDC985410;
  static constexpr unsigned BFLY_STAGE1a_BOT_I = 0xEFAB6723;
  static constexpr unsigned BFLY_STAGE1a_TOP_O = 0xAB328910;
  static constexpr unsigned BFLY_STAGE1a_BOT_O = 0xEF76CD54;
  auto itr = aie::begin_restrict_vector<8>(data);
  auto itw = aie::begin_restrict_vector<8>(data);
  aie::vector<float,32> vec;
  aie::vector<float,8> v_top;
  aie::vector<float,8> v_bot;
  aie::vector<float,8> v_mn;
  aie::vector<float,8> v_mx;
  for (unsigned ii=0; ii < SIZE/32; ii++) // Process two groups of 16 samples per loop body
    chess_prepare_for_pipelining
  {
    vec.insert(0,*itr++);
    vec.insert(1,*itr++);
    vec.insert(2,*itr++);
    vec.insert(3,*itr++);
    vec.insert<16>(0,fpshuffle16(vec.extract<16>(0),0,BFLY_STAGE1a_TOP_I,BFLY_STAGE1a_BOT_I));
    vec.insert<16>(1,fpshuffle16(vec.extract<16>(1),0,BFLY_STAGE1a_TOP_I,BFLY_STAGE1a_BOT_I));
    v_top = vec.extract<8>(0);
    v_bot = vec.extract<8>(1);
    v_mx = aie::max(v_top, v_bot);
    v_mn = aie::min(v_top, v_bot);
    vec.insert(0,aie::concat(v_mn,v_mx));
    v_top = vec.extract<8>(2);
    v_bot = vec.extract<8>(3);
    v_mx = aie::max(v_top, v_bot);
    v_mn = aie::min(v_top, v_bot);
    vec.insert(1,aie::concat(v_mn,v_mx));
    vec.insert<16>(0,fpshuffle16(vec.extract<16>(0),0,BFLY_STAGE1a_TOP_O,BFLY_STAGE1a_BOT_O));
    vec.insert<16>(1,fpshuffle16(vec.extract<16>(1),0,BFLY_STAGE1a_TOP_O,BFLY_STAGE1a_BOT_O));
    *itw++ = vec.extract<8>(0);
    *itw++ = vec.extract<8>(1);
    *itw++ = vec.extract<8>(2);
    *itw++ = vec.extract<8>(3);
  }
}

// ------------------------------------------------------------
// Stage 1 All
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp1024::stage1_all( void )
{
  stage1a_all();
  stage0_all();
}

// ------------------------------------------------------------
// Stage 2a
// ------------------------------------------------------------

void __attribute__((noinline))
bitonic_fp1024::stage2a_all( void )
{
  static constexpr unsigned BFLY_STAGE2a_TOP_I = 0xBA983210;
  static constexpr unsigned BFLY_STAGE2a_BOT_I = 0xCDEF4567;
  static constexpr unsigned BFLY_STAGE2a_TOP_O = 0x89AB3210;
  static constexpr unsigned BFLY_STAGE2a_BOT_O = 0xCDEF7654;
  auto itr = aie::begin_restrict_vector<8>(data);
  auto itw = aie::begin_restrict_vector<8>(data);
  aie::vector<float,32> vec;
  aie::vector<float,8>  v_top;
  aie::vector<float,8>  v_bot;
  aie::vector<float,8>  v_mx;
  aie::vector<float,8>  v_mn;
  for (unsigned ii=0; ii < SIZE/32; ii++)  // Process two groups of 16 samples per loop body
    chess_prepare_for_pipelining
  {
    vec.insert(0,*itr++);
    vec.insert(1,*itr++);
    vec.insert(2,*itr++);
    vec.insert(3,*itr++);
    vec.insert<16>(0,fpshuffle16(vec.extract<16>(0),0,BFLY_STAGE2a_TOP_I,BFLY_STAGE2a_BOT_I));
    vec.insert<16>(1,fpshuffle16(vec.extract<16>(1),0,BFLY_STAGE2a_TOP_I,BFLY_STAGE2a_BOT_I));
    v_top = vec.extract<8>(0);
    v_bot = vec.extract<8>(1);
    v_mx = aie::max(v_top, v_bot);
    v_mn = aie::min(v_top, v_bot);
    vec.insert(0,aie::concat(v_mn,v_mx));
    v_top = vec.extract<8>(2);
    v_bot = vec.extract<8>(3);
    v_mx = aie::max(v_top, v_bot);
    v_mn = aie::min(v_top, v_bot);
    vec.insert(1,aie::concat(v_mn,v_mx));
    vec.insert<16>(0,fpshuffle16(vec.extract<16>(0),0,BFLY_STAGE2a_TOP_O,BFLY_STAGE2a_BOT_O));
    vec.insert<16>(1,fpshuffle16(vec.extract<16>(1),0,BFLY_STAGE2a_TOP_O,BFLY_STAGE2a_BOT_O));
    *itw++ = vec.extract<8>(0);
    *itw++ = vec.extract<8>(1);
    *itw++ = vec.extract<8>(2);
    *itw++ = vec.extract<8>(3);
  }
}

// ------------------------------------------------------------
// Stage 2b
// ------------------------------------------------------------

void __attribute__((noinline))
bitonic_fp1024::stage2b_all( void )
{
  static constexpr unsigned BFLY_STAGE2b_TOP_I = 0xDC985410;
  static constexpr unsigned BFLY_STAGE2b_BOT_I = 0xFEBA7632;
  static constexpr unsigned BFLY_STAGE2b_TOP_O = 0xBA329810;
  static constexpr unsigned BFLY_STAGE2b_BOT_O = 0xFE76DC54;
  auto itr = aie::begin_restrict_vector<8>(data);
  auto itw = aie::begin_restrict_vector<8>(data);
  aie::vector<float,32> vec;
  aie::vector<float,8>  v_top;
  aie::vector<float,8>  v_bot;
  aie::vector<float,8>  v_mx;
  aie::vector<float,8>  v_mn;
  for (unsigned ii=0; ii < SIZE/32; ii++) // Process two groups of 16 samples per loop body
    chess_prepare_for_pipelining
  {
    vec.insert(0,*itr++);
    vec.insert(1,*itr++);
    vec.insert(2,*itr++);
    vec.insert(3,*itr++);
    vec.insert<16>(0,fpshuffle16(vec.extract<16>(0),0,BFLY_STAGE2b_TOP_I,BFLY_STAGE2b_BOT_I));
    vec.insert<16>(1,fpshuffle16(vec.extract<16>(1),0,BFLY_STAGE2b_TOP_I,BFLY_STAGE2b_BOT_I));
    v_top = vec.extract<8>(0);
    v_bot = vec.extract<8>(1);
    v_mx = aie::max(v_top, v_bot);
    v_mn = aie::min(v_top, v_bot);
    vec.insert(0,aie::concat(v_mn,v_mx));
    v_top = vec.extract<8>(2);
    v_bot = vec.extract<8>(3);
    v_mx = aie::max(v_top, v_bot);
    v_mn = aie::min(v_top, v_bot);
    vec.insert(1,aie::concat(v_mn,v_mx));
    vec.insert<16>(0,fpshuffle16(vec.extract<16>(0),0,BFLY_STAGE2b_TOP_O,BFLY_STAGE2b_BOT_O));
    vec.insert<16>(1,fpshuffle16(vec.extract<16>(1),0,BFLY_STAGE2b_TOP_O,BFLY_STAGE2b_BOT_O));
    *itw++ = vec.extract<8>(0);
    *itw++ = vec.extract<8>(1);
    *itw++ = vec.extract<8>(2);
    *itw++ = vec.extract<8>(3);
  }
}

// ------------------------------------------------------------
// Stage 2
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp1024::stage2_all( void )
{
  stage2a_all();
  stage2b_all();
  stage0_all();
}

// ------------------------------------------------------------
// Stage 3a All
// ------------------------------------------------------------

void __attribute__((noinline))
bitonic_fp1024::stage3a_all( void )
{
  auto itr = aie::begin_restrict_vector<8>(data);
  auto itw = aie::begin_restrict_vector<8>(data);
  for (unsigned ii=0; ii < SIZE/32; ii++) // Process two groups of 16 samples per loop body
    chess_prepare_for_pipelining
  {
    aie::vector<float,8> v0_top = *itr++;
    aie::vector<float,8> v0_bot = *itr++;
    aie::vector<float,8> v1_top = *itr++;
    aie::vector<float,8> v1_bot = *itr++;
    aie::vector<float,8> v0_mx = fpmax(v0_top,v0_bot,0,0x01234567);
    aie::vector<float,8> v0_mn = fpmin(v0_top,v0_bot,0,0x01234567);
    aie::vector<float,8> v1_mx = fpmax(v1_top,v1_bot,0,0x01234567);
    aie::vector<float,8> v1_mn = fpmin(v1_top,v1_bot,0,0x01234567);
    *itw++ = v0_mn;
    *itw++ = v0_mx; // Algorithm works whether or not we use aie::reverse() here -- Omit to save cycles
    *itw++ = v1_mn;
    *itw++ = v1_mx; // Algorithm works whether or not we use aie::reverse() here -- Omit to save cycles
  }
}

// ------------------------------------------------------------
// Stage 3b All
// ------------------------------------------------------------

void __attribute__((noinline))
bitonic_fp1024::stage3b_all( void )
{
  static constexpr unsigned BFLY_STAGE3b_TOP_I = 0xBA983210;
  static constexpr unsigned BFLY_STAGE3b_BOT_I = 0xFEDC7654;
  static constexpr unsigned BFLY_STAGE3b_TOP_O = 0xBA983210;
  static constexpr unsigned BFLY_STAGE3b_BOT_O = 0xFEDC7654;
  auto itr = aie::begin_restrict_vector<8>(data);
  auto itw = aie::begin_restrict_vector<8>(data);
  aie::vector<float,32> vec;
  aie::vector<float,8>  v_top;
  aie::vector<float,8>  v_bot;
  aie::vector<float,8>  v_mx;
  aie::vector<float,8>  v_mn;
  for (unsigned ii=0; ii < SIZE/32; ii++) // Process two groups of 16 samples per loop body
    chess_prepare_for_pipelining
  {
    vec.insert(0,*itr++);
    vec.insert(1,*itr++);
    vec.insert(2,*itr++);
    vec.insert(3,*itr++);
    vec.insert<16>(0,fpshuffle16(vec.extract<16>(0),0,BFLY_STAGE3b_TOP_I,BFLY_STAGE3b_BOT_I));
    vec.insert<16>(1,fpshuffle16(vec.extract<16>(1),0,BFLY_STAGE3b_TOP_I,BFLY_STAGE3b_BOT_I));
    v_top = vec.extract<8>(0);
    v_bot = vec.extract<8>(1);
    v_mx = aie::max(v_top, v_bot);
    v_mn = aie::min(v_top, v_bot);
    vec.insert(0,aie::concat(v_mn,v_mx));
    v_top = vec.extract<8>(2);
    v_bot = vec.extract<8>(3);
    v_mx = aie::max(v_top, v_bot);
    v_mn = aie::min(v_top, v_bot);
    vec.insert(1,aie::concat(v_mn,v_mx));
    vec.insert<16>(0,fpshuffle16(vec.extract<16>(0),0,BFLY_STAGE3b_TOP_O,BFLY_STAGE3b_BOT_O));
    vec.insert<16>(1,fpshuffle16(vec.extract<16>(1),0,BFLY_STAGE3b_TOP_O,BFLY_STAGE3b_BOT_O));
    *itw++ = vec.extract<8>(0);
    *itw++ = vec.extract<8>(1);
    *itw++ = vec.extract<8>(2);
    *itw++ = vec.extract<8>(3);
  }
}

// ------------------------------------------------------------
// Stage 3 All
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp1024::stage3_all( void )
{
  stage3a_all();
  stage3b_all();
  stage2b_all();
  stage0_all();
}

// ------------------------------------------------------------
// Stage 4 All
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp1024::stage4_all( void )
{
  stageY<32,32,2>();
  stageB<64, 8,1>();
  stage3b_all();
  stage2b_all();
  stage0_all();
}

// ------------------------------------------------------------
// Stage 5 All
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp1024::stage5_all( void )
{
  stageY<16,64,4>();
  stageB<32,16,2>();
  stageB<64, 8,1>();
  stage3b_all();
  stage2b_all();
  stage0_all();
}

// ------------------------------------------------------------
// Stage 6 All
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp1024::stage6_all( void )
{
  stageY< 8,128,8>();
  stageB<16, 32,4>();
  stageB<32, 16,2>();
  stageB<64,  8,1>();
  stage3b_all();
  stage2b_all();
  stage0_all();
}

// ------------------------------------------------------------
// Stage 7 All
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp1024::stage7_all( void )
{
  stageY< 4,256,16>();
  stageB< 8, 64, 8>();
  stageB<16, 32, 4>();
  stageB<32, 16, 2>();
  stageB<64,  8, 1>();
  stage3b_all();
  stage2b_all();
  stage0_all();
}

// ------------------------------------------------------------
// Stage 8 All
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp1024::stage8_all( void )
{
  stageY< 2, 512, 32>();
  stageB< 4, 128, 16>();
  stageB< 8,  64,  8>();
  stageB<16,  32,  4>();
  stageB<32,  16,  2>();
  stageB<64,   8,  1>();
  stage3b_all();
  stage2b_all();
  stage0_all();
}

// ------------------------------------------------------------
// Stage 9 All
// ------------------------------------------------------------

void __attribute__((always_inline))
bitonic_fp1024::stage9_all( void )
{
  stageY< 1, 1024, 64>();
  stageB< 2,  256, 32>();
  stageB< 4,  128, 16>();
  stageB< 8,   64,  8>();
  stageB<16,   32,  4>();
  stageB<32,   16,  2>();
  stageB<64,    8,  1>();
  stage3b_all();
  stage2b_all();
  stage0_all();
}

// ------------------------------------------------------------
// Stage Yellow
// ------------------------------------------------------------

// Group: 32, Span: 32, Iter: 2 (Example)
// --> There are 32 'groups' of wide-span butterflies to process
// --> Each group of butterflies has a maximum 'span' of 32 samples
// --> We process 'iter' operations of 16 samples each before hopping to the next 'group'
// --> Should have 'group x span x iter' = 1024 to process all samples
// --> The 'top' group address should hop by 'span' after all 'iter' are complete for that group
// --> The 'bot' group address should also hop by 'span'.
// --> Within each group, the 'top butterfly' address should advance   by 8 samples
// --> Within each group, the 'bot butterfly' address should decrement by 8 samples

template<unsigned GROUP,unsigned SPAN,unsigned ITER>
void __attribute__((noinline))
bitonic_fp1024::stageY(void)
{
  float* __restrict gTop = &data[0];
  float* __restrict gBot = &data[SPAN];
  float* __restrict pTop = gTop;
  float* __restrict pBot = gBot - 8;
  // printf("GROUP: %d   SPAN: %d  ITER: %d\n",GROUP,SPAN,ITER);
  for (unsigned ii=0, jj=0; ii < GROUP*ITER; ii++)
    chess_prepare_for_pipelining
    {
      // printf("ii: %d  jj: %d\n",ii,jj);
      // ------------------------------------------------------------
      aie::accum<accfloat,8> v_top; v_top.from_vector(aie::load_v<8>(pTop));
      aie::vector<float,8>   v_bot = aie::load_v<8>(pBot);
      aie::vector<float,8>   v_mx = fpmax(v_top,v_bot,0,0x01234567);
      aie::vector<float,8>   v_mn = fpmin(v_top,v_bot,0,0x01234567);
      v_mn.store(pTop);
      v_mx.store(pBot); // Algorithm works whether or not we use aie::reverse() here -- Omit to save cycles
      // ------------------------------------------------------------
      if (jj==ITER-1) {
        // Time to hop to next group:
        pTop = gTop + SPAN;
        pBot = gBot + SPAN - 8;
        gTop = gTop + SPAN;
        gBot = gBot + SPAN;
        jj=0;
      }
      else {
        // Next set of 8 butterflies:
        pTop = pTop + 8;
        pBot = pBot - 8;
        jj++;
      }
    }
}

// ------------------------------------------------------------
// Stage Blue
// ------------------------------------------------------------

// Group: 32, Span: 16, Iter: 2 (Example)
// --> There are 32 'groups' of wide-span butterflies to process
// --> Each group of butterflies has a maximum 'span' of 16 samples
// --> We process 'iter' operations of 16 samples each before hopping to the next 'group'
// --> Should have 'group x span x iter' = 1024 to process all samples
// --> The 'top' group address should hop by '2 x span' after all 'iter' are complete for that group
// --> The 'bot' group address should also hop by '2 x span'.
// --> Within each group, the 'top butterfly' address should advance by 8 samples
// --> Within each group, the 'bot butterfly' address should advance by 8 samples


template<unsigned GROUP,unsigned SPAN,unsigned ITER>
void __attribute__((noinline))
bitonic_fp1024::stageB(void)
{
  float* __restrict gTop = &data[0];
  float* __restrict gBot = &data[SPAN];
  float* __restrict pTop = gTop;
  float* __restrict pBot = gBot;
  // printf("GROUP: %d   SPAN: %d  ITER: %d\n",GROUP,SPAN,ITER);
  for (unsigned ii=0, jj=0; ii < GROUP*ITER; ii++)
    chess_prepare_for_pipelining
    {
      // printf("ii: %d  jj: %d\n",ii,jj);
      // ------------------------------------------------------------
      aie::vector<float,8>  v_top = aie::load_v<8>(pTop);
      aie::vector<float,8>  v_bot = aie::load_v<8>(pBot);
      aie::vector<float,8>  v_mx  = aie::max(v_top,v_bot);
      aie::vector<float,8>  v_mn  = aie::min(v_top,v_bot);
      v_mn.store(pTop);
      v_mx.store(pBot);
      // ------------------------------------------------------------
      if (jj==ITER-1) {
        // Time to hop to next group:
        gTop = gTop + 2*SPAN;
        gBot = gBot + 2*SPAN;
        pTop = gTop;
        pBot = gBot;
        jj=0;
      }
      else {
        // Next set of 8 butterflies:
        pTop = pTop + 8;
        pBot = pBot + 8;
        jj++;
      }
    }
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void __attribute__((noinline))
bitonic_fp1024::run( input_stream<float>* sig_i, output_stream<float>* sig_o )
{
  load_data(sig_i);

  stage0_all();
  stage1_all();
  stage2_all();
  stage3_all();
  stage4_all();
  stage5_all();
  stage6_all();
  stage7_all();
  stage8_all();
  stage9_all();

  store_data(sig_o);
}



