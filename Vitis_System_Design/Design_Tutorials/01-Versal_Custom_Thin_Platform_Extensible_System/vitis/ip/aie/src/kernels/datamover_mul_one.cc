//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

#include <adf.h>
#include "coeffs.h"

// This example uses mul8 to copy 8 lanes of data with value 1 (represented as 32767 i.e. shift of 15)
// This should examplify typlical delay through a mul command
// As no decimation is used we will read 256 bits and write 256 bits each clock cycle.
// Compared with fir11t we need use mul8 to fit the 256 bit read and write sizes (8 cint16)

// temporary block inline to allow vitis_analyzer to display data
//void datamover
void __attribute__ ((noinline)) datamover_mul_one
(
 input_window_cint16 * restrict cb_input,
 output_window_cint16 * restrict cb_output
)
{

  const int shift = MOVER_SHIFT ;
  const unsigned output_samples = window_size(cb_output);

  // Pointer to coefficients
  const v16int16 coe =  *((const v16int16 *) one_vector) ;

  // Vector register data variable
  v32cint16 vreg_data = undef_v32cint16();
  // Temporary Vector data aligned for 256 bit reads
  v8cint16 vdata = undef_v8cint16();

  // Pre-amble
  // load 2x256 bits to vector reg before entering the loop
  // align with v8cint16 according to vdata type
  window_readincr(cb_input, vdata);
  vreg_data = upd_w(vreg_data, 0, vdata);
  window_readincr(cb_input, vdata);
  vreg_data = upd_w(vreg_data, 1, vdata);

  const unsigned lc = (output_samples / 4 / 4 );
  for ( unsigned l=0; l<lc; ++l )
  chess_loop_range(8,)
  chess_prepare_for_pipelining
  {
    // Soft unroll loop into 4 phase scheme
    window_readincr(cb_input, vdata);
    vreg_data = upd_w(vreg_data, 2, vdata);
    // Implicit accumulator v8cacc48 variable below
    window_writeincr(cb_output, srs( mul8(vreg_data, 0, 0x76543210, 1, coe, 0, 0x00000000, 1), shift));

    window_readincr(cb_input, vdata);
    vreg_data = upd_w(vreg_data, 3, vdata);
    window_writeincr(cb_output, srs( mul8(vreg_data, 8, 0x76543210, 1, coe, 0, 0x00000000, 1), shift));

    // Vector register wrap automatically
    window_readincr(cb_input, vdata);
    vreg_data = upd_w(vreg_data, 0, vdata);
    window_writeincr(cb_output, srs( mul8(vreg_data, 16, 0x76543210, 1, coe, 0, 0x00000000, 1), shift));

    window_readincr(cb_input, vdata);
    vreg_data = upd_w(vreg_data, 1, vdata);
    window_writeincr(cb_output, srs( mul8(vreg_data, 24, 0x76543210, 1, coe, 0, 0x00000000, 1), shift));
  }
}

