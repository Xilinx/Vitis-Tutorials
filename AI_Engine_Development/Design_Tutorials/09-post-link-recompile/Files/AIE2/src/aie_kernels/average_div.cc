/*
 * (c) Copyright 2021 Xilinx, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


/* 
 * Kernel average_div
 *
 * Compute the weighted average by dividing each value by 36.
 */

#include <adf.h>
#include <stdio.h>
#include "../include.h"

void average_div(input_window_int32 * in, output_window_int32 * out)
{
  for (unsigned i = 0; i < NUM_SAMPLES; i++)
  {
    int32 c1;
    window_readincr(in, c1);
    window_writeincr(out, c1/36);
  }
}


#define SHIFT 30
int32 s = (1<<SHIFT)/36;
int32_t chess_storage(%chess_alignof(v8int32)) scale[8] =
	{ s, s, s, s, s, s, s, s};

void vectorized_average_div(input_window_int32 * restrict in, output_window_int32 * restrict out)
{

	v8int32 sc = *(v8int32*) scale;

	v8int32 data = undef_v8int32();
	v8int32 dataout = undef_v8int32();
	v8acc80 acc;


  for (unsigned i = 0; i < NUM_SAMPLES/8; i++)
		chess_prepare_for_pipelining
		chess_loop_range(4,32)
  {
	data = window_readincr_v8(in);
	acc = mul(data,sc);
    window_writeincr(out, srs(acc,SHIFT));
  }
}



void vectorized_cpx_average_div(input_window_cint32 * restrict in, output_window_cint16 * restrict out)
{

	v8int32 sc = *(v8int32*) scale;


  for (unsigned i = 0; i < NUM_SAMPLES/4; i++)
		chess_prepare_for_pipelining
		chess_loop_range(4,32)
  {
	  v4cint32 cdata = window_readincr_v4(in);
	  v8int32 data = as_v8int32(cdata);

	  v8acc80 acc = mul(data,sc);

	v8int32 dataout32 = srs(acc,SHIFT);
	v8int16 dataout = srs(ups(dataout32,0),0);


	v4cint16 cdataout = as_v4cint16(dataout);

    window_writeincr(out, cdataout);
  }
}
