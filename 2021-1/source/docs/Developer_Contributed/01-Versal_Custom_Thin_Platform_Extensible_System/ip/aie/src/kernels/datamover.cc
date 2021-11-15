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

// This example uses mul8 to copy 8 lanes of data with value 1 (represented as 32767 i.e. shift of 15)
// This should examplify typlical delay through a mul command
// As no decimation is used we will read 256 bits and write 256 bits each clock cycle.
// Compared with fir11t we need use mul8 to fit the 256 bit read and write sizes (8 cint16)

// temporary block inline to allow vitis_analyzer to display data
//void datamover
void __attribute__ ((noinline)) datamover
(
 input_window_cint16 * restrict cb_input,
 output_window_cint16 * restrict cb_output
)
{

  const int shift = MOVER_SHIFT ;
  const unsigned output_samples = window_size(cb_output);

  // Pointer to coefficients
  const v16int16 coe =  *((const v16int16 *) one_vector) ;

  // Input data temp variable
  v32cint16 inp = undef_v32cint16();
  // Vector data
  v8cint16 vdata = undef_v8cint16();

  // Set rounding and saturation methods
  // not needed here as we multiply with 1 LSB and dont shift data
  //set_rnd(2);
  //set_sat();
#if 0
  cint16 tdata;
  for (int i = 0; i < output_samples; i++)
  {
    window_readincr(cb_input, tdata);
    window_writeincr(cb_output, tdata);
  }
#else

  window_readincr(cb_input, vdata);
  inp = upd_w(inp, 0, vdata);

  const unsigned lc = (output_samples / 4 / 4 );
  for ( unsigned l=0; l<lc; ++l )
  chess_loop_range(8,)
  chess_prepare_for_pipelining
  {
    window_readincr(cb_input, vdata);  // align with v8cint16 according to vdata type
    inp = upd_w(inp, 1, vdata);
    // Implicit accumulator v8cacc48 variable below
    window_writeincr(cb_output, srs( mul8(inp, 0, 0x76543210, 1, coe, 0, 0x00000000, 1), shift));

    window_readincr(cb_input, vdata);
    inp = upd_w(inp, 0, vdata);
    window_writeincr(cb_output, srs( mul8(inp, 8, 0x76543210, 1, coe, 0, 0x00000000, 1), shift));
  }
#endif
}

