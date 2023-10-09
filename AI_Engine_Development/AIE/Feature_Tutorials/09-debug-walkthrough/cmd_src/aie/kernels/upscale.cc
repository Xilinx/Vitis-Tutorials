/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "kernels.h"
void upscale(
    input_buffer<float> & __restrict in_pie,
    input_stream_int32 * __restrict outmax,
    output_buffer<float> & __restrict out_upscale
)
{
	auto InIter = aie::begin_vector<16>(in_pie);
	auto OutIter = aie::begin_vector<16>(out_upscale);
	int32 max_value;
	int32 scale;
	aie::vector<float,16> in_t = aie::zeros<float,16>();
	aie::accum<accfloat,16> upscale_t = aie::zeros<accfloat,16>();
	
	for(int i=0;i<NUM_SAMPLES;i++) {
		
		in_t = *InIter++;
		max_value = readincr(outmax);                   //Max value out of 16-lane vector
		scale = (max_value < THRESHOLD) ? 2 : 1;        //If max value > Threshold upscale by 2, if not upscale by 1
		upscale_t = aie::mul(in_t,aie::to_float(scale));//Input scaled based on Threshold using aie::mul API
		*OutIter++ = upscale_t.to_vector<float>(0);	
	}
}
