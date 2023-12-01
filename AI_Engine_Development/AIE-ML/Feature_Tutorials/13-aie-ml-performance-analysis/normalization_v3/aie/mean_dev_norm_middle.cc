/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
using namespace adf;
alignas(aie::vector_decl_align) static float accum_zero[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static bfloat16 mean_val=0;
static bfloat16 dev_val=0;

template<int COL, int ROW>
__attribute__((noinline)) void mean_dev_norm_middle(input_buffer<bfloat16> & __restrict data, input_stream<bfloat16> * __restrict mean_dev, input_cascade<accfloat> * __restrict partial_in, output_buffer<bfloat16> & __restrict out, output_cascade<accfloat> * __restrict partial_out){
	//for computing mean
	auto inIter=aie::begin_vector<32>(data);
	aie::accum<accfloat,32> acc;
	acc.from_vector(aie::load_v<32>(accum_zero),0);
	for(int i=0;i<ROW;i++){
		for(int j=0;j<COL/32;j++){
			acc=aie::add(acc,*inIter++);
		}
	}
	auto tmp=readincr_v<32>(partial_in);
	acc=aie::add(acc,tmp);
	writeincr(partial_out,acc);
	chess_separator_scheduler();

	//for computing deviation
	inIter=aie::begin_vector<32>(data);
	mean_val=readincr(mean_dev);
	aie::vector<bfloat16,32> vm=aie::broadcast<bfloat16,32>(mean_val);
	acc.from_vector(aie::load_v<32>(accum_zero));
	for(int i=0;i<ROW;i++){
		for(int j=0;j<COL/32;j++){
			aie::vector<bfloat16,32> tmp=aie::sub(*inIter++,vm);
			auto tmp2=aie::mul(tmp,tmp);
			acc=aie::add(acc,tmp2);
		}
	}
	tmp=readincr_v<32>(partial_in);
	acc=aie::add(acc,tmp);
	writeincr(partial_out,acc);
	chess_separator_scheduler();

	//for computing normalization
	inIter=aie::begin_vector<32>(data);
	auto outIter=aie::begin_vector<32>(out);
	dev_val=readincr(mean_dev);
	vm=aie::broadcast<bfloat16,32>(mean_val);
	for(int i=0;i<ROW;i++){
		for(int j=0;j<COL/32;j++){
			auto tmp=aie::sub(*inIter++,vm);
			auto norm_vec=aie::div(tmp,dev_val);
			*outIter++=norm_vec;
		}
	}
}

