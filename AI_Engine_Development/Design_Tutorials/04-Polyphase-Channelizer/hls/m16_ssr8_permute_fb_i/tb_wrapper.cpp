//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "m16_ssr8_permute_fb_i.h"
#include <iostream>
#include <fstream>

using namespace std;

using TT_DUT = m16_ssr8_permute_fb_i;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

extern void m16_ssr8_permute_fb_i_wrapper( TT_DUT::TT_STREAM sig_i[TT_DUT::SSR_I],
                                           TT_DUT::TT_STREAM sig_o[TT_DUT::SSR_O] );

// Use this to convert samples from I/O files:
typedef ap_fixed<16,1> TT;


int main( void )
{
  static constexpr unsigned NSAMP = 128;
  TT_DUT::TT_STREAM sig_i[TT_DUT::SSR_I];
  TT_DUT::TT_STREAM sig_o[TT_DUT::SSR_O];

  std::ifstream ss_i, ss_o;
  ss_i.open("sig_i.txt",std::ifstream::in);
  ss_o.open("sig_o.txt",std::ifstream::in);

  if ( ss_i.is_open() == 0 ) cerr << "Cannot open file sig_i.txt" << endl;
  if ( ss_o.is_open() == 0 ) cerr << "Cannot open file sig_o.txt" << endl;

  bool flag = 0;
  for (unsigned ii=0; ii < NSAMP/2; ii++) {
    double val0_re, val0_im, val1_re, val1_im;
    double val2_re, val2_im, val3_re, val3_im;
    for (unsigned xx = 0; xx < TT_DUT::SSR_I; xx++) {
      ss_i >> val0_re >> val0_im >> val1_re >> val1_im >> val2_re >> val2_im >> val3_re >> val3_im;
      TT_DUT::TT_AXI4 val_i = (TT(val3_im).range(15,0),TT(val3_re).range(15,0),TT(val2_im).range(15,0),TT(val2_re).range(15,0),
                               TT(val1_im).range(15,0),TT(val1_re).range(15,0),TT(val0_im).range(15,0),TT(val0_re).range(15,0)); // Note order
      sig_i[xx].write(val_i);
    }

    // Run DUT:
    m16_ssr8_permute_fb_i_wrapper( sig_i, sig_o );

    // Check result:
    for (unsigned xx = 0; xx < TT_DUT::SSR_O; xx++) {
      ss_o >> val0_re >> val0_im >> val1_re >> val1_im >> val2_re >> val2_im >> val3_re >> val3_im;
      TT_DUT::TT_AXI4 val_g = (TT(val3_im).range(15,0),TT(val3_re).range(15,0),TT(val2_im).range(15,0),TT(val2_re).range(15,0),
                               TT(val1_im).range(15,0),TT(val1_re).range(15,0),TT(val0_im).range(15,0),TT(val0_re).range(15,0)); // Note order
      TT_DUT::TT_AXI4 val_a = sig_o[xx].read();
      flag |= ( val_a != val_g ) ? 1 : 0;
      std::cout << val_a << " " << val_g << std::endl;
    }
  } //ii
  ss_i.close();
  ss_o.close();
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

