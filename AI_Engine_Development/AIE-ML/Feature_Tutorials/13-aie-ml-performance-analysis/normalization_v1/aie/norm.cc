/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
using namespace adf;
static int iteration=0;
static bfloat16 mean_val=0;
static bfloat16 dev_val=0;

template<int COL, int ROW, int REPEAT>
__attribute__((noinline)) void norm(input_buffer<bfloat16> & __restrict data,input_async_buffer<bfloat16> & __restrict mean_dev, output_buffer<bfloat16> & __restrict out){
	if(iteration==0){
		mean_dev.acquire();
		bfloat16 *pm_v=mean_dev.data();
		mean_val=*pm_v++;
		dev_val=*pm_v;
		if(dev_val<(bfloat16)0.00001f){
			dev_val=(bfloat16)0.00001f;
		}
		mean_dev.release();
	}
	iteration++;

	aie::vector<bfloat16,32> vm=aie::broadcast<bfloat16,32>(mean_val);
	auto inIter=aie::begin_vector<32>(data);
	auto outIter=aie::begin_vector<32>(out);
	for(int i=0;i<ROW;i++){
		for(int j=0;j<COL/32;j++){
			auto tmp=aie::sub(*inIter++,vm);
			auto norm_vec=aie::div(tmp,dev_val);
			*outIter++=norm_vec;
		}
	}
	
	if(iteration==REPEAT){
		iteration=0;
	}
}
