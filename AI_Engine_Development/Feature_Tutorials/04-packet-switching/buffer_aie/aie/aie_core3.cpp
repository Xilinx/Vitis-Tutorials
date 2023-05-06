/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
using namespace adf;
void aie_core3(input_buffer<int32,extents<8>> &in,output_buffer<int32,extents<8>> &out){
	auto inIter=aie::begin(in);
	auto outIter=aie::begin(out);
	for(int i=0;i<8;i++){
		*outIter++=*inIter++ +3;
	}
}
