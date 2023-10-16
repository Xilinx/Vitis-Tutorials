/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
using namespace adf;
const int ELEM=16;
static int iteration=0;
template<int NUM>
void vect_add(input_buffer<int32,extents<NUM>>& __restrict in,output_buffer<int32,extents<NUM>>& __restrict out,const int32 (&value)[NUM], int32 &iter){
	iter=iteration++;
	auto inIter=aie::begin_vector<ELEM>(in);
	auto outIter=aie::begin_vector<ELEM>(out);
	auto vIter=aie::begin_vector<ELEM>(value);
	for(int i=0;i<NUM/ELEM;i++)
	chess_prepare_for_pipelining
	{
		aie::vector<int32,ELEM> vdata=*inIter++;
		aie::vector<int32,ELEM> vv=*vIter++;
		aie::vector<int32,ELEM> vresult=aie::add(vdata,vv);
		*outIter++=vresult;
	}
}
