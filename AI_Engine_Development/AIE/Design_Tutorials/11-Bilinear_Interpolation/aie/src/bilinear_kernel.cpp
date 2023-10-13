//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#include "bilinear_kernel.h"
#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include "buffers.h"

using namespace adf;

void bilinear_kernel::interp(adf::input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in, 
                             adf::output_buffer<int32, extents<BUFFER_SIZE_OUT>>& __restrict out)
{
	// iterator for input buffer with vector length 8
	auto pIn8 = aie::begin_vector<8>(in);

	// iterator for large work buffer with vector length 8
	auto pLbuf8 = aie::begin_restrict_vector<8>(lgbuf);     
		
	// get constant data vectors used to compute x,y vectors
	aie::vector<float,8> mi = aie::load_v<8>(mul_init);
	aie::vector<float,8> ai = aie::load_v<8>(acc_init);

	for (unsigned i = 0; i < PXLPERGRP; i++)
		chess_prepare_for_pipelining
		chess_loop_range(PXLPERGRP,PXLPERGRP)
	{
		// get data for a single pixel from input
		auto vin = (*pIn8++).cast_to<float>();

		// compute and store x,y vectors
		*pLbuf8++ = fpmac(ai,vin,0,0x55554444,mi,0,0x76543210);
	}

    pLbuf8 -= PXLPERGRP;           // reset iterator for next loop

	chess_separator();

	// iterators for small work buffer with vector length 8 and large work 
	// buffer with vector length 16
	auto pSbuf8 = aie::begin_restrict_vector<8>(smbuf);     
	auto pLbuf16 = aie::begin_restrict_vector<16>(lgbuf);     

	for (unsigned i = 0; i < (PXLPERGRP/2); i++)
		chess_prepare_for_pipelining
		chess_loop_range(PXLPERGRP/2,PXLPERGRP/2)
	{
		// get two pixels worth of data from large buffer
		auto xyin = *pLbuf16++;

		// compute and store xy product vector
		*pSbuf8++ = fpmul(xyin,0,0xBA983210,0,0xFEDC7654);
	}

	pSbuf8 -= (PXLPERGRP/2);       // reset iterator for next loop

	chess_separator();

	// iterator for input buffer with vector length 16
	auto pIn16 = aie::begin_vector<16>(in);

	for (unsigned i = 0; i < (PXLPERGRP/2); i++)
		chess_prepare_for_pipelining
		chess_loop_range(PXLPERGRP/2,PXLPERGRP/2)
	{
		// retrieve xy products
		auto xy = *pSbuf8++;

		// get two pixels worth of data from input
	    auto vin = (*pIn16++).cast_to<float>();

	    // compute and store weighted pixel values
	    *pLbuf8++ = fpmul(vin,0,0xBA983210,xy,0,0x76543210);
	}

    pIn16 -= (PXLPERGRP/2);        // reset input iterator for next loop
    pLbuf8 -= (PXLPERGRP/2);       // reset iterator for next loop

	chess_separator();

	// iterator for output buffer
	auto pOut = aie::begin_vector<4>(out);

	// compute interpolated pixel values and insert into output along with pixel index
	aie::vector<float,4> vout;     // output vector
	aie::vector<float,8> red_sum;  // vector for computing summation of terms

	for (unsigned i = 0; i < (PXLPERGRP/2); i++)
		chess_prepare_for_pipelining
		chess_loop_range(PXLPERGRP/2,PXLPERGRP/2)
	{
		// get output pixel index from input
		auto vin = (*pIn16++).cast_to<float>();

	    // get x*y*p products and compute sum of terms
		auto pxlp = *pLbuf8++;
		red_sum = fpadd(pxlp,pxlp,0,0x67452301);
		red_sum = fpadd(red_sum,red_sum,0,0x54761032);

        // assign values to output
		vout[0] = vin[6];
		vout[2] = vin[14];
		vout[1] = red_sum[0];
		vout[3] = red_sum[4];

		// send to output buffer
		*pOut++ = vout.cast_to<int32>();
    }
}
