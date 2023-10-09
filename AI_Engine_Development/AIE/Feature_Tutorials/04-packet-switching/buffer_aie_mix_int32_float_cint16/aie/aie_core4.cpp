/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
using namespace adf;
void aie_core4(input_buffer<cint16,extents<8>> &in,output_buffer<cint16,extents<8>> &out){
	auto inIter=aie::begin(in);
	auto outIter=aie::begin(out);
	cint16 tmp;
	for(int i=0;i<8;i++){
		tmp=*inIter++;
		tmp.real=tmp.real+4;
		tmp.imag=tmp.imag+4;
		*outIter++=tmp;
	}
}
