/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
using namespace adf;
const int ELEM=16;
template<int NUM>
void vect_add(input_buffer<int32,extents<NUM>>& __restrict in,output_buffer<int32,extents<NUM>>& __restrict out,int32 value){
	auto inIter=aie::begin_vector<ELEM>(in);
	auto outIter=aie::begin_vector<ELEM>(out);
	aie::vector<int32,ELEM> vv=aie::broadcast<int32>(value);
	for(int i=0;i<NUM/ELEM;i++)
	chess_prepare_for_pipelining
	{
		aie::vector<int32,ELEM> vdata=*inIter++;
		aie::vector<int32,ELEM> vresult=aie::add(vdata,vv);
		*outIter++=vresult;
	}
}
