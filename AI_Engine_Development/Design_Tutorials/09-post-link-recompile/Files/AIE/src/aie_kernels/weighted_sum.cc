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
 * Kernel weighted_sum
 *
 * Compute the weighted sum of the last 8 values.
 */

#include <adf.h>
#include <stdio.h>
#include "../include.h"

void weighted_sum(input_window_int32 * in, output_window_int32 * out)
{
  for (unsigned i = 0; i < NUM_SAMPLES; i++)
  {
    int32 val;
    int32 wsum = 0;

    window_decr(in, 7);

    for (unsigned j = 1; j <= 8; j++)
    {
      window_readincr(in, val);

      if (i + j >= 8)
      {
        wsum = wsum + (j * val);
      }
    }

    window_writeincr(out, wsum);
  }
}

void weighted_sum_with_margin(input_window_int32 * in, output_window_int32 * out)
{
  window_incr(in, 8);

  for (unsigned i = 0; i < NUM_SAMPLES; i++)
  {
    int32 val;
    int32 wsum = 0;

    window_decr(in, 7);

    for (unsigned j = 1; j <= 8; j++)
    {
      window_readincr(in, val);
      wsum = wsum + (j * val);
    }

    window_writeincr(out, wsum);
  }
}

int32_t chess_storage(%chess_alignof(v8int32)) weights[8] =
	{ 1, 2, 3, 4, 5, 6, 7, 8};

void vectorized_weighted_sum_with_margin(input_window_int32 * restrict in, output_window_int32 * restrict out)
{
	const int32_t * restrict w = weights;
	v8int32 coeffs = *(v8int32*) weights;

	v16int32 data = undef_v16int32();
	v4acc80 acc;
	v8int32 dataout = undef_v8int32();
	data = upd_w(data,0,window_readincr_v8(in));

	for(unsigned i=0; i<NUM_SAMPLES/8; i++)
		chess_prepare_for_pipelining
		chess_loop_range(4,32)
	{

		acc = lmul4(    data, 1,0x3210,1,coeffs,0,0x0000,1);
		data = upd_w(data,1,window_read_v8(in));
		acc = lmac4(acc,data, 3,0x3210,1,coeffs,2,0x0000,1);
		acc = lmac4(acc,data, 5,0x3210,1,coeffs,4,0x0000,1);
		acc = lmac4(acc,data, 7,0x3210,1,coeffs,6,0x0000,1);

		dataout = upd_v(dataout,0,srs(acc,0));

		acc = lmul4(    data, 5,0x3210,1,coeffs,0,0x0000,1);
		acc = lmac4(acc,data, 7,0x3210,1,coeffs,2,0x0000,1);
		data = upd_w(data,0,window_readincr_v8(in));
		acc = lmac4(acc,data, 9,0x3210,1,coeffs,4,0x0000,1);
		acc = lmac4(acc,data,11,0x3210,1,coeffs,6,0x0000,1);

		dataout = upd_v(dataout,1,srs(acc,0));

		window_writeincr(out,dataout);

	}

}


//int16_t chess_storage(%chess_alignof(v8int16)) weights16[16] =
//	{ 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8};


int16_t chess_storage(%chess_alignof(v8int16)) weights16[16] =
{ 1, 2, 3, 4, 5, 6, 7, 8,0,0,0,0,0,0,0,0};

void vectorized_weighted_cpx_sum_with_margin(input_window_cint16 * restrict in, output_window_cint32 * restrict out)
{
	v16int16 coeffs = *(v16int16*) weights16;

	v16cint16 cdata = undef_v16cint16();

	cdata = upd_w(cdata,0,window_readincr_v8(in));

	for(unsigned i=0; i<NUM_SAMPLES/8; i++)
		chess_prepare_for_pipelining
		chess_loop_range(4,32)
	{

		//v32int16 data = as_v32int16(cdata);

		v4cacc48 cacc = mul4(    cdata, 1,0x3210,1,coeffs,0,0x0000,1);

		cdata = upd_w(cdata,1,window_read_v8(in));
		//data = as_v32int16(cdata);

		cacc = mac4(cacc,cdata, 5,0x3210,1,coeffs,4,0x0000,1);

		v4cint32 cdataout = lsrs(cacc,0);

		window_writeincr(out,cdataout);

		cacc = mul4(    cdata, 5,0x3210,1,coeffs,0,0x0000,1);

		cdata = upd_w(cdata,0,window_readincr_v8(in));
		//data = as_v32int16(cdata);

		cacc = mac4(cacc,cdata, 9,0x3210,1,coeffs,4,0x0000,1);

		cdataout = lsrs(cacc,0);

		window_writeincr(out,cdataout);

	}

}
