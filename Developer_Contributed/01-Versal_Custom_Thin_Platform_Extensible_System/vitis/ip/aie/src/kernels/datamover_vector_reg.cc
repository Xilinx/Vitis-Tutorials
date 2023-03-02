// /**********
// © Copyright 2020 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// **********/

#include <adf.h>
#include "coeffs.h"

// This example uses vector register as a circular buffer to copy data.
// 256 bit corresponds to 8 cint16 samples, so this datamover will theoretically
// run 8 times faster than incoming data.
// By using soft unrolling a 1024 bit vector register is divided into 4 phases,
// each working on 256 bit data. 
// To avoid read/write access conflicts to vector register, the update of
// each 256 bit vector is one phase (clock cycle) ahead of the extract from
// the vector register.

// temporary block inline to allow vitis_analyzer to display data
//void datamover
void __attribute__ ((noinline)) datamover_vector_reg
(
 input_window_cint16 * restrict cb_input,
 output_window_cint16 * restrict cb_output
)
{

  const int shift = MOVER_SHIFT ;
  const unsigned output_samples = window_size(cb_output);

  // Pointer to coefficients
  const v16int16 coe =  *((const v16int16 *) one_vector) ;

  // Vector register temp variable
  v32cint16 vreg_data = undef_v32cint16();
  // Vector data
  v8cint16 vdata = undef_v8cint16();

  // Pre-amble
  // Load first 256 bits before entering loop
  window_readincr(cb_input, vdata);
  vreg_data = upd_w(vreg_data, 0, vdata);

  const unsigned lc = (output_samples / 4 / 4 );
  for ( unsigned l=0; l<lc; ++l )
  chess_loop_range(8,)
  chess_prepare_for_pipelining
  {
    window_readincr(cb_input, vdata);  // align with v8cint16 according to vdata type
    vreg_data = upd_w(vreg_data, 1, vdata);
    window_writeincr(cb_output, ext_w(vreg_data, 0));

    window_readincr(cb_input, vdata);
    vreg_data = upd_w(vreg_data, 2, vdata);
    window_writeincr(cb_output, ext_w(vreg_data, 1));

    window_readincr(cb_input, vdata);
    vreg_data = upd_w(vreg_data, 3, vdata);
    window_writeincr(cb_output, ext_w(vreg_data, 2));

    window_readincr(cb_input, vdata);
    vreg_data = upd_w(vreg_data, 0, vdata);
    window_writeincr(cb_output, ext_w(vreg_data, 3));
  }
}

