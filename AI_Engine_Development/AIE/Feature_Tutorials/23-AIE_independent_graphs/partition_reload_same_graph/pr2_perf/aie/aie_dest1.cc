/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
using namespace adf;
__attribute__ ((noinline)) void aie_dest1(input_buffer<int32,extents<32>> &in, 
        output_stream<int32> *out, output_buffer<int32,extents<32>> &outm){
	auto inIter=aie::begin_vector<4>(in);
	auto outmIter=aie::begin_vector<4>(outm);
	aie::vector<int32,4> tmp;
	for(int i=0;i<8;i++)
	chess_prepare_for_pipelining
	{
		tmp=*inIter++;
		writeincr(out,tmp);
		*outmIter++=tmp;
	}
}

