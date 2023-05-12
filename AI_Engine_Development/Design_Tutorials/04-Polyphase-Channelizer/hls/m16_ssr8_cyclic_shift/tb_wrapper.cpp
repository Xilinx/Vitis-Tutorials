//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Chandra Sekar Balakrishnan

#include "m16_ssr8_cyclic_shift.h"
#include <iostream>
#include <fstream>

using namespace std;

using TT_DUT = m16_ssr8_cyclic_shift;

// ------------------------------------------------------------
// Generate Random Data
// ------------------------------------------------------------

float gen_rand( void )
{
  return( static_cast<float>(rand())/static_cast<float>(RAND_MAX) );
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

extern void m16_ssr8_cyclic_shift_wrapper( TT_DUT::TT_STREAM sig_i[TT_DUT::SSR_I],
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

  if ( ss_i.is_open() == 0 ) { std::cerr << "sig_i.txt does not exist" << std::endl; exit(0); }
  if ( ss_o.is_open() == 0 ) { std::cerr << "sig_o.txt does not exist" << std::endl; exit(0); }

  bool flag = 0;
  for (unsigned ii=0; ii < NSAMP/2; ii++) {
    for (unsigned ss=0; ss < TT_DUT::SSR_I; ss++) {
      double val0_re, val0_im, val1_re, val1_im;
      double val2_re, val2_im, val3_re, val3_im;
      // Generate stimulus:
      ss_i >> val0_re >> val0_im >> val1_re >> val1_im >> val2_re >> val2_im >> val3_re >> val3_im;
      // TT_DUT::TT_AXI4 val_i;

	  // TT_DUT::TT_AXI4 val_i = (TT(val1_im).range(15,0),TT(val1_re).range(15,0),TT(val0_im).range(15,0),TT(val0_re).range(15,0)); // Note order
      TT_DUT::TT_AXI4 val_i = (TT(val3_im).range(15,0),TT(val3_re).range(15,0),TT(val2_im).range(15,0),TT(val2_re).range(15,0),
                               TT(val1_im).range(15,0),TT(val1_re).range(15,0),TT(val0_im).range(15,0),TT(val0_re).range(15,0)); // Note order

      // val_i.data0 = TT_DUT::TC_DATA(val0_re,val0_im);
      // val_i.data1 = TT_DUT::TC_DATA(val1_re,val1_im);
      // val_i(15,0) = TT(val0_re,val0_im);
      // val_i(31,16) = TT_DUT::TC_DATA(val1_re,val1_im);

      sig_i[ss].write(val_i);
    } // ss

    // Run DUT:
    m16_ssr8_cyclic_shift_wrapper( sig_i, sig_o );

    // Check result:
    for (unsigned ss=0; ss < TT_DUT::SSR_O; ss++) {
      double val0_re, val0_im, val1_re, val1_im;
      double val2_re, val2_im, val3_re, val3_im;
      // ss_o >> val0_re >> val0_im >> val1_re >> val1_im;
      ss_o >> val0_re >> val0_im >> val1_re >> val1_im >> val2_re >> val2_im >> val3_re >> val3_im;

      TT_DUT::TT_AXI4 val_g = (TT(val3_im).range(15,0),TT(val3_re).range(15,0),TT(val2_im).range(15,0),TT(val2_re).range(15,0),
                               TT(val1_im).range(15,0),TT(val1_re).range(15,0),TT(val0_im).range(15,0),TT(val0_re).range(15,0)); // Note order

      // val_g.data0 = TT_DUT::TC_DATA(val0_re,val0_im);
      // val_g.data1 = TT_DUT::TC_DATA(val1_re,val1_im);
      ///val_g(15,0)  = TT_DUT::TC_DATA(val0_re,val0_im);
      ///val_g(31,16) = TT_DUT::TC_DATA(val1_re,val1_im);

      TT_DUT::TT_AXI4 val_a = sig_o[ss].read();
      // For debug
      // cout << val_a.data0 << " " << val_g.data0 << " "<< val_a.data1 << " " << val_g.data1 << endl;
      // flag |= ( val_a.data0 != val_g.data0 || val_a.data1 != val_g.data1 ) ? 1 : 0;
	  flag |= ( val_a != val_g ) ? 1 : 0;
	  std::cout << val_a << " " << val_g << std::endl;

    } // ss
  } //ii
  ss_i.close();
  ss_o.close();
  if ( flag == 0 ) cout << "--- PASSED ---" << endl;
  else             cout << "*** FAILED ***" << endl;
  return( flag );
}

