/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
using namespace adf;
alignas(aie::vector_decl_align) static float accum_vec[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static int iteration=0;

template<int COL, int ROW, int REPEAT>
__attribute__((noinline)) void mean(input_buffer<bfloat16> & __restrict data, output_async_buffer<bfloat16> & __restrict out){
	iteration++;

	auto inIter=aie::begin_vector<32>(data);
	aie::accum<accfloat,32> acc;
	acc.from_vector(aie::load_v<32>(accum_vec),0);
	for(int i=0;i<ROW;i++){
		for(int j=0;j<COL/32;j++){
			acc=aie::add(acc,*inIter++);
		}
	}
	
	if(iteration==REPEAT){
		out.acquire();
		bfloat16* pout=out.data();
		*pout=(bfloat16)(aie::reduce_add(acc.to_vector<float>()) / ROW / COL / REPEAT);
		out.release();
		aie::store_v(accum_vec,aie::broadcast<float,32>(0));
		iteration=0;
	}else{
		aie::store_v(accum_vec,acc.to_vector<float>());
	}
}
