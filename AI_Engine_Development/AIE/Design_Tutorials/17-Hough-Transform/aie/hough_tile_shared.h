//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

namespace hough {

  // ------------------------------------------------------------
  // Clear histogram counts:
  // ------------------------------------------------------------

  template <int RHO_DEPTH>
  inline void clear_counts( TT_COUNT (&COUNTS)[THETA_NUM*RHO_DEPTH] )
  {
    auto it = aie::begin_vector<16>( (TT_COUNT*) COUNTS );
    aie::vector<TT_COUNT,16> zero = aie::zeros<TT_COUNT,16>();
    for (int ii=0; ii < RHO_DEPTH/16; ii++)
      chess_prepare_for_pipelining
        chess_loop_range(64,)
      {
        *it = zero;
      }
  }

  // ------------------------------------------------------------
  // Update Counts
  // ------------------------------------------------------------

  template <int COUNT_NUM>
  inline void update_countsA( TT_COUNT (&COUNTS)[COUNT_NUM],
                              aie::vector<TT_COUNT,16>& rho, aie::vector<TT_COUNT,8>& pixels )
  {
    COUNTS[rho[ 0]] += pixels[0];
    COUNTS[rho[ 1]] += pixels[0];
    COUNTS[rho[ 2]] += pixels[0];
    COUNTS[rho[ 3]] += pixels[0];
    COUNTS[rho[ 4]] += pixels[1];
    COUNTS[rho[ 5]] += pixels[1];
    COUNTS[rho[ 6]] += pixels[1];
    COUNTS[rho[ 7]] += pixels[1];
    COUNTS[rho[ 8]] += pixels[2];
    COUNTS[rho[ 9]] += pixels[2];
    COUNTS[rho[10]] += pixels[2];
    COUNTS[rho[11]] += pixels[2];
    COUNTS[rho[12]] += pixels[3];
    COUNTS[rho[13]] += pixels[3];
    COUNTS[rho[14]] += pixels[3];
    COUNTS[rho[15]] += pixels[3];
  }

  template <int COUNT_NUM>
  inline void update_countsB( TT_COUNT (&COUNTS)[COUNT_NUM],
                              aie::vector<TT_COUNT,16>& rho, aie::vector<TT_COUNT,8>& pixels )
  {
    COUNTS[rho[ 0]] += pixels[4];
    COUNTS[rho[ 1]] += pixels[4];
    COUNTS[rho[ 2]] += pixels[4];
    COUNTS[rho[ 3]] += pixels[4];
    COUNTS[rho[ 4]] += pixels[5];
    COUNTS[rho[ 5]] += pixels[5];
    COUNTS[rho[ 6]] += pixels[5];
    COUNTS[rho[ 7]] += pixels[5];
    COUNTS[rho[ 8]] += pixels[6];
    COUNTS[rho[ 9]] += pixels[6];
    COUNTS[rho[10]] += pixels[6];
    COUNTS[rho[11]] += pixels[6];
    COUNTS[rho[12]] += pixels[7];
    COUNTS[rho[13]] += pixels[7];
    COUNTS[rho[14]] += pixels[7];
    COUNTS[rho[15]] += pixels[7];
  }

  // ------------------------------------------------------------
  // Theta Computation
  // ------------------------------------------------------------

