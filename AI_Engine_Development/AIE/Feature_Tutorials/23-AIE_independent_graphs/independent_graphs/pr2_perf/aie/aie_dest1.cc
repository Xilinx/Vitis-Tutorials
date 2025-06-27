/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
using namespace adf;
__attribute__ ((noinline)) void aie_dest1(input_buffer<int32,extents<1024>> & __restrict in, 
        output_buffer<int32,extents<1024>> & __restrict out1, output_buffer<int32,extents<1024>> & __restrict out2){
	auto inIter=aie::begin_vector<4>(in);
	auto outIter1=aie::begin_vector<4>(out1);
	auto outIter2=aie::begin_vector<4>(out2);
	aie::vector<int32,4> tmp;
	for(int i=0;i<128;i++)
	chess_prepare_for_pipelining
	{
		tmp=*inIter++;
		*outIter1++=tmp;
		*outIter2++=tmp;
		tmp=*inIter++;
		*outIter1++=tmp;
		*outIter2++=tmp;
	}
}

