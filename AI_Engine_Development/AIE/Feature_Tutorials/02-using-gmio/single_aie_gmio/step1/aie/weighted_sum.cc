/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"

using namespace adf;
int32_t weights[8]={1,2,3,4,5,6,7,8};

void weighted_sum_with_margin(input_buffer<int32,extents<256>,margin<8>> & in, output_buffer<int32,extents<256>> & out) 
{
	auto inIter=aie::begin_random_circular(in);
	auto outIter=aie::begin_random_circular(out);

	inIter+=8;

	for (unsigned i = 0; i < 256; i++) {
		int32 val;
		int32 wsum = 0;
		
		inIter-=7;

		for (unsigned j = 0; j < 8; j++){
			val=*inIter++;
			wsum = wsum + (weights[j] * val);
		}
		*outIter++=wsum;
	}
}
