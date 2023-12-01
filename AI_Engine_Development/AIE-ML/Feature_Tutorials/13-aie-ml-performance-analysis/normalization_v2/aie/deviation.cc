/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
using namespace adf;
static bfloat16 mean_val=0;
static float accum_vec[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static int iteration=0;

template<int COL, int ROW, int REPEAT>
__attribute__((noinline)) void deviation(input_buffer<bfloat16> & __restrict data, input_async_buffer<bfloat16> & __restrict mean, output_async_buffer<bfloat16> & __restrict out){
	if(iteration==0){
		mean.acquire();
		bfloat16* pmean=mean.data();
		mean_val=*pmean;
		mean.release();
	}
	iteration++;

	auto inIter=aie::begin_vector<32>(data);
	aie::vector<bfloat16,32> vm=aie::broadcast<bfloat16,32>(mean_val);
	aie::accum<accfloat,32> acc;
	acc.from_vector(aie::load_v<32>(accum_vec));
	for(int i=0;i<ROW;i++){
		for(int j=0;j<COL/32;j++){
			aie::vector<bfloat16,32> tmp=aie::sub(*inIter++,vm);
			auto tmp2=aie::mul(tmp,tmp);
			acc=aie::add(acc,tmp2);
		}
	}

	if(iteration==REPEAT){
		float accum_val=aie::reduce_add(acc.to_vector<float>()) / ROW / COL / REPEAT;
		out.acquire();
		bfloat16* pout=out.data();
		*pout++=mean_val;
		*pout=(bfloat16)sqrtf(accum_val);
		out.release();
		iteration=0;
		aie::vector<float,32> zero=aie::broadcast<float,32>(0);
		aie::store_v(accum_vec,zero);
	}else{
		aie::store_v(accum_vec,acc.to_vector<float>());
	}

}
