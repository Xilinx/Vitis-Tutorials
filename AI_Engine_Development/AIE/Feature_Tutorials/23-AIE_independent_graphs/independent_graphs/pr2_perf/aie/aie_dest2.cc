/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
using namespace adf;
__attribute__ ((noinline)) void aie_dest2(input_buffer<int32,extents<1024>> & __restrict in1, input_buffer<int32,extents<1024>> & __restrict in2, 
        output_stream<int32> * __restrict outm){
	auto inIter1=aie::begin_vector<4>(in1);
	auto inIter2=aie::begin_vector<4>(in2);
	aie::vector<int32,4> tmp;
	aie::vector<int32,4> tmp2;
	for(int i=0;i<128;i++)
	chess_prepare_for_pipelining
	{
		tmp=*inIter1++;
		tmp2=*inIter2++;
		writeincr(outm,aie::add(tmp,tmp2));
		tmp=*inIter1++;
		tmp2=*inIter2++;
		writeincr(outm,aie::add(tmp,tmp2));
	}
}

