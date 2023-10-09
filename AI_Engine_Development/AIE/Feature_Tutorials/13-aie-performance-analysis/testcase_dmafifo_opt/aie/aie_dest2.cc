/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
using namespace adf;
__attribute__ ((noinline)) void aie_dest2(input_stream<int32> * __restrict in, input_buffer<int32,extents<1024>> & __restrict inm, 
        output_stream<int32> * __restrict outm){
	auto inmIter=aie::begin_vector<4>(inm);
	aie::vector<int32,4> tmp;
	aie::vector<int32,4> tmp2;
	for(int i=0;i<128;i++)
	chess_prepare_for_pipelining
	{
		tmp=readincr_v<4>(in);
		tmp2=*inmIter++;
		writeincr(outm,aie::add(tmp,tmp2));
		tmp=readincr_v<4>(in);
		tmp2=*inmIter++;
		writeincr(outm,tmp+tmp2);
	}
}

