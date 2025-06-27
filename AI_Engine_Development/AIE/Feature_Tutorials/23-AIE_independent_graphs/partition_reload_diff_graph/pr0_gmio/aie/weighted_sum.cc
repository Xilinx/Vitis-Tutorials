/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"
using namespace adf;
alignas(aie::vector_decl_align) int32 weights[8] = {1,2,3,4,5,6,7,8};

void vectorized_weighted_sum_with_margin(input_buffer<int32,extents<256>,margin<8>> & restrict in, output_buffer<int32,extents<256>> & restrict out)
{
	auto inIter=aie::begin_vector<8>(in);
	auto outIter=aie::begin_vector<8>(out);
	aie::vector<int32,8> coeffs = aie::load_v<8>(weights);

	aie::vector<int32,16> data;
	aie::vector<int32,8> dataout;
	data.insert(0,*inIter++);

	for(unsigned i=0; i<256/16; i++)
		chess_prepare_for_pipelining
		chess_loop_range(4,32)
	{
		data.insert(1,*inIter++);
		auto acc=aie::sliding_mul<4,8>(coeffs,0,data,1);
		dataout.insert(0,acc.to_vector<int32>());
		acc=aie::sliding_mul<4,8>(coeffs,0,data,5);
		dataout.insert(1,acc.to_vector<int32>());
		*outIter++=dataout;

		data.insert(0,*inIter++);
		acc=aie::sliding_mul<4,8>(coeffs,0,data,9);
		dataout.insert(0,acc.to_vector<int32>());
		acc=aie::sliding_mul<4,8>(coeffs,0,data,13);
		dataout.insert(1,acc.to_vector<int32>());
		*outIter++=dataout;
	}
}
