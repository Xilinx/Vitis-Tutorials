/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"
using namespace adf;

void vect_incr(input_buffer<int32,extents<256>> & restrict in, output_buffer<int32,extents<256>> & restrict out)
{
	auto inIter=aie::begin_vector<8>(in);
	auto outIter=aie::begin_vector<8>(out);

	for(unsigned i=0; i<256/16; i++)
		chess_prepare_for_pipelining
		chess_loop_range(4,32)
	{
		*outIter++=aie::add(*inIter++,1);
		*outIter++=aie::add(*inIter++,1);
	}
}