  template<int RR, int CC, int RHO_MAX, int COUNT_NUM, int THETA_NUM>
  void theta_compute( TT_THETA (&SINCOS)[THETA_NUM], TT_COUNT (&COUNTS)[COUNT_NUM], TT_COUNT (&SCRATCH)[THETA_NUM*CC],
                      input_stream<TT_PIXEL>* p_i )
  {
    // Data fill for vector (x,y) increments and address offsets:
    // --> Format: { x0,x1,x2,x3,x4,x5,x6,x7,y0,y1,y2,y3,y4,y5,y6,y7}
    TT_COUNT         incX[16] = { 8,8,8,8,8,8,8,8,0,0,0,0,0,0,0,0 };
    TT_COUNT         incY[16] = { 0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1 };
    TT_COUNT        initX[ 8] = { 0,1,2,3,4,5,6,7 };
    TT_COUNT        initY[ 8] = { 0,0,0,0,0,0,0,0 };
    TT_COUNT theta_offset[16] = { 0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3 };
    TT_COUNT       scale4[16] = { 4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; // Scale 'rho' by 4

    // Declare vectors:
    aie::vector<TT_THETA,16> chess_storage(xa)  theta;
    aie::vector<TT_COUNT,32> chess_storage(xb)  rho_addr;
    aie::vector<TT_COUNT,16> chess_storage(wc0) xy_vec;
    aie::vector<TT_COUNT,16> chess_storage(wc1) rho1;
    aie::vector<TT_COUNT,16> chess_storage(wd0) rho2;
    aie::vector<TT_COUNT,16> chess_storage(wd1) incxy;
    aie::accum<TT_ACC,16>                       acc1;
    aie::accum<TT_ACC,16>                       acc2;
    aie::accum<TT_ACC,16>                       acc_max;          // Stores 'rho_max' for adding offset
    aie::accum<TT_ACC,16>                       acc_theta_offset; // Stores 'theta_offset' for addressing histogram
    aie::vector<TT_COUNT,8>                     pixels;

    // Read & replicate theta values from SINCOS LUT:
    // --> Format: { r0,i0,r1,i1,r2,i2,r3,i3, r0,i0,r1,i1,r2,i2,r3,i3, r0,i0,r1,i1,r2,i2,r3,i3, r0,i0,r1,i1,r2,i2,r3,i3  }
    theta.insert(0,aie::load_v<THETA_NUM>(SINCOS));
    theta.insert(1,aie::load_v<THETA_NUM>(SINCOS));
    theta.insert(2,aie::load_v<THETA_NUM>(SINCOS));
    theta.insert(3,aie::load_v<THETA_NUM>(SINCOS));

    // Initialize;
    xy_vec.insert(0,aie::load_v<8>(initX));
    xy_vec.insert(1,aie::load_v<8>(initY));
    acc_max.from_vector( aie::broadcast<TT_COUNT,16>(RHO_MAX), SHIFT );
    acc_theta_offset.from_vector( aie::load_v<16>(theta_offset), 0 ); // Shift is zero
    incxy = aie::load_v<16>(scale4);

    // Loop over (X,Y) image coordinates:
    int xs = 0, zs = 0, zstep = 8;
    unsigned int xoL = 0x06040200, xoH = 0x0E0C0A08, xsq = 0x3210;
    unsigned int zoLa = 0x11110000, zoHa = 0x33332222, zoLb = 0x55554444, zoHb = 0x77776666;
    for (int yy = 0; yy < RR; yy++)
      chess_unroll_loop(1)
    {
      auto itW = aie::begin_vector<16>(SCRATCH);
      for (int xx = 0, cc=0; xx < CC / 8; xx++) // Each iteration processes 8 image pixels
        chess_prepare_for_pipelining
        chess_loop_range(64,)
      {
        // Vectorized 'rho' compute (16 lanes at a time):
        acc1 = mac16(acc_max,as_v32int16(theta.cast_to<TT_COUNT>()),xs,xoL,xoH,xsq,as_v16int16(xy_vec),zs,zoLa,zoHa,zstep);
        *itW++ = acc1.to_vector<TT_COUNT>(SHIFT);

        // Vectorized 'rho' compute (16 lanes at a time):
        acc2 = mac16(acc_max,as_v32int16(theta.cast_to<TT_COUNT>()),xs,xoL,xoH,xsq,as_v16int16(xy_vec),zs,zoLb,zoHb,zstep);
        *itW++ = acc2.to_vector<TT_COUNT>(SHIFT);

        // Increment 'x' for (x,y) for next inner loop iteration:
        xy_vec = aie::add( xy_vec, aie::load_v<16>(incX) );
      } // xx

      chess_separator_scheduler();

      // This loop updates the 2D histogram addresses
      // --> Take 'rho_addr', scale by 4, then introduce theta addresses 0, 1, 2, 3 for each pixel processed:
      auto ipR = aie::begin_restrict_vector<16>(SCRATCH);
      auto ipW = aie::begin_restrict_vector<16>(SCRATCH);
      for (int xx = 0, cc=0; xx < CC / 8; xx++) // Each iteration processes 8 image pixels
        chess_prepare_for_pipelining
        chess_unroll_loop(2)
      {
        rho_addr.insert(0,*ipR++);
        rho_addr.insert(1,*ipR++);
        acc1 = mac16(acc_theta_offset,rho_addr, 0,0x03020100,0x07060504,0x0100,incxy,0,0x0,0x0,1);
        acc2 = mac16(acc_theta_offset,rho_addr,16,0x03020100,0x07060504,0x0100,incxy,0,0x0,0x0,1);
        *ipW++ = acc1.to_vector<TT_COUNT>(0);
        *ipW++ = acc2.to_vector<TT_COUNT>(0);
      }

      chess_separator_scheduler();

      auto itR = aie::begin_vector<16>(SCRATCH);
      for (int xx = 0, cc=0; xx < CC / 8; xx++) {
        pixels = readincr_v8( p_i );
        update_countsA( COUNTS, *itR++, pixels );
        update_countsB( COUNTS, *itR++, pixels );
      }
      // Reload 'y' and reset 'x' for (x,y) for new image line:
      xy_vec = aie::add( xy_vec, aie::load_v<16>(incY) );
      xy_vec.insert(0,aie::load_v<8>(initX));
    } // yy
  }

}; // hough::
