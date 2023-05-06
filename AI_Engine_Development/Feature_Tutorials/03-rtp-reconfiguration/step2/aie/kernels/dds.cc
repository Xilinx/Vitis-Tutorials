/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "stdlib.h"
#include <stdio.h>
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include "aie_api/utils.hpp"
static int32 phase_in;
using namespace adf;

alignas(aie::vector_decl_align) static int16 dds_stored [16] = {32767, 0, 32767, 0, 32767, 0,  32767, 0, 32767, 0, 32767, 0, 32767, 0, 32767, 0};

void init_dds() {
	aie::vector<cint16,8> dds =  aie::load_v<8>((cint16*)dds_stored);
	phase_in = 0;
  
	auto [sin_,cos_] = aie::sincos(phase_in ); // phase_in + 0
	cint16 scvalues={cos_,sin_};
  
	dds.push(scvalues);
	aie::store_v((cint16*)dds_stored, dds);
}

void sine(const int32 phase_increment,output_buffer<cint16,extents<128>> & owin) {
	//Initialize vector from memory
	aie::vector<cint16,8> dds=aie::load_v<8>((cint16*)dds_stored);
	auto owinIter=aie::begin(owin);

	for (unsigned k = 0; k<128; k++){
		phase_in += (phase_increment << 6);
		auto [sin_,cos_] = aie::sincos(phase_in << 14) ; // phase_in + (7i + j + 1) * phase_increment
		cint16 scvalues={cos_,sin_};
		dds.push(scvalues);

		*owinIter++=scvalues;
		aie::store_v((cint16*)dds_stored, dds);
	};
};

