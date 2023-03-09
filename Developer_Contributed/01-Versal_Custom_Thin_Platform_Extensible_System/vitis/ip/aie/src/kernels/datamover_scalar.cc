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
void __attribute__ ((noinline)) datamover_scalar
(
 input_window_cint16 * restrict cb_input,
 output_window_cint16 * restrict cb_output
)
{

  const unsigned output_samples = window_size(cb_output);

  // Input data temp variable
  cint16 tdata;

  for (int i = 0; i < output_samples; i++)
  {
    window_readincr(cb_input, tdata);
    window_writeincr(cb_output, tdata);
  }
}

