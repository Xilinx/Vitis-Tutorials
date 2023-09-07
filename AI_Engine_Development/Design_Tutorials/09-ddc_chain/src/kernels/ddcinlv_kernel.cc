// 
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
// 

#include "ddcinlv_kernel.h"

ddcinlv_kernel::ddcinlv_kernel(void)
{
}

void ddcinlv_kernel::run(input_buffer<cint16> & __restrict cb_input, output_buffer<cint16> & __restrict cb_output)
{
	const unsigned LSIZE = (HBF47D_INPUT_SAMPLES/8/2);
	
	// bypass the header
	auto inputw_lItr = aie::begin_vector<8>(cb_input);
	auto outputwItr = aie::begin_vector<8>(cb_output);
	*outputwItr++ = *inputw_lItr++;
	
	// interleave even/odd samples
	for(unsigned i=0;i<LSIZE; i++)
		chess_prepare_for_pipelining
        chess_loop_range(8,)
	{
		aie::vector<cint16,16> buf;
		aie::vector<int32,16> y;
		
		buf.insert(0,aie::vector<cint16,8>(*inputw_lItr++));
		buf.insert(1,aie::vector<cint16,8>(*inputw_lItr++));
		
		aie::vector<int32,16> buf_casted = aie::vector_cast<int32>(buf);
		y = aie::concat(aie::filter_even(buf_casted,1),aie::filter_odd(buf_casted,1));
		
		*outputwItr++ = aie::vector_cast<cint16>(y.extract<8>(0));
		*outputwItr++ = aie::vector_cast<cint16>(y.extract<8>(1));
	}
	
}
