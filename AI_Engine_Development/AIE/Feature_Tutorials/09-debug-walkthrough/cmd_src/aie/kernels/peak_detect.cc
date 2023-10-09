/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "kernels.h"

void peak_detect(
    input_buffer<int32> & __restrict inx, 
    output_buffer<int32> & __restrict to_next,
    output_stream_int32 * __restrict outmax,
    output_buffer<float> & __restrict outmin_v
)
{
	auto InIter = aie::begin_vector<16>(inx);
	auto OutIter_1 = aie::begin_vector<16>(to_next);
	auto OutIter_2 = aie::begin_vector<16>(outmin_v);
	
	aie::vector<int32,16> v_in = aie::zeros<int32,16>(); //16-lane Vector of type int32, Initialized to '0'
	aie::accum<accfloat,16> res = aie::zeros<accfloat,16>();//16-lane Vector of type accfloat, Initialized to '0'
	aie::accum<accfloat,16> mul_pi = aie::zeros<accfloat,16>();

	for(int i=0;i<NUM_SAMPLES;i++) {

		v_in = *InIter++;
		int32 max_out = aie::reduce_max(v_in);             //max{inx[0]....[15]} - Output is 1 value
		
		mul_pi = aie::mul(aie::to_float(v_in),(PIE_VALUE));//Multiply inx[0]...[15] with PIE
		int32 min_m = aie::reduce_min(v_in);        	   //min_m = min{inx[0]....[15]}
		res = aie::add(mul_pi,aie::to_float(min_m));	   //(inx[0]*PIE+(min_m).....inx[15]*PIE+(min_m)) - 16-lane output
		
		*OutIter_1++ = v_in;
		writeincr(outmax,max_out);
		*OutIter_2++ = res.to_vector<float>(0); 
	}
}
