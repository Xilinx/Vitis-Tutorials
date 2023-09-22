//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#include "bilinear_kernel.h"
#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
#include "buffers.h"

using namespace adf;

void bilinear_kernel::interp(adf::input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in, adf::output_buffer<int32, extents<BUFFER_SIZE_OUT>>& __restrict out)
{
	// iterators for I/O
	auto pIn = aie::begin_vector<16>(in);
	auto pOut = aie::begin_vector<4>(out);

	// iterator for x vector memory
	auto pXbuf = aie::begin_restrict_vector<8>(xbuf);     
		
	// get constant data vectors to compute x vector	
	aie::vector<float,8> xm = aie::load_v<8>(xmul_init);
	aie::vector<float,8> xa = aie::load_v<8>(xacc_init);

	for (unsigned i = 0; i < (PXLPERGRP/2); i++)
		chess_prepare_for_pipelining
		chess_loop_range(PXLPERGRP/2,PXLPERGRP/2)
	{
		// get two pixels worth of data from input
		auto vin = (*pIn++).cast_to<float>();

		// compute and store x vector
		*pXbuf++ = fpmac(xa,vin,0,0xCCCC4444,xm,0,0x76543210);
	}

    pIn -= (PXLPERGRP/2);             // reset input iterator for next loop
    pXbuf -= (PXLPERGRP/2);           // reset xbuf iterator for next loop

	chess_separator();

	// iterator for y vector memory
	auto pYbuf = aie::begin_restrict_vector<8>(ybuf);     
	
	// get constant data vectors to compute y vector
	aie::vector<float,8> ym = aie::load_v<8>(ymul_init);
	aie::vector<float,8> ya = aie::load_v<8>(yacc_init);

	for (unsigned i = 0; i < (PXLPERGRP/2); i++)
		chess_prepare_for_pipelining
		chess_loop_range(PXLPERGRP/2,PXLPERGRP/2)
	{
		// get two pixels worth of data from input
		auto vin = (*pIn++).cast_to<float>();

		// compute and store y vector
		*pYbuf++ = fpmac(ya,vin,0,0xDDDD5555,ym,0,0x76543210);
	}

	pIn -= (PXLPERGRP/2);             // reset input iterator for next loop
    pYbuf -= (PXLPERGRP/2);           // reset ybuf iterator for next loop

	chess_separator();

	// iterator for z vector memory
	auto pZbuf = aie::begin_restrict_vector<8>(zbuf);     

	aie::vector<float,8> xv;          // x vector
	aie::vector<float,8> yv;          // y vector
	aie::vector<float,8> xy;          // xy product vector

	for (unsigned i = 0; i < (PXLPERGRP/2); i++)
		chess_prepare_for_pipelining
		chess_loop_range(PXLPERGRP/2,PXLPERGRP/2)
	{
		// retrieve x and y vectors and multiply them
		xv = *pXbuf++;
		yv = *pYbuf++;
		xy = fpmul(xv,yv);

		// get two pixels worth of data from input
	    auto vin = (*pIn++).cast_to<float>();

	    // compute and store weighted pixel values
	    *pZbuf++ = fpmul(vin,0,0xBA983210,xy,0,0x76543210);
	}

    pIn -= (PXLPERGRP/2);             // reset input iterator for next loop
    pZbuf -= (PXLPERGRP/2);           // reset zbuf iterator for next loop

	chess_separator();

	// compute interpolated pixel values and insert into output along with pixel index
	aie::vector<float,4> vout;        // output vector
	aie::vector<float,8> pxlp;        // vector of pixel products to be summed

	for (unsigned i = 0; i < (PXLPERGRP/2); i++)
		chess_prepare_for_pipelining
		chess_loop_range(PXLPERGRP/2,PXLPERGRP/2)
	{
		// get output pixel index from input
		auto vin = (*pIn++).cast_to<float>();

	    // get x*y*p products to sum
		pxlp = *pZbuf++;

        // assign values to output
		vout[0] = vin[6];
		vout[2] = vin[14];
		vout[1] = aie::reduce_add(pxlp.extract<4>(0));
		vout[3] = aie::reduce_add(pxlp.extract<4>(1));

		// send to output buffer
		*pOut++ = vout.cast_to<int32>();
    }
}
